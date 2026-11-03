# Chunk 735 notes (w22/chunk735)

## Result
- Matched (RELOCATION_MATCH + test PASS, vland on the full header mirror, payload.json):
  - 0x0047F58B `CTCInGameMenu::AddRegionAndTimeInfo` (/GS)
  - 0x00597228 `NUISystem::CFrontEndManager::AddRegionAndTimeInfo` (/GS, byte twin)
  - 0x005729D8 `CTCInventoryExperience::AddStatToList`
  - 0x00573792 `CTCInventoryExperience::AddStatToListPC`
- Parked (differ/):
  - 0x00570332 `AddAbilityToList`, DIFFER(3918v3918) dl=175, 31 attempts
  - 0x00571280 `AddAbilityToListPC`, DIFFER(3962v3962) dl=190, 11 attempts
  - 0x00577AC0 `OpenInventoryInternalPC` (vfunc_87), DIFFER(2157v2157) dl=11, 25 attempts
- Siblings: SIBLINGS.tsv. All 16 landed rows that reach the changed headers are still RELOCATION_MATCH PASS on the final overlay. 0x00B3EFA0 needs its revised source (sib/00b3efa0_revised.cpp, the `ReadSLONG()` spelling).

## Overlay (hdr/)
Every changed repo header has default=same in hdrcheck.txt (hdrcheck2), with one FABLE_RETAIL_<CLASS> branch per class.
The base/ snapshots still equal the repo after landing round e (d0f753026).

Changed this session:
- CRGBFloatColour.h: FABLE_RETAIL_CRGBFLOATCOLOUR adds a __forceinline GetRGBColour().
- CTCHeroExperience.h: adds the inline GetExperiencePointsAvailableToSpend() (PDB QBE).
- CTCInventoryExperience.h:
  - TutorialSoundHandles, Animators and AnimIt are now typed.
  - Adds ConstructCategoryStrengthPC, GetOriginalExperience and SaveExperienceValuesForCountDown.
  - pYesNo is now `NUISystem::CYesNo*`.
- CTCInventoryStats.h: FABLE_RETAIL_CTCINVENTORYSTATS adds StatGetInterfaceType (TCI_INVENTORY_STATS) and CreateExperienceSliders.
- NInventory_CTCInventoryBase.h: FABLE_RETAIL_CTCINVENTORYBASE adds `virtual OpenInventory()` and types Components as `vector<CComponent*>`.
- CPlayerGui.h: the retail branch adds GetPHeroInfoScreensManager() const.
- stlport/fable_containers.h: FABLE_RETAIL_STLP_VECTOR_ASSIGN declares the out-of-class vector::operator=.

New headers:
- CSliderAnimator.h
- NHeroInformationScreens_CManager.h (#error gate)
- NUISystem_CYesNo.h (#error gate)
- NUISystem_CUIScreenUnitConverter.h

Earlier in this chunk (see the hdrcheck.txt list):
- CDataInputStream, CPersistContext(+ReadSection), CTCInGameMenu, CDiskFileWin32, CFileDataInputStream, CFrontEndManager, NUISystem_CManager (CreateTickSlider), NUISystem_CDeletion, NUISystem_CClickable (AddParam overloads), CText, CMovie, CTCInventoryAbilities, CTCHeroStats, fable_game_text_bank.h.
- NUISystem_CActionParamSLONG.h and NUISystem_CComponentContainer.h (new).

## Levers that mattered
- `#pragma optimize("s", on)` alone gives the biased frame; FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND expands GetTC's Find.
- Same-TU callee bodies:
  - GetExperienceStatColour above AddStatToList removes a `mov ecx,esi`.
  - GetOriginalExperience above OpenInventoryInternalPC is auto-inlined. That gives the register round trip retail has for the strength, skill and will originals.
- AddAbilityToList: `GetExperienceAbilityColour(ability).GetRGBColour()` in the argument list cut dl from 515 to 157. The named blank CWideString and the late `bool showMorality` also helped.
- OpenInventoryInternalPC:
  - `CList* screen` (push_back binds a converted temporary).
  - The out-of-line vector operator=.
  - A named C2DVector position.
  - `UIActionParams.push_back` (0x0042B646 inserts before the sentinel).

## Open residue
- Ability rows: retail evaluates GetAbilityLevel before GetAbilityMaximumLevel in the first compare (spill to [ebp+54h], reload, `cmp ecx,eax`). VC7.1 picks Max first for every spelling tried (13 forms). The frame-slot packing difference follows from that.
- OpenInventoryInternalPC:
  - The int sum of the four Points members comes out in a VC-internal order (source order and parentheses ignored).
  - One `lea` is scheduled one instruction early.
- 0x0066E799 is SelectScreenOfType by PDB name and signature only (one source): LABELS lists it as a candidate.
