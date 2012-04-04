// A tutorial for Open Dynamics Engine
// sample12 by Kosei Demura 2006-2011
// http://demura.net/
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#ifdef dDOUBLE
#define dsDrawBox      dsDrawBoxD
#define dsDrawSphere   dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule  dsDrawCapsuleD
#endif

static dWorldID world;
static dSpaceID space;
static dGeomID  ground;
static dGeomID  slope;
static dJointGroupID contactgroup;
dsFunctions fn;

dReal side2[3] = {0.2,0.2,0.1};

typedef struct {
  dBodyID body;
  dGeomID geom;
  dReal r, m;
} MyObject;
MyObject box[2];

static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
  const int N = 10;
  dContact contact[N];

  if (((o1 == ground) && (o2 == slope )) ||
      ((o1 == slope)  && (o2 == ground))) return;

  int isGround = ((ground == o1 || slope == o1)
		           || (ground == o2 || slope == o2));

  int n =  dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
  if (isGround)  {
    for (int i = 0; i < n; i++) {
      if ((o1 == box[0].geom) || (o2 == box[0].geom)) {
        // friciton: dContactApprox1 is the friction pyramid model
        contact[i].surface.mode = dContactApprox1|dContactSoftERP|dContactSoftCFM;
      }
      else {
        // friction: default is the constant force limit model
        contact[i].surface.mode = dContactSoftERP|dContactSoftCFM;
      }
      contact[i].surface.mu   = 1.0; // dInfinity;
      contact[i].surface.soft_erp = 1.0; //1.0;
      contact[i].surface.soft_cfm = 1e-10;
      dJointID c = dJointCreateContact(world, contactgroup, &contact[i]);
      dJointAttach(c,dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));
    }
  }
}

static void simLoop(int pause)
{
  static dReal angle = 0;
  const dReal *pos,*R;
  dReal angle_deg;
  dMatrix3 R1;

  // 斜面の傾斜を変更
  angle_deg = angle * M_PI / 180.0;         // 回転角
  dRFromAxisAndAngle(R1, 0, 1, 0, angle_deg);
  dGeomSetRotation(slope, R1);              // 姿勢の設定
  angle += 0.105;
  printf("angle=%.1f \n",angle);


  if (!pause) {
    dSpaceCollide(space,0,&nearCallback);
    dWorldStep(world, 0.01);
    dJointGroupEmpty(contactgroup);
  }

  for (int i = 0; i < 2; i++) {
    if (i == 0) dsSetColor(1.0, 0.0, 0.0);
    else        dsSetColor(0.0, 0.0, 1.0);
    pos = dBodyGetPosition(box[i].body);
    R   = dBodyGetRotation(box[i].body);
    dsDrawBox(pos, R, side2);
  }

  dsSetColor(1.2, 1.2, 0.0);
  pos = dGeomGetPosition(slope);
  R   = dGeomGetRotation(slope);
  dVector3 sides;
  dGeomBoxGetLengths(slope, sides);
  dsDrawBox(pos, R, sides);
}

void start()
{
  static float xyz[3] = {   3.0, 0.0, 1.0};
  static float hpr[3] = {-180.0, 0.0, 0.0};
  dsSetViewpoint(xyz, hpr);
  dsSetSphereQuality(3);
}

void  setDrawStuff()
{
  fn.version = DS_VERSION;
  fn.start   = &start;
  fn.step    = &simLoop;
  fn.command = NULL;
  fn.stop    = NULL;
  fn.path_to_textures = "../textures";
}

// ボックスの生成
void makeBox()
{
  dMass m1;
  dReal x0 =  -2.0, y0 = 0.0, z0 = 0.2;

  for (int i=0; i < 2; i++) {
    box[i].r = 0.2;
    box[i].m = 1.0;
    box[i].body = dBodyCreate(world);
    dMassSetZero(&m1);
    dMassSetBoxTotal(&m1, box[i].m, side2[0],side2[1],side2[2]);
    dBodySetMass(box[i].body, &m1);
    dBodySetPosition(box[i].body, x0, y0-1+2*i, z0);

    box[i].geom = dCreateBox(space, side2[0],side2[1],side2[2]);
    dGeomSetBody(box[i].geom, box[i].body);
  }
}

// 斜面の生成
void makeSlope()
{
  dReal lx = 10,  ly = 4, lz = 0.01;         // 絶対座標系x,y,z軸に沿った長さ
  dReal  x = 0,    y = 0,  z = 0;            // 重心の座標
  dReal ax = 0,   ay = 1, az = 0;            // 回転軸ベクトル

  slope  = dCreateBox(space, lx, ly, lz);    // 直方体ジオメトリの生成

  dGeomSetPosition(slope, x, y, z);          // 位置の設定
}

int main(int argc, char *argv[])
{
  setDrawStuff();
  dInitODE();
  world = dWorldCreate();
  space = dHashSpaceCreate(0);
  contactgroup = dJointGroupCreate(0);
  dWorldSetGravity(world,0,0,-9.8);
  ground = dCreatePlane(space,0,0,1,0);
  makeSlope();
	makeBox();
  dsSimulationLoop(argc,argv,640,480,&fn);
  dWorldDestroy(world);
  dCloseODE();
  return 0;
}
