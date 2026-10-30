// CM_HiddenBooty::Init() -- the hidden-booty minigame script's set-up: clear its three state flags and mark the "M_HiddenBooty" digging spot as hidden through the script interface
// Leaf: ?Init@CM_HiddenBooty@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E93080, length 146 bytes.
// Label correction: the catalog's "CKG_Sheriff::Init" is propagated; the manifest neighbours 0x00E93020 / 0x00E93120 are
// CM_HiddenBooty::OnPredicateFail / Main and the body names "M_HiddenBooty" (0x012ECAD8).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the script interface (+4) virtual slots 72 (+0x120,
// GetThingWithScriptName @ 0x008A7D60) and 578 (+0x908, SetDiggingSpotAsHidden(const CScriptThing&, bool) @ 0x0089F2A0),
// the CScriptThing temporary's inline destructor (vtable 0x01238C8C, counted pimpl release: destroy + operator delete
// @ 0x00BFE9BC, CBaseClass::~CBaseClass @ 0x0099A2E0), CCharString::~CCharString @ 0x0099EAE0.
// Model: as the landed CGuardToHelpDefend::Init 0x00D1EAE0 (interface +4, the script's own thing +8); the three flags at
// +0x1c..+0x1e are modelled descriptively. Shape: the zero register ebx serves the flag stores and the pimpl reset; the
// trailing `true` is pushed before the nested lookup (right to left).
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CScriptThingImp;
template <class T> class CCountedPointer { public:                          // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern const char g_hiddenBootyName[];             // 0x012ECAD8 ("M_HiddenBooty")
struct CM_HiddenBooty {
    virtual void v00();
    CGameScriptInterface* Interface;               // +0x04
    CScriptThing          Me;                      // +0x08
    unsigned char         _pad14[8];
    bool                  m_flag1c;                // +0x1c
    bool                  m_flag1d;                // +0x1d
    bool                  m_flag1e;                // +0x1e
    void Init();
};

void CM_HiddenBooty::Init()
{
    m_flag1c = false;
    m_flag1d = false;
    m_flag1e = false;
    {
        CCharString spotName(g_hiddenBootyName, -1);
        Interface->SetDiggingSpotAsHidden(Interface->GetThingWithScriptName(spotName), true);
    }
}