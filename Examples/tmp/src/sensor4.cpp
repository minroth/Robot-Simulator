// �ȒP�I���H�I���{�b�g�V�~�����[�V����
// Open Dynamics Engine�ɂ�郍�{�b�g�v���O���~���O
// �o��������, �X�k�o�� (2007) http://demura.net/
// ���̃v���O�����͏�{�̃T���v���v���O�����ł��D
// �v���O���� 9.1:  �́E�g���N�Z���T
// sensor.cpp by Kosei Demura (2007-5-19)
//
// This program is a sample program of my book as follows
//�gRobot Simulation - Robot programming with Open Dynamics Engine,
// (260pages, ISBN:978-4627846913, Morikita Publishing Co. Ltd.,
// Tokyo, 2007)�h by Kosei Demura, which is written in Japanese (sorry).
// http://demura.net/simulation
// Please use this program if you like. However it is no warranty.
// sensor4.cpp by Kosei Demura (2007-5-18)
//
// �X�V�����@(change log)
// 2008-7-7: dInitODE(),dCloseODE()�̒ǉ�
#include <stdio.h>
#include <stdlib.h>
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"

#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
#endif

#ifdef dDOUBLE
#define dsDrawCapsule dsDrawCapsuleD
#define dsDrawBox     dsDrawBoxD
#define dsDrawLine    dsDrawLineD
#endif

#define NUM 4                  // �Z���T��

dWorldID      world;           // ���͊w�v�Z�p�̃��[���h
dSpaceID      space;           // �Փˌ��o�p�̃X�y�[�X
dGeomID       ground;          // �n��
dJointID fixed[NUM];           // �Z���T�p�Œ�W���C���g
dJointGroupID contactgroup;    // �ڐG�_�O���[�v
dsFunctions   fn;              // �h���[�X�^�b�t�̕`��֐�
dJointFeedback feedback[NUM];  // �t�B�[�h�o�b�N�\����

typedef struct {
  dBodyID body;
  dGeomID geom;
} MyLink;
MyLink box,sensor[NUM];        // �{�b�N�X, �Z���T

static dReal SX = 0, SY = 0, SZ = 0.1; // �{�b�N�X�̏����ʒu
static dReal box_l = 0.25, box_w  = 0.15,box_h = 0.05,box_m = 20.0;

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
  static const int MAX_CONTACTS = 10;
  int i;

  // �Q�̃{�f�B���W���C���g�Ō�������Ă����牽�����Ȃ��Ŗ߂�
  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);
  if (b1 && b2 && dAreConnected (b1,b2)) return;

  dContact contact[MAX_CONTACTS];
  int numc = dCollide(o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact));
  if (numc > 0) {
    for (i=0; i<numc; i++) {
      if ((o1 == ground) || (o2 == ground)) {
        dVector3 End;
        dReal kl = 500000.0;
        End[0] = contact[i].geom.pos[0] + (contact[i].geom.normal[0] * contact[i].geom.depth*kl);
        End[1] = contact[i].geom.pos[1] + (contact[i].geom.normal[1] * contact[i].geom.depth*kl);
        End[2] = contact[i].geom.pos[2] + (contact[i].geom.normal[2] * contact[i].geom.depth*kl);
        dsDrawLine(contact[i].geom.pos,End);
      }

      contact[i].surface.mode  =  dContactSoftCFM | dContactSoftERP;
      contact[i].surface.mu       = dInfinity;
      contact[i].surface.soft_cfm = 1e-8;
      contact[i].surface.soft_erp = 1.0;
      dJointID c = dJointCreateContact(world,contactgroup,&contact[i]);
      dJointAttach (c,dGeomGetBody(contact[i].geom.g1),
		      dGeomGetBody(contact[i].geom.g2));
    }
  }
}

void drawObjects()
{
 // �����́i���̂���j�̕`��
  dsSetColorAlpha(1.3, 1.3, 1.3,1.0);
  dVector3 sides1;
  dGeomBoxGetLengths(box.geom,sides1);
  dsDrawBoxD(dBodyGetPosition(box.body),
	     dBodyGetRotation(box.body),sides1);

  // �Z���T�̕`��
  dsSetColor(1.0,0.0,0.0);
  dVector3 sides2;
  for (int i = 0; i < NUM; i++) {
    dGeomBoxGetLengths(sensor[i].geom,sides2);
    dsDrawBoxD(dBodyGetPosition(sensor[i].body),
	      dBodyGetRotation(sensor[i].body),sides2);
  }
}

static void simLoop(int pause)
{
  dSpaceCollide(space,0,&nearCallback);
  dWorldStep(world,0.01);
  dJointGroupEmpty(contactgroup);

  dJointFeedback *feedback;
  dReal fx[NUM],fy[NUM],fz[NUM],tx[NUM],ty[NUM],tz[NUM];
  for (int i = 0; i < NUM; i++) {
    feedback = dJointGetFeedback(fixed[i]); // �t�B�[�h�o�b�N�̎擾
    fx[i] = feedback->f1[0];  // �� x����
    fy[i] = feedback->f1[1];  // �� y����
    fz[i] = feedback->f1[2];  // �� z����
    tx[i] = feedback->t1[0];  // �g���N x���܂��
    ty[i] = feedback->t1[1];  // �g���N y���܂��
    tz[i] = feedback->t1[2];  // �g���N z���܂��
    printf("Force fx=%6.2f fy=%6.2f fz=%6.2f \n ",fx[i],fy[i],fz[i]);
  }

  // �̓Z���T�̒l�ɔ�Ⴕ��������\������
  dVector3 endP;    // ���̏I�_
  dReal k1 = 0.01;  // ���萔�C�����ύX����Ɛ��̒������ς��
  for (int i = 0; i < NUM; i++) {
     const dReal *pos = dBodyGetPosition(sensor[i].body);
     endP[0] = pos[0];  endP[1] = pos[1];
     endP[2] = k1 * (fz[i] + pos[2]);
     dsDrawLine(pos, endP); // pos����endP�܂ł̒�����`��
  }

  drawObjects();  // �Z���T�⑫�̕`��
}

/*** �Z���T�̐����Ǝ��t�� ***/
void makeForceSensor()
{
	dReal sensor_l = 0.01, sensor_w = 0.01;
  dReal sensor_h = 0.01, sensor_m = 0.01;
  for (int i = 0; i < NUM; i++) {
    sensor[i].body   = dBodyCreate(world);
    dMass mass;
    dMassSetZero(&mass);
    dMassSetBoxTotal(&mass,sensor_m,sensor_l,sensor_w,sensor_h);
    dBodySetMass(sensor[i].body,&mass);
    dReal x =  (2 * (int) (i/2) - 1) * (box_l - sensor_l)/2 + SX;
    dReal y =  (2 * (int) (i%2) - 1) * (box_w - sensor_w)/2 + SY;
    dBodySetPosition(sensor[i].body, x, y, SZ - (box_h+sensor_h)/2);
    sensor[i].geom = dCreateBox(space,sensor_l,sensor_w,sensor_h);
    dGeomSetBody(sensor[i].geom,sensor[i].body);

    // �Œ�W���C���g�i�Z���T�Ƒ��̌Œ�j
    fixed[i] = dJointCreateFixed(world,0);
    dJointAttach(fixed[i],box.body,sensor[i].body);
    dJointSetFixed(fixed[i]);

    dJointSetFeedback(fixed[i],&feedback[i]);   // Feedback�̐ݒ�
  }
}

void start()
{
  static float xyz[3] = { 0.0f,-3.0f,1.0f};
  static float hpr[3] = {90.0f, 0.0f,0.0f};
  dsSetViewpoint (xyz,hpr);
}

void  setDrawStuff() {
  fn.version = DS_VERSION;
  fn.start   = &start;
  fn.step    = &simLoop;
  fn.command = NULL;
  fn.stop    = NULL;
  fn.path_to_textures = "../../drawstuff/textures";
}

int main (int argc, char *argv[])
{
  dInitODE();
  setDrawStuff();
  world = dWorldCreate();
  space = dHashSpaceCreate(0);
  contactgroup = dJointGroupCreate(0);
  dWorldSetGravity(world,0,0,-9.8);
  ground = dCreatePlane(space,0,0,1,0);

  // ������(���j
  dMass mass;
  box.body   = dBodyCreate(world);
  dMassSetZero(&mass);
  dMassSetBoxTotal(&mass,box_m,box_l,box_w,box_h);
  dBodySetMass(box.body,&mass);
  dBodySetPosition(box.body,SX,SY,SZ);
  box.geom = dCreateBox(space,box_l,box_w,box_h);
  dGeomSetBody(box.geom,box.body);

  makeForceSensor(); // �Z���T�̐���

  dsSimulationLoop(argc,argv,800, 600,&fn);
  dSpaceDestroy(space);
  dWorldDestroy(world);
  dCloseODE();
  return 0;
}
