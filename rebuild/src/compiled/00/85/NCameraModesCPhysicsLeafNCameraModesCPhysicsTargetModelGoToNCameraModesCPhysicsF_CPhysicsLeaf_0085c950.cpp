// NCameraModes::CPhysicsLeaf<NCameraModes::CPhysicsTargetModelGoTo,NCameraModes::CPhysicsForceModelRigid,NCameraModes::CPhysicsCoordRigid>::CPhysicsLeaf(NCameraModes::CBase&, const CGameCameraManager&) -- camera physics leaf: bind owner and camera manager, then create its force model and coordinate model on the owner's go-to state
// Leaf: ??0
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085C950, 172 bytes (+4 int3 padding), `__fastcall`, `ret 8`. FableWin.pdb:
//   ??0?$CPhysicsLeaf@VCPhysicsTargetModelGoTo@NCameraModes@@VCPhysicsForceModelRigid@2@VCPhysicsCoordRigid@2@@NCameraModes@@QAE@AAVCBase@1@ABVCGameCameraManager@@@Z.
// Callees: operator new @ 0x00BFEA1A (x2), CPhysicsForceModelBase(CBase&, const CGameCameraManager&, CPhysicsState&) @ 0x00850410,
//   ~CPhysicsForceModelBase @ 0x00850360 (non-virtual, out of line), operator delete @ 0x00BFE9BC. Callers: 0x0085C7EC ...
// Vtables: leaf 0x01276AEC, CPhysicsForceModelRigid 0x01276AC8, CPhysicsCoordRigid 0x01276AC0. Label: the catalog names the GotoOutsideSimple/Cartesian instantiation; the vtables (rtti-vtables.tsv: CPhysicsForceModelRigid 0x01276AC8, CPhysicsCoordRigid 0x01276AC0) make it the Rigid/Rigid one.
// Layout (Ego_r.pdb, 20 bytes): CSubComponentBase {vptr, Owner +4, CameraManager +8}, scoped_ptr<CPhysicsForceModelBase>
//   OurForce +0xC, scoped_ptr<CPhysicsCoordModelBase> OurCoords +0x10. The force model is 0xAC bytes in retail (its own
//   ctor is inline: the base ctor call, then its vtable store); the coordinate model is 16 bytes {vptr, state +4,
//   owner +8, manager +0xC} built inline. The state both models follow is the owner's member at +0x54 (retail offset;
//   the PDB's NCameraModes::CBase places GoTo at +0x68), read back through this->Owner once.
// Shape: the root base's two stores come before the leaf's vtable store (the root's own vtable store is dead), the
//   scoped pointers' inline ctors zero them; reset(p) is `T* old = ptr; ptr = p; delete old;` -- the force model's
//   delete is null test + dtor + operator delete, the coordinate model (trivial dtor) is a bare operator delete.
void* __cdecl operator new(unsigned int size);           // 0x00BFEA1A
void __cdecl operator delete(void* block);                // 0x00BFE9BC

class CGameCameraManager;
namespace NCameraModes {
struct CPhysicsState { float m_values[16]; };           // 64 bytes (Ego_r.pdb)

class CBase {
public:
    unsigned char _pad00[0x54];
    CPhysicsState m_goToState;                            // +0x54 (retail)
};

template <class T> class scoped_ptr {
public:
    T* ptr;
    scoped_ptr() : ptr(0) { }
    void reset(T* p)
    {
        T* old = ptr;
        ptr = p;
        delete old;
    }
};

class CSubComponentBase {                                 // vtable 0x012758F4
public:
    CBase*                    Owner;                      // +0x04
    const CGameCameraManager* CameraManager;              // +0x08
    CSubComponentBase(CBase& owner, const CGameCameraManager& cameraManager) : Owner(&owner), CameraManager(&cameraManager) { }
    virtual void FrameUpdate();
    virtual void Reset();
};

class CPhysicsForceModelBase {
public:
    virtual void Update();
    unsigned char _body[0xA8];
    CPhysicsForceModelBase(CBase& owner, const CGameCameraManager& cameraManager, CPhysicsState& state);   // 0x00850410
    ~CPhysicsForceModelBase();                            // 0x00850360
};

class CPhysicsForceModelRigid : public CPhysicsForceModelBase {        // vtable 0x01276AC8
public:
    CPhysicsForceModelRigid(CBase& owner, const CGameCameraManager& cameraManager, CPhysicsState& state)
        : CPhysicsForceModelBase(owner, cameraManager, state) { }
    virtual void Update();
};

class CPhysicsCoordModelBase {
public:
    virtual void Update();
    CPhysicsState*            State;                      // +0x04
    CBase*                    Owner;                      // +0x08
    const CGameCameraManager* CameraManager;              // +0x0C
    CPhysicsCoordModelBase(CBase& owner, const CGameCameraManager& cameraManager, CPhysicsState& state)
        : State(&state), Owner(&owner), CameraManager(&cameraManager) { }
};

class CPhysicsCoordRigid : public CPhysicsCoordModelBase {       // vtable 0x01276AC0, 16 bytes
public:
    CPhysicsCoordRigid(CBase& owner, const CGameCameraManager& cameraManager, CPhysicsState& state)
        : CPhysicsCoordModelBase(owner, cameraManager, state) { }
    virtual void Update();
};

class CPhysicsTargetModelGoTo;

template <class TTarget, class TForce, class TCoord> class CPhysicsLeaf : public CSubComponentBase {   // vtable 0x01276AEC
public:
    scoped_ptr<CPhysicsForceModelBase> OurForce;          // +0x0C
    scoped_ptr<CPhysicsCoordModelBase> OurCoords;         // +0x10
    CPhysicsLeaf(CBase& owner, const CGameCameraManager& cameraManager);
    virtual void FrameUpdate();
    virtual void Reset();
};

template <class TTarget, class TForce, class TCoord>
CPhysicsLeaf<TTarget, TForce, TCoord>::CPhysicsLeaf(CBase& owner, const CGameCameraManager& cameraManager)
    : CSubComponentBase(owner, cameraManager)
{
    CPhysicsState& state = Owner->m_goToState;
    OurForce.reset(new TForce(owner, cameraManager, state));
    OurCoords.reset(new TCoord(owner, cameraManager, state));
}
}

template class NCameraModes::CPhysicsLeaf<NCameraModes::CPhysicsTargetModelGoTo, NCameraModes::CPhysicsForceModelRigid, NCameraModes::CPhysicsCoordRigid>;