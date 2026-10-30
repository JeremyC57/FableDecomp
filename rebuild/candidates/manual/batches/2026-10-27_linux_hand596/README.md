# Chunk 596 (2026-10-30): 9 of 10 engine-range released pass 9 rows landed

9 RELOCATION_MATCH, all PASS (g++ and VC7.1). Proven from public source plus callers / vtables / class strings: DirectShow
BaseClasses `CBaseInputPin::Receive` 0x00CA8B90 (amfilter.cpp verbatim; the TU defines CBaseFilter::NotifyEvent so it
inlines); libvorbis 1.0.1 `vorbis_synthesis_headerin` 0x00C8E580 (/Gr /GS; four info.c statics in the TU) and
`floor1_inverse2` 0x00CA3420 (/Gr; same-TU `static __declspec(noinline)` render_line gets retail's register convention
without /GL; new data binding `_FLOOR1_fromdB_LOOKUP` -> 0x013ABE28); `CParticleProgram::~CParticleProgram` 0x00AE3670
(vtable 0x0129F0D8 + Ego_r; body `Clear()`); STLport `vector<CEngineOcclusionManager::COccluderFlareEntry>::_M_fill_insert`
0x00B8F4E0 (C3DVector needs a memberwise copy ctor or the copy becomes `rep movsd`) and
`vector<pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>>::insert` 0x00AF7730. Descriptive (libpk, no
PDB): `C16BitSamplePtr::C16BitSamplePtr(short*, long)` 0x00C62A50 (class from its `.\C16BitSamplePtr.cpp` assert string;
/EHsc; the libpk assert-macro model is reusable), `CReverbDiffuser::UpdateGain` 0x00C756C0 / `UpdateGainRamped` 0x00C75820
(landed names kept; third parameter is the shared CRingBuffer delay line). All 10 manifest labels were wrong.
Parked (../../pending/2026-10-27_linux_hand596/differ/): 0x00B22F30 `NParticleEngine::GFInterpolateArray<unsigned long>`
354v340 (register allocation: retail spills the hidden result pointer to [esp+1Ch], keeps a/b in esi/edi and the sizes in
ebx/ebp, and puts the `i < sizeA` arm after the epilogue; ~25 spellings tried; the fld/fistp pairs at 0x00B22F7F/83 and
0x00B22FE5/E9 make it a retail_asm candidate with its `// Retail-asm:` line written).
NAME_FIXES.tsv: 11 rows (`@CParticleProgram_dtor@4` stand-in at the ??_G 0x00AE8520; four vector helpers 0x00B8F370 /
0x00B8EED0 / 0x00B8E930 / 0x00B8E880 over COccluderFlareEntry, not pair<float,CCameraTag> / CHeroMarriageInfo; the insert
spelling; 0x00AF6690 `CComponentInstance::operator=`; 0x00B22F30 template spelling; the two reverb rows' parameter types;
0x00C74D30 Crossfade not Blend) + 4 coordinator review rows (0x00C74CE0 CDspGain::Apply two spellings HI vs _NK;
0x00AE8520 respelling; the four reverb callers 0x00C75A20 / 0x00C75AB0 / 0x00C891A0 / 0x00C89350; 0x0099EAE0 ~100 bindings).
Catalog label corrections for all 10 rows plus 0x00CA1E80 render_line, 0x00C8E200 / 0x00C8E2E0 _vorbis_unpack_comment /
_vorbis_unpack_books are in NOTES.md.
Header gaps: engine/CParticleProgram.h, CMovableResource.h, CMemoryDataOutputStream.h are flattened (0x00AE3670 uses local
models with `// Layout:` lines); none for CParticleSimulation, COccluderFlareEntry, C3DVector, CArray, CComponentInstance,
C16BitSamplePtr, the libpk reverb classes, strmbase classes, libvorbis structs. SHARED_HEADER_AVAILABLE warnings for
CMemoryDataOutputStream, CMovableResource, CEngineOcclusionManager, CBaseObject (the last is the strmbase false positive).
Lessons (LESSONS.md): a same-TU `static __declspec(noinline)` libvorbis helper gets retail's private register convention
(reopen hand448's 0x00CA2010 render_line0 "LTCG gap" and 0x00CA1E80 render_line); info.c statics match as real bodies with
`__stdcall` on the callee-pop ones; a strmbase inline helper must be defined in the TU to inline; an x87 conversion as a
named local moves the fmul after the next integer compare; initialising both chunk lengths before their wrap tests gives
retail's register pair.
