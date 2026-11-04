// NScript::CQS_ScytheInfoScript::CScytheMarker::Main() -- the scythe marker entity script: spawn the rival hero Scythe (CREATURE_RIVAL_HERO_SCYTHE) at the marker with the ScytheNearOracle script, then remove the marker
// Leaf: ?Main@CScytheMarker@CQS_ScytheInfoScript@NScript@@EAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E29F50, length 195 bytes (`ret`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 and ~CCharString @ 0x0099EAE0 (the two names),
// CScriptThing slot 6 GetPos (+0x18), CGameScriptInterface slots 91 CreateCreature(const CCharString&, const C3DVector&,
// const CCharString&, bool) (CScriptThing by value) and 108 RemoveThing(const CScriptThing&, bool, bool), operator delete
// @ 0x00BFE9BC and CBaseClass::~CBaseClass @ 0x0099A2E0 (the inlined ~CScriptThing of the created creature).
// Layout = CEntityScriptBase {vptr, Game +4, CScriptThing Me +8} (+ ParentClass +0x14, PMasterData +0x18; PDB 28 bytes).
// Shape: the landed CSP_NymphTrigger::Main 0x00DED380 spawn (implicit CCharString conversions built before every other
// argument, the result a named local) without the wait loop; in this module ~CScriptThing is inline -- the CScriptThing
// vtable store (0x01238C8C), the PImp counted-pointer release (`--count; if (count == 0)` re-reading the block, the
// destroy member pointer on the item, delete, both words cleared) and the out-of-line CBaseClass destructor -- as in
// the landed CScriptThingFunctor_00e241b0 destructor.
// Naming wave 35: identity reg 00e29b69 "ScytheMarker" in 00e29b20; Alloc 00e29e80; vtable 012e2380 slot 1; PDB ?Main@CScytheMarker@CQS_ScytheInfoScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CScytheMarker::Main (wave3/labels_alloc.tsv); PDB ?Main@CScytheMarker@CQS_ScytheInfoScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CScytheMarker@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own)); same bytes.
#include <stddef.h>
void __cdecl operator delete(void* block);          // 0x00BFE9BC

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#include "engine/C3DVector.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CCountedItem { };
struct SCountBlock {
    long m_references;                               // +0x00
    void (CCountedItem::*m_pfnDestroy)();            // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                           // +0x08
};
class CGameScriptThing;
template <class T> class CCountedPointer { public:             // CCountedPointer<CGameScriptThing>
    CGameScriptThing* Data;                          // +0x00
    SCountBlock*      Info;                          // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
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
inline CScriptThing::~CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQS_ScytheInfoScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CScytheMarker;
};
}
using NScript::CQS_ScytheInfoScript;
namespace NScript {
class CQS_ScytheInfoScript::CScytheMarker {
public:
        CGameScriptInterface* Game;                      // +0x04
    CScriptThing          Me;                        // +0x08
    void*                 ParentClass;               // +0x14
    void*                 PMasterData;               // +0x18
private:
    virtual void Main();                          // PDB ?Main@CScytheMarker@CQS_ScytheInfoScript@NScript@@EAEXXZ
};
}

void NScript::CQS_ScytheInfoScript::CScytheMarker::Main()
{
    CScriptThing scythe = Game->CreateCreature("CREATURE_RIVAL_HERO_SCYTHE", Me.GetPos(), "ScytheNearOracle", false);
    Game->RemoveThing(Me, false, true);
}