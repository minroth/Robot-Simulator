// �ȒP�I���H�I���{�b�g�V�~�����[�V����
// Open Dynamics Engine�ɂ�郍�{�b�g�v���O���~���O
// �o��������, �X�k�o�� (2007) http://demura.net/
// ���̃v���O�����͏�{�̃T���v���v���O�����ł��D
// �v���O���� 5.2:  �S�����ړ��^���{�b�g omni.cpp by Kosei Demura (2007-5-18)
//
// This program is a sample program of my book as follows
//�gRobot Simulation - Robot programming with Open Dynamics Engine,
// (260pages, ISBN:978-4627846913, Morikita Publishing Co. Ltd.,
// Tokyo, 2007)�h by Kosei Demura, which is written in Japanese (sorry).
// http://demura.net/simulation
// Please use this program if you like. However it is no warranty.
// arm2.cpp by Kosei Demura (2007-5-18)
//
// �X�V�����@(change log)
// �X�V����
// 2008-8-28: sqrt()�̈�����2����2.0�ɕύX�@(Yasu����w�E�j
// 2008-8-21:�@drawCircle()��pos1,pos2�̎����C���@�iLW����w�E�j
// 2008-7-7: dInitODE(),dCloseODE()�̒ǉ�
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"

#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
#endif

#ifdef dDOUBLE
#define dsDrawCapsule dsDrawCapsuleD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawBox     dsDrawBoxD
#define dsDrawLine    dsDrawLineD
#define dsDrawSphere  dsDrawSphereD
#endif

#define WHEEL_NUM 4
#define GOAL_PARTS_NUM 7

static dWorldID    world;
static dSpaceID    space;
static dGeomID     ground;
static dGeomID     goal_parts[GOAL_PARTS_NUM];
static dGeomID     goal_parts2[GOAL_PARTS_NUM];

static dJointGroupID contactgroup;
static dsFunctions fn;

typedef struct
{
    dBodyID  body;
    dGeomID  geom;
    dJointID joint;
    dReal    v;         // �p���x
    dReal    m,r,x,y,z; // ���ʁC���a�C�ʒu(x,y,z)
    dReal    lx,ly,lz;  // sizes[3]
} MyObject;

MyObject wheel[WHEEL_NUM], base, ball;


static dReal  M = 9.4, Z = 0.1;  // ���ʁC�����ʒu�̍���
static dReal  SIDE[3] = {0.45,0.45,0.1};
static dReal  offset_z = 0.0;
static int  POWER = 100;

// 0: right panel, 1:left panel, 2:back panel, 3;bar,
// 4: right wall paper, 5: left wall paper, 6: back wall paper
static const dReal GOAL_SIDES[GOAL_PARTS_NUM][3] = {{0.635, 0.125, 1.0},
    {0.635, 0.125, 1.0}, {0.125, 2.01,   1.0}, {0.125,  2.27,  0.125},
    {0.5, 0.01, 1.0}  , {0.5, 0.01, 1.0}, {0.01, 2.01, 1.0}
};
static const dReal FIELD_SIDES[3] = {14,10,offset_z};

/*** ��Ԃ̐��� ***/
static void makeBase()
{
    dMass mass;
    base.x = base.y = 0.0;
    base.z = Z;
    base.lx = SIDE[0];
    base.ly = SIDE[1];
    base.lz = SIDE[2];

    base.body  = dBodyCreate(world);
    dMassSetZero(&mass);
    dMassSetBoxTotal(&mass,M,base.lx, base.ly, base.lz);
    dBodySetMass(base.body,&mass);

    base.geom = dCreateBox(space,base.lx, base.ly, base.lz);
    dGeomSetBody(base.geom, base.body);
    dBodySetPosition(base.body, base.x, base.y, base.z);
}

/*** �ԗւ̐��� ***/
void makeWheel()
{
    dMass mass;
    dReal r = 0.1;
    dReal w = 0.024;
    dReal m = 0.15;
    dReal d = 0.01;
    dReal tmp_z = Z;

    dReal wx[WHEEL_NUM] = {SIDE[0]/2+w/2+d, - (SIDE[0]/2+w/2+d), 0, 0};
    dReal wy[WHEEL_NUM] = {0, 0, SIDE[1]/2+w/2+d, - (SIDE[1]/2+w/2+d)};
    dReal wz[WHEEL_NUM] = {tmp_z, tmp_z, tmp_z, tmp_z};
    dReal jx[WHEEL_NUM] = {SIDE[0]/2, - SIDE[0]/2, 0, 0};
    dReal jy[WHEEL_NUM] = {0, 0, SIDE[1]/2, - SIDE[1]/2};
    dReal jz[WHEEL_NUM] = {tmp_z, tmp_z, tmp_z, tmp_z};

    for (int i = 0; i < WHEEL_NUM; i++)
    {
        wheel[i].v    = 0.0;
        wheel[i].body = dBodyCreate(world);
        dMatrix3 R;
        if (i >= 2)
        {
            dRFromAxisAndAngle(R,1,0,0,M_PI/2.0);
            dBodySetRotation(wheel[i].body,R);
        }
        else
        {
            dRFromAxisAndAngle(R,0,1,0,M_PI/2.0);
            dBodySetRotation(wheel[i].body,R);
        }

        dMassSetZero(&mass);
        if (i < 2)
        {
            dMassSetCylinderTotal(&mass,m, 1, r, w);
        }
        else
        {
            dMassSetCylinderTotal(&mass,m, 2, r, w);
        }
        dBodySetMass(wheel[i].body,&mass);

        wheel[i].geom = dCreateCylinder(space,r, w);
        dGeomSetBody(wheel[i].geom,wheel[i].body);
        dBodySetPosition(wheel[i].body, wx[i], wy[i], wz[i]);

        wheel[i].joint = dJointCreateHinge(world,0);
        dJointAttach(wheel[i].joint,base.body,wheel[i].body);
        if (i < 2)
        {
            dJointSetHingeAxis(wheel[i].joint, 1, 0, 0);
        }
        else
        {
            dJointSetHingeAxis(wheel[i].joint, 0, -1, 0);
        }
        dJointSetHingeAnchor(wheel[i].joint, jx[i], jy[i], jz[i]);
    }
}

/*** �{�[���̐��� ***/
static void makeBall()
{
    dMass mass;
    ball.m = 0.45;
    ball.r = 0.11;
    ball.x = 1.0;
    ball.y = 0.0;
    ball.z = 0.14 + offset_z;

    ball.body = dBodyCreate(world);
    dMassSetZero(&mass);
    dMassSetSphereTotal(&mass,ball.m,ball.r);
    dBodySetMass(ball.body,&mass);

    ball.geom = dCreateSphere(space,ball.r);
    dGeomSetBody(ball.geom,ball.body);
    dBodySetPosition(ball.body,ball.x, ball.y, ball.z);
}

/*** �S�[���̐��� ***/
static void makeGoal()
{
    // 0:right panel, 1:left panel, 2:back panel, 3;bar,
    // 4: right wall paper, 5: left wall paper, 6: back wall paper

    dReal pos[GOAL_PARTS_NUM][3] = {{-6.318, 1.08, 0.505},{-6.318, -1.08, 0.505},
        {-6.59, 0, 0.505},{-6.0625, 0, 1.07},
        {-6.25, 1.005, 0.505},{-6.25, -1.005, 0.505}, {-6.5055, 0, 0.505}
    };

    for (int i = 0; i < GOAL_PARTS_NUM; i++)
    {
        goal_parts[i] = dCreateBox(space, GOAL_SIDES[i][0],
                                   GOAL_SIDES[i][1], GOAL_SIDES[i][2]);
        dGeomSetPosition(goal_parts[i],pos[i][0], pos[i][1], pos[i][2]);
    }

    dReal pos2[GOAL_PARTS_NUM][3] = {{6.318, 1.08, 0.505},{6.318, -1.08, 0.505},
        {6.59, 0, 0.505},{6.0625, 0, 1.07},
        {6.25, 1.005, 0.505},{6.25, -1.005, 0.505}, {6.5055, 0, 0.505}
    };

    for (int i = 0; i < GOAL_PARTS_NUM; i++)
    {
        goal_parts2[i] = dCreateBox(space, GOAL_SIDES[i][0],
                                    GOAL_SIDES[i][1], GOAL_SIDES[i][2]);
        dGeomSetPosition(goal_parts2[i],pos2[i][0], pos2[i][1], pos2[i][2]+offset_z);
    }
}

/*** ��Ԃ̕`�� ***/
static void drawBase()
{
    // dsSetColor(1.3, 1.3, 0.0);   // ���F
    // dsSetColor(0.0, 0.0, 1.3);   // ��
    dsSetColor(0.1, 0.1, 0.1); // ��
    dsDrawBox(dBodyGetPosition(base.body),dBodyGetRotation(base.body),SIDE);
}

/*** �ԗւ̕`�� ***/
void drawWheel()
{
    dReal radius, length;
    dsSetColor(1.1,1.1,1.1);

    for (int i=0; i< WHEEL_NUM; i++)
    {
        dGeomCylinderGetParams(wheel[i].geom, &radius, &length);
        dsDrawCylinder(dGeomGetPosition(wheel[i].geom),
                       dGeomGetRotation(wheel[i].geom),length, radius);
    }
}

/*** �{�[���̕`�� ***/
void drawBall()
{
    dsSetColor(1.0,0.0,0.0);
    dsDrawSphere(dGeomGetPosition(ball.geom),
                 dGeomGetRotation(ball.geom),dGeomSphereGetRadius(ball.geom));
}

/*** �S�[���̕`�� ***/
static void drawGoal()
{
    dsSetTexture(DS_NONE);
    for (int i = 0; i < GOAL_PARTS_NUM; i++)
    {
        if (i < 4)   dsSetColor(1.3, 1.3, 1.3);
        else         dsSetColor(1.3, 1.3, 0.0);
        dsDrawBox(dGeomGetPosition(goal_parts[i]),
                  dGeomGetRotation(goal_parts[i]),GOAL_SIDES[i]);

        if (i < 4)   dsSetColor(1.3, 1.3, 1.3);
        else         dsSetColor(0.0, 0.0, 1.3);
        dsDrawBox(dGeomGetPosition(goal_parts2[i]),
                  dGeomGetRotation(goal_parts2[i]),GOAL_SIDES[i]);
    }
}

/*** �~�̕`�� ***/
static void drawCircle(dReal r,dReal center[2], int angle1, int angle2)
{
    dReal pos1[3],pos2[3],z=offset_z + 0.005;
    pos1[0] = r * cos(angle1 * M_PI/180.0) + center[0];
    pos1[1] = r * sin(angle1 * M_PI/180.0) + center[1];
    pos1[2] = z;
    for (int i = angle1; i < angle2; i++)
    {
        pos2[0] = r* cos(i * M_PI/180.0) + center[0];
        pos2[1] = r* sin(i * M_PI/180.0) + center[1];
        pos2[2] = z;
        dsDrawLine(pos1,pos2);
        pos1[0] = pos2[0];
        pos1[1] = pos2[1];
        pos1[2] = pos2[2];
    }
}

/*** �����̕`�� ***/
static void drawLine()
{
    dReal z = offset_z + 0.005;
    dReal center_r = 1.0, corner_r = 0.5;
    dReal pos[][3] = {{6.0, 4.0, z},{6.0, -4.0, z},{-6.0, -4.0, z},
        {-6.0, 4.0, z},{ 0.0, 4.0, z},{ 0.0, -4.0, z},
        { 6.0, 1.5, z},{ 5.5, 1.5, z},{ 5.5, -1.5, z}, { 6.0, -1.5, z},
        {-6.0, 1.5, z},{-5.5, 1.5, z},{-5.5, -1.5, z}, {-6.0, -1.5, z},
        { 6.0, 2.0, z},{ 4.5, 2.0, z},{ 4.5, -2.0, z}, { 6.0, -2.0, z},
        {-6.0, 2.0, z},{-4.5, 2.0, z},{-4.5, -2.0, z}, {-6.0, -2.0, z}
    };
    dsSetColor(1.3,1.3,1.3);
    dsDrawLine(pos[0],pos[1]);
    dsDrawLine(pos[1],pos[2]);
    dsDrawLine(pos[2],pos[3]);
    dsDrawLine(pos[3],pos[0]);
    dsDrawLine(pos[4],pos[5]);
    dsDrawLine(pos[6],pos[7]);
    dsDrawLine(pos[7],pos[8]);
    dsDrawLine(pos[8],pos[9]);
    dsDrawLine(pos[10],pos[11]);
    dsDrawLine(pos[11],pos[12]);
    dsDrawLine(pos[12],pos[13]);
    dsDrawLine(pos[14],pos[15]);
    dsDrawLine(pos[15],pos[16]);
    dsDrawLine(pos[16],pos[17]);
    dsDrawLine(pos[18],pos[19]);
    dsDrawLine(pos[19],pos[20]);
    dsDrawLine(pos[20],pos[21]);

    // �Z���^�[�T�[�N���̕`��
    dReal pos_center[2] = {0,0};
    dReal pos_corner[4][2] = {{-6, -4},{-6,4},{6,4},{6,-4}};
    drawCircle(center_r,pos_center, 0, 360);
    drawCircle(corner_r,pos_corner[0],0,90);
    drawCircle(corner_r,pos_corner[1],-90,0);
    drawCircle(corner_r,pos_corner[3],90,180);
    drawCircle(corner_r,pos_corner[2],-180,-90);
}

/*** �X�^�[�g�֐� ***/
static void start()
{
    float xyz[3] = {   1.7f, -1.4f, 0.4f};
    float hpr[3] = { 144.0f, -4.0f, 0.0f};
    dsSetViewpoint(xyz,hpr);
    dsSetSphereQuality(3);
}

/*** �R�[���o�b�N�֐� ***/
static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{

    dVector3 tmp_fdir = {0, 0, 0, 0};
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    if (b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact)) return;

    int wheel_flag = 0;
    for (int j = 0; j < WHEEL_NUM; j++)
    {
        if ((o1 == wheel[j].geom)||(o2 == wheel[j].geom))
        {
            wheel_flag = 1;
            dJointGetHingeAxis(wheel[j].joint,tmp_fdir);
            break;
        }
    }

    static const int N = 10;
    dContact contact[N];
    int n = dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));
    if (n > 0)
    {
        if (wheel_flag == 1)
        {
            for (int i=0; i<n; i++)
            {
                contact[i].surface.mode =  dContactFDir1| dContactMu2 | dContactSoftERP | dContactSoftCFM;
                contact[i].fdir1[0] = tmp_fdir[0];   // ��P���C�����̐ݒ�(x������)
                contact[i].fdir1[1] = tmp_fdir[1]; 	 // ��P���C�����̐ݒ�(y������)
                contact[i].fdir1[2] = tmp_fdir[2];   // ��P���C�����̐ݒ�(z������)
                contact[i].surface.mu =  0.1;        // �Ԏ������̖��C�W��
                contact[i].surface.mu2 = dInfinity;  // �ԗ֕����̖��C�W��
                contact[i].surface.soft_erp = 0.9;
                contact[i].surface.soft_cfm = 0.001;

                dJointID c = dJointCreateContact(world,contactgroup,&contact[i]);
                dJointAttach(c,b1,b2);

            }
        }
        else
        {
            for (int i=0; i<n; i++)
            {
                contact[i].surface.mode = dContactSoftERP | dContactSoftCFM;
                contact[i].surface.mu  	= dInfinity;
                contact[i].surface.soft_erp = 0.8;
                contact[i].surface.soft_cfm = 1e-5;
                dJointID c = dJointCreateContact(world,contactgroup,&contact[i]);
                dJointAttach(c,b1,b2);
            }
        }
    }
}

/*** �L�[�{�[�h�R�}���h�̏����֐� ***/
static void command(int cmd)
{
    switch (cmd)
    {
    case '1':
        float xyz1[3],hpr1[3];
        dsGetViewpoint(xyz1,hpr1);
        printf("xyz=%4.2f %4.2f %4.2f ",xyz1[0],xyz1[1],xyz1[2]);
        printf("hpr=%6.2f %6.2f %5.2f \n",hpr1[0],hpr1[1],hpr1[2]);
        break;
    case 'r':
        wheel[0].v += 0.8;
        break; // ���i
    case 'e':
        wheel[0].v -= 0.8;
        break; // �E�i
    case 'u':
        wheel[1].v += 0.8;
        break; // ���i
    case 'i':
        wheel[1].v -= 0.8;
        break; // �E�i
    case 'f':
        wheel[2].v += 0.8;
        break; // �O�i
    case 'd':
        wheel[2].v -= 0.8;
        break; // ��i
    case 'j':
        wheel[3].v += 0.8;
        break; // �O�i
    case 'k':
        wheel[3].v -= 0.8;
        break; // ��i
    case 's':
        wheel[0].v = wheel[1].v = wheel[2].v = wheel[3].v = 0.0; // ��~
    case '+':
        if (POWER >= 100) POWER = 100;
        else              POWER++;
        break;
    case '-':
        if (POWER <=   0) POWER =  0;
        else              POWER--;
        break;
    case 'x':       // �S���V���[�g
    {
        const dReal *bp  = dBodyGetPosition(ball.body);
        const dReal *p   = dBodyGetPosition(wheel[0].body);
        const dReal *R   = dBodyGetRotation(base.body);
        dReal dist  = sqrt(pow(bp[0]-p[0],2)+pow(bp[1]-p[1], 2));
        if (dist < 0.3)
        {
            dReal vmax = POWER * 0.01 * 8.0;
            dBodySetLinearVel(ball.body,vmax * R[0],vmax * R[4], 0.0);
            // printf("z:vx=%f vy=%f \n",vmax*R[0],vmax*R[4]);
        }
    }
    break;
    case 'l':       // ���[�v�V���[�g
    {
        const dReal *bp  = dBodyGetPosition(ball.body);
        const dReal *p   = dBodyGetPosition(wheel[0].body);
        const dReal *R   = dBodyGetRotation(base.body);
        dReal dist  = sqrt(pow(bp[0]-p[0],2)+pow(bp[1]-p[1],2));
        if (dist < 1.0)
        {
            dReal vmax45 = POWER * 0.01 * 8.0 / sqrt(2.0);
            dBodySetLinearVel(ball.body,vmax45 * R[0],vmax45 * R[4], vmax45);
            // printf("z:vx=%f vy=%f \n",vmax*R[0],vmax*R[4]);
        }
    }
    break;
    case 'b':
        dBodySetPosition(ball.body,0,0,0.14+offset_z);
        dBodySetLinearVel(ball.body,0,0,0);
        dBodySetAngularVel(ball.body,0,0,0);
        break;
    }
}

/*** ��  �� ***/
static void control()
{
    dReal fMax  = 10.0;

    for (int i=0; i<WHEEL_NUM;i++)
    {
        dJointSetHingeParam(wheel[i].joint, dParamVel , wheel[i].v);
        dJointSetHingeParam(wheel[i].joint, dParamFMax, fMax);
    }
    printf("POWER=%d \r",POWER);
}

/*** �V�~�����[�V�������[�v ***/
static void simLoop(int pause)
{
    if (!pause)
    {
        dSpaceCollide(space,0,&nearCallback); // add
        control();
        dWorldStep(world, 0.01);
        dJointGroupEmpty(contactgroup); // add
    }
    drawBase();
    drawWheel();  // add
    drawBall();   //add
    drawGoal();
    drawLine();
}

/*** �`��֐��̐ݒ� ***/
static void setDrawStuff()
{
    fn.version = DS_VERSION;
    fn.start   = &start;
    fn.step    = &simLoop;
    fn.command = &command;
    fn.path_to_textures = "../textures";
}

/*** �S�����ړ��^���{�b�g�̐��� ***/
void makeOmni()
{
    makeBase();
    makeWheel();
}

/*** ���C���֐� ***/
int main(int argc, char *argv[])
{
    dInitODE(); // ODE�̏�����
    setDrawStuff(); // �`��֐��̐ݒ�
    world        = dWorldCreate();              // ���[���h�̐���
    space        = dHashSpaceCreate(0);         // �X�y�[�X�̐���
    contactgroup = dJointGroupCreate(0);        // �ڐG�_�O���[�v�̐���
    ground       = dCreatePlane(space,0,0,1,0); // �n�ʂ̐���
    dWorldSetGravity(world, 0.0, 0.0, - 9.8); // �d�͉����x�̐ݒ�
    dWorldSetCFM(world,1e-3); // CFM�̐ݒ�
    dWorldSetERP(world,0.8);  // ERP�̐ݒ�

    makeOmni(); // �S�����ړ����{�b�g�̐���
    makeBall(); // �{�[���̐���
    makeGoal(); // �S�[���̐���
    dsSimulationLoop(argc,argv,640,480,&fn); // �V�~�����[�V�������[�v

    dJointGroupDestroy(contactgroup);  // �ڐG�_�O���[�v�̔j��
    dSpaceDestroy(space);              // �X�y�[�X�̔j��
    dWorldDestroy(world);              // ���[���h�̔j��
    dCloseODE(); // ODE�̏I��
    return 0;
}
