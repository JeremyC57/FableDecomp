# Chunk 690 (NA033, 471-1850 band) -- 6 of 10 landed (2026-11-01)

Six rows landed (`verify.log`, every row RELOCATION_MATCH + PASS; bindings multi-check 0): zlib 1.2.1 crc32.c `crc32_little` 0x00C059D0 and
`crc32_big` 0x00C05CB0 (the landed crc32.c TU; crc32 0x00C05FD0 tail-jumps to them), libvorbis 1.0.1 res0.c `res2_forward` 0x00CA1360 (/Gr; the
forward slot of residue2_exportbundle 0x013ABE1C), DirectShow wxutil.cpp `Int64x32Div32` 0x00CB0700 as grade `retail_asm` (the SDK text verbatim,
with the two inlined expansions of the published `__inline` EnlargedUnsignedDivide asm block, `// Retail-asm:` evidence at 0x00CB0892 / 0x00CB08BA;
landed through its own one-row payload because the chunk's finish.py reads only src/), CTCObjectAugmentations::UpdateParticleAttachment 0x00767DA0
(PDB name, 21 probes) and ~CEngineResourceManager 0x00B3B8E0 (primary vtable 0x012A1B3C, bases CEngineComponent / CAsyncLoadingProcessCallback;
local retail-layout model, see header debt). Every catalog label was a stand-in; 15 labels applied (`ghidra_out/labels_chunk690_2026-10-27.tsv`),
incl. CBankFile::SetBankUpdateStatus / SetBankUpdateProgress, ~CShaderDataBank 0x00A620D0 and ~CBaseClassNonCopyable 0x0099A300 in the label DB.

Extent (coordinator error, corrected): the cut gave CBankFile::Update 0x009CEC60 1684 bytes, taking 0x009CF2F4 ("Catch@009cf2f4") for a catch
funclet. It is not one: it is the code after a no-return `_CxxThrowException` call, in the same ebp frame, and the function runs to its single
`ret` at 0x009CF399 = 1850 bytes. BOUNDS_FIX now says len 1850 and MIDSTART skip for 0x009CF2F4 (removed from the manifest). Also corrected: the
chunk 689 records said `_tr_flush_block` is 0x00C059D0; it is 0x00C057D0 (the call to send_all_trees is at 0x00C0597E).

Decision (coordinator, under the owner delegation): ~CEngineResourceManager lands on its local retail-layout model with vland's
SHARED_HEADER_AVAILABLE warnings, because engine/CEngineResourceManager.h is wrong past VShaderBanks (CShaderDataBank is 0x128 bytes in retail,
0x13C in the header; ShaderBankPixel onward sits 0x230 lower; sizeof 0x31E0) and models members as opaque arrays. The header fix goes to the
engine-header migration wave; the row is re-pointed to the shared header there.

Parked: CBankFile::Update 0x009CEC60 (1840v1850 against the true extent, 15 probes: register rotation in the first scan loop, per-branch store of the
entry type, the second loop's redundant compare), CMatrix4x4::AlignedPreMultiplyTransposed 0x00ADEFC0 (retail_asm, 472v471, 33 probes + 24
permutations: x87 fallback term order), CDspOps::Biquad 0x00C76880 (500v513, 14: s0 kept in a frame slot, the libpk lane gap of chunk 684),
CAudioSource::UpdateStream 0x00C73050 (673v672, 22: ebx/ebp swap of sound vs queue). Best sources in `../../pending/2026-10-27_linux_hand690/differ/`.
NAME_FIXES.tsv: 15 rows (queued c690). Lessons in LESSONS.md.
