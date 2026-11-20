# 2026-11-20 landing nf128: PRIORITY slice 7, third remainder (5 rows; 0x008E9A30 held)

Records: `naming/2026-11-20_agentNF128/` (selfcheck 0 problems; NF128's APPLIED / RENAMES there are its hand-back as written). 8 entries.
NF128 finished q6394, q6395 and q6454, but all three rest on 0x008E9A30, which is HELD from this landing (below), so they stay open and
went to NF130 with the fix. q6453 / q6455 partial (12 of 18 referencing rows on the claim); q6408, q4802, q4747 not started.

## Held: 0x008E9A30 (advisor decision, owner delegated)
Its object emits a pick-any COMDAT of the inline `CAnimFilterOfGroup::SetCurrentGroupName` (opt-in SETCURRENTGROUPNAME_INLINE, landed here),
while the landed owner 0x0073DFF0 defines that symbol strongly: a link of the two fails LNK2005 (GOTCHAS nf80 / nf86 / nf88). Its 6 RENAMES
lines were removed from this landing; NF130 lands it together with the nf88 scaffolding on 0x0073DFF0, proven by a link test.

## Rows (5): 5 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- 0x0073AAC0 CModeBase::OnActivate QAE -> UAE (`virtual` added to the row's existing local nested CModeBase model); 0x008EA500 QAE -> AAE;
  0x008D6AC0 QAE -> UAE (both PDBs). 0x008621A0 and 0x008D6AC0 / 0x008EA500 bind the nested GetWorld claim (q6453 / q6455).
- 0x008D8CE0 (NF126's blocker) matched with the 0x00665860 define set (FABLE_RETAIL_CTHING_ORIGINAL_VTABLE).
- 0x008EA500 now binds `?GetDefHover{Slow,Medium,Fast}Speed@CThingCreatureBase@@QBEMXZ` (both PDBs) at 0x006620E0 / 0x006620F0 /
  0x00662100. At HEAD it bound the stand-ins `?GetHoverSpeed{Slow,Medium,Fast}@...` (HEAD pairs), different from the self rows' July
  `GetAirResistance` stand-ins: each address stays at 2 spellings (not created). Self-row respell queued.

## Headers (add-only opt-ins, default path token-identical; pin_asserts re-pinned 2 asserts to their existing tags)
NCreatureMode_CModeStrafe.h ONACTIVATE, CAnimFilterOfGroup.h SETCURRENTGROUPNAME_INLINE, CThingCreatureBase.h GETDEFHOVERSPEEDS,
NCreatureMode_CModeSitFloor.h CMODESITBASE_ANIMUPDATE. assert_names_check 0 mismatches.

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0. 1225 rows rechecked: 1225 good / 0 bad. No-new-split gate 0. Table spelling sets:
0x0070B460 8 -> 7, 0x0073A680 6 -> 5, 0x0073A9B0 4 -> 3, 0x0073AAC0 3 -> 2, 0x00861950 / 0x00861C90 2 -> 1; none grew.
