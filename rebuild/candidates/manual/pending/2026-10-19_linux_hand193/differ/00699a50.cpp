// CGameCameraManager::PushLookAround() -- with a live observed thing, switch to manual control and push the free
// Grade: DIFFER(ours v retail) -- 177v175: callee-saved colouring swapped (&PObservedThing in ebx, def in edi; retail edi/ebx, no extra push) and the shared constant 1 lands in al after copying the item to ecx (retail cl)
// look-around (orbit) camera mode, playing ROTATE_START
// Leaf: PushLookAround
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00699A50, length 176 bytes, `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// the `__fastcall` PlayCameraSoundThroughObservedThing(CThing*, const CCameraManagerDef*, const CCharString&)
// @ 0x00698A30, ~CCharString @ 0x0099EAE0, operator new @ 0x00BFEA1A, NCameraModes::CBase::CBase(
// CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) @ 0x0084E420 (labelled CActionParam_Constructor),
// CCountedPointer<NCameraModes::CBase>::CCountedPointer(CBase*) @ 0x0056C880 (an ICF-shared body, labelled for
// CFileInstaller::CFileInfo), CGameCameraManager::SetCameraMode(CCountedPointer<NCameraModes::CBase>) @ 0x00697E20.
// String 0x0125C73C "ROTATE_START"; the default camera mode def 0x013BA8D8; NCameraModes::COrbitGame vtable
// 0x0125C6DC, 0xBC bytes.
// Retail layout: see 0x00698B50 (PObservedThing +0x100, UsingCameraManually +0x12C, BlockModeChange +0x12D, PDef
// +0x134; engine/CGameCameraManager.h has the PDB offsets).
// Shape: the manager def is a named local read after the sound string (so the call order is retail's: PDef into a
// callee-saved register, the string pushed, GetPItem last); the liveness test shares the constant 1 with the two flag stores; the sound string is a named local in its
// own block; the orbit mode is a new-expression whose inline ctor calls the CBase ctor and stores its vtable, wrapped
// in a CCountedPointer argument built in place by the out-of-line raw-pointer ctor.

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


void __fastcall PlayCameraSoundThroughObservedThing(CThing* thing, const CCameraManagerDef* def, const CCharString& sound);   // 0x00698A30

extern CDefPointer<const CCameraModeDef> g_CameraModeDef;   // 0x013BA8D8

class CGameCameraManager;
namespace NCameraModes {
struct CBase {
    CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
    virtual ~CBase();
    unsigned char _body[0xb8];
};
struct COrbitGame : CBase {                         // vtable 0x0125C6DC
    COrbitGame(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) : CBase(manager, def) {}
    virtual ~COrbitGame();
};
}

template<class T> struct CCountedPointer {
    T* Data;
    void* Info;
    CCountedPointer(T* data);                       // 0x0056C880
    CCountedPointer(const CCountedPointer& other);
};

class CGameCameraManager {
public:
    virtual void v00();
    unsigned char _pad004[0xfc];
    CIntelligentPointer<CThing> PObservedThing;                     // +0x100
    unsigned char _pad108[0x24];
    bool UsingCameraManually;                                       // +0x12C
    bool BlockModeChange;                                           // +0x12D
    unsigned char _pad12e[0x6];
    const CCameraManagerDef* PDef_Object;                           // +0x134
    void SetCameraMode(CCountedPointer<NCameraModes::CBase> mode);  // 0x00697E20
    void PushLookAround();
};

void CGameCameraManager::PushLookAround()
{
    if (PObservedThing.GetPItem() && !PObservedThing.GetPItem()->m_dead) {
        BlockModeChange = true;
        UsingCameraManually = true;
        {
            CCharString sound("ROTATE_START", -1);
            const CCameraManagerDef* def = PDef_Object;
            PlayCameraSoundThroughObservedThing(PObservedThing.GetPItem(), def, sound);
        }
        SetCameraMode(CCountedPointer<NCameraModes::CBase>(new NCameraModes::COrbitGame(*this, g_CameraModeDef)));
    }
}
