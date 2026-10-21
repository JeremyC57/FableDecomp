// CGameScriptInterface::CameraMoveToPosAndLookAtPos(const C3DVector& pos, const C3DVector& lookAt, float) const -- hand the camera manager a scripted go-to-and-look-at camera mode for a fixed target position
// Leaf: ?CameraMoveToPosAndLookAtPos@CGameScriptInterface@@UBEXABVC3DVector@@0M@Z
// Retail: Fable.exe @ 0x00892410, 153 bytes, `ret 0xc`; Ego_r.pdb same symbol. The float argument is unused.
// Body: World (+0x04)->PCurrentGameCamera (+0x34)->GetCameraManager(&manager) (slot 9, +0x24);
// manager->SetScriptedCameraMode(CCountedPointer<CScriptModuleBase>(new CScriptModuleGoToLookAt(pos, lookAt))) --
// operator new 0x00BFEA1A (0x6c bytes), the position ctor 0x007187C0 (vptr 0x012657D8, RTTI
// NCameraModes::CScriptModuleGoToLookAt), SetScriptedCameraMode 0x00699E60. Unlike the matched 0x008924B0 the
// CCountedPointer(T*) ctor is inline here: Info = new 0xc {RefCount 1, DeleteFunc 0x005ECF50, Data}, or 0 for null.
// Layout: as 0x008924B0 (CWorld::PCurrentGameCamera +0x34); CCountedPointer is 8 bytes (object, info).
// Shape: /O2 (16-aligned entry); the three tails after the inlined null checks are duplicated by the compiler. The
// CCountedPointer(T*) ctor needs __forceinline (VC7.1 keeps it out of line otherwise, 75v153) and must hand the info block
// the Object member, not the parameter: retail reloads Data from the argument slot (`mov ecx,[esi]`).
class C3DVector;
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();
};
class CScriptModuleGoToLookAt : public CScriptModuleBase {
public:
    unsigned char _pad04[0x6c - 4];
    CScriptModuleGoToLookAt(const C3DVector& pos, const C3DVector& lookAt);   // 0x007187C0
};
}
struct CCPPointerInfo {
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
    virtual void CameraMoveToPosAndLookAtPos(const C3DVector& pos, const C3DVector& lookAt, float time) const;
    CWorld* World;                                       // +0x04
};
void CGameScriptInterface::CameraMoveToPosAndLookAtPos(const C3DVector& pos, const C3DVector& lookAt, float time) const
{
    CGameCameraManager* manager;
    World->PCurrentGameCamera->GetCameraManager(&manager);
    manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(new NCameraModes::CScriptModuleGoToLookAt(pos, lookAt)));
}