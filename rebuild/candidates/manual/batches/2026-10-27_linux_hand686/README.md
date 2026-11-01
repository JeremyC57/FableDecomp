# Chunk 686 (NA029, 398-1010 band) -- 6 of 10 landed (2026-11-01)

Six rows landed (`verify.log`; bindings multi-check 0): CEaxContext::SetEnvironment 0x00C68A50 (libpk; the 31 sibling setters as same-TU
noinline bodies), NVectorMath::GFInterpolateVectors 0x00A5BBA0 at grade **retail_asm** (the SSE loop retail ran as inline asm, same style
and evidence form as chunk 684's 0x00A5B870; accepted under the owner exception; the agent's duplicate copy in src/ was set aside, the
retail_asm/ file is the landed one), C3DPrimitive2::ConvertToVBFormatStaticNonBump 0x00A8EFA0 (MATCH), COptimisedVertex::RemapGroups
0x00AC97D0 (full STLport _tree model + the `anyRemapped` scan flag), the STLport `__introsort_loop` instantiation 0x00CBDAD0 over
pair<CCharString, CCountedPointer<CEntityScriptBindingBase>> (the real algorithm text, not a hand quicksort) and libvorbis
vorbis_lsp_to_curve 0x00CA4570 (/QIfist). All ten manifest labels were stand-ins; 20 labels applied (`ghidra_out/labels_chunk686_2026-10-27.tsv`,
incl. CFlammableDef::GetAllFireHelpers / GetEffectAtHelperPoint landed under tree-owner stand-ins, CEaxContext::SetRoomHF / SetRoomLF labelled
CLight, CMatrix3x4::SetFromVectorAndAngle bound as a nonexistent ctor, CCharString::operator<).

Decisions at landing (coordinator, under the owner delegation):
- 0x00A3A410 CWADBank::AddFile (parked 918v920): the `__forceinline` on STLport vector::resize next to the shared header is the only lever that
  expands it twice -- acceptable only with a `// Retail-inline:`-style comment citing why (VC7.1 refuses the second expansion), and only if the
  remaining frame-slot order is found; otherwise the row stays parked. Not landed now.
- 0x00B202D0 TransformPosToMaintainShape: the division form (`v /= len` through `operator/=` as `*this *= 1.0f / s`) is retail's x87 sequence;
  keep it in the differ for the retry (residue: VC hoists all 12 identity stores where retail keeps 9 after the normalisation).
- The 00/c6 setter bodies landed twice with different class tails and SetReverbFlags carries noinline copies of the 8 scale setters: a
  dedupe-by-function-name item for migration wave 10 (header debt); 0x00CBDAD0's callers (0x00CBDFAD binds `IntrosortLoop_00cbdad0`) are queued c686.

Parked: TransformPosToMaintainShape 0x00B202D0 (850v850, 18 probes), CWADBank::AddFile 0x00A3A410 (918v920, 16 probes), CTCFlammable::
InitialiseFirePoints 0x007808B0 (1027v923, 12 probes: retail inlines the 5-arg CFirePoint ctor at the loop site but calls it at the fallback),
vector<CTVertexSeaBackground>::_M_fill_insert 0x00B801F0 (523v523, one load order). Best sources in `../../pending/2026-10-27_linux_hand686/differ/`.
NAME_FIXES.tsv: 23 rows (queued c686). Lessons in LESSONS.md.
