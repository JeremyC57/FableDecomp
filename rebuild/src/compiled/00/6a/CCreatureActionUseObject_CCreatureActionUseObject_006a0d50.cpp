// CCreatureAction_UseObject::CCreatureAction_UseObject(const CCreatureAction_UseObject&) -- copies the action base, counted used-item pointer and trailing state
// Leaf: ??0CCreatureAction_UseObject@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1), @ 0x006A0D50, 247 bytes, ret 4.
// Callees: CCreatureActionBase copy ctor @ 0x0062EBC0; CBaseIntelligentPointer ctor/GetPItem/SetPItem @ 0x00A01B10/0x00A01B50/0x00A01B90.
// Identity: Ego_r.pdb 0x006C56A0 + FableWin.pdb 0x02212AD0 ??0CCreatureAction_UseObject@@QAE@ABV0@@Z.
// NF270 (2026-11-22, slot 1 port): moved off its local models (CCreatureActionBase, CSoundPair, CBaseIntelligentPointer / CIntelligentPointer,
//   the derived class, the two aliased header includes) onto engine/CCreatureAction_UseObject.h (FABLE_RETAIL_CCREATUREACTION_USEOBJECT:
//   PUsedThing +0xA8, PreviousPhysicsComponent +0xB0, CRightHandedSet Dummy +0xB4, C3DVector DummyPos +0xCC, Proximity +0xD8, the four
//   flags +0xDC..+0xDF) on the retail CCreatureActionBase, with the base copy constructor 0x0062EBC0 declared by the add-only opt-in
//   FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR (both PDBs). The emitted vtable now inherits slot 1 as ?Validate@CBaseObject@@UBEXXZ (retail
//   0x0125CEFC slot 1 = 0x0099A400; no PDB has a CCreatureActionBase::Validate) and binds slot 12 as the private
//   ?Clone@CCreatureAction_UseObject@@EBEPAVCCreatureActionBase@@XZ (Ego_r 0x006C5980 / FableWin 0x022129F0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR
#define FABLE_RETAIL_CCREATUREACTION_USEOBJECT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UseObject.h"

CCreatureAction_UseObject::CCreatureAction_UseObject(const CCreatureAction_UseObject& other)
    : CCreatureActionBase(other)
    , PUsedThing(other.PUsedThing)
    , PreviousPhysicsComponent(other.PreviousPhysicsComponent)
    , Dummy(other.Dummy)
    , DummyPos(other.DummyPos)
    , Proximity(other.Proximity)
    , UsingDummy(other.UsingDummy), MovingToDummy(other.MovingToDummy), TurningToDummy(other.TurningToDummy), AllowTurn(other.AllowTurn)
{
}
