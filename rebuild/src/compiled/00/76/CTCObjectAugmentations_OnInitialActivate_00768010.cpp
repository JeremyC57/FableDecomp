// CTCObjectAugmentations::OnInitialActivate() -- on a fresh (not save-loaded) object, put each of the def's initial augmentations into a free slot; then attach the augmentation particles and set up the sounds
// Leaf: ?OnInitialActivate@CTCObjectAugmentations@@UAEXXZ
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- STLport begin()/end() forms for size() and operator[] plus the initial-augmentation element indexed twice (no index local), as 0x007661C0; together they give retail's loop-end `mov ebp,[eax+40h] ; lea edx,[eax+40h] ; mov eax,[edx+4] ; sub eax,ebp`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00768010, 146 bytes, ends in a tail `jmp`.
// Identity: CTCObjectAugmentations vtable 0x0126A3A4 slot 6 (rtti-vtables.tsv); Ego_r.pdb
//   ?OnInitialActivate@CTCObjectAugmentations@@UAEXXZ. The catalog's "CTCObjectAugmentations_InitializeDrunkenness" is
//   an autoname.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0, CDefinitionManager::GetDef<CThingBaseDef>(long,
//   CDefPointer<const T>*) const @ 0x0042AF3C (straight into the free slot), AttachAugmentationParticles() @ 0x007677E0,
//   SetupAugmentationSounds() @ 0x00766C00 (tail jump).
// Shape: `if (!LoadedFromSaveGame)` guards an index loop over PDef->InitialAugmentations (the def re-read through PDef
//   after every GetDef call, the vector's address CSE'd within a pass); GetFreeSlot() is inlined (the landed 0x00765D50
//   loop: size re-read each pass, the element walked by a pointer started at the first _M_start load, -1 when none),
//   then `slot >= 0` and `InitialAugmentations[i] > 0` (`jl`, `jle`) gate GetDef(InitialAugmentations[i], &Augmentations[slot]) with its arguments pushed
//   before Get() yields the manager.
// Layout: PDB CTCObjectAugmentations with retail 12-byte containers: PDef +0x0C, LoadedFromSaveGame +0x10,
//   Augmentations +0x14; CObjectAugmentationsDef (retail 12-byte map): AugmentationParticles +0x30, EmptySlotName +0x3C,
//   InitialAugmentations (vector<long>) +0x40.
// Name fix (agentNF10 2026-11-11): moved off the local CDefClassBase / CDefPointer / CThingBaseDef / CDefinitionManager / vector /
// CObjectAugmentationsDef / CTCObjectAugmentations stand-ins onto the shared headers (engine/CTCObjectAugmentations.h
// FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_MEMBERS, CObjectAugmentationsDef.h, CGameDefinitionManager.h). GetDef comes from the header's
// noinline template body, so 0x0042AF3C binds ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z
// (was the non-template ?GetDef@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z); the row's own symbol is the PDB
// virtual ?OnInitialActivate@CTCObjectAugmentations@@UAEXXZ (was QAEXXZ); the PDB-less GetInitialAugmentations accessor is gone (the
// def's InitialAugmentations is read in place). GetFreeSlot's body is the landed 0x00765D50 (same retail TU), expanded here as retail does.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_MEMBERS
#include "fable_string.h"
#include "engine/CThingBaseDef.h"
#include "engine/CObjectAugmentationsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCObjectAugmentations.h"

long CTCObjectAugmentations::GetFreeSlot() const
{
    for (unsigned int i = 0; i < Augmentations.size(); ++i) {
        if (Augmentations[i].IsNull())
            return i;
    }
    return -1;
}

void CTCObjectAugmentations::OnInitialActivate()
{
    if (!LoadedFromSaveGame) {
        for (unsigned int i = 0; i < PDef->InitialAugmentations.size(); ++i) {
            long slot = GetFreeSlot();
            if (slot >= 0) {
                if (PDef->InitialAugmentations[i] > 0)
                    CGameDefinitionManager::Get().GetDef(PDef->InitialAugmentations[i], &Augmentations[slot]);
            }
        }
    }
    AttachAugmentationParticles();
    SetupAugmentationSounds();
}
