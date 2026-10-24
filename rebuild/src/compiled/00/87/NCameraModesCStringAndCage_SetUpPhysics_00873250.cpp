// NCameraModes::CStringAndCage::SetUpPhysics() -- give the camera mode a rigid go-to physics leaf as a new sub-component
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873250, 124 bytes, plain `ret`.
// Leaf: ?SetUpPhysics@CStringAndCage
// Name: vtable 0x0125C404 slot 26; FableWin.pdb ?SetUpPhysics@CStringAndCage@NCameraModes@@MAEXXZ (the landed CStringAndCageBase::OnInit
//       0x00872630 calls slot 26 first, as the physics/base reset).
// Callees: operator new @ 0x00BFEA1A (0x14-byte leaf, 0xc-byte pointer info), CPhysicsLeaf<CPhysicsTargetModelGoTo,
//          CPhysicsForceModelRigid, CPhysicsCoordRigid>::CPhysicsLeaf(CBase&, const CGameCameraManager&) @ 0x0085C950,
//          NCameraModes::CBase::AddSubComponent(CCountedPointer<CSubComponentBase>, bool) @ 0x0084E810 (FableWin.pdb
//          ?AddSubComponent@CBase@NCameraModes@@QAEXV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@_N@Z; manifest
//          "ScriptEventList::InsertEvent" is propagated); the info block's delete function CCountedPointer<CSubComponentBase>::
//          DeleteData @ 0x00850000.
// Layout: NCameraModes::CBase::CameraManager at +0x40 (retail).
// Shape: the bool `false` pushed first, then the by-value counted pointer built straight in its argument slot (`sub esp,8 ;
//        mov esi,esp`: it has user ctors and a dtor, so the callee owns it); its T* ctor as in CFixedDolly::GetPhysicsMode
//        0x0087B770 (`if (Data) Info = new info; else Info = 0;`), so the call is emitted once per Info exit.
void* __cdecl operator new(unsigned int size);       // 0x00BFEA1A
class CGameCameraManager;
namespace NCameraModes {
class CSubComponentBase;
class CBase;
class CPhysicsTargetModelGoTo;
class CPhysicsForceModelRigid;
class CPhysicsCoordRigid;
}
struct CCPPointerInfo {
    long  RefCount;                                  // +0x00
    void (__fastcall* DeleteFunc)(void* data);       // +0x04
    void* Data;                                      // +0x08
    CCPPointerInfo(void (__fastcall* deleteFunc)(void*), void* data) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    static void __fastcall DeleteData(void* data);   // 0x00850000 for T = NCameraModes::CSubComponentBase
    CCountedPointer(T* data) : Data(data)
    {
        if (Data)
            Info = new CCPPointerInfo(DeleteData, Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();
};
namespace NCameraModes {
class CSubComponentBase {
public:
    virtual ~CSubComponentBase();
    CBase*                    Owner;                 // +0x04
    const CGameCameraManager* CameraManager;         // +0x08
};
template <class TTarget, class TForce, class TCoords> class CPhysicsLeaf : public CSubComponentBase {
public:
    void* OurForce;                                  // +0x0c scoped_ptr<CPhysicsForceModelBase>
    void* OurCoords;                                 // +0x10 scoped_ptr<CPhysicsCoordModelBase>
    CPhysicsLeaf(CBase& owner, const CGameCameraManager& manager);   // 0x0085C950 (Rigid/Rigid instantiation)
};
typedef CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelRigid, CPhysicsCoordRigid> CRigidPhysicsLeaf;

class CBase {
public:
    virtual void v00();
    unsigned char       _pad04[0x40 - 0x04];
    CGameCameraManager* CameraManager;               // +0x40
    unsigned char       _pad44[0xa4 - 0x44];
    void AddSubComponent(CCountedPointer<CSubComponentBase> component, bool flag);  // flag meaning unknown   // 0x0084E810
};
class CStringAndCageBase : public CBase {
public:
    virtual void SetUpPhysics();
};
class CStringAndCage : public CStringAndCageBase {
public:
    virtual void SetUpPhysics();
};
void CStringAndCage::SetUpPhysics()
{
    AddSubComponent(CCountedPointer<CSubComponentBase>(new CRigidPhysicsLeaf(*this, *CameraManager)), false);
}
}