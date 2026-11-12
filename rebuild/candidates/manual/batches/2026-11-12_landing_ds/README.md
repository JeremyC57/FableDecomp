# 2026-11-12 landing ds -- naming agent FAM11: 115 engine `CUnproven*` rows get their real types, queue do/dp done

Source: `naming/2026-11-12_agentFAM11/` (STATUS, APPLIED, REJECTED, RENAMES, LEFT, NOTES, LESSONS, ROWS, QUEUE, hdr README).

**Family (438 engine rows naming `CUnproven*` types): 115 rows respelled, 79 of 382 placeholder ids proven, 323 rows left.**
- Each id is proven from two sources: a landed caller with real types that binds the exact spelling (reloc_bindings), plus an
  Ego_r member type or a FableWin / Ego_r `_Tmap_traits` / `_Tset_traits` name set for the same element types.
- Respelled: tree ctors/dtors, clear, `_M_erase`, `_M_insert`, `insert_unique`, erase, map ctors/dtors, about 35 iterator `++`/`--`
  and `__distance` rows, `_M_create_node`, find, list `push_front`, `uninitialized_fill_n` / `__copy_backward*` (CQuaternion,
  C3DVertexBlend2, CExpressionSet::CEntry), and `CDefPointer<const CAppearanceModifierDef>`.
- Left (LEFT.tsv, one line per row): 120 keep FAM07's reason; 187 have no real-typed landed caller and no PDB tie; 16 are named one
  by one.

**Queue do / dp (all done):**
- The three DeleteData rows landing do held back are now on shared headers: 0x00419086 (CAEnvironmentInteraction), 0x00AF5CD0
  (CPSCBase), and 0x00718080, which is really `CCountedPointer<NTCScriptedControl::CScriptConversation>` (RTTI vtable 0x01265520;
  new header engine/NTCScriptedControl_CScriptConversation.h).
- 0x008147D0 is CTCBoastingPosition::SetBoastingPodium.
- 0x0063E590 binds ForEachInRoughArea's `const Proc&` spelling.
- NF27's leftovers were not done (stand-in classes, unproven owning headers); they are in NOTES.

**Coordinator fixes at landing:**
- RENAMES section 1 addresses had no `0x` prefix (batch_rerow needs it); normalised in `RENAMES_applied.tsv`.
- **16 respelled tests did not compile under VC7.1** (`TESTS_KEPT_previous.txt`). The agent's mechanical respell left
  `v._M_key._M_payload = N` on what is now `std::pair<const K, V>`, and one test lost its `<utility>` include; its STATUS claimed
  PASS. Their sources all MATCH / RELOCATION_MATCH. Those 16 rows keep their previous test text, which is a standalone model copy
  and still passes (`logs/recheck_16_previous_tests.log`). The agent has been sent the 16 back to respell properly.

**Headers** (opt-ins only): CTCBoastingPosition.h PBOASTINGPODIUM, CThingSearchTools.h FOREACHINROUGHAREA_CONSTPROC, new
NTCScriptedControl_CScriptConversation.h.

**Verification:** all 120 changed rows MATCH / RELOCATION_MATCH. 104 tests PASS as handed back, and 16 PASS with the previous test.
naming/rebind.py: removed 20, added 17. bindings_multi_check stays at its baseline, and funclet_includes_check finds 1317 checked,
0 broken. reserved_check: 0 reserved rows (11 rows cite reserved callers in comments only).
