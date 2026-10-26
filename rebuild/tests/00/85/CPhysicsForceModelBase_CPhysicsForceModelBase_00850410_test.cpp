#include <cstdio>
#ifndef _MSC_VER
#define __cdecl
#endif
// CPhysicsForceModelBase::CPhysicsForceModelBase(CBase*, CGameCameraManager*, CPhysicsState*) -- initialize linked state and three 0x24-byte force submodels; retail @ 0x00850410, 199 bytes.
// The landed GetVelocity model confirms the vptr +0 and LinkedState +4 base layout. Each submodel constructor call is the external helper at 0x008FB170, with six scalar parameters stored at subobject offsets +0xc..+0x20. The trailing state, float, flags, owner and camera-manager fields follow the retail stores at +0x74..+0xa8.
struct CBase;
class CGameCameraManager;
struct CPhysicsState;
struct CPhysicsForceSubmodel {
    unsigned char _storage[0x24];
    CPhysicsForceSubmodel(long kind, float a, float b, long c, long d, long e); // 0x008FB170
};
struct CPhysicsForceModelBase {
    virtual void Slot00();
    CPhysicsState* LinkedState;
    CPhysicsForceSubmodel first;
    CPhysicsForceSubmodel second;
    CPhysicsForceSubmodel third;
    long value74, value78, value7c, value80, value84, value88, value8c, value90, value94;
    float scale98;
    unsigned long flags9c;
    unsigned char enabledA0;
    unsigned char padA1[3];
    CBase* owner;
    CGameCameraManager* cameraManager;
    CPhysicsForceModelBase(CBase* owner, CGameCameraManager* camera, CPhysicsState* state);
};
CPhysicsForceModelBase::CPhysicsForceModelBase(CBase* owner, CGameCameraManager* camera, CPhysicsState* state)
    : LinkedState(state),
      first(10, 0.0f, 0.05f, 255, 0, 0),
      second(20, 0.25f, 0.26f, 255, 180, 60),
      third(10, 0.25f, 0.05f, 0, 0, 255),
      value74(0), value78(0), value7c(0), value80(0), value84(0), value88(0), value8c(0), value90(0), value94(0),
      scale98(1.0f), flags9c(0xffffffff), enabledA0(0), owner(owner), cameraManager(camera)
{
}

void CPhysicsForceModelBase::Slot00() {}
static int calls;
static long kinds[3], cvals[3], dvals[3], evals[3];
static CPhysicsForceSubmodel* receivers[3];
static float avals[3], bvals[3];
CPhysicsForceSubmodel::CPhysicsForceSubmodel(long k, float a, float b, long c, long d, long e) {
    int i=calls++;
    receivers[i]=this;
    kinds[i]=k; avals[i]=a; bvals[i]=b; cvals[i]=c; dvals[i]=d; evals[i]=e;
    unsigned int* words=(unsigned int*)_storage; words[3]=(unsigned int)k; words[4]=(unsigned int)c; words[5]=(unsigned int)d; words[6]=(unsigned int)e;
}
int main() {
    CBase* owner=(CBase*)0x1010;
    CGameCameraManager* camera=(CGameCameraManager*)0x2020;
    CPhysicsState* state=(CPhysicsState*)0x3030;
    CPhysicsForceModelBase model(owner,camera,state);
    if (calls!=3 || receivers[0]!=&model.first || receivers[1]!=&model.second || receivers[2]!=&model.third || model.LinkedState!=state || model.owner!=owner || model.cameraManager!=camera ||
        model.value74 || model.value78 || model.value7c || model.value80 || model.value84 ||
        model.value88 || model.value8c || model.value90 || model.value94 || model.enabledA0 ||
        model.scale98!=1.0f || model.flags9c!=0xffffffffUL ||
        kinds[0]!=10 || avals[0]!=0.0f || bvals[0]!=0.05f || cvals[0]!=255 || dvals[0] || evals[0] ||
        kinds[1]!=20 || avals[1]!=0.25f || bvals[1]!=0.26f || cvals[1]!=255 || dvals[1]!=180 || evals[1]!=60 ||
        kinds[2]!=10 || avals[2]!=0.25f || bvals[2]!=0.05f || cvals[2] || dvals[2] || evals[2]!=255) {
        std::printf("00850410_TEST FAIL\n"); return 1;
    }
    std::printf("00850410_TEST PASS\n"); return 0;
}