#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "0085c950"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

static int g_forceCtors = 0;
class CGameCameraManager { int id; };
struct CPhysicsState { float m_values[16]; };
class CBase { public: unsigned char _pad00[0x54]; CPhysicsState m_goToState; };
template <class T> class scoped_ptr {
public:
    T* ptr;
    scoped_ptr() : ptr(0) { }
    void reset(T* p) { T* old = ptr; ptr = p; delete old; }
};
class CSubComponentBase {
public:
    CBase* Owner; const CGameCameraManager* CameraManager;
    CSubComponentBase(CBase& owner, const CGameCameraManager& cameraManager) : Owner(&owner), CameraManager(&cameraManager) { }
    virtual ~CSubComponentBase() { }
};
class CPhysicsForceModelBase {
public:
    CBase* m_owner; const CGameCameraManager* m_manager; CPhysicsState* m_state;
    CPhysicsForceModelBase(CBase& owner, const CGameCameraManager& cameraManager, CPhysicsState& state)
        : m_owner(&owner), m_manager(&cameraManager), m_state(&state) { ++g_forceCtors; }
    virtual void Update() { }
};
class CPhysicsForceModelRigid : public CPhysicsForceModelBase {
public:
    CPhysicsForceModelRigid(CBase& owner, const CGameCameraManager& cameraManager, CPhysicsState& state) : CPhysicsForceModelBase(owner, cameraManager, state) { }
    virtual void Update() { }
};
class CPhysicsCoordModelBase {
public:
    virtual void Update() { }
    CPhysicsState* State; CBase* Owner; const CGameCameraManager* CameraManager;
    CPhysicsCoordModelBase(CBase& owner, const CGameCameraManager& cameraManager, CPhysicsState& state)
        : State(&state), Owner(&owner), CameraManager(&cameraManager) { }
};
class CPhysicsCoordRigid : public CPhysicsCoordModelBase {
public:
    CPhysicsCoordRigid(CBase& owner, const CGameCameraManager& cameraManager, CPhysicsState& state) : CPhysicsCoordModelBase(owner, cameraManager, state) { }
    virtual void Update() { }
};
class CPhysicsTargetModelGoTo;
template <class TTarget, class TForce, class TCoord> class CPhysicsLeaf : public CSubComponentBase {
public:
    scoped_ptr<CPhysicsForceModelBase> OurForce;
    scoped_ptr<CPhysicsCoordModelBase> OurCoords;
    CPhysicsLeaf(CBase& owner, const CGameCameraManager& cameraManager)
        : CSubComponentBase(owner, cameraManager)
    {
        CPhysicsState& state = Owner->m_goToState;
        OurForce.reset(new TForce(owner, cameraManager, state));
        OurCoords.reset(new TCoord(owner, cameraManager, state));
    }
};
int main()
{
    CBase owner; CGameCameraManager manager;
    CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelRigid, CPhysicsCoordRigid> leaf(owner, manager);
    CHECK(leaf.Owner == &owner && leaf.CameraManager == &manager && g_forceCtors == 1);
    CHECK(leaf.OurForce.ptr != 0 && leaf.OurForce.ptr->m_state == &owner.m_goToState && leaf.OurForce.ptr->m_owner == &owner);
    CHECK(leaf.OurCoords.ptr != 0 && leaf.OurCoords.ptr->State == &owner.m_goToState && leaf.OurCoords.ptr->Owner == &owner
          && leaf.OurCoords.ptr->CameraManager == &manager);
    std::printf("0085c950_TEST PASS\n");
    return 0;
}