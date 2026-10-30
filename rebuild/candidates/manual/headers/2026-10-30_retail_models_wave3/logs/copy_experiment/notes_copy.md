Copy-member experiment (2026-10-30, ~1.3 h):
- temp guard FABLE_DEF_IMPLICIT_COPY dropped the CDefClassBase/PDCB copy declarations; chain.h = shared retail chain + typed
  PDCB members (CDefString {TablePos}, CVectorMap<K,V,C> : std::vector<pair<K,V>> {Compare, Dirty}, under pack(8) so the map
  is 0x10 and the PDCB size assert holds inside a row's pack(1)); vector::operator= declared only.
- standalone emissions via helper (/O2): ED0 RELOCATION_MATCH; F60 (CDefPointeeBase implicit) MATCH; F10 DIFFER 102v77
  (inlines F60; retail calls it). /Os: ED0 39v49 (calls CVectorMap op=), F10 72v77 (calls F60 but RMW bitfield codegen), F60 18v45.
  inline_depth(1): F10 RELOCATION_MATCH but ED0 then inlines F10 (105v49). inline_depth(2/3): no change.
- /Os callers: 39/39 RELOCATION_MATCH on the implicit chain (38 rows that include the shared header + CSoundThemeDef also
  with a fully implicit derived op= reached through Copy). Indistinguishable from the out-of-line declaration.
- /O2 callers that retail makes CALL ED0: 0x00434370 CEngineDef (implicit derived op=) DIFFER 153v123, 0x00625350
  CConfigOptionsDefaultsDef DIFFER 145v115 -- the implicit ED0 is inlined. Refutes "implicit" for them.
- /O2 expander 0x00432E60 CFrontEndDef: retail inlines ED0 + F10, calls F60. Every natural model gives 81 bytes calling ED0:
  implicit chain (implicit or explicit derived op=), explicit non-inline ED0/F10 defined earlier in the TU (h1/h3), explicit
  `inline` (i1/i2), inline_depth 1-3, optimize s/t/g, and the QFE 4035 compiler. Only row-local __forceinline reproduces it.
- The two directions are opposite to RTM's heuristic (it inlines into the scalar-only CEngineDef, not into CFrontEndDef with a
  vector member), so no declaration of the shared members expresses both. Rule: declare out of line (reproduces all 108
  callers incl. the 3 /O2 ones); the 5 expanders keep their row-local __forceinline chain as residue.
