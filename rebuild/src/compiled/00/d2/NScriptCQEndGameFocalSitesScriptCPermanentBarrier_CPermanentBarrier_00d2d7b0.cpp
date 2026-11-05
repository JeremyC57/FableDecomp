// NScript::CQ_EndGameFocalSitesScript::CPermanentBarrier::CPermanentBarrier(CQ_EndGameFocalSitesScript& parent, CScriptDataBase* data, const CScriptThing& me) -- construct the permanent-barrier entity script: the parent's quest interface, its thing, parent, master data and two empty thing vectors
// Leaf: ??0CPermanentBarrier@CQ_EndGameFocalSitesScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D2D7B0, length 76 bytes.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90. Vtables 0x012C3224 (the entity-script
// base, as in the landed Create_CVtable_* factories) and 0x012CC48C (this class, no RTTI).
// Identity: the manifest label CProjectileWeapon::CProjectileWeapon is propagated. The factory 0x00D2D730
// allocates 0x34 bytes; of the Ego_r.pdb entity scripts whose retail size is 0x34 only CPermanentBarrier
// (ParentClass CQ_EndGameFocalSitesScript*, PMasterData, two vector<CScriptThing>) has six zero-initialised
// words after PMasterData; its Main uses the "HK_MAGIC_BARRIER" string at 0x012CC4A8.
// Shape: the root's interface pointer before the base vtable (dead root vtable), the thing copy after it,
// ParentClass in the init list (before the derived vtable store), the two vectors' inline constructors,
// PMasterData a body assignment (stored last).
// Naming wave 35: identity reg 00d2af3b "PermanentBarrier" in 00d2aa40; Alloc 00d2d730; vtable 012cc48c; called by the Alloc after operator new; PDB ??0CPermanentBarrier@CQ_EndGameFocalSitesScript@NScript@@; refs(call)=Alloc; was under-qualified CPermanentBarrier::CPermanentBarrier (wave3/labels_alloc.tsv); PDB ??0CPermanentBarrier@CQ_EndGameFocalSitesScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z in Ego_r.pdb / FableWin.pdb.
//   Was `??0CPermanentBarrier@@QAE@PAUCQ_EndGameFocalSitesScript@@PAUCQ_SunnyvaleMasterData@@ABUCScriptThing_Calls@@@Z`: the class is nested in NScript::CQ_EndGameFocalSitesScript with the PDB parameter types; the row's
//   CScriptThing_Calls stand-in is spelled CScriptThing (its copy ctor 0x004ABE90 is ??0CScriptThing@@QAE@ABV0@@Z); same bytes.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

struct CGameScriptInterface_Calls;

struct CQuestScriptBase_Retail {                            // the parent quest script: interface pointer at +0x40
    unsigned char         _pad[0x40];
    CGameScriptInterface_Calls* GSI;                              // +0x40
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameFocalSitesScript : public CQuestScriptBase_Retail {
public:
    class CPermanentBarrier;            // the entity class below (naming wave 35)
};
}
using NScript::CQ_EndGameFocalSitesScript;
using NScript::CQ_SunnyvaleMasterData;
class CScriptDataBase;

struct CEntityScriptRoot {                                  // root: its vtable store is dead
    virtual void v00();
    CGameScriptInterface_Calls* PQuest;                           // +0x04
    CEntityScriptRoot(CGameScriptInterface_Calls* quest) : PQuest(quest) { }
};

class CEntityScriptBase : public CEntityScriptRoot { public:              // vtable 0x012C3224
    virtual void v00();
    CScriptThing Thing;                                     // +0x08
    CEntityScriptBase(CQuestScriptBase_Retail* parent, const CScriptThing& thing)
        : CEntityScriptRoot(parent->GSI), Thing(thing) { }
};

struct CScriptThingVector {                                 // retail std::vector<CScriptThing> (12 bytes)
    CScriptThing* m_pFirst;
    CScriptThing* m_pLast;
    CScriptThing* m_pEnd;
    CScriptThingVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

namespace NScript {
class CQ_EndGameFocalSitesScript::CPermanentBarrier : public CEntityScriptBase {
public:              // vtable 0x012CC48C, 0x34 bytes
    virtual void v00();
    CQ_EndGameFocalSitesScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;                // +0x18
    CScriptThingVector          particle_effectVector;      // +0x1c
    CScriptThingVector          MyMarkerVector;             // +0x28
    CPermanentBarrier(CQ_EndGameFocalSitesScript& parent, CScriptDataBase* data, const CScriptThing& me);
};
}

NScript::CQ_EndGameFocalSitesScript::CPermanentBarrier::CPermanentBarrier(CQ_EndGameFocalSitesScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(&parent, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}