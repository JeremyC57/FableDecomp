# Chunk 591 (2026-10-30): 10 of 10 engine-range released pass 9 rows landed (9 RELOCATION_MATCH + 1 retail_asm)

All PASS (g++ and VC7.1). Proven: std::vector<CMatrix3x4> / <CPreTransposedBoneMatrix>::_M_insert_overflow, std::map<long,
map<long,CCountedPointer<CEngineSceneMap>>>::operator[] (full STLport _Rb_tree model), libvorbis vorbis_analysis_wrote and
_vp_tonemask, strmbase CBaseRenderer::GetMediaPositionInterface (/EHsc), DXUT WA_GetCandidateList. Descriptive (libpk):
Process_00c172f0 (/EHsc /GS), CCombFilter::Process, ConvertFloat128BlockToIntInPlace -- grade **retail_asm** (SSE2/MMX
rounding loops with MXCSR save/restore at 0x00C6B4AB..0x00C6B522; purity.classify = retail_asm; landed with `vland.py --land`
from retail_asm_payload.json; source copy in retail_asm_src/). All 10 chunk labels were wrong.
**Lesson that reopens parked rows (LESSONS 1):** a same-TU static whose real body is in the file gets retail's private
registers on its own; a leftover `add esp,N` after the call means retail's static pops its own arguments -> declare it
`static __stdcall`. Retry the "static register lane gap" rows: seed_loop 0x00C92E20 / max_seeds 0x00C93080 (chunk 447),
_fetch_headers 0x00C8A0A0 (590), _get_prev_page 0x00C89D70 (587). seed_chase 0x00C92xxx grades 360v362 as a by-product.
NAME_FIXES.tsv: 10 rows (PKCAutoFile and CLoadLog ctor/dtor landed under unrelated names; AddMatrix; CRendererPosPassThru PBG;
CCombFilter takes CRingBuffer) + 3 coordinator rows (ImeUi allocator globals with two names each; CUnknown ctor PBD/PBG bindings
look swapped). Header gaps: no engine header for any class here.
