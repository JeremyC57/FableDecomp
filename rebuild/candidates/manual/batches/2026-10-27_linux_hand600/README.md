# Chunk 600 (2026-10-31): 6 of 10 engine-range pass-9 rows landed, 4 parked

Rows 394-406 bytes from the released ChatGPT pass 9 reservation (engine range). Landed 6, all RELOCATION_MATCH + PASS, no
retail_asm row: 0x00CB7F60 CScriptBase::KillAllThreads (PDB), 0x00CE4C50 NScript::CHeroBoastsScript::BoastHobbeCaveInnocentSacrificed
(PDB; ControlNewBoast stores the address next to the literal), 0x00BEFA20 std::__introsort_loop decal-pair instantiation (bound by
landed caller 0x00BEFC20, /Gr), 0x00C8C240 libvorbis `_ov_getlap` (upstream vorbisfile.c; caller 0x00C8C580), 0x00C1A370
PKCDefaultFileManager::CloseFile (class from its trace string; method name descriptive; /EHsc) and 0x00C783F0 CEffectPart260::Configure
(libpk mixer, descriptive, spelling from the landed caller's binding; stage callees are address placeholders — queued).
Parked in `../../pending/2026-10-27_linux_hand600/differ/` (all four names PDB-proven): 0x00B33390 CEnginePrimitiveRenderer::
ProcessPrimitiveStats 396v396 (frame layout only: retail homes the string locals in the dead argument slots; worth one more attempt),
0x00BCA510 CEngineVertexBufferPool::GetBuffer 405v398 (rounded count spilled to its argument slot, zero kept in ebp), 0x00B9E5B0
CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer 403v402 (callee-saved assignment), 0x00BA9CB0
CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap 399v403 (bounding-sphere copy, GraphicList begin reload; contains a FloorToLong
retail_asm block at 0x00BA9DE2 — retail_asm/ when it matches).
Every manifest label in this chunk was wrong (nine corrections in NOTES.md). NAME_FIXES.tsv: 7 rows (KillAllThreads not Shutdown;
0x00CBB090 is STLport `_List_base<CCountedPointer<CSpawnedFuncBase>>::clear` not CEventList::clear; non-const SVoiceEffectParameters*;
CVertexBufferBase::IsBusy / Sizeof; BuildNewBuffer FableWin signature; CollectPrimitiveStatsForArea) + 4 review rows (libpk
descriptive convention; g_pResourceManager 0x01436E98 ~20 spellings; the CIEngine stubs 0x00B23430/40; 0x00BEC8B0 local struct).
Lessons: a dead arm `f = Flags|4; if (f == 0) Stage(); else Flags = f;` reproduces retail's flag step; a same-TU stand-in `Remove`
lets VC7.1 reuse the dead key argument slot; unsigned `<= 0` gives `ja`; the FindNode chain needs `inline_recursion(on)` +
`inline_depth` + `__forceinline`; a libvorbis static added to the landed TU sits inside `extern "C"`; CEngine `mov ecx,[g]; jmp`
forwarders list component methods in PDB virtual order (names unnamed methods); a CCharString returned by value and passed to
push_back must be a named local. Header gaps: engine/CScriptBase.h and CBankFile.h containers opaque; no CBankFileAsync /
CGraphicDataBank / CBankFileEntryUpdateData headers; CMeshDataBank.h, C3DMeshInfo.h, CVertexBufferWin32.h flat; CEnginePrimitiveRenderer.h
and CEngineResourceManager.h declare no methods.
Coordinator: land_chunk.sh 6/6, bindings_multi_check 0.
