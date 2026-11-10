# Header overlay used by chunk 1008 (`FABLE_HDR_OVERLAY=$A/hdr`)

Every src/ row in this workspace was graded with `FABLE_HDR_OVERLAY=$A/hdr`. Each file here is a copy of the repo HEAD header
with ADDITIONS ONLY, each behind a new `FABLE_RETAIL_*` opt-in; with none of the new macros defined the headers expand exactly as
HEAD does (checked by diff: no removed or changed lines).

## engine/CTCInventoryAbilities.h
- `FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN` (rows 0x005F0C5F, 0x005F0DC5, 0x005F1320; declared for the unmatched 0x005F11A9,
  0x005F64DD, 0x005F6793, 0x005F6BD6, 0x005EE49A): forward `namespace NUISystem { class CComponent; }` after the includes, and the
  public quick-slot members in the PDB spellings: `AssignAbility(long, unsigned long)`, `AssignAbilityPC(long, unsigned long,
  NUISystem::CComponent*)`, `UnAssignAbility(unsigned long)`, `UnAssignAbilityPC(unsigned long, NUISystem::CComponent*)`,
  `CycleAssignedAbilities(bool)`, `CycleAssignedExpressions(bool)`, `SetAbilityAsAvailable(EHeroAbility, bool, bool)`,
  `IsAbleToUseAbility(EHeroAbility, bool) const`. Declarations only, no layout change.
- `FABLE_RETAIL_CTCINVENTORYABILITIES_ITEMLISTS` (rows 0x005F077F, 0x005F1526, and 0x005F3F7B / 0x005F45B5 which call them):
  includes `engine/CCountedPointer.h`, forwards `NUISystem::CComponent` / `CList`, declares public
  `ConstructPCItemLists(std::vector<std::pair<long,long> >&, NUISystem::CList*, NUISystem::CComponent*, CCountedPointer<NUISystem::CComponent>&)`
  (0x005F1526) and private `ContructItemLists(...)` (0x005F077F; FableWin.pdb's own spelling, AAE = private). No layout change.

## engine/CTCHeroStats.h
- `FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX` (row 0x005EE77C): declares `long GetMoralityMax() const;` (Ego_r.pdb
  ?GetMoralityMax@CTCHeroStats@@QBEJXZ, out of line at 0x0057A67E, landed as CTCHeroStats_GetMoralityMax_0057a67e). Placed after
  GetMoralityPercentage in the retail (`FABLE_RETAIL_CTCHEROSTATS`) branch. Declaration only.

Landing: copy both files over rebuild/include/engine/ (the opt-ins are inert for every other TU).
