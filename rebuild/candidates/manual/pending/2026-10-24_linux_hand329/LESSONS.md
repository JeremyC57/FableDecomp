# Chunk 329 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Pool refill loop:** CDynamicObjectHeap<N,M>::CreateMoreBlocks needs a pointer-walk loop
  (`CObjectEntry* e = block->Blocks; for (i...; ++i, ++e)`). With `&Blocks[i]`, the first two lea's of the
  4x-unrolled bodies are scheduled differently (00b48f20, 00b48f90, 00b49010). The double link store is
  `e->NextEntry = FreeList;` plus the inline FreeObject push.
- **Member-function pointer size:** a 4-byte PMF member needs `struct __single_inheritance X;` when the class is only
  forward-declared. Otherwise the PMF is 16 bytes and every later member shifts (00e95390).
- **Guarded bool return:** `if (p && !p->F()) return false; return true;` returns the callee's own al (a `ret` right
  after `jne`). The `!p || p->F()` form gives `xor eax,eax` / `mov eax,1` (009fd7f0).
- **Count compare through an accessor:** a value-returning inline accessor (`long GetRefCount() const { return Info ?
  Info->RefCount : 0; }`) gives `mov eax,[eax] ; cmp eax,1`. A direct `Info->RefCount == 1` folds into
  `cmp dword ptr [eax],1` (00a42a80).
- **Pointer re-base after relocation:** write it in place with integer arithmetic:
  `if (p) p = (T*)((long)p + ((long)newBase - (long)oldBase));`. A char* return-value helper reassociates to
  `p - old + new` and moves p out of its register (00bddde0, 00bddee0, 00bde0c0).
- **Key kept across a find call:** retail keeps a by-reference key in a register across an out-of-line
  `LowerBound(const K&)` call when the template body is in the same TU under `#pragma auto_inline(off)`.
  Declaration-only, VC re-reads the key after the call (00af3080, 98 -> 97 bytes).
- **Def-module float zeros:** in /Os def modules, inline CEngineGraphic constructors keep `fldz ; fstp` through a
  `lea eax` member pointer. Under /O2 the zeros fold into integer stores (00454f7c). The 0x0044C/0x00454 def
  rows want `#pragma optimize("s", on)`.
- **Component vtable slots:** CPersistContext_TransferableComponent vtables hold slot 0 = TransferOut(context, CCharString&) and
  slot 1 = TransferIn(context, CStringParser&). Several catalog "TransferIn" rows are slot-0 TransferOuts
  (006421f0, 00485348, 006bad20). Read the vtable before naming.
- **C3DVector component rows:** the C3DVector component rows (0x00485xxx) are /Os with frame pointers:
  `#pragma optimize("s", on)` + `#pragma optimize("y", off)`.
- **STLport wide-string erase:** the erase in basic_string<unsigned short> moves `(finish - last) + 1` elements (the
  terminator). char_traits move `n == 0 ? s1 : memmove(s1, s2, n*2)` gives the odd `sub eax,esi ; sar ; inc ; je`
  shape (0099cd60).
- **Copy from a default temporary:** a member copy-initialised from a default temporary (`P(T())`) is ELIDED by
  VC7.1 (18 bytes). Retail's non-elided temp + copy ctor + temp dtor pattern matched only as a `??_F` default-ctor
  closure, minus the `this` return (006c5110, parked).

Review at landing:
- 006c5110 parked after 3 grades. Its owning class is unknown (no references).
- Two RELOC_CONFLICTs for `??0CBase@@QAE@XZ` (009e5200, 00a38500). They bind 0x0099A2F0 like ~113 landed
  rows, but landed 00df2380 binds the same symbol to 0x0099A2D0. 00df2380 should take the N_0099a2d0 name.
- Landed 0x00BDD8E0 (`CVertexBufferChainOwner_00bdd8e0::DestroyProceduralTextureVertexBuffers`) is
  CLocalDetailCacheMap::CObjectTypeCollection::OnPostRelocation. 00bddee0 calls it under the real name (RELOC_NAME
  warning). Rename the landed row.
- Local callee models sit next to existing engine headers. These headers are vptr-only/POD flattenings that carry no
  methods or virtuals:
  - CIntelligentPointer bases.
  - CIVCountedPointeeBase (00a42a80).
  - CCombatAbilityData (0044c503).
  - CEngineLandscapeMemoryManager (00bfc140, 00bde0c0).
  - CMiniMapDisplay: no nested CMarkerInfo (0082c4c0).
  - 00bde0c0 now uses engine/CMovableResource.h.
- 00bfc140 uses the catalog-majority callee/global symbols (`CEngineLandscapeMemoryManager::AllocVirtual(long, ...)`,
  `SEngine_01436ea8* g_pEngine_01436ea8`). The landed siblings 0x00BFBE40/0x00BFBF40/0x00BFC040 bind the same
  addresses as `CEngineLandscapeMemoryManager_Methods::AllocVirtual(int, ...)` / `g_LandscapeRenderer`.
- 00af3080 defines the CVectorMap LowerBound template (retail 0x00AF2C50) in its TU. Check that the lander accepts
  the extra symbol.
- 0067b820 is named descriptively (`CPersistContext::TransferBinary_0067b820`). FableWin only has the named
  `Transfer<CReactionFrequencyTraitsArray>(const char*, T&[, const T&])`. The 1-argument body with a constant ""
  key looks like an LTCG-specialised copy.
