# Hand chunk 600 (CScriptBase .. libvorbis): 6 of 10 byte-exact

All six are RELOCATION_MATCH (none MATCH, no retail_asm row landed); every one has a behaviour test that passes under g++
and VC7.1. Four rows are parked in differ/; all four carry proven PDB names.

## What matched (6)
- **CScriptBase::KillAllThreads 0x00CB7F60** (proven: Ego_r.pdb + FableWin.pdb name, PDB members Threads +4,
  EntityScripts +8, PActiveThread +0x2C): terminates each spawned thread with PActiveThread set to it and clears the
  list, then terminates every active entity script and erases the map. Shape: engine/CScriptBase.h +
  engine/CActiveEntityScriptBase.h (FABLE_RETAIL_NSCRIPT / FABLE_RETAIL_SUSPENDABLE_PROCESS), inline CScriptThing copy
  ctor / dtor and GetEntity by value, a same-TU stand-in `__copy`, and `CSpawnedFuncBase* thread = it->_M_data.Data;
  PActiveThread = thread; thread->TerminateProcess();` with a named local.
- **NScript::CHeroBoastsScript::BoastHobbeCaveInnocentSacrificed 0x00CE4C50** (proven: PDB name; ControlNewBoast stores
  this address next to the literal "BoastHobbeCaveInnocentSacrificed" 0x012C3A68, boast 22): the boast wait loop over
  the master data long +0x110. Matched first grade on engine/CLevelScriptBase.h + engine/CGameScriptInterfaceBase.h.
- **PKCDefaultFileManager::CloseFile 0x00C1A370** (class proven by the trace "\.PKCDefaultFileManager.cpp" and its
  sibling GetFileSize; **method name descriptive**, libpk has no PDB/RTTI): releases one reference under the lock, closes
  and deletes the record on the last. Shape: same-TU Find body plus a same-TU stand-in `PKCTRRedblackMap::Remove`
  (calls the declared-only DeleteNode) so VC7.1 reuses the dead key slot; `SOpenFile* file;` declared before the lock;
  `file->m_refCount <= 0` on an unsigned count gives retail's `ja`; traces at lines 65 and 68; `/EHsc`.
- **std::__introsort_loop<pair<pair<long,long>,CEngineTextureDecal>*> 0x00BEFA20** (name bound by the landed caller
  0x00BEFC20): STLport 4.6 introsort. Shape: the landed STL_insertionsort_00bef640 TU model (same-TU
  `__uninitialized_copy` taking `const __false_type&`) plus the 0x00BEF1D0 introsort body; `/Gr`.
- **CEffectPart260::Configure 0x00C783F0** (libpk, **descriptive**; callee names are address-suffixed placeholders
  `Stage_00c78150` .. `MillibelsToGain_00c77700`, plus the bound `MixChannels_00c77d90`): applies the dirty flag stages.
  Shape: retail keeps a dead arm, `unsigned long flags = Flags | 4; if (flags == 0) Stage_00c780c0(...); else Flags = flags;`
  (VC7.1 does not fold the test; the plain `Flags |= 4` is 13 bytes short).
- **libvorbis _ov_getlap 0x00C8C240** (vorbisfile.c 1.1-era; bound by the landed caller 0x00C8C580): the landed
  vorbisfile.c TU of 0x00C8A540 with the upstream body appended (including upstream's `sizeof(**pcm)*lapsize-lapcount`
  bug) inside `extern "C" { }`, `static __declspec(noinline) __stdcall`, plus fastcall declarations of
  vorbis_synthesis_read 0x00C90930 / vorbis_synthesis_lapout 0x00C90950.

## Parked for Fable 5.1 (4)
- 00b33390 CEnginePrimitiveRenderer::ProcessPrimitiveStats DIFFER(396v396): frame packing only, every instruction and register matches; retail homes the texture line / mesh symbol in the dead `total` / `names` argument slots and the empty __false_type tag in the `stats` slot (frame 8), VC7.1 here uses a 0x10 frame and puts the tag in `total`'s slot | tried: unnamed GetEntrySymbol temporary (385v396), named symbol local (396v396), trivial __false_type (401v396), CArray::push_back wrapper (396v396) | next: a different source structure that frees the argument homes earlier (e.g. the loop body in a same-TU inline helper taking the line by reference), or lane gap: stack packing
- 00bca510 CEngineVertexBufferPool::GetBuffer DIFFER(405v398): register allocation, the rounded count stays in edi where retail homes it in its argument slot, keeps `this` in ebx and a zero register in ebp (null tests, CListHeader zero stores, PoolNext = 0) | tried: all helpers `__forceinline` + inline_depth(4) (this file), plain inline (169v398), ternary GetGenericObject (411v398), by-reference rounding helper (no change) | next: write the zero once into a named local used by the stores, or lane gap: register allocation
- 00b9e5b0 CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer DIFFER(403v402): callee-saved register tie, textureStages in ebp (retail edi) and the inline AttachTextureToStage core pointer / stage in ebx / edi (retail ebp / ebx); the `[ebp+0]` load is the extra byte | tried: the landed sibling 0x00B9E750 TU with AttachTextureToStage plain inline (338v402) and `__forceinline` (403v402) | next: read textureStages fields into locals before the loop, or lane gap: register allocation
- 00ba9cb0 CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap DIFFER(399v403): the bounding-sphere copy, retail loads Centre.X / .Y on the x87 stack and copies .Z / .Radius through esi into frame words; also the GraphicList begin reload after the bounds test and the zero result from edx | tried: implicit struct copy (399), memberwise copy ctor (372), by-value transform helper (366) | next: a C3DBoundingSphere copy through a non-trivial copy ctor that copies two floats then two dwords; it contains the FloorToLong retail_asm block (`// Retail-asm:` line at 0x00BA9DE2), so a match belongs in retail_asm/

## Names (proven vs descriptive)
- Proven (two sources): KillAllThreads, BoastHobbeCaveInnocentSacrificed, ProcessPrimitiveStats,
  RenderMeshPrimitiveBlocksIndexBuffer, GetAppropriateMipmap, GetBuffer, the introsort and _ov_getlap instantiations
  (bound by landed callers + upstream source).
- Descriptive (libpk, no PDB/RTTI): PKCDefaultFileManager::CloseFile (class proven, method descriptive),
  CEffectPart260::Configure and its stage callees (address-suffixed placeholders).
- Global `g_pResourceManager` 0x01436E98 (00b33390) is the landed spelling from CEngine_SetVideoOptions 0x00B24690;
  0x01436E98 has about twenty bound spellings and no PDB global name (see Review at landing).

## Skipped
None.

## Label corrections
- 0x00B33390 "ProcessMorphStats" -> CEnginePrimitiveRenderer::ProcessPrimitiveStats.
- 0x00CB7F60 "Quest_DeactivateAllProcesses" -> CScriptBase::KillAllThreads.
- 0x00BCA510 "CEngineVertexBufferPool_AllocateBuffer" -> CEngineVertexBufferPool::GetBuffer.
- 0x00BA9CB0 "ComputeParticleVisibilityDistance" -> CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap.
- 0x00C1A370 "FileManager_CloseHandle" -> PKCDefaultFileManager::CloseFile.
- 0x00CE4C50 "sub_00ce4c50" -> NScript::CHeroBoastsScript::BoastHobbeCaveInnocentSacrificed.
- 0x00BEFA20 "std_make_heap_build_loop" -> std::__introsort_loop (decal pair instantiation).
- 0x00C783F0 "CAudioChannel_ProcessEffects" -> CEffectPart260::Configure (descriptive).
- 0x00C8C240 "VertexBuffer_FillData" -> libvorbis _ov_getlap.
- 0x00B321B0 landed as CEnginePrimitiveRenderer::CollectPrimitiveStats -> CollectPrimitiveStatsForArea (NAME_FIXES.tsv).

## Header debt
- engine/CScriptBase.h: the Threads list / EntityScripts CVectorMap are opaque bytes; 00cb7f60 overlays local STLport
  list / vector / CVectorMap models with reinterpret_cast.
- engine/CBankFile.h: Symbols / UpdateData are opaque `unsigned char[0xc]`; 00b33390 reads UpdateData's begin through a
  reinterpret_cast and declares CBankFileEntryUpdateData locally (no header). No headers for CBankFileAsync /
  CGraphicDataBank; engine/CMeshDataBank.h is flat (no CBankFileAsync base), so 00b33390 defines the bank chain locally.
- engine/C3DMeshInfo.h flattens its RTTI base CMeshDataBankEntry (+4 shows as `long f4`, +8 as pad).
- engine/CEnginePrimitiveRenderer.h / CEngineResourceManager.h have no method declarations: the donor pattern
  (`#define X XDonor` + derived class) is used.
- CVertexBufferWin32.h is a flat 0x34; CEnginePrimitiveManagerMeshBase has no usable header (local models in differ/).
