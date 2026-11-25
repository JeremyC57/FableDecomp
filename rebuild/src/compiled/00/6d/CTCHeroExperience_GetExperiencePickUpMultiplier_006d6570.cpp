// CTCHeroExperience::GetExperiencePickUpMultiplier() const -- the experience pick-up multiplier: ExperienceMultiplierParam over the def's per-unit
//   combo-multiplier step, plus one, truncated, never below 1
// Leaf: ?GetExperiencePickUpMultiplier@CTCHeroExperience@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D6570, 31 bytes: mov eax,[ecx+54h] ; fld [ecx+30h] ; fdiv [eax+64h] ; fadd qword [01237F00h] (= 1.0) ;
//   call _ftol ; cmp eax,1 ; jge ; mov eax,1 ; ret. Retail's out-of-line (pick-any) copy of the header-inline member.
// Naming (NF446, 2026-11-25): self respelt from the free stand-in ?CTCHeroExperience_GetExperiencePickUpMultiplier@@YIJPBVCTCHeroExperience@@@Z
//   to ?GetExperiencePickUpMultiplier@CTCHeroExperience@@QBEJXZ. Evidence: both PDB name sets (Ego_r 0x00587F00, FableWin 0x01EEE710; the other
//   bound spelling ?GetComboMultiplier@CTCHeroStats@@QBEJXZ is in neither); every caller fetches the component through interface 0x68 =
//   TCI_HERO_EXPERIENCE (0x0077A180 PeekTC<CTCHeroExperience>(0x68), 0x00899C90, 0x008E7040); the body is engine/CTCHeroExperience.h's inline
//   GetExperiencePickupMultiplierForParam(ExperienceMultiplierParam +0x30) with PDef (+0x54) ->ExperienceMultiplierParamPerUnitComboMultiplier (+0x64).
// Form: a pick-any re-row (GOTCHAS nf86 / nf88): the member is the shared header's inline (FABLE_RETAIL_CTCHEROEXPERIENCE_PICKUP_MULTIPLIER, the
//   define set of the landed 0x006D8300, whose object already emits this COMDAT); the scaffolding helper below makes this TU emit it too. The
//   copy is object-equal to 0x006D8300's (comdat_eq OBJECT_EQ, selection included). An out-of-line definition on the HEAD local model gave a
//   second, non-pick-any definition (/FORCE link LNK4006 against 0x006D8300), and HEAD's model added 0.5 where retail adds 1.0 (0x01237F00);
//   RELOCATION_MATCH does not compare constant values, so HEAD still graded.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_STATEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_PICKUP_MULTIPLIER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/EHeroExperienceStatCategory.h"
#include "engine/CThing.h"
#include "engine/CHeroExperienceDef.h"
#include "engine/CTCHeroExperience.h"
namespace {
// Fidelity: scaffolding, not retail: takes the address of the header's inline CTCHeroExperience::GetExperiencePickUpMultiplier so this TU emits
// its pick-any COMDAT (retail's out-of-line copy at 0x006D6570). Internal to this TU.
long (CTCHeroExperience::*ReferenceExperiencePickUpMultiplier())() const
{
    return &CTCHeroExperience::GetExperiencePickUpMultiplier;
}
}
