# Unit A (L129, LocalDetailEngine): identity of 0x00BD9CA0 and the 0x01436EA8 binders (NF271, 2026-11-22; mnemonics only)

## Call target of the three "FreeAlignedVirtual" sites is 0x00BD9CA0, not 0x00BD9B90
- xdis of retail 0x00BFAAE0 (CLocalDetailPrimitiveRepeatedMesh dtor): `mov edi,[eax+6B0h]` (renderer 0x01436EA8 -> MemoryManager), then
  call 0x00BD9B90, call 0x00BD9CA0, call 0x00BD9CA0, call 0x00BD9B90, call 0x00BD9CA0 (sites 0x00BFAAFB / 0x00BFAB06 / 0x00BFAB11 / 0x00BFAB1C / 0x00BFAB27).
- callers.py 00bd9ca0: 0x00BFAB06 / 0x00BFAB11 / 0x00BFAB27 (row 0x00BFAAE0) and 0x00BFB660 / 0x00BFB670 / 0x00BFB690 (no landed row).
- reloc_bindings.tsv already maps the stand-in `?FreeAlignedVirtual@LandscapeMemoryMethods@@QAEXPAX@Z` to target 00bd9ca0 (witness 0x00BFAAE0);
  the row comments ("// 0x00BD9B90") and the nf262 / nf263 notes ("untabled ... for 0x00BD9B90") were wrong.

## Name of 0x00BD9CA0: ?FreeVirtualAligned@CEngineLandscapeMemoryManager@@QAEXPAX@Z
- Source 1, both PDB name sets: Ego_r 009c2430 and FableWin 02cb7a40 (ghidra_out/egor_pdb_names.tsv / fablewin_pdb_names.tsv).
- Source 2, donor order: Ego_r FreeVirtual 009c2300 / DeleteVertexBufferBlockWhileBusy 009c2370 / DeleteIndexBufferBlockWhileBusy 009c23a0 /
  AllocVirtualAligned 009c23d0 / FreeVirtualAligned 009c2430 / DeleteTextureBlockWhileBusy 009c2450 = retail 0x00BD9B90 / 0x00BD9C00 /
  0x00BD9C20 / 0x00BD9C40 (catalog AllocVirtualAligned) / 0x00BD9CA0 / 0x00BD9CC0; both FreeVirtualAligned bodies are 0x20 apart from the next.
- Source 3, body: `mov eax,[esp+4] ; test eax,eax ; je ret ; push esi ; mov esi,[eax-4] ; mov edx,eax ; sub edx,esi ; pop esi ;
  mov [esp+4],edx ; jmp 0x00BD9B90 ; ret 4` = free (block - block[-1]) through FreeVirtual, the inverse of AllocVirtualAligned's stored offset.
- Source 4, callers: the three sites free LandscapeNormalArray (float*), SubSections and ObjectOffsets (C4DVector*), the arrays FableWin's
  `DeallocateAlignedArrayOfObjectsNoDestructors<float / CSubsectionElement / C4DVector>` templates name as aligned; the two FreeVirtual sites free
  WindDelayArray (unsigned char*) and ObjectMatricies, FableWin's non-aligned `DeallocateArrayOfObjectsNoDestructors<unsigned char / C4DVector>`.
- The catalog already names it `CEngineLandscapeMemoryManager::FreeVirtualAligned`; the self row defined `?FreeVirtualAligned@CEngineLandscapeMemoryManagerMethods@@QAEXPAX@Z`
  (method-projection stand-in, no Leaf line).

## 0x01436EA8
- `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A` (three PDBs; landed opt-in FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER,
  nf256 / nf260 proof); MemoryManager at +0x6B0 in both renderer header branches (static asserts), as the retail `mov edi,[eax+6B0h]` reads.
