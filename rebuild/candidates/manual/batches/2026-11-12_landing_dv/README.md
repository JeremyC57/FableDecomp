# 2026-11-12 landing dv -- naming agent FAM12: 145 engine `_unattributed::` rows named, queue dr and FAM11's gaps

Source: `naming/2026-11-12_agentFAM12/` (STATUS, APPLIED, REJECTED, RENAMES, LEFT, NOTES, LESSONS, ROWS, QUEUE, BINDER_FIXES,
CALLEE_FIXES, hdr README).

**Family (718 engine `_unattributed::` rows): 145 renamed, 573 left** (LEFT.tsv has a reason per row; most have no real-typed landed
caller and no vtable slot). Each name has two sources: a landed caller's binding in reloc_bindings.tsv, plus the PDB symbol or the
PDB name set for the same container types.
- TransferBinaryIn: 85 of 87 are `CPersistTraits<E>::TransferBinaryIn`. 16 frame-pointer-less rows keep their landed local stream
  view, because the shared header's read orders the exit paths differently.
- STLport instantiations over stlport/fable_map.h / fable_containers.h: 14 tree `insert_unique`, 13 `map::insert(iterator, v)`,
  13 `vector::_M_clear`, 6 tree `erase(first, last)`, 3 `_M_erase`, 2 `_M_insert`.
- 5 `CPlayer::UninitMode<X>`.
- 4 AssignTarget rows: CTCDiceGame::SetCurrentPlayer, SetHero, SetSeat, RegisterGameMaster.

**Queue dr and FAM11's gaps:**
- 0x00929550 is CTCDiceGame::SetCurrentPlayer.
- 0x009285D0's Leaf is the PDB's protected virtual (MAE).
- 0x00429160 is CCountedPointer<NUISystem::CAction>::ReleaseData; the dtor is the 5-byte jump at 0x00429B72.
- 0x0042ABCA rejected: it IS `~list<CComponent*>`; the one wrong binder, 0x00545B10, is respelled.
- 0x007776A0 is on `map<long, CShopItem>`.
- 0x0053C97C is `list<NUISystem::CList*>::insert`.
- 0x005BAA41 is ported onto engine/CDefPointer.h.
- 0x005BC4B2 / 0x0066E799 are partial: one-instruction bodies shared by several classes.

**Coordinator fix at landing:** RENAMES section 1 addresses had no `0x` prefix (normalised in `RENAMES_applied.tsv`).
**Headers** (opt-ins only; hdrcheck2 default=same): stlport/fable_map.h STLP_TREE_ERASE_BODY, engine/CTCTavernGame.h
REGISTERGAMEMASTER, engine/CAInputProcess.h REMOVEFROMINTERFACE.
**Verification:** all 152 changed rows recheck 1 MATCH and 151 RELOCATION_MATCH, test PASS (`logs/recheck.log`). naming/rebind.py:
removed 42, added 36. bindings_multi_check stays at its baseline, and funclet_includes_check finds 1317 checked, 0 broken.
reserved_check: 0 reserved rows (9 rows cite reserved callers in comments).
**Follow-ups handed to naming agent FAM13:** BINDER_FIXES.tsv (42 callers that bind the renamed rows under other spellings),
CALLEE_FIXES.tsv (31 callee rows still under stand-in names), and about 19 callers that bind the dtor name to 0x00429160.
