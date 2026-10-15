// CGameCameraManager::RemoveCombatCameraMode() -- unless the main player is in mode 0x2f, look up the camera manager set's main camera manager def, install it (SetCameraManagerDef) and clear ObservedThingInMelee
// Grade: DIFFER(116v104) -- the local CDefPointer destructor: retail `mov ecx,[slot] ; test ; je ; dec [ecx+4] ; jne`, the lane loads into eax and emits load/`add eax,4`/dec/store/test (every release form tried: member decrement, local-copy decrement, pointee-side Release(), CDefPointer::Release() -- all give the same 12 extra bytes); also the local's zero store is scheduled before the index load where retail puts it after `push eax`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069A920, length 104 bytes (+ int3 padding).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::HasMode(EPlayerMode) @ 0x00634160 (as the landed RemoveBuildingCameraMode
// 0x0069B610), CDefinitionManager::GetDef(long, CDefPointer&) @ 0x0069CE80 (the world's definition manager at +0x10),
// CGameCameraManager::SetCameraManagerDef @ 0x0069A850 (landed), the def's virtual slot 1 (Destroy) in the local's destructor.
// Globals: the engine singleton @ 0x013B86A0 (player manager at +0x1c).
// Layout: retail CGameCameraManager as the landed SetCameraManagerDef / RemoveBuildingCameraMode (World +0x74, ObservedThingInMelee
// +0xa9, PCameraManagerSet +0x130); the set def's main manager def index at +0x44.
// Shape: `push 2Fh` before the GetMainPlayer call (the HasMode argument is evaluated before the object expression); the local
// CDefPointer's zero initialisation is scheduled after the GetDef argument pushes; its destructor releases with `dec [ecx+4] ; jne`.
struct CPlayer {
    bool HasMode(long mode);                        // 0x00634160
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                       // 0x00449970
};
struct CEngine {
    unsigned char _pad[0x1c];
    CPlayerManager* m_pPlayerManager;               // +0x1c
};
extern CEngine* g_pEngine;                          // 0x013B86A0
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1: runs when the last reference goes
    long m_refCount;                                // +4
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
struct CCameraManagerSetDef : CDefPointeeBase {
    unsigned char _pad08[0x3c];
    long          MainCameraManagerDef;             // +0x44 global def index of the set's main camera manager def
};
template <class T>
struct CDefPointer {
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->Release();                   // the release lives on the pointee: `this` stays in ecx (`dec [ecx+4] ; jne`)
    }
    T* operator->() const { return m_pObject; }
    void Release()
    {
        T* old = m_pObject;                         // a local copy: retail decrements through the register (`dec [ecx+4] ; jne`)
        if (old) {
            if (--old->m_refCount == 0) old->Destroy();
            m_pObject = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            Release();
            m_pObject = other.m_pObject;            // re-read after Destroy (`mov eax,[edi]`)
            if (m_pObject) ++m_pObject->m_refCount;
        }
        return *this;
    }
};
struct CCameraManagerDef : CDefPointeeBase { };

struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CCameraManagerDef>& out);   // 0x0069CE80 (manifest names the CThingPhysicalSwitchDef instantiation: identical body)
};
struct CWorld {
    unsigned char       _pad[0x10];
    CDefinitionManager* m_pDefinitionManager;      // +0x10
};
struct CGameCameraManager {
    void*         __vftable;                        // +0x00
    unsigned char _pad04[0x70];                     // +0x04 Camera, OldCamera
    CWorld*       World;                            // +0x74
    unsigned char _pad78[0x30];                     // +0x78 CamTools, CurrentMode, PendingMode
    bool          NewAutoMode;                      // +0xa8
    bool          ObservedThingInMelee;             // +0xa9
    bool          ObservedThingInBuilding;          // +0xaa
    unsigned char _padab[0x85];                     // +0xab
    CDefPointer<CCameraManagerSetDef> PCameraManagerSet;   // +0x130
    CDefPointer<CCameraManagerDef>    PDef;                // +0x134
    void SetCameraManagerDef(const CDefPointer<CCameraManagerDef>& def);   // 0x0069A850 (landed)
    void RemoveCombatCameraMode();
};

void CGameCameraManager::RemoveCombatCameraMode()
{
    if (!g_pEngine->m_pPlayerManager->GetMainPlayer()->HasMode(0x2f)) {
        CDefPointer<CCameraManagerDef> def;
        World->m_pDefinitionManager->GetDef(PCameraManagerSet->MainCameraManagerDef, def);
        SetCameraManagerDef(def);
        ObservedThingInMelee = false;
    }
}
