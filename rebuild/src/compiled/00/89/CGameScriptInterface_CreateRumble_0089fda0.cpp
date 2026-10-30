// CGameScriptInterface::CreateRumble(const C3DVector& pos, float strength, float radius, CCharString scriptName) const -- create a rumble at a position, give it the script name and return it as a script thing (the null script thing when nothing live was made)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089FDA0, length 120 bytes.
// Callees: the rumble factory (__fastcall: pos in ecx, the bool in dl, two floats on the stack) @ 0x007F1AC0, CCharString::operator= @ 0x0099EFB0,
//          CThing::GetScriptThing @ 0x004C7E50, CBaseClass::CBaseClass @ 0x0099A2D0, CCharString::~CCharString @ 0x0099EAE0 (the by-value name).
// Shape: CScriptThing returned by hidden pointer; the null script thing is built inline.
// Leaf: CreateRumble
class CCharString { public:                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class C3DVector { public: float x, y, z; };
struct SCountBlock;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
template <class T> class CCountedPointer { public:                          // the counted pointer member: an inline constructor zeroes both words after the vtable store
    CScriptThingImp* Data;                         // +4
    SCountBlock*     Info;                         // +8
    CCountedPointer() : Data(0), Info(0) {}
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
class CThing { public:
    unsigned char  _pad00[0x74];
    CCharString    ScriptName;                     // +0x74
    unsigned char  _pad78[0x91 - 0x78];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    CScriptThing GetScriptThing();                 // 0x004C7E50 (manifest "GFPredicateAnd<...>" is propagated)
};
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager_Retail {
    static CDefinitionManager_Retail& Get();       // 0x0044C6B0
};
class CWorld; class CMainGameComponent; class CDisplayEngine; struct CPlayerManager_Retail; class CThingSearchTools;
class CTCDRumble {
public:
    static CThing* __fastcall Create(const C3DVector&, float, float, bool);   // 0x007F1AC0
};   // 0x007F1AC0
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

CScriptThing CGameScriptInterface::CreateRumble(const C3DVector& pos, float strength, float radius, CCharString scriptName) const
{
    CThing* t = CTCDRumble::Create(pos, strength, radius, false);
    if (t && !t->m_dead) {
        t->ScriptName = scriptName;
        return t->GetScriptThing();
    }
    return CScriptThing();
}