// CGameScriptInterface::CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const -- hand the camera manager a scripted dolly mode circling a position at an offset
// Leaf: ?CameraCircleAroundPos@CGameScriptInterface@@UBEXABVC3DVector@@0M@Z
// Retail: Fable.exe @ 0x00892C20, 204 bytes, `ret 0xc`; Ego_r.pdb same symbol.
// Body: World (+0x04)->PCurrentGameCamera (+0x34)->GetCameraManager(&manager) (slot 9); manager->SetScriptedCameraMode(
// CCountedPointer<CScriptModuleBase>(new CScriptModuleDolly(pos, offset, time))) -- operator new 0x00BFEA1A (0x68 bytes);
// inline ctor: base CScriptModuleBase() 0x005ED070, vptr 0x01278160 (RTTI NCameraModes::CScriptModuleDolly), PosAround,
// DollyVector, TimeFor360Turn. Twin of the matched 0x00892B50.
// Layout: Ego_r.pdb CScriptModuleDolly (0x68): PosAround +0x4c, DollyVector +0x58, TimeFor360Turn +0x64.
// Shape: /O2 (16-aligned entry); __forceinline CCountedPointer(T*) handing Object to the info block; C3DVector user copy
// ctor (direct-offset member copies).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
namespace NCameraModes {
class CScriptModuleBase {
public:
    unsigned char _pad04[0x4c - 4];
    CScriptModuleBase();                                 // 0x005ED070
    virtual ~CScriptModuleBase();
};
class CScriptModuleDolly : public CScriptModuleBase {          // vtable 0x01278160
public:
    C3DVector PosAround;                                 // +0x4c
    C3DVector DollyVector;                               // +0x58
    float     TimeFor360Turn;                            // +0x64
    CScriptModuleDolly(const C3DVector& posAround, const C3DVector& dollyVector, float timeFor360Turn)
        : PosAround(posAround), DollyVector(dollyVector), TimeFor360Turn(timeFor360Turn) {}
};
}
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(void (__fastcall* deleteFunc)(void*), void* data) : RefCount(1), DeleteFunc(deleteFunc), Data(data) {}
};
template <class T> class CCountedPointer {
public:
    T*              Object;
    CCPPointerInfo* Info;
    static void __fastcall DeleteObject(void* data);     // 0x005ECF50 (`delete (T*)data`, folded)
    __forceinline CCountedPointer(T* object) : Object(object)
    {
        if (object)
            Info = new CCPPointerInfo(DeleteObject, Object);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();
};
class CGameCameraManager {
public:
    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode);   // 0x00699E60
};
class CAIGameCameraBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** manager);   // slot 9 (+0x24)
};
class CWorld {
public:
    unsigned char      _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;               // +0x34
};
class CGameScriptInterface {
public:
    virtual void CameraCircleAroundPos(const C3DVector& pos, const C3DVector& offset, float timeFor360Turn) const;
    CWorld* World;                                       // +0x04
};
void CGameScriptInterface::CameraCircleAroundPos(const C3DVector& pos, const C3DVector& offset, float timeFor360Turn) const
{
    CGameCameraManager* manager;
    World->PCurrentGameCamera->GetCameraManager(&manager);
    manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
        new NCameraModes::CScriptModuleDolly(pos, offset, timeFor360Turn)));
}