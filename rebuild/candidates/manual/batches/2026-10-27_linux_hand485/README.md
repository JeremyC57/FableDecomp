# Chunk 485 (agent485, 2026-10-27): 39 of 40 single functions byte-exact

The first of six queued 40-row chunks of single functions (no shape twin, under 128 bytes, address order; chunk_history
485-490). One Opus 5.5 agent at high effort.

- **Landed:** 39 rows with `vland.py --land` (verify.log: 16 MATCH, 23 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). Six rows carry `#pragma optimize("s", on)` (an /Os translation unit: packed unaligned address,
  member in edx, floats zeroed with integer stores).
- **Skipped:** 0x004A1C41 `Catch@004a1c41` is a catch block of its parent (parent's ebp frame, reached only from the
  handler table at .rdata 0x01369280), not a function.
- **Identities:** 17 proven, 12 probable, 10 descriptive (NOTES.md); proven names in
  ghidra_out/labels_chunk485_2026-10-27.tsv.

Review at landing (NOTES.md, LESSONS.md): landed 0x004376B0 `SetCrimeAsDisplaying` is SetMoneyAsDisplaying (+0x2CC);
0x0052CCA0 should bind `??0CChangingStateComponent@NUISystem@@QAE@ABV01@@Z`; the `Catch@0047e8cc` labels on 0x00441B90 /
0x00442190 / 0x00442E60 and their CLandscapeBackgroundPatch stand-in bindings are wrong; 0x0047B830 (Dinkumware
`_Uninit_copy` label), 0x00448300 and 0x00448860 carry wrong labels (RELOC_NAME warnings); 0x013B8790 (player GUI
pointer) has 57 binding spellings. CObserver (0x0041B870) and CResource (0x0046A130) keep local models because their
generated headers flatten the vtable pointer (header debt).
