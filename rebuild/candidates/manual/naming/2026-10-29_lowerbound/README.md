# Naming pass: CVectorMap LowerBound (2026-10-29)

0x0040F020 / 0x004365B0 are the STLport-style `CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound`
members (DECISIONS.tsv, proven); landed rows modelled them as `CInterfaceMap::LowerBound(const long*)`. The wave 2B script
`../2026-10-28_wave2b/scripts/fam_lowerbound.py` (byte-neutral by construction: same sizes, offsets, argument passing)
was run over every source with the canonical CInterfaceMap model: 1,090 files edited. Full recheck (recheck.log): 1,086
of 1,090 RELOCATION_MATCH with tests PASS. Reverted (FILES.txt excludes them): 0x007338A0, 0x007A0400, 0x007C6A40 (their
unedited tests no longer link against the respelled source) and 0x0093AD70 (source no longer compiles).
The old `?LowerBound@CInterfaceMap@@...` bindings stay: ~380 rows with other map shapes still emit them.
