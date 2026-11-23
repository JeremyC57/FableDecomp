# NF298 lessons (2026-11-23)

- **NScript Main binders name their entity-script allocators from the binding string.** The nested allocator struct must be `C` + the
  string literal passed to `CEntityScriptBinding` ("MK_OFI_GWLL_WHIS2" -> CMK_OFI_GWLL_WHIS2). A draft that "tidied" the name (dropping an
  underscore, or using a descriptive name like CWhisperMarker) binds a stand-in. The string literal is a second source for the PDB name.
- **Address-taken targets have no E8/E9 callers.** Allocs and spawned-process members (MagicRuneProcess) appear only as imm32 pushes in
  the Main, so xsites.py finds nothing. Use absrefs.py for the binder census.
- **A stand-in line and a PDB line that share one witness are a cheap stale-witness test.** Scanning all 535 entries for that shape found
  16 such lines beyond entry 60 (plus the 7 MurderTwist lines inside 1-60). I compiled 8 free witnesses (one compile each, about 3
  minutes in total), and all 8 were stale: 17 table lines deleted.
- **proof.sh pairs old and new symbols in sorted order.** That mispairs rows with several renames whose sort orders differ (0x00DCC770:
  `CMK_..` sorts before `CM_..`, but `CWhisperFarmRaidIntro` before `CWhisperMarker`). objsec then reports a false DIFF, or a false
  ALL_EQ when the swapped symbols have identical uses. Build the map from ENTRIES (tools/proofx.py) and check that it covers the
  DEF/UND symbol diff.
- **A clang test failure inside VC7.1 iostream is a D1 case.** Lock the row, then run `cl /Zs` on the test in the background.
