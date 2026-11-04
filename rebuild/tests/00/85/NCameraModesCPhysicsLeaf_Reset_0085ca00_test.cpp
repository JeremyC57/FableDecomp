// Behaviour test for NCameraModes::CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelRigid, CPhysicsCoordRigid>::Reset
// 0x0085CA00: resetting the leaf resets its own force model through the force model's virtual Reset (the derived override
// runs), once.
#include <cstdio>
struct CPhysicsForceModelBase { virtual ~CPhysicsForceModelBase() {} virtual void Reset() {} };
struct CPhysicsForceModelRigid : CPhysicsForceModelBase { int resets; CPhysicsForceModelRigid() : resets(0) {} void Reset() { ++resets; } };
struct CSubComponentBase { virtual ~CSubComponentBase() {} virtual void Reset() {} void* Owner; void* CameraManager; };
template <class TForce> struct CPhysicsLeaf : CSubComponentBase {
    CPhysicsForceModelBase* OurForce; void* OurCoords;
    void Reset() { OurForce->Reset(); }
};
int main()
{
    CPhysicsForceModelRigid force;
    CPhysicsLeaf<CPhysicsForceModelRigid> leaf; leaf.OurForce = &force;
    CSubComponentBase& sub = leaf;
    sub.Reset();
    if (force.resets != 1) { std::printf("0085ca00_TEST FAIL\n"); return 1; }
    std::printf("0085ca00_TEST PASS\n");
    return 0;
}