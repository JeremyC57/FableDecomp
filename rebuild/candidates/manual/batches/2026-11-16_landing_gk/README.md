# 2026-11-16 landing gk -- six operand-order near misses closed by hand (agent w24A)

Owner: "work on the closest parked rows that arent reserved by other accounts / ais". Agent w24A took ten parked rows whose residue was
the operand order of one multiply or address add (plus one branch target) and rewrote them by hand. Six land here, all RELOCATION_MATCH
with their behaviour tests passing, `LANDVERIFY_STRICT_HEADERS=1`, on the repo headers (`logs/vland_land.log`).

| row | function | what closed it |
|---|---|---|
| 0x0047042A | CSoundMap::TransferBinaryIn | a release-ASSERT dead arm in `CArray<T>::operator[]` (new `FABLE_RETAIL_CARRAY_INDEX_ASSERT`, with `FABLE_RETAIL_ASSERT_DEAD_ARM`) gives retail's `lea ecx,[eax+ecx+4]`; the assert is INFERRED (FableWin.pdb has the out-of-line indexer 0x01D2F450, its body is not readable without FableWin.exe) and its effect on the SIB order was graded, not measured |
| 0x0090C900 | CThingFilter_IsValidGossipee::operator() | retail pairs its two `return false` copies crosswise; the factionless-enemy test and the village mismatch share one return block, written as a `goto` into the village arm's return -- a FITTED structure (no evidence for the original's shape); callee 0x00658D5D is IsSociableTime |
| 0x00959380 | CThingSearchTools::GetAllThingsInRoughArea<const CThingObject, vector<const CThingObject*>, CThingFilter_None> | ported from a local model to the shared rough-area template; SIB order through the FITTED `FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET` (the byte-offset cell spelling the landed siblings 0x006F3570 / 0x007197C0 carry); identity from the landed caller 0x00959800 |
| 0x0095C7C0 | CThingSearchTools::GetAllThingsInRoughArea<CThingObject, vector<CThing*>, CThingFilter_None> | as 0x00959380 |
| 0x00872110 | NCameraModes::CStringAndCageBase::UpdateAutoGoBehind | shared headers; the three ground-plane vectors as whole-vector expressions followed by `.Z = 0.0f`, default shared Dot; callee 0x00A14440 is AccurateNormalise |
| 0x00C6B9D0 | CStateVariableFilter::FilterWithGain | the damping product's operand order follows the other statements' spelling (`m_band += high * f; m_low += m_band * f;`, `in + gain * damping * high`); retail_asm through libpk/FtoL.h (no asm in the row); keeps the local libpk class model its landed siblings use |

Headers (`hdr_notes/README.md`): six files (CArray.h, CThingSearchTools.h, CThingObject.h, NGraphicAppearance_CTCGraphicAppearance.h,
NCameraModes_CStringAndCageBase.h, CTCVillage.h), all behind opt-ins; one assert pinned AT(104) in CThingObject.h (pincheck 0
mismatches). CTCVillage.h's `FABLE_RETAIL_CTCVILLAGE_CURFEW` declaration (no landed user) is corrected in place from IsCurfewTime to
IsSociableTime at 0x00658D5D (IsCurfewTime is the landed 0x00658DB3). Whole-tree equivalence check (exact): 56,383 landed rows
identical input except 0x00CBFB7D (`__FILE__` path only).

Names: five entries queued as `land1116gk`. Audit item: the landed caller 0x0095B3E0 binds a local stand-in spelling
(`?GetAllThingsInRoughArea@CThingSearchTools@@QBEXABVC2DBoxI@@ABUCThingFilter_All@@AAUCThingPtrVector@@@Z`) to 0x0095C7C0; it needs
respelling to the template symbol this landing gives that address. Lander warnings: RELOC_NAME (catalog labels), STANDIN_LEAF (the
propagated catalog label GetPBestThingInRoughArea on the two rough-area rows), RELOC_ALIAS (__CIacos import thunk).

Not matched (best drafts in `pending/2026-11-16_w24/parked_A/`, residues in `STATUS.tsv`): 0x00681E60, 0x006AD700, 0x00B9D730,
0x00BCF040 -- each keeps one product's operand order after 14 to 130 graded variants.
