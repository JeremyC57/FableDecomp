# Chunk 76 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Def-module constructors: in retail the `CParentDefClassBase` root ends at +0x3c (PDB +0x40) and vectors are 12 bytes;
  with that shift, PDB members modelled as inline-constructed vector members (`CRetailVector<T>`) reproduce retail
  exactly with real member names. `or dword ptr [..],-1` is a `CDefString` inline constructor under `optimize("s")`.
- A by-value parameter's key that retail keeps in ebx across out-of-line calls in a loop: read it into a named local
  first (else the lane keeps it in edx and reloads the parameter slot; 006fb960, SGI `__unguarded_linear_insert`).
- One shared `xor al,al` exit for a null test AND a search loop = the loop nested in `if (p) { ... } return false;`
  (an early `return false` gets its own exit).
- An argument byte stored before the zero stores and before the vtable store = a body assignment written FIRST (as an
  initialiser it is loaded into cl and stored later; 00af8b40).
- `m_map[key] = value`: the value is pushed first and kept on the stack across `operator[]` -> parameter order is
  (value, key) (009a8150).
- A copy constructor whose second member is copied through re-based pointers (`lea edi,[ebx+4]` / `add esi,4`) = a
  nested record with its own inline memberwise copy constructor (009a9ad0).
- A 32-byte intrinsic `memset(this,0,sizeof)` = eight zero-register stores. 0x00C8DF70 is libvorbis
  `vorbis_info_init` (the exe links "Xiph.Org libVorbis I 20030909").

Review at landing: the already-landed `CSoundEmitterLH` constructor (0x00AAC580) types the word at +4 as a
`CGameScriptThing`; it is the owning `CSoundManagerLH` -- fix that landed source's model/comment (bytes unaffected,
re-run the lander on its batch after editing).
Parked 004570d3 / 00457da9 / 00458c19 / 004e00bc are the 24i `lea`-before-zero-stores lane gap; 005bc6d6: try
`optimize("s")` first (unaligned start). Skipped 00aacf90 (two functions in one row). 21 label corrections in NOTES.md.
