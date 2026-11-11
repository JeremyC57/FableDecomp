// CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations) -- product of the DamageMultiplier of every augmentation def named by a positive def index (1.0 with none)
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the element is indexed twice (`if (augmentations[i] > 0)` ... `GetDef(augmentations[i], &def)`, no index local) through STLport's `*(begin() + n)` operator[]; together (neither alone) they give retail's `mov ecx,[eax+esi*4] ... mov eax,ecx ; push eax` with the index in esi and the def in edi
// Leaf: ?GetDamageMultiplierFromAugmentationSet@CTCObjectAugmentations@@SIMABV?$vector@JV?$allocator@J@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007661C0, 158 bytes (+int3 pad to 160), plain `ret` (static __fastcall:
//   the vector reference in ecx, the float in st0).
// Identity: Ego_r.pdb ?GetDamageMultiplierFromAugmentationSet@CTCObjectAugmentations@@SIMABV?$vector@JV?$allocator@J@std@@@std@@@Z
//   (static, __fastcall); the catalog's name is right without the class.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (static __fastcall, returns the manager by reference),
//   CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const T>*) const @ 0x0042AF3C,
//   CDefClassBase::GetSubDef<CAugmentationDef>(CDefPointer<const T>*) const @ 0x0042B248, the defs' slot-1 release.
// Shape: the instance twin of the landed GetDamageMultiplier 0x00766140 over def indices: `for (unsigned i = 0; i <
//   size(); ++i)` (size re-read each pass, `jb`), `if (augmentations[i] > 0)` (`jle`); GetDef's arguments are pushed before the
//   Get() call yields the manager (`mov ecx,eax`), each def-pointer local zeroed after its address is pushed; the thing
//   def is read once into edi (the same-TU GetDef/GetSubDef stand-ins write only their out slot, so the address does
//   not escape); the augmentation def's release has no null test (it was just dereferenced), the thing def's keeps it.
// Layout: CAugmentationDef DamageMultiplier +0x34 (as the landed 0x00766140); def reference count +4, release slot 1.
// Name fix (agentNF10 2026-11-11): moved off the local CDefClassBase / CDefPointer / CThingBaseDef / CDefinitionManager / vector
// stand-ins onto the shared headers (engine/CTCObjectAugmentations.h, CGameDefinitionManager.h, CThingBaseDef.h, CAugmentationDef.h,
// the STLport vector). GetDef / GetSubDef come from the headers' noinline template bodies, so 0x0042AF3C binds
// ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z (was the non-template
// ?GetDef@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z) and 0x0042B248 binds GetSubDef<CAugmentationDef>
// (was ?GetSubDef@CThingBaseDef@@QBE_NPAV?$CDefPointer@$$CBVCAugmentationDef@@@@@Z).
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
#include "fable_string.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAugmentationDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCObjectAugmentations.h"

float __fastcall CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations)
{
    float multiplier = 1.0f;
    for (unsigned int i = 0; i < augmentations.size(); ++i) {
        if (augmentations[i] > 0) {
            CDefPointer<const CThingBaseDef> def;
            CGameDefinitionManager::Get().GetDef(augmentations[i], &def);
            CDefPointer<const CAugmentationDef> augmentation;
            def->GetSubDef(&augmentation);
            multiplier *= augmentation->DamageMultiplier;
        }
    }
    return multiplier;
}
