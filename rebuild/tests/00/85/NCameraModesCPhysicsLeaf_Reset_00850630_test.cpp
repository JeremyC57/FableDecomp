// Behaviour test for NCameraModes::CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelGotoOutsideSimple, CPhysicsCoordCartesian>::Reset
// 0x00850630: resetting the leaf resets its own force model through the force model's virtual Reset (the derived override
// runs), once.
#include <cstdio>
struct CPhysicsForceModelBase { virtual ~CPhysicsForceModelBase() {} virtual void Reset() {} };
struct CPhysicsForceModelGotoOutsideSimple : CPhysicsForceModelBase { int resets; CPhysicsForceModelGotoOutsideSimple() : resets(0) {} void Reset() { ++resets; } };
struct CSubComponentBase { virtual ~CSubComponentBase() {} virtual void Reset() {} void* Owner; void* CameraManager; };
template <class TForce> struct CPhysicsLeaf : CSubComponentBase {
    CPhysicsForceModelBase* OurForce; void* OurCoords;
    void Reset() { OurForce->Reset(); }
};
int main()
{
    CPhysicsForceModelGotoOutsideSimple force;
    CPhysicsLeaf<CPhysicsForceModelGotoOutsideSimple> leaf; leaf.OurForce = &force;
    CSubComponentBase& sub = leaf;
    sub.Reset();
    if (force.resets != 1) { std::printf("00850630_TEST FAIL\n"); return 1; }
    std::printf("00850630_TEST PASS\n");
    return 0;
}