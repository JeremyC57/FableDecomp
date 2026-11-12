# agentNF25b header overlay

Each file is a full copy of the current repo header (rebuild/include/engine/) with `#ifdef` additions only (diff shows `>` lines only).
Copy them over the repo headers at landing; no existing branch changes.

| Header | Opt-in | What it adds | Rows that define it |
|---|---|---|---|
| engine/CEnvironmentThemePriorityStackEntry.h | FABLE_RETAIL_CENVIRONMENTTHEMEPRIORITYSTACKENTRY_STACK_FRIEND | `friend class CEnvironmentThemePriorityStack;` (access only: CalculateTheme reads each entry's BaseWeight +0x00) | 0x008889C0 |
| engine/CTCWeapon.h | FABLE_RETAIL_CTCWEAPON_RESETAIMING | `void ResetAiming();` 0x005DA4E0 (Ego_r + FableWin ?ResetAiming@CTCWeapon@@QAEXXZ) | 0x005DA4E0 (+ its test), 0x008BF500 |
| engine/CCombatManager.h | FABLE_RETAIL_CCOMBATMANAGER_ISATTACKER | `bool IsAttacker(const CThingCreatureBase&) const;` 0x006E92C0 (Ego_r + FableWin) | 0x008BF500 |
| engine/CCarryableDef.h | FABLE_RETAIL_CCARRYABLEDEF_ONKILLFX (nested in FABLE_RETAIL_CCARRYABLEDEF) | types Ego_r `long OnKillFX` +0x34 out of the opaque tail (same layout, sizeof 0x50) | 0x007037D0 |
| engine/CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_ISWORKTIME | public `IsTimeBetween(const std::vector<long>&, long) const` 0x00658620 and `IsWorkTime() const` 0x00658B20 | 0x00658B20 |

Now unused after landing (only 0x008BF500 defined them; it moved to the new opt-ins): FABLE_RETAIL_CCOMBATMANAGER_ISATTACKING
(`IsCreatureAttacking`, in neither PDB) and FABLE_RETAIL_CTCWEAPON_READYPROJECTILE (`ReadyProjectile`, in neither PDB). They can be
deleted from the repo headers once these rows land (I did not delete them: overlay files are additions only).

Grade with `FABLE_HDR_OVERLAY=$S/w22/agentNF25b/hdr`.
