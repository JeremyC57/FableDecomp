# Retail bytes say wide text and 16-bit index data share one instantiation (read before landing this wave)

The 2026-10-28 FINDINGS entry said no retail byte can tell native wchar_t from the unsigned-short typedef. One chain of
retail call targets can tell them apart, given the project's rule that retail has no identical-code folding
(GOTCHAS 1494/1498/1544: byte-identical COMDATs stay at distinct addresses).

Retail call targets, decoded from Fable.exe .text (E8 rel32):
- 0x00AA6960 `C3DMeshFileXTriangleStripChunk::ReadFromFile` calls 0x00517900 (`vector<unsigned short>::resize`, reached
  through `CArray<unsigned short>::Resize`). The data is triangle-strip indices, which are genuine 16-bit data.
- 0x00517900 calls 0x005149F0 `vector<unsigned short>::_M_fill_insert`, which calls **0x005123A0** `uninitialized_fill_n`.
- 0x0099CB00 `basic_string<wide>::append(n, c)`, 0x0099CC80 `push_back` and 0x0099B9A0 `CWideString::operator+=(wchar_t)`
  call the same **0x005123A0**. A whole-.text scan finds exactly four E8 callers: 0x00514A96, 0x0099BA11, 0x0099CB62
  and 0x0099CCE0.
- The 32-byte body of 0x005123A0 has byte-identical twins at 0x0050E4A0, 0x0070F170 and 0x00AADD60. The linker did
  not fold those twins, so it did not fold 0x005123A0 with another body either.

STLport 4.6 `basic_string<_CharT>::append(size_type, _CharT)` calls `uninitialized_fill_n(_M_finish + 1, n - 1, c)`,
which is instantiated on `_CharT*`. `vector<_Tp>::_M_fill_insert` calls it on `_Tp*`. With native wchar_t these are two
instantiations: `<wchar_t*, unsigned, wchar_t>` and `<unsigned short*, unsigned, unsigned short>`. That means two
bodies at two addresses. Retail has one body for both callers, so in the retail build wide text and `unsigned short`
were the same type. In other words, retail was compiled without /Zc:wchar_t, and wchar_t was the `<stddef.h>`
typedef. STLport supports that configuration (`_STLP_WCHAR_T_IS_USHORT` when `_NATIVE_WCHAR_T_DEFINED` is absent).
This also matches the earlier observation that the PDB builds (Dinkumware STL, not retail's STLport) are not the
retail configuration.

What this means for the wave:
- Under the decision, 0x005123A0 has to carry two spellings: `??$uninitialized_fill_n@PA_WI_W@std@@...` from the
  basic_string rows and `??$uninitialized_fill_n@PAGIG@std@@...` from 0x005149F0. That breaks "one retail address, one
  name" (BINDINGS.tsv marks it TYPE CONFLICT). No spelling under native wchar_t can be correct for both callers.
- The PDB `_W` spellings are what the Ego_r/FableWin builds emitted. Retail's own mangled names, if it had any, would be
  `G`.
- Byte parity does not depend on the choice. All 368 rows are MATCH / RELOCATION_MATCH either way.

The owner might reconsider before this wave lands. Two options:
- (a) Keep the decision as a naming convention: "PDB spellings, even where retail's type identity differs". Record the
  0x005123A0 conflict as a known exception.
- (b) Revert to the typedef. Wide text keeps `G`, and `_W` appears nowhere.

This wave's scripts are reversible either way. Nothing is landed, and FLAGS.tsv plus fam_wchar.py are the whole change.

FINDINGS line ready to paste (2026-10-29): `wchar_t retail evidence: 0x005123A0 uninitialized_fill_n is the single
callee of both vector<unsigned short>::_M_fill_insert 0x005149F0 (triangle-strip indices, via 0x00517900 from
C3DMeshFileXTriangleStripChunk::ReadFromFile 0x00AA6960) and basic_string<wide>::append/push_back 0x0099CB00/0x0099CC80
+ CWideString::operator+=(wchar_t) 0x0099B9A0 (4 E8 callers in .text). Its 32-byte body has 3 unfolded twins (0x0050E4A0,
0x0070F170, 0x00AADD60), so no ICF: retail's wide character type == unsigned short, i.e. no /Zc:wchar_t in retail.
Native-wchar_t naming (owner decision 2026-10-28) therefore gives 0x005123A0 two spellings; see naming/wchar/EVIDENCE.md.`
