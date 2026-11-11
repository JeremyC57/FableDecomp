# 2026-11-11 landing dc -- naming agent NF23: the name-fix queue from landings cx-da, 95 landed rows changed, 43 renamed

Source: `naming/2026-11-11_agentNF23/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, HELD_BACK, hdr README).
- The queue had 39 entries: 32 done, 6 partial, 1 rejected.
  - Rejected: 0x00658D5D is `?IsSociableTime@CTCVillage@@QBE_NXZ`, not IsCurfewTime. The Ego_r donor order IsNightTime /
    IsSociableTime / IsCurfewTime has the same 0x56 spacing as retail 0x00658D07 / D5D / DB3, and this body reads the sociable times.
    Its 5 binders were respelled to IsSociableTime. The real IsCurfewTime, 0x00658DB3, is queued as land1011dc.
  - Partial: GetPHitBy, ~CBaseIntelligentPointer, SetPItem, GPreviousSelected and push_back. The binders still go through stand-ins,
    or including the shared header clashes with the row's local std::pair.
- 6 of NF23's 101 rows were held back (HELD_BACK.txt): 0x0056D440, 0x00776AA0, 0x009C1DC0, 0x00B3E6E0, 0x00B3E750, 0x00B3E8D0. Their
  `_M_erase` retype added new local models of CQuestManager, CShopItem, CDisplayModeDesc and CEngineResourceManager, which have
  shared headers. These are the same classes as FAM07's held-back rows; both sets wait for a port onto the shared headers.

**Verification**
- vland --recheck-landed in two parts: all 95 RELOCATION_MATCH, with no HARD finding.
- Running every test exe found its catalog PassPattern in 95 of 95. vland's own FAIL column marks the custom-PassPattern rows.
- Bindings were re-derived. bindings_multi_check is at its NullCoord baseline.
- NF23 copied its rows from 2e12a6504, and none changed since.
- The new header opt-in CTCPHYSICSSTANDARD_FLUSHONGROUND is a superset of HEAD.

**Changes**
- 0x00749880 is now CTCNoise::SetCreator.
- 0x00723A10 is now CTCPhysicsStandard::SetRHSetAsFlushOnGround, and its callee 0x0063AB10 binds CWorldMap::GetGroundFaceNormalAt.
- STLport `_M_erase` rows:
  - Retyped over FAM07's proven container types.
  - The FreeTree stand-ins were rebuilt as `_M_erase`.
  - QAE became AAE, because `_M_erase` is private.
  - Their clear / ~_Rb_tree / erase members were renamed with them.
- The ~vector<...Def> destructors.
- `_List_base<...>::clear` for CList* and CMiniMapDisplay::CMarkerInfo.
- ~CBaseIntelligentPointer (16 binders) and SetPItem (19 binders).

**Tooling**

landcf/batch_rerow.py (scratch) now builds file names without template arguments and caps them at 80 characters. A full
`std::_Rb_tree<...>` module name overflowed the path limit.

Labels: ghidra_out/labels_landing_2026-11-11dc.tsv.
