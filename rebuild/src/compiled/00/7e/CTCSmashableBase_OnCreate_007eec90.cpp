// CTCSmashableBase::OnCreate() -- clear the hit frame and the smashed flag, fetch the parent thing's definition and, when found, its smashable sub-def into Definition
// Leaf: ?OnCreate@CTCSmashableBase@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EEC90 (CTCSmashableBase vtable slot 4; Ego_r.pdb ?OnCreate@CTCSmashableBase@@MAEXXZ).
// Callees: CTCBase::GetDefinitionManager() 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer*) 0x0042AF3C,
// CDefClassBase::GetSubDef<CSmashableDef>(CDefPointer*), the def's Release.
// Layout: engine/CTCSmashableBase.h (Ego_r.pdb): LastWorldFrameHit +0x0c, Smashed +0x10, Definition +0x14.
// Shape: the GetDef result is tested BEFORE the def object is read into edi (`test al,al ; mov edi,[esp+8] ; je`): the bool is a named
// local; the def's release follows the conditional sub-def lookup.
// Name fix (agentNF8 2026-11-11): moved off the local CDefinitionManager / CDefClassBase / CTCSmashableBase stand-ins (same-TU stand-in
// GetDef / GetSubDef bodies) onto the shared headers: GetDefinitionManager / GetDef<CThingBaseDef> bind the PDB spellings and the
// sub-def lookup binds ??$GetSubDef@VCSmashableDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSmashableDef@@@@@Z; the row's own
// symbol is the PDB protected virtual (MAE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCSmashableBase.h"
#include "engine/CGameDefinitionManager.h"

void CTCSmashableBase::OnCreate()
{
    LastWorldFrameHit = 0;
    Smashed = false;
    CDefPointer<const CThingBaseDef> def;
    bool found = GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &def);
    if (found)
        def->GetSubDef(&Definition);
}
