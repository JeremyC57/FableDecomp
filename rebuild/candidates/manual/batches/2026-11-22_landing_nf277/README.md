# Landing nf277 (Account A, 2026-11-22): 0x00BFE900 as a fold of 0x00BFDE20, and three CLandscapeLayerMesh tests that link their rows (1 row, 3 tests)

Agent NF277 (entry 1 done; entry 2: 0x00BFDE20 / 0x00BFDDB0 tests done, 0x00BFD480 blocked, four not started). Hand-back:
`pending/2026-11-22_nf277_handback/`. No header, table, CompilerFlags or name change.
- **0x00BFE900** (`??_GCLandscapeLayerMesh@@UAEPAXI@Z`): the July row's local models replaced by a by-path include of 0x00BFDE20's landed file with no
  code of its own (the nf251 / 0x00BEB4D0 fold pattern). Leaf RELOCATION_MATCH, relocation diff against HEAD empty, cmpobj SAME; fold object
  objsec ALL_EQ to 0x00BFDE20's. vtcheck: the 4-slot `??_7CLandscapeLayerMesh` lands on retail 0x012A835C slot for slot (0x00BFE900, 0x00BFDE80,
  0x00BFDDB0, 0x009D5100) and `??_7CMovableResource` on 0x0129B860 (at HEAD both were 3 slots, shifted by one). Gone from the object:
  `ForceNew_00bfe900`, two ctors, the local `CMovableResource::Verify` (returned 0 where retail returns 0x7B) and CanRelocateData copies. It now
  defines a strong `??1CLandscapeLayerMesh@@UAE@XZ` identical to 0x00BFDE20's (pk EQ): one equal-copy LNK4006; both link orders keep the same
  correct tables (the nf276 link-order table race is gone). Two rows now include `CLandscapeLayerMesh_CLandscapeLayerMesh_00bfde20.cpp` by path
  (funclet_includes_check: 1,359 checked, 0 broken).
- **Tests:** 0x00BFE900's landed 20 KB host test linked and passed without the row (never tested it); the new test is on the shared headers with the
  row's class block copied verbatim (class header quarantined), never constructs the class (no `??_7` / `??_G` / `??1` in the test object), calls
  the dtor directly and reaches slot 0 through `delete`, including a chained `Next` mesh: LNK2019 alone, PASS with the row. 0x00BFDE20's test: same
  harness, rows unchanged. 0x00BFDDB0's test reaches the private member through a test-only `friend`. Each fails to link alone and passes with its row.
- **Recheck** (`logs/recheck.log`, landed tree): 0x00BFE900 / 0x00BFDE20 / 0x00BFDDB0 + 0x00BFDE80 / 0x00BFDD70 / 0x00BFDDD0 (6 rows): 6 PASS (5 RELOCATION_MATCH, 1 MATCH), 0 DIFFER, 0 RELOC_CONFLICT.

## Coordinator rulings on NF277's questions
1. 0x00BFE900 row and test land as one set (the extra identical `??1` is the accepted equal-copy shape).
2. `?Verify@CMovableResource@@UBEKXZ` (0x009D5100) has no landed definer; five July rows (0x00BE8E60, 0x00BFBD80, 0x00AE04E0, 0x00AE0540, 0x00AE05A0)
   still define it pick-any returning 0. Queued: their ports onto the shared CMovableResource; a 0x009D5100 row goes on the authoring list (no
   authoring agent runs now).
3. The test-only `friend` in 0x00BFDDB0's test is accepted (tests are not retail code; the row is unchanged).
4. 0x00BFD480 queued as a row unit in the 0x00BFDE20 shape (`??1...QAE` -> `UAE`, both PDBs; table already binds UAE, binder 0x00BFDC90), then its test.
5. One commit.
Slip (the entry-2 batch launched as `nohup timeout 2000 bash e2a.sh`, `timeout 900` on each line inside): noted.
