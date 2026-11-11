// CThingHolySite::SerialiseGameState(CPersistContext& context) -- base CThingPhysical serialisation; unless the context is saving, re-fetches the def by index and re-reads PlayerStart from it
// Leaf: ?SerialiseGameState@CThingHolySite@@UAE_NAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BB40, length 90 bytes (CThingHolySite vtable 0x012452DC slot 3).
// Identity: Ego_r.pdb / FableWin.pdb ?SerialiseGameState@CThingHolySite@@UAE_NAAVCPersistContext@@@Z (the catalog's "vfunc_3").
// Callees: CThingPhysical::SerialiseGameState 0x006A5AF0 (called by name), CThing::PeekDefinitionManager 0x004C7990,
//   CDefinitionManager::GetDef<CThingHolySiteDef> 0x0083BD10.
// Shape: the context pointer is kept in edi for the inline IsSaving (Mode +0x18 against 1 and 3), then edi is reused for &Def (+0xD0);
//   PlayerStart (+0xD4) is written from the def's byte (`test al,al ; setne dl`).
// Name fix (agentNF9 2026-11-11): moved off the local CThingPhysical / CThingHolySite / CPersistContext / CDefinitionManager models onto
//   the shared headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, 0x0083BD10 the PDB
//   GetDef<CThingHolySiteDef> (was ?GetDef@CDefinitionManager@@QAE_NJAAUCDefPointerHolySiteDef@@@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGHOLYSITE_DEFINITION
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGPHYSICAL_SERIALISE
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingHolySite.h"
#include "engine/CPersistContext.h"

bool CThingHolySite::SerialiseGameState(CPersistContext& context)
{
    if (CThingPhysical::SerialiseGameState(context)) {
        if (!context.IsSaving()) {
            PeekDefinitionManager().GetDef(DefGlobalIndex, &Def);
            PlayerStart = PeekDefinition().PlayerStart != 0;
        }
        return true;
    }
    return false;
}
