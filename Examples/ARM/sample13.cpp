//  sample13.cpp  3 DOF manipulator by Kosei Demura  2006-2011
// Change logs
// 2009-03-12: Addition of English comments, and change initialize of
//             mass parameter mass in the main funciton; Thanks Richard Kooijman.
#include <ode/ode.h>             // ODE
#include <drawstuff/drawstuff.h> // The drawing library for ODE; ODEの描画ライブラリ
#define NUM 4         // No. of links; リンク数(土台含む)

dWorldID    world;         // a dynamic world; 動力学計算世界
dBodyID     link[NUM];     // links, link[0] is a base; リンク　link[0]は土台
dJointID    joint[NUM];    // joints, joint[0] is a fixed joint; 関節    joint[0]は土台と地面を固定
static double THETA[NUM] = { 0.0, 0.0, 0.0, 0.0}; // target angle of joints; 関節目標角[rad]
static double l[NUM]  = { 0.10, 0.90, 1.00, 1.00};  // length of links; リンク長[m]
static double r[NUM]  = { 0.20, 0.04, 0.04, 0.04};  // radius of links; リンク半径[m]

void control()    /***  p control; P制御  ****/
{
    static int step = 0;     // step number; シミュレーションのステップ数
    double k1 =  10.0,  fMax  = 100.0; // k1:gain, fMax: max torqeu; k1:比例ゲイン,  fMax：最大トルク[Nm]
    printf("\r%6d:",step++);
    for (int j = 1; j <NUM; j++)
    {
        double tmpAngle = dJointGetHingeAngle(joint[j]);  // current joint angle; 現在の関節角[rad]
        double z = THETA[j] - tmpAngle;  // z= target - current; z: 残差=目標関節角－現在関節角
        dJointSetHingeParam(joint[j],  dParamVel,  k1*z); // angular velocity; 角速度の設定
        dJointSetHingeParam(joint[j], dParamFMax, fMax); // max torqeu; 最大トルクの設定
    }
}

void start()   /*** Initialize drawing API; 描画APIの初期化 ***/
{
    float xyz[3] = {  3.04, 1.28, 0.76};   // view point; 視点x, y, z　[m]
    float hpr[3] = { -160.0, 4.50, 0.00};  // view direction; 視線(heading, pitch, roll)　[°]
    dsSetViewpoint(xyz,hpr);               // setting of view point and direction; 視点と視線の設定
}

void command(int cmd)   /***  key control function; キー操作関数 ***/
{
    switch (cmd)
    {
    case 'j':
        THETA[1] += 0.05; //jキーが押されるとTHETA[1]の角度が0.05[rad]増加する
        // increases THETA[1] when j key is pressed
        break;
    case 'f':
        THETA[1] -= 0.05;
        break;
    case 'k':
        THETA[2] += 0.05;
        break;
    case 'd':
        THETA[2] -= 0.05;
        break;
    case 'l':
        THETA[3] += 0.05;
        break;
    case 's':
        THETA[3] -= 0.05;
        break;
    }

    // English: limit target angles not to destroy a robot
    // Japanese:目標角度が関節可動域を越えないように制限する
    if (THETA[1] <  - M_PI)    THETA[1] =  - M_PI; // M_PI：円周率
    if (THETA[1] >    M_PI)    THETA[1] =    M_PI;
    if (THETA[2] < -2*M_PI/3)  THETA[2] =  - 2*M_PI/3;
    if (THETA[2] >  2*M_PI/3)  THETA[2] =    2*M_PI/3;
    if (THETA[3] < -2*M_PI/3)  THETA[3] =  - 2*M_PI/3;
    if (THETA[3] >  2*M_PI/3)  THETA[3] =    2*M_PI/3;
}

// English: simulation loop.
// Japanese: シミュレーションループ。簡単にするため衝突検出に関するコードは省略。
void simLoop(int pause)
{
    control();
    dWorldStep(world, 0.02);
    // draw a robot; ロボットの描画
    dsSetColor(1.0,1.0,1.0); // set color; 色の設定(r, g, b) 各値は0～1、ここでは白に設定
    for (int i = 0; i <NUM; i++ ) // ロボットのリンクをカプセルで描画
        dsDrawCapsuleD(dBodyGetPosition(link[i]), const double *(dBodyGetRotation(link[i])), l[i], r[i]);
}

int main(int argc, char *argv[])
{
    dsFunctions fn; // an variable for drawstuff; 描画関数

    double x[NUM] = {0.00}, y[NUM] = {0.00};  // COG; 重心位置
    double z[NUM]         = { 0.05, 0.50, 1.50, 2.55};
    double m[NUM] = {10.00, 2.00, 2.00, 2.00};           // mass; 質量
    double anchor_x[NUM]  = {0.00}, anchor_y[NUM] = {0.00}; // anchor point; 回転中心
    double anchor_z[NUM] = { 0.00, 0.10, 1.00, 2.00};
    double axis_x[NUM]  = { 0.00, 0.00, 0.00, 0.00};  // rotation axis; 回転軸
    double axis_y[NUM]  = { 0.00, 0.00, 1.00, 1.00};
    double axis_z[NUM]  = { 1.00, 1.00, 0.00, 0.00};

    fn.version = DS_VERSION;
    fn.start   = &start;
    fn.step    = &simLoop;
    fn.command = &command;
    fn.path_to_textures = "../textures";

    dInitODE();  // Initalize ODE; ODEの初期化
    world = dWorldCreate();  // create a dynamic world; 動力学計算世界の生成
    dWorldSetGravity(world, 0, 0, -9.8); // set gravity; 重力設定

    for (int i = 0; i <NUM; i++)         // creation and setting of links; リンクの生成と設定
    {
        dMass mass;  // mass parameter; 質量パラメータ
        link[i] = dBodyCreate(world);     // create link; リンクの生成
        dBodySetPosition(link[i], x[i], y[i], z[i]); // set position; 位置の設定
        dMassSetZero(&mass);      // initialize the mass parameter; 質量パラメータの初期化
        dMassSetCapsuleTotal(&mass,m[i],3,r[i],l[i]);  // calculate the mass parameter; リンクの質量計算
        dBodySetMass(link[i], &mass);  // set the mass parameter to the body; 質量の設定
    }

    joint[0] = dJointCreateFixed(world, 0); // a fixed joint; 固定関節（土台と地面の固定）
    dJointAttach(joint[0], link[0], 0);     // attach the fixed joint to the ground; 固定関節の取付け
    dJointSetFixed(joint[0]);               // set the fixed joint; 固定関節の設定
    for (int j = 1; j <NUM; j++)
    {
        joint[j] = dJointCreateHinge(world, 0);     // create a hinge joint; ヒンジ関節生成
        dJointAttach(joint[j], link[j-1], link[j]); // attach the joints; 関節の取付け
        dJointSetHingeAnchor(joint[j], anchor_x[j], anchor_y[j],anchor_z[j]); // set an anchor point; 関節中心の設定
        dJointSetHingeAxis(joint[j], axis_x[j], axis_y[j], axis_z[j]); // set an rotation axis; 関節回転軸の設定
    }
    dsSimulationLoop(argc, argv, 640, 570, &fn); // simulation loop; シミュレーションループ
    dCloseODE(); // close ODE
    return 0;
}
