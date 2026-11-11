# agentNF8 header overlay

Seven headers changed, each by `#ifdef` opt-in additions only. Without the new macros, every header is byte-identical to the repo copy (checked with `diff`: added lines only).
Grade every row in `../rows/` with `FABLE_HDR_OVERLAY=$S/w22/agentNF8/hdr`. No landed row defines any of the new macros.

| Header | New opt-in | What it adds | Evidence | Rows |
|---|---|---|---|---|
| `engine/CTCShop.h` | `FABLE_RETAIL_CTCSHOP_ATTITUDEPRICE` | `GetAttitudeBuyPriceMultiplier() const` 0x007735D0 and `GetAttitudeSellPriceMultiplier() const` 0x00773790 | Ego_r.pdb + FableWin.pdb `?..@CTCShop@@QBEMXZ`; catalog row 0x00773790 | 007742C0, 007743B0 |
| `engine/CTCHeroStats.h` | `FABLE_RETAIL_CTCHEROSTATS_DRUNKENNESS` | `IsDrunk() const` 0x0057A874 and `GetMaxDrunkenness() const` 0x0057A89D | Ego_r.pdb + FableWin.pdb; Ego_r donor order | 0057A874, 0057A89D |
| `engine/CTCHeroStats.h` | `FABLE_RETAIL_CTCHEROSTATS_ONHEALTHMODIFIED` | `OnHealthModified()` 0x0057A753 | Ego_r.pdb + FableWin.pdb; donor order; its caller InternalModifyHealth calls TC 4 | 0057A753 |
| `engine/CTCInventoryAbilities.h` | `FABLE_RETAIL_CTCINVENTORYABILITIES_INITIALISE` | `InitialiseExpressions()` 0x005F3E3E, `InitialiseAbilities()` 0x005EEDE2, `GetExpressionPDef(long) const` 0x005EE207 | Ego_r.pdb + FableWin.pdb | 005F3E3E, 005EE207 |
| `engine/CThingCreatureBase.h` | `FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY` | The appearance-animation queries (see below) | Ego_r.pdb spellings; donor order 0x0053D8E0.. = retail 0x00662A00.. | 00662B10, 00662C10, 00662C70, 00662FA0, 00663260, 006632A0 |
| `engine/CAnimationSet.h` | `FABLE_RETAIL_CANIMATIONSET_QUERIES` | `GetAnimsWithNameStartingWith` 0x005DCB00 and `GetRandomLoadedAnim` 0x005DCD10, plus the `stlport/fable_containers.h` include they need, inside the same `#ifdef` | Ego_r.pdb | 00662C10, 00662FA0 |
| `engine/CTCWeapon.h` | `FABLE_RETAIL_CTCWEAPON_ONCREATE` (nested in `_MEMBERS`) | Types WeaponDef as `CDefPointer<const CWeaponDef>` (same 4 bytes at +0x0c, and includes `engine/CDefPointer.h`); declares the private virtual `OnCreate()` 0x005DA6E0 | Ego_r.pdb `?OnCreate@CTCWeapon@@EAEXXZ` and member type | 005DA6E0 |
| `engine/CThingSwitch.h` | `FABLE_RETAIL_CTHINGSWITCH_PEEKDEFINITION` | protected `PeekDefinition() const` 0x0083C1E0, returning `CDefPointer<const CThingSwitchDef>` by value | Ego_r.pdb `?PeekDefinition@CThingSwitch@@IBE?AV?$CDefPointer@$$CBVCThingSwitchDef@@@@XZ` | 0083C1E0 |

The `ANIMQUERY` declarations, all public and out of line in retail:
- `GetAnimFromName(name, filter, index) const` 0x00662B10
- `HasAnim` 0x00662C70
- `GetRandomLoadedAnimFromName(name, bool) const` 0x00662FA0
- `GetAnimsWithNameStartingWith` 0x00662C10. It needs `std::vector`, so the row includes `stlport/fable_containers.h` first.
- `PreloadAnims` 0x00663140
- `PreloadAnimation(const CCharString&)` 0x006631C0
- `GetCombatAnimationSet() const` 0x00663260
- `IsCombatAnimationValid(const CCharString&) const` 0x006632A0
- `PreloadBasicAnimations` 0x00663610
