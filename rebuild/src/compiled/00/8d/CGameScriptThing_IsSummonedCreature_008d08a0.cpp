// CGameScriptThing::IsSummonedCreature() const -- a live thing that is a summoned creature: its summonable-creature component says it was summoned, or it is the GHOSTLY_SWORD def
// Leaf: ?IsSummonedCreature@CGameScriptThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D08A0, 191 bytes, plain `ret`. Vtable 0x012852CC slot 65 (.?AVCGameScriptThing@@);
//   Ego_r.pdb / FableWin.pdb ?IsSummonedCreature@CGameScriptThing@@UBE_NXZ (the CScriptThing slot-65 override).
// Callees (PDB spellings): CBaseIntelligentPointer::GetPItem 0x00A01B50 (CIntelligentPointer<CThing>::Get, read afresh at every use),
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (the forced-inline PeekTC<const CTCSummonableCreature>,
//   TCI_SUMMONABLE_CREATURE 0xA0), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CThing::PeekDefinitionManager
//   0x004C7990, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410.
// Literal: "GHOSTLY_SWORD" 0x0125D2A4.
// Layout: CGameScriptThing PThing +0x0C; CThing Dead bit +0x91, DefGlobalIndex +0x8C (ushort); CTCSummonableCreature HasBeenSummoned +0x10.
// Name fix (agentNF11 2026-11-11): moved off the local PointerView / CThingNative / ThingMethods / SummonableCreature stand-ins onto the
// shared headers (new engine/CTCSummonableCreature.h). 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ
// (was a local GetDefinitionManager), 0x009AD410 the PDB const GetDefGlobalIndexFromName; the row's own symbol is the PDB virtual
// const override (was CGameScriptThingMethods).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTTHING
#define FABLE_RETAIL_CGAMESCRIPTTHING_QUERIES
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCSummonableCreature.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameScriptThing.h"

bool CGameScriptThing::IsSummonedCreature() const
{
    if (PThing.Get() && !PThing.Get()->IsDead()) {
        const CTCSummonableCreature* summonable;
        if (PThing.Get()->PeekTC(&summonable) && summonable->GetHasBeenSummoned())
            return true;
        long ghostly_sword = PThing.Get()->PeekDefinitionManager().GetDefGlobalIndexFromName("GHOSTLY_SWORD");
        if (ghostly_sword > 0 && PThing.Get()->GetDefGlobalIndex() == ghostly_sword)
            return true;
    }
    return false;
}
