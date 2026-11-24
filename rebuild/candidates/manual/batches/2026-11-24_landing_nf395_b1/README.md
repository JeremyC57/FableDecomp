# Landing nf395 b1: the CMeshMorphSetParts uninitialized helpers get their STLport names, compiled /Gr (rulings R1, R8, R10, R11)

These three rows are one unit with the landed 0x00728D10. Each self is respelt from a made-up `SPairLongCSecond_005bbdf1` helper to the table's STLport spelling. The names come from the vendor source, with no PDB (ruling R6). Every caller is non-const.
- 0x00728A40 is `std::__uninitialized_fill_n<CMeshMorphSetParts*, unsigned int, CMeshMorphSetParts>`.
- 0x00728B10 is `std::__uninitialized_copy<CMeshMorphSetParts*, CMeshMorphSetParts*>`.
- 0x00728B50 is `std::uninitialized_fill_n<...>`.
- **Flags:** the three build entries now carry `CompilerFlags = '/O2 /Oy /W3 /Gr'`. Retail's `_Construct<CMeshMorphSetParts>` at 0x00728A20 takes its arguments in ecx/edx, and STLport writes no calling-convention keyword, so the translation unit was /Gr. The rows drop their `__fastcall` keywords; this checks the advisor's "from the flags, not an added keyword". The same-unit 00728d10 / 00728df0 still use keywords (T3 nf395 0x00728D10).
- **Extras:** each self is now Any and EQ by COMDAT to the copy the landed 00728d10 emits (coordinator `comdat_cmp`), as is every CArray / CEngineTextureMorphSet / CMeshMorphSetParts member and `??0__false_type`. The YI `_Construct` is tier 2 (non-unique; the only reloc-bound match is 0x00728A20). The old `SPairLongCSecond` copy pairs with the new `CMeshMorphSetParts` copy (R11).
- **Table:** delete `??0CSecond_005bbdf1@@QAE@ABU0@@Z -> 005bbdf1`. Only the three run rows use that type name.
- **Coordinator checks:**
  - landed 3 of 3, RELOCATION_MATCH;
  - each full self is defined in its new object and equals `self_symbols.tsv` (the Leaf lines are prefixes, unit u1);
  - each source equals the graded source;
  - baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- **NF395 gates:** pk_all / pickany_vs_retail with no NEQ and 0 reach flips; /FORCE link duplicates 0 -> 0 in both orders; a 30-row census by header reach was clean.
- **T3:** nf392 0x00728A40 and nf393 0x00728B10 are closed (T3_CLOSED_BY_HAND.tsv).
