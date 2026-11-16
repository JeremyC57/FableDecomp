# 2026-11-16 landing gl -- naming-blocked rows, part 1 (five rows + one respelling)

Owner: "do those 13 blocked on naming now." Thirteen rows the w24 triage found byte-matched but held for want of a proven name.
This landing takes the six whose names have two-source evidence (or follow an owner convention); the remaining eight are in
`STATUS.tsv` (all landed in landing gm). All rows RELOCATION_MATCH or MATCH with their behaviour tests passing, `LANDVERIFY_STRICT_HEADERS=1`, repo headers
(`logs/vland_land.log`, `logs/vland_recheck_006afd40.log`).

| row | function | name evidence |
|---|---|---|
| 0x00686D90 | CTCBase::PeekThingSearchTools | Ego_r.pdb 0x006B2180 / FableWin.pdb 0x021915D0, right after GetThingSearchTools in both, as retail 0x00686D90 follows 0x00686D80; jumps to the landed const twin CThingManager::PeekThingSearchTools 0x0051E480 |
| 0x00C28380 | CHandleBasedMemoryPool_012b4098::GetPointerFromHandle | Ego_r.pdb 0x00B6D5E0 member, right before GetSizeFromHandle (retail 0x00C28380 before the landed 0x00C283A0); libpk-copy class spelling per the owner decision of 2026-10-31; the landed caller 0x00C26090 already binds this symbol |
| 0x007D97E0 | std::iter_swap<KnickKnack*,KnickKnack*> | FableWin.pdb 0x027565F0 instance; KnickKnack is Ego_r.pdb's 16-byte {C3DVector Pos; CThingObject* Thing}; the CTCTavernGameSpotTheAddition TU (landed random_shuffle<KnickKnack*> 0x007D9B40). iter_swap v swap between the two twins is INFERRED from the sibling TU's order (0x00806220 iter_swap before 0x008068B0 swap) |
| 0x007D9A00 | std::swap<KnickKnack> | FableWin.pdb 0x027568A0 instance; as 0x007D97E0 |
| 0x009AA440 | std::pair<const CCharString,CBankInfo_009aa210>::~pair | byte twin of the landed 0x009A8FE0 in the bank module's second TU, whose value type is the landed CBankInfo_009aa210 (0x009AA210); DESCRIPTIVE class suffix, as that row |

Respelled: the landed 0x006AFD40 (was CTCBase::PeekThingSearchTools, a label shared by three addresses) is
`?GetThingSearchTools@CTCPhysicsBase@@IBEABVCThingSearchTools@@XZ` -- Ego_r.pdb 0x006EC500 / FableWin.pdb 0x0246E010, donor order,
and its callers 0x00725B70 / 0x00726BE0 / 0x0075AA80 / 0x008E57D0 are CTCPhysics* components (0x00725B70 already binds this spelling).
Files renamed to `CTCPhysicsBase_GetThingSearchTools_006afd40.*`, ported from its local model to engine/CTCPhysicsBase.h (new opt-in
FABLE_RETAIL_CTCPHYSICSBASE_GETTHINGSEARCHTOOLS) + engine/CWorld.h; regraded RELOCATION_MATCH + PASS (strict headers).

Catalog labels corrected at landing: 0x007D97E0 / 0x007D9A00 (were CNearestBuildingWithProperty::CNearestBuildingWithProperty, a
24-byte class that does not fit the 16-byte record), 0x009AA440 (was the first copy's CBankFileManager::CBankInfo spelling),
0x00C28380 (was the engine class without the libpk suffix).

Headers (`hdr_notes/README.md`): engine/CTCPhysicsBase.h only. 17 later asserts pinned AT(old line) (pincheck 0 mismatches).
Whole-tree equivalence check (exact): every landed row identical input except 0x006AFD40 itself and the `__FILE__`-path-only rows
0x00CBFB7D and 0x0047042A (the CArray.h ASSERT arm records the tree path; the two trees' paths differ) --
`logs/equivalence_changed_input_rows.tsv`.

Names (`NAME_FIXES.tsv`, queued as `land1116gl`): three landed callers bind wrong spellings -- 0x00726BE0 and 0x008E57D0 to
0x006AFD40, 0x00C26240 (`?Compute@SubObjectAt360@@QAEHH@Z`, an invented placeholder) to 0x00C28380.
