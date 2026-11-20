# NF207 header overlay

## engine/CCombatManager.h
- Base: repo working tree `rebuild/include/engine/CCombatManager.h` (copy in `orig/engine/CCombatManager.h`).
- Added (add-only, member-specific macro): `FABLE_RETAIL_CCOMBATMANAGER_ISATTACKERON`, a declaration of the public const
  `bool IsAttackerOn(const CThingCreatureBase& attacker, const CThingCreatureBase& defender) const;`
  (Ego_r.pdb 0x00712570 + FableWin.pdb 0x02412B50 `?IsAttackerOn@CCombatManager@@QBE_NABVCThingCreatureBase@@0@Z`; out of line at
  0x006E9310). Placed after the FABLE_RETAIL_CCOMBATMANAGER_ISATTACKER block. Declaration only: no layout change.
- Default path token-identical (the block is skipped unless the macro is defined; no other line changed: `diff orig/ hdr/`).
- pin_asserts.py: pinned 0 (the one assert is `FABLE_STATIC_ASSERT_AT(103, ...)`, explicit counter; nothing moved).
- Landed includers (`grep -rlE 'engine/CCombatManager\.h' rebuild/src/compiled`): 37 rows (0x00834530, 0x006EA430, 0x006E99B0,
  0x006EA0C0, 0x006ECEA0, 0x006ED9C0, 0x006E94D0, 0x0093E240, 0x0093E560, 0x0093C030, 0x007FC820, 0x004A5A40, 0x004A6E30,
  0x006AABE0, 0x0095C1F0, 0x0095D3A0, 0x00954A20, 0x0095BCF0, 0x009540C0, 0x0095D200, 0x0097B780, 0x00972110, 0x00977770,
  0x00972CC0, 0x00972490, 0x00979F70, 0x00972990, 0x007E5B40, 0x00962B80, 0x0096EC10, 0x0096EDB0, 0x0096E830, 0x0091D850,
  0x007355F0, 0x008BF500, 0x008BF800, 0x008B0D10). None defines the new macro (repo grep: no hit), so the opt-in changes none of them.
- Draft-only opt-ins (FABLE_RETAIL_CCOMBATMANAGER_ISDEFENDERDUEFORATTACK for the 0x006EA050 identity draft) are NOT in this overlay:
  they sit in work/hdr_drafts/engine/CCombatManager.h with the draft row.
- Other agents' overlays (NF205: engine/CQ_EndGameFocalSitesScript.h; NF208: none) do not touch this header.

## stlport/fable_map.h
- Not overlaid in the hand-back. Two draft opt-ins written for the blocked 0x006EDA80 (FABLE_RETAIL_STLP_TREE_DEC_DECL_ONLY,
  FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY) neither cured its pick-any _M_decrement copy; they are kept in
  work/hdr_drafts/stlport/fable_map.h for the record and are used by no handed-back row.
