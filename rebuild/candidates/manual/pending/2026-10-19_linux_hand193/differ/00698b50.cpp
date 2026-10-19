// CGameCameraManager::PushUpManualCameraList() -- the player steps the manual camera list up: when the observed thing
// Grade: DIFFER(ours v retail) -- 221v218: our build fetches the observed thing (GetPItem into edi) right after each string ctor and merges the arms at the call; retail pushes the string address first, merges there, then loads PDef into esi and calls GetPItem last; also cmp eax,[mem] vs a register load
// is alive and no script camera module is pending or blocking, move the list index back one mode and prepare it
// (sound CAMERA_LIST_PUSH_UP), or play CAMERA_LIST_PUSH_ERROR at the top of the list
// Leaf: PushUpManualCameraList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00698B50, length 224 bytes, `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, NCameraModes::CScriptModuleBase virtual slot 2 (+8, whether
// the script module allows manual control), CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// CGameCameraManager::PreparePendingMode(CDefPointer<const CCameraModeDef>) @ 0x00698840, the `__fastcall`
// PlayCameraSoundThroughObservedThing(CThing*, const CCameraManagerDef*, const CCharString&) @ 0x00698A30,
// ~CCharString @ 0x0099EAE0. Strings: 0x0125C304 "CAMERA_LIST_PUSH_ERROR", 0x0125C2F0 "CAMERA_LIST_PUSH_UP".
// Retail layout (engine/CGameCameraManager.h has the PDB offsets, 0xC..0x18 higher from the script modules on, so
// `this` is modelled locally with the PDB names): PCurrentScriptModule +0xD4, PPendingScriptModule +0xDC,
// PObservedThing +0x100 (CIntelligentPointer), CameraList begin +0x108 (STLport 12-byte vector), CameraListIndex
// +0x114, UsingCameraManually +0x12C, BlockModeChange +0x12D, PDef +0x134 (CDefPointer<const CCameraManagerDef>).
// Shape: the two sounds are two complete calls (an inline PlayCameraSound(const CCharString&) member) each with a
// named CCharString local in its own arm (VC7.1 cross-jumps the identical
// call/dtor tails); the observed thing's liveness is the GetPItem null test plus the dead bit of a second GetPItem;
// the CDefPointer argument is built in its argument slot by the inline copy ctor (`inc [eax+4]` AddRef).

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_dead : 1;                       // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CDefPointeeBase {
    virtual void v00();
    mutable long m_refCount;
    void AddRef() const { ++m_refCount; }
};
struct CCameraModeDef : CDefPointeeBase {};
struct CCameraManagerDef : CDefPointeeBase {};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
};

namespace NCameraModes {
struct CScriptModuleBase {
    virtual void v00();
    virtual void v01();
    virtual bool AllowsManualCameraControl();       // slot 2 (+0x08)
};
}

void __fastcall PlayCameraSoundThroughObservedThing(CThing* thing, const CCameraManagerDef* def, const CCharString& sound);   // 0x00698A30

class CGameCameraManager {
public:
    virtual void v00();
    unsigned char _pad004[0xd0];
    NCameraModes::CScriptModuleBase* PCurrentScriptModule_Data;   // +0xD4
    void* PCurrentScriptModule_Info;                                // +0xD8
    NCameraModes::CScriptModuleBase* PPendingScriptModule_Data;   // +0xDC
    unsigned char _pad0e0[0x20];
    CIntelligentPointer<CThing> PObservedThing;                     // +0x100
    CDefPointer<const CCameraModeDef>* CameraList_M_start;         // +0x108
    CDefPointer<const CCameraModeDef>* CameraList_M_finish;        // +0x10C
    CDefPointer<const CCameraModeDef>* CameraList_M_end_of_storage;// +0x110
    CDefPointer<const CCameraModeDef>* CameraListIndex;            // +0x114
    unsigned char _pad118[0x14];
    bool UsingCameraManually;                                       // +0x12C
    bool BlockModeChange;                                           // +0x12D
    unsigned char _pad12e[0x6];
    const CCameraManagerDef* PDef_Object;                           // +0x134
    void PreparePendingMode(CDefPointer<const CCameraModeDef> mode);   // 0x00698840
    __forceinline void PlayCameraSound(const CCharString& sound) { PlayCameraSoundThroughObservedThing(PObservedThing.GetPItem(), PDef_Object, sound); }
    void PushUpManualCameraList();
};

void CGameCameraManager::PushUpManualCameraList()
{
    BlockModeChange = false;
    UsingCameraManually = true;
    if (PObservedThing.GetPItem() && !PObservedThing.GetPItem()->m_dead && PPendingScriptModule_Data == 0 &&
        (PCurrentScriptModule_Data == 0 || PCurrentScriptModule_Data->AllowsManualCameraControl())) {
        if (CameraListIndex == CameraList_M_start) {
            CCharString sound("CAMERA_LIST_PUSH_ERROR", -1);
            PlayCameraSound(sound);
        } else {
            --CameraListIndex;
            PreparePendingMode(*CameraListIndex);
            CCharString sound("CAMERA_LIST_PUSH_UP", -1);
            PlayCameraSound(sound);
        }
    }
}
