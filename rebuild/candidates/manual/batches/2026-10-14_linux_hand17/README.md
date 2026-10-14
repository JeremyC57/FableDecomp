# Batch `2026-10-14_linux_hand17` — hand-authored CCham .. CChunkedFileSuperChunk functions: 40 bodies (35 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the eighteenth chunk of
class-labelled open functions of 17–96 bytes: `CCharString` / `CWideString` string bodies
(own-copy, assignment, erase, substring, save, compare, find), an MFC `CString::ConcatCopy`
(the 0x00C0xxxx library module, propagated label corrected), `CChangingStateComponent` state
queries (136-slot component vtable modelled as plain virtuals), the `CVertexBufferWin32` /
`CChunkedFile*` / `CChecksumCacheFile` constructors and sub-chunk removal, a DirectShow
`CBasePin::SetMediaType` and `CBaseVideoRenderer::OnRenderStart` (propagated labels corrected),
and `CCham` / `CChestHidingAssassin` initialisers (TLC-extended past the Ego PDB size). Engine
headers are included where they fit; retail-vs-PDB layout notes are in each header comment
(`CCharStringData` packed 17 bytes, `CWideStringData` 16 bytes, `CChecksumCacheFile` map 12 bytes).

Lessons recorded by the pass: `strcmp(text, "")` is a VC7.1 intrinsic (`repe cmpsb ; sete`);
two hidden-pointer iterator finds share frame slots only in separate scope blocks; DirectShow
`FAILED(hr) ? hr : NOERROR` is the branchless `setge ; dec ; and`; an inline null-or-text accessor
used twice reproduces the double null test; a 5-byte `memset` plus 4-byte `memcpy` into a
`char[5]` keeps the `lea` in two registers; under `optimize("s")` an inline component-wise copy
constructor needs `__forceinline`. Five bodies differ by register allocation or byte-copy
scheduling only (`Truncate`, `Insert`, `CWideString::operator!=`, the two `CChangeText` constructors).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
