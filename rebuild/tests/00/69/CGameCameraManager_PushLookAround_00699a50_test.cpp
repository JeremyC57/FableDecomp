// CGameCameraManager::PushLookAround() -- with a live observed thing, switch to manual control and push the free
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
// callee-saved register, the string pushed, GetPItem last); the liveness test is an inline IsDead() over the +0x91 flag byte
// (`(flags & 1) != 0`; a 1-bit bit-field copies the item to ecx and recolours &PObservedThing/def) and shares the constant 1
// with the two flag stores; the sound string is a named local in its
// own block; the orbit mode is a new-expression whose inline ctor calls the CBase ctor and stores its vtable, wrapped
// in a CCountedPointer argument built in place by the out-of-line raw-pointer ctor.

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0 dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CDefPointeeBase { public:
    virtual void v00();
    mutable long m_refCount;
    void AddRef() const { ++m_refCount; }
};
class CCameraModeDef : public CDefPointeeBase { public:};
class CCameraManagerDef : public CDefPointeeBase { public:};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
};


void __fastcall PlayCameraSoundThroughObservedThing(CThing* thing, const CCameraManagerDef* def, const CCharString& sound);   // 0x00698A30

extern CDefPointer<const CCameraModeDef> g_CameraModeDef;   // 0x013BA8D8

class CGameCameraManager;
namespace NCameraModes {
class CBase { public:
    CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
    virtual ~CBase();
    unsigned char _body[0xb8];
};
struct COrbitGame : CBase {                         // vtable 0x0125C6DC
    COrbitGame(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) : CBase(manager, def) {}
    virtual ~COrbitGame();
};
}

template<class T> class CCountedPointer { public:
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
    if (PObservedThing.GetPItem() && !PObservedThing.GetPItem()->IsDead()) {
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

#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static char g_soundName[32]; static int g_strings = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; ++g_strings; strcpy(g_soundName, text); }
CCharString::~CCharString() { --g_strings; }
static CThing* g_item = 0; static int g_getItem = 0;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_getItem; return (CBaseObject*)g_item; }
static CThing* g_playThing = 0; static const CCameraManagerDef* g_playDef = 0; static int g_play = 0; static int g_liveStrings = -1;
void __fastcall PlayCameraSoundThroughObservedThing(CThing* thing, const CCameraManagerDef* def, const CCharString& sound)
{ ++g_play; g_playThing = thing; g_playDef = def; g_liveStrings = g_strings; }
void CDefPointeeBase::v00() { }
static unsigned char g_defMem[sizeof(CDefPointer<const CCameraModeDef>)];
CDefPointer<const CCameraModeDef> g_CameraModeDef(*(CDefPointer<const CCameraModeDef>*)g_defMem);   // zeroed copy: no AddRef
static CGameCameraManager* g_baseMgr = 0; static const void* g_baseDef = 0;
NCameraModes::CBase::CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) { g_baseMgr = &manager; g_baseDef = &def; }
NCameraModes::CBase::~CBase() { }
NCameraModes::COrbitGame::~COrbitGame() { }
template<class T> CCountedPointer<T>::CCountedPointer(T* data) : Data(data), Info(0) { }
template<class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { }
static NCameraModes::CBase* g_mode = 0; static int g_setMode = 0;
void CGameCameraManager::SetCameraMode(CCountedPointer<NCameraModes::CBase> mode) { ++g_setMode; g_mode = mode.Data; }
void CGameCameraManager::v00() { }

int main()
{
    static CGameCameraManager mgr;
    static CCameraManagerDef def;
    mgr.PDef_Object = &def; mgr.UsingCameraManually = false; mgr.BlockModeChange = false;
    static CThing thing; memset(&thing, 0, sizeof(thing));
    g_item = 0;
    mgr.PushLookAround();
    CHECK(g_play == 0 && g_setMode == 0 && !mgr.UsingCameraManually);
    g_item = &thing; thing.m_flags91 = 1;      // dead
    mgr.PushLookAround();
    CHECK(g_play == 0 && g_setMode == 0 && !mgr.BlockModeChange);
    thing.m_flags91 = 0x80;                    // other bits only: alive
    mgr.PushLookAround();
    CHECK(mgr.UsingCameraManually && mgr.BlockModeChange);
    CHECK(g_play == 1 && g_playThing == &thing && g_playDef == &def && g_liveStrings == 1 && g_strings == 0);
    CHECK(strcmp(g_soundName, "ROTATE_START") == 0);
    CHECK(g_setMode == 1 && g_mode != 0 && g_baseMgr == &mgr && g_baseDef == &g_CameraModeDef);
    delete g_mode;
    if (fails == 0) printf("00699a50_TEST PASS\n");
    return fails;
}