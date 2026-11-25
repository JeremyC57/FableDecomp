// CGameScriptInterface::CreateExplosion(const CCharString& defName, const C3DVector& pos, const C3DVector* dir, CCharString scriptName) const -- create the named explosion definition at a position (optionally aimed), give it the script name and return it as a script thing (the null script thing when the definition is unknown or nothing live was made)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089FC80, length 147 bytes.
// Callees: CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
//          the explosion factories (__fastcall) @ 0x0080C500 (with a direction) and @ 0x0080C460, CCharString::operator= @ 0x0099EFB0,
//          CThing::GetScriptThing @ 0x004C7E50, CBaseClass::CBaseClass @ 0x0099A2D0, CCharString::~CCharString @ 0x0099EAE0 (the by-value name).
// Shape: CScriptThing returned by hidden pointer; the null script thing is built inline (CBase ctor out of line, vtable, two zeroes from the shared zero register).
// Leaf: CreateExplosion
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
class CCharString { public:                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
#include "engine/C3DVector.h"
struct SCountBlock;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
#include "engine/CCountedPointer.h"
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
    CScriptThing GetScriptThing() const;                 // 0x004C7E50 (manifest "GFPredicateAnd<...>" is propagated)
};
// NF132 (q6233): CGameDefinitionManager is the shared engine/CGameDefinitionManager.h class (: CDefinitionManager); 0x009AD410 binds
//   ?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z (Ego_r.pdb 0x00B05360 + FableWin.pdb 0x03062820, const), was the
//   local CDefinitionManager_Retail stand-in.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "engine/CGameDefinitionManager.h"
class CWorld; class CMainGameComponent; class CDisplayEngine; struct CPlayerManager_Retail; class CThingSearchTools;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x0080C500 ?Create@CTCDExplosion@@SIPAVCThing@@JABVC3DVector@@0@Z (was ?CreateExplosionThing@@YIPAVCThing@@JABVC3DVector@@0@Z)
//   0x0080C460 ?Create@CTCDExplosion@@SIPAVCThing@@JABVC3DVector@@@Z (was ?CreateExplosionThing@@YIPAVCThing@@JABVC3DVector@@@Z)
class CTCDExplosion { public: static CThing* __fastcall Create(long, C3DVector const&, C3DVector const&); static CThing* __fastcall Create(long, C3DVector const&); };
CScriptThing CGameScriptInterface::CreateExplosion(const CCharString& defName, const C3DVector& pos, const C3DVector* dir, CCharString scriptName) const
{
    long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName);
    if (defIndex > 0) {
        CThing* t = dir ? CTCDExplosion::Create(defIndex, pos, *dir) : CTCDExplosion::Create(defIndex, pos);
        if (t && !t->m_dead) {
            t->ScriptName = scriptName;
            return t->GetScriptThing();
        }
    }
    return CScriptThing();
}