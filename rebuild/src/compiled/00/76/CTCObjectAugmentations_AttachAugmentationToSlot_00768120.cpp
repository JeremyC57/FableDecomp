// CTCObjectAugmentations::AttachAugmentationToSlot(long defIndex, long slot) -- fetch the thing def for the global index; when it carries an augmentation sub-def, store it in the slot; then re-attach the augmentation particles and sounds
// Leaf: ?AttachAugmentationToSlot@CTCObjectAugmentations@@QAEXJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00768120, 166 bytes (`ret 8`; PDB ?AttachAugmentationToSlot@CTCObjectAugmentations@@QAEXJJ@Z).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CGameDefinitionManager::GetDef<T> @ 0x0042AF3C, CDefClassBase::
//   GetSubDef<CAugmentationDef> @ 0x0042B248, AttachAugmentationParticles() @ 0x007677E0, SetupAugmentationSounds() @ 0x00766C00
//   (manifest "CTCObjectAugmentations_UpdateParticleMorphs"; the body looks sounds up through CDefStringTable::GetString; both
//   callees private in Ego_r/FableWin, ?...@CTCObjectAugmentations@@AAEXXZ, as 0x768210 binds them), the
//   def release through vtable slot 1.
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
// Name fix (agentNF10 2026-11-11): moved off the local CDefPointer / CDefHolder / CDefClassBase / CGameDefinitionManager / CTCBase /
// CTCObjectAugmentations stand-ins (and their same-TU GetDef / GetSubDef stand-in bodies) onto the shared headers
// (engine/CTCObjectAugmentations.h FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_MEMBERS, CTCBase.h, CGameDefinitionManager.h, CAugmentationDef.h).
// 0x0042AF3C now binds ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z (was
// ??$GetDef@$$CBVCThingBaseDef@@@CGameDefinitionManager@@QBE_NJAAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z) and 0x0042B248 binds
// ??$GetSubDef@VCAugmentationDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCAugmentationDef@@@@@Z (was a by-reference spelling).
// The slot assignment is CDefPointer's copy assignment (the held def is copied, the slot's old def released).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CAUGMENTATIONDEF
#define FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_MEMBERS
#include "fable_string.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAugmentationDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCObjectAugmentations.h"

void CTCObjectAugmentations::AttachAugmentationToSlot(long augmentation_def_index, long slot)
{
    CDefPointer<const CThingBaseDef> def;
    GetDefinitionManager().GetDef(augmentation_def_index, &def);
    CDefPointer<const CAugmentationDef> augmentation;
    if (def->GetSubDef(&augmentation))
        Augmentations[slot] = def;
    AttachAugmentationParticles();
    SetupAugmentationSounds();
}
