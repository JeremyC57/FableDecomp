# hand4d header overlay
Landed already: NCreatureMode.h NCREATUREMODE_PUSHABLEBYSELFAVOIDANCE (b3).
Open:
- engine/CFilter_Bind4.h (copied from the repo 2026-11-19; no asserts in the file): new opt-in FABLE_RETAIL_CFILTER_BIND4_FORCEINLINE
  (the template call operator FABLE_FORCEINLINE), for row 0x0085B2A0 (batch 4). Default path unchanged.
- engine/CThunderBattleDef.h (NEW header, batch 5, row 0x008DD7E0): CThunderBattleDef : CParentDefClassBase with the Ego_d / Ego_r
  members (LightningDamage +0x44 in retail). The generated header of the same name sits in engine/_quarantine/.
