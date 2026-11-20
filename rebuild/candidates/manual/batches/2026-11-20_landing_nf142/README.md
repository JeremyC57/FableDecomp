# 2026-11-20 landing nf142: PRIORITY slice 11 fifth pass (15 rows; 0x0122A600 held)

Records: `naming/2026-11-20_agentNF142/` (selfcheck 0 problems). 8 entries: q4291, q4292, q6168, q4295, q4299, q4659, q6305 done (with
nf139's self rows); q4296 blocked (authoring row 0x009E15E0; 0x0122A600 held, below).

## Rows (15): RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- The 13 binders NF139 left (locked to NF136 then): 0x00405650, 0x00499D90, 0x0066CA80, 0x009C1AC0, 0x009C2520, 0x009E45D0, 0x0082CD00,
  0x00642A60, 0x0066C7A0, 0x00B4B830, 0x009C1050, 0x0040F0E0, 0x00A10E10, onto the claimed spellings (shared headers where they exist).
- Self renames on referenced rows (advisor decision, owner delegated: every referrer already binds the new spelling, so each address ends
  on one): 0x0040F0E0 `?ResetDimensions@CUIScreenUnitConverter@NUISystem@@SIXXZ`, 0x00A10E10 `?DetachTarget@CRenderTarget@@QAEXXZ` (both
  PDBs; each now one spelling in the table).
- Compiler-generated scalar deleting destructors on the nf86 / nf88 pick-any scaffolding: 0x009F61C0 `??_GCInputEvent@@QAEPAXI@Z`,
  0x0041BB00 `??_GCTexture@@QAEPAXI@Z` (FableWin.pdb names; the bodies equal VC7.1's output; neither name is bound at any address).
- Files renamed with their rows (sources and tests).

## Held: 0x0122A600 (atexit destructor of the static `CTexture GFontTextureCache`)
Written the 0x01228CE0 way (the static defined in the row), its TU-hashed `?GFontTextureCache@?A0xb50312da@@3VCTexture@@A` became a third
spelling at 0x013C5A80 (2 -> 3; the anonymous-namespace hash is per TU, so no canonical spelling exists). Advisor condition: land only if
0x013C5A80 does not grow. Left at HEAD; q4296 stays blocked.

## Checks
- 328 rows rechecked, 326 good. 0x009E15E0 is DIFFER at HEAD (nf139 proof). 0x009DF060 includes nothing changed by nf142 or any landing
  today; queued for the full-tree regrade. Gate 0; bindings_multi_check 1.
- Table: no set grew. Shrank: 0x009F2E20 6 -> 1, 0x009BEDC0 3 -> 1, 0x009F9F70 3 -> 1, 0x0099B510 13 -> 11, and nine more.
- One definition (`logs/symbol_diff.txt`): own leaves, scaffolding helpers, shared-header inlines and COMDATs. Strong out-of-line
  definitions of every newly emitted member were searched; the one hit, 0x00B870A0 (`~CSurface` and four other header destructors defined
  strongly), was compiled and linked with the 15 objects and all 323 rechecked objects (`logs/link_context.log`). No LNK2005 involves a
  landing object; 0x00B870A0's clashes are with untouched rows (pre-existing, queued as DEBT). Link test over the 15 objects: none.
