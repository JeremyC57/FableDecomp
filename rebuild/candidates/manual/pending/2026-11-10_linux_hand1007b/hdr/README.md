# Header overlay (chunk 1007b)

Every grade and probe this pass ran with `FABLE_HDR_OVERLAY=$A/hdr`. The default path of each header is byte-identical to the repo.

## engine/CTCInventory.h (changed copy of rebuild/include/engine/CTCInventory.h)
- New nested opt-in `FABLE_RETAIL_CTCINVENTORY_ASSIGNPC` (inside the existing CTCINVENTORY opt-in, before `OpenInventoryShortcut`):
  - `void SetThingAsQuickAccessItem(long item, long slot, bool giveTutorial);` 0x00584157, `?SetThingAsQuickAccessItem@CTCInventory@@QAEXJJ_N@Z`
  - `void AssignThingPC(long thingID, unsigned long slot, NUISystem::CComponent* container);` 0x00584B86, `?AssignThingPC@CTCInventory@@QAEXJKPAVCComponent@NUISystem@@@Z`
  - Evidence: Ego_r.pdb / FableWin.pdb public symbols (both). Used by differ/00584b86.cpp only.

## engine/CAbilityDef.h (new)
- `class CAbilityDef : public CSubDefClassBase` with the inline `static CCharString __fastcall StatGetTypeName()` ("CAbilityDef")
  and `EHeroAbility Ability;` at +0x28; static asserts offsetof(Ability)==0x28, sizeof==0x2c. Needs FABLE_RETAIL_DEF_CLASS_BASE.
- Evidence: ghidra_out/struct_layouts_egor.tsv CAbilityDef (44 bytes, Ability at 40); retail 0x00584E4C reads `mov ecx,[ecx+28h]`
  off a GetSubDef<CAbilityDef> result. Used by differ/00584b86.cpp only. Neither matched src row needs the overlay.
