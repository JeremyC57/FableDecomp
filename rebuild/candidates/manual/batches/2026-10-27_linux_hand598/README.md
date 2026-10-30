# Chunk 598 (2026-10-30): 9 of 10 engine-range released pass 9 rows landed (7 C++ + 2 retail_asm)

9 RELOCATION_MATCH, all PASS. Proven from public source plus vtables / callers: DirectShow BaseClasses
`CMediaSample::SetProperties` 0x00CA9510 (amfilter.cpp verbatim; vtable slot 20 at 0x012BF730) and
`CBaseVideoRenderer::SendQuality` 0x00CA56E0 (renbase.cpp verbatim); libvorbis 1.0.1 `floor0_map_lazy_init` 0x00CA36E0
(floor0.c static, /Gr, private register convention with its caller floor0_inverse2 in the TU) and
`_analysis_output_always` 0x00C90D60 (analysis.c, /Gr /GS; todB stand-in in the TU). Descriptive (libpk, no PDB):
`CAmbientChannel::Update` 0x00C621B0, `CDSoundStreamBuffer::CreateBuffer` 0x00C57140 (/GS), `CAudioObject::Open` 0x00C42DD0
(/EHsc; its unwind deletes through 0x00C33100, a jump to 0x00C3C370). Grade `retail_asm` (retail_asm_payload.json /
retail_asm_oracle.tsv): `CReverbProcessor::Allocate` 0x00C6A460 (class proven from the landed ctor 0x00C6A650 / dtor
0x00C6A780; bare `fld` 0x00C6A4CE / `fistp dword` 0x00C6A4D2, bytes d9 44 24 1c / db 5c 24 14 verified) and
`CVtable_012bed90::RebuildGain` 0x00C6DEA0 (four bare fld/fistp pairs, first at 0x00C6DED1 / 0x00C6DED5): libpk's
float-to-long helper is inline asm, no VC7.1 C++ form emits a bare `fistp dword`. All 10 catalog labels were wrong.
Parked (../../pending/2026-10-27_linux_hand598/differ/): 0x00C6E050 `CVtable_012bed90::SetParameters` (vtable slot 4) 367v369,
frame only: retail keeps six separate float temporaries in a 0x20 frame and the `c+0.5` CSE in the unused `pan` slot, VC7.1
packs ours into 0xC; named intermediates, expression forms, hoisting, inline helpers and the QFE compiler all tried; next: a
declaration shape forcing eight float slots.
NAME_FIXES.tsv: 2 rows (0x00C62150 is the thiscall member `CAmbientChannel::GetHandleTable`, not a `__stdcall` free function:
retail loads ecx before the call; 0x00C6A460's caller 0x00C6A8B0 uses the placeholder `BufferState::Build`) + 4 coordinator
review rows (reverb class names across hand444 / hand342 parked copies; five addresses with several bound names incl.
0x0143BC2C ~10 and 0x00C3C360 ~20; the 0x00C6EC80 / 0x00C6E050 twin's first argument; IQualityControl::Notify takes `Quality`
by value). NOTES.md lists 9 more wrongly labelled callees.
Header gaps: no engine headers for the DirectShow base classes, libpk or libvorbis (local models); `engine/CBaseObject.h`
is Fable's 8-byte CBaseObject, never the DirectShow one (SHARED_HEADER_AVAILABLE on 0x00CA9510 is the known false positive).
Lessons (LESSONS.md): when retail loads ecx before a call whose callee never reads it, the callee is a thiscall member that
ignores `this` (a `__stdcall` free function drops that load); a member-by-member copy produces retail's `[eax+14h]` loads where
a struct assignment does not; declaring Lock output parameters in their own block scope puts them in the unused argument
slots; reading `m_caps` into a local after the memset fixes load order and which register holds 1.0f; an inline accessor
fixes argument evaluation order; an inline helper plus out-of-line calls reproduces RebuildGain.
