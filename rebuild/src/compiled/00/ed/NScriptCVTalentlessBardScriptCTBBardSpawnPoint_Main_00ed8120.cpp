// NScript::CV_TalentlessBardScript::CTB_BardSpawnPoint::Main() -- spawn the Bowerstone bard at the spawn point and hand him to the parent quest as stBard
// Leaf: ?Main@CTB_BardSpawnPoint@CV_TalentlessBardScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Identity (naming wave 11): ?Main@CTB_BardSpawnPoint@CV_TalentlessBardScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F34BC slot 1 is stored by the factory 0x00ED8050 registered as "TB_BardSpawnPoint"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED8120, length 240 bytes (entity-script vtable 0x012F34C0 slot 1).
// Callees: CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, ~CBaseClass @ 0x0099A2E0,
// operator delete @ 0x00BFE9BC, CScriptThing::GetPos (virtual slot 6, +0x18) on Me, and
// CGameScriptInterface::CreateCreature (virtual slot 91, +0x16C).
// Layout = Ego_r.pdb CTB_BardSpawnPoint (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
// ParentClass +0x14 (CV_TalentlessBardScript*), PMasterData +0x18. In the parent, stBard sits at PDB +0x68,
// retail +0x54 (the quest script base is 0x14 bytes smaller in retail; see CQ_HangingTreeGoodScript 0x00D74F80).
// Shape: `ParentClass->stBard = Game->CreateCreature(def, Me.GetPos(), name, false);` -- the two literals are named
// locals ("TB_Bard" built first, so it sits above the def name), the returned CScriptThing is a frame temporary whose
// inline destructor (vtable store, counted release, zero stores, ~CBaseClass) runs after the inlined PImp
// assignment of the compiler-generated CScriptThing::operator=.
// Naming wave 35: identity reg 00ed7879 "TB_BardSpawnPoint" in 00ed7830; Alloc 00ed8050; vtable 012f34bc slot 1; PDB ?Main@CTB_BardSpawnPoint@CV_TalentlessBardScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CTB_BardSpawnPoint::Main (wave3/labels_alloc.tsv); PDB ?Main@CTB_BardSpawnPoint@CV_TalentlessBardScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was already this symbol, with a bare `// Leaf:` line and the old file name; this source now defines the PDB member (class model unchanged); same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/C3DVector.h"

#include "engine/CCountedPointer.h"

class CScriptThing;

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CV_TalentlessBardScript {
public:
    unsigned char _base[0x54];                           // quest script base and earlier members
    CScriptThing  stBard;                                // +0x54 (PDB +0x68)
public:
    class CTB_BardSpawnPoint;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_TalentlessBardScript;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CV_TalentlessBardScript::CTB_BardSpawnPoint : public CEntityScriptBase {
public:
    CV_TalentlessBardScript* ParentClass;                // +0x14
    void*                    PMasterData;                // +0x18 CQ_SunnyvaleMasterData*
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CV_TalentlessBardScript::CTB_BardSpawnPoint::Main()
{
    CCharString scriptName("TB_Bard");
    CCharString defName("CREATURE_BOWERSTONE_BARD");
    ParentClass->stBard = Game->CreateCreature(defName, Me.GetPos(), scriptName, false);
}