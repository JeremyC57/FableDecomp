# Landing nf403 b2: 0x0057FEFB `_Rb_tree<CCharString, pair<const CCharString, CTCHeroStats::MirroredCriminalStatus>>::_M_erase`

NF396 held this row because of an R1 residue: a cdecl `_Destroy<pair<...>>` copy that exists nowhere in retail.

NF403 added nf398's `FABLE_RETAIL_STLP_DESTROY_FASTCALL` to the row text, which changes only that COMDAT:
- the cdecl copy becomes a `__fastcall` 5-byte `jmp ??1CCharString`;
- the leaf stays RELOCATION_MATCH.

Retail homes for that copy:
- four unreferenced `jmp 0x0099EAE0` function starts in the row's TU region (0x0057EC36, 0x0057EC3B, 0x0057ED02, 0x0057F841);
- 0 sites of the cdecl shape.

R1's last clause (nf391, advisor decision, owner delegated) counts a non-unique masked home for an unreferenced pick-any copy that binds no name, so NF403's Q1 was already answered: this is ACCEPT.

Other gates:
- vland PASS on both sides;
- pk: no NEQ, no reach flips;
- /FORCE link: duplicates 0 -> 0.

Coordinator checks:
- landed 1/1, 0 LITERAL_DIFF;
- self_symbols respelt (no table line binds this address);
- T3 nf396 and nf398 0x0057FEFB closed;
- baselines OK.
