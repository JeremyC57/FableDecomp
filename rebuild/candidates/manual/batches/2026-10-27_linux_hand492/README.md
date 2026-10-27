# Chunk 492 (agent492, 2026-10-27): 37 of 40 single functions byte-exact

Queued 40-row chunk of single functions under 128 bytes (address order, 0x00A446CE..0x00B806D0). One Opus 5.5 agent at
high effort.

- **Landed:** 37 rows with `vland.py --land` (verify.log: 20 MATCH, 17 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). Nothing parked.
- **Skipped (3), fragments not functions:** 0x00A446CE and 0x00A44719 (run on the parent's ebp frame; part of
  CSuspendableProcessMicrothread::ThreadEntry, see chunk 491), 0x00A46594 (pops registers it never pushed).
- **Identities:** 27 proven (ghidra_out/labels_chunk492_2026-10-27.tsv), 7 probable, 3 descriptive (NOTES.md).

Label corrections (NOTES.md): 0x00A44930 is vector<CASuspendableProcess*>::empty and 0x00A44A80 its pop_back (catalog
labels were a filter operator() and CGameScriptThing::DecrementScriptCounter); landed 0x00A44A70 `C00a44a70::M` is
vector::back; landed 0x00A45150 binds 0x00A44C90 as `BoneMorphLess` (C3DSkeletalBoneMorph::CompareCRCs); landed
0x00A80000 binds STLport pair as a class (`V`, should be struct `U`); landed 0x00B5EE00 defines its method on a derived
shell class; 0x00A242C0 has 9 binding spellings (PDB: PeekAnimation); 0x01436E8C has 7 global spellings
(g_pEngineWorldMap); 0x00BDC450 is probably CEngineLandscapeMap::Clear. Header debt in NOTES.md.
