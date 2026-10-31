# Chunk 619 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A `const C3DVector& n = Normal;` reference alias changes VC7.1's x87 operand order: with the alias the dot product loads the
  argument component first (`fld [edi+8] ; fmul [esi+8]`), the direct `Normal.Z` spelling loads the member first
  (0x00A42470 CPlane::GetIntersectionWithTriangle; closed a 14-line same-length residue).
- Dead named locals change spill-reload order at loop exits (VC7.1 numbers variables by declaration): 0x0066A1E0's old model
  with two unused offset locals reloaded edx/esi in retail order, the PDB-shaped model without them did not. Not a landable
  lever (dead code), but it shows the residue is variable numbering, not a missing statement.
- nd.py-style diff counters flag `mov [esp+x], <reloc>` vs our `mov [esp+x], 4` as a difference when the relocated global
  carries an addend (0x0137DADC = table + 4); q.py masks it -- read such lines before counting them as residue.
- A masked relocation can hide a swapped callee: 0x005D8978's comment had the binary In/Out serialisers on the wrong modes;
  check each call site's argument (PLoadStream +0x24 vs PSaveStream +0x28) against the callee's parameter type.

Review at landing:
- 0x00A42470: the only match; vland RELOCATION_MATCH + test PASS (tmp/vland.log).
- Six landed callers use wrong spellings for 0x0049C6F0 / 0x0049C700 / 0x0045C95A / 0x0045C91D / 0x0045CA09 / 0x009D2DD0
  (NAME_FIXES.tsv): 0x006B2720, 0x0063FAE0, 0x006CE300 (CThingSearchTools_Calls stand-in), 0x0045C8D3 (_ULongLong stand-ins),
  0x005CDCCB (CKillMap::insert), 0x00AD91F0 (map::insert_unique), 0x009F0570 / 0x009F03B0 / 0x009F05A0 (find stand-ins).
- Every parked row here has now had more than 4 grades across the retry sweeps (this chunk added 1-8 each); treat them as
  lane gaps unless a new lever appears.
- 008c4fd0 / 005d8978 parked sources carry name debt (see NOTES.md) -- respell before any future landing.
