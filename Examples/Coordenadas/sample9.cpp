// sample9.cpp  by Kosei Demura 2006-2011
// My web site is http://demura.net
// This program uses the Open Dynamics Engine (ODE) by Russell Smith.
// The ODE web site is http://ode.org/

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCappedCylinder dsDrawCappedCylinderD
#endif

static dWorldID      world;
static dSpaceID      space;
static dGeomID       ground;
static dJointGroupID contactgroup;
dsFunctions fn;

typedef struct
{
    dBodyID body;
    dGeomID geom;
} MyObject;
MyObject ball,pillar;

// Create a ball
void createBall()
{
    dMass m1;
    dReal x0 = 0.0, y0 = 0.5, z0 = 1.0, radius = 0.1; //[m]
    dReal mass = 1.0; //[kg]

    ball.body = dBodyCreate(world);   // changed
    dMassSetZero(&m1);
    dMassSetSphereTotal(&m1,mass,radius);
    dBodySetMass(ball.body,&m1);             // changed
    dBodySetPosition(ball.body, x0, y0, z0); // changed

    ball.geom = dCreateSphere(space,radius); // add
    dGeomSetBody(ball.geom,ball.body);       // add
}

// Create a pillar
void createPillar()
{
    dMass m1;
    dReal radius = 0.1, length = 0.5, mass = 2.0; // 半径、長さ、質量

    pillar.body = dBodyCreate(world);
    dMassSetZero(&m1);
    dMassSetCylinderTotal(&m1,mass,3,radius,length);
    dBodySetMass(pillar.body,&m1);
    dBodySetPosition(pillar.body,0.0,0.0,1.0); // (0.0,0.0,1.0)[m]に重心を設定

    dMatrix3 R;
    dRFromAxisAndAngle(R,1.0,0.0,0.0, M_PI/4.0); // ｘ軸周りにπ/4[rad]回転
    dBodySetRotation(pillar.body,R);
    pillar.geom = dCreateCylinder(space,radius,length);
    dGeomSetBody(pillar.geom,pillar.body);
}

static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
    const int N = 10;
    dContact contact[N];

    int isGround = ((ground == o1) || (ground == o2));

    int n =  dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));
    if (isGround)
    {
        for (int i = 0; i < n; i++)
        {
            contact[i].surface.mode = dContactBounce|dContactSoftERP|dContactSoftCFM;
            contact[i].surface.mu   = dInfinity;
            contact[i].surface.bounce = 0.5; // (0.0~1.0)
            contact[i].surface.soft_erp = 1.0;
            contact[i].surface.soft_cfm = 0.0;
            dJointID c = dJointCreateContact(world,contactgroup,&contact[i]);
            dJointAttach (c,dGeomGetBody(contact[i].geom.g1),
                          dGeomGetBody(contact[i].geom.g2));
        }
    }
}

void destroyBall()
{
    dBodyDestroy(ball.body);
    dGeomDestroy(ball.geom);
}

void destroyCylinder()
{
    dBodyDestroy(pillar.body);
    dGeomDestroy(pillar.geom);
}

void drawObject(dGeomID gID,float red,float green,float blue)
{
    switch (dGeomGetClass(gID))
    {
    case dSphereClass:
    {
        dsSetColor(red,green,blue);
        dsDrawSphere(dGeomGetPosition(gID),dGeomGetRotation(gID),
                     dGeomSphereGetRadius(gID));
        break;
    }
    case dCylinderClass:
    {
        dReal r,length;
        dsSetColor(red,green,blue);
        dGeomCylinderGetParams(gID, &r,&length);
        dsDrawCylinder(dGeomGetPosition(gID),dGeomGetRotation(gID),length,r);
        break;
    }
    }
}

void printPos(dBodyID obj_body) // print a position
{
    const dReal *pos;
    // Do not forget const to prevent an error
    dReal x, y, z;
    pos = dBodyGetPosition(obj_body);
    // Return value is an pointer to a structure
    x = pos[0];
    y = pos[1];
    z = pos[2];
    printf("x=%f y=%f z=%f \n", x, y, z);
}


static void simLoop (int pause)
{
    if (!pause)
    {
        dSpaceCollide(space,0,&nearCallback);
        dWorldStep(world,0.01);
        dJointGroupEmpty(contactgroup);
    }
    printPos(ball.body);
    drawObject(ball.geom,1.3,0,0);
    drawObject(pillar.geom,0,0,1.3);

}

void start()
{
    static float xyz[3] = {   3.0, 0.0,1.0};
    static float hpr[3] = {-180.0, 0.0,0.0};
    dsSetViewpoint (xyz,hpr);
}

void restart()
{
    // destroy
    dJointGroupDestroy(contactgroup);
    destroyBall();
    destroyCylinder();

    // create
    contactgroup = dJointGroupCreate(0);
    createBall();
    createPillar();
}

void  prepDrawStuff()
{
    fn.version = DS_VERSION;
    fn.start   = &start;
    fn.step    = &simLoop;
    fn.command = NULL;
    fn.path_to_textures = "../textures";
}

int main (int argc, char **argv)
{
    prepDrawStuff();

    dInitODE();

    world = dWorldCreate();
    space = dHashSpaceCreate(0);
    contactgroup = dJointGroupCreate(0);

    dWorldSetGravity(world,0,0,-0.5);
    dWorldSetERP (world, 1.0);

    ground = dCreatePlane(space,0,0,1,0);

    createBall();      // create a ball
    createPillar();    // create a pillar

    dsSimulationLoop (argc,argv,640,480,&fn);

    dJointGroupDestroy(contactgroup);
    dSpaceDestroy(space);
    dWorldDestroy(world);
    dCloseODE();
    return 0;
}
