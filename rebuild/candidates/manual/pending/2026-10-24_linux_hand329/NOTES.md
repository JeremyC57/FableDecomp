# Hand chunk 329 (CDynamicObjectHeap<24 .. CPersistContext_TransferableComponent<float>): 39 of 40 byte-exact

All 39 src/ rows grade MATCH/RELOCATION_MATCH with gall.sh, pass tall.sh (g++) and tall.py (VC7.1), and come back
as WINS in a `verify_and_land.py` dry run (LANDVERIFY_WORK in this directory, no `--land`; log `dryrun.txt`).
Most catalog labels in this chunk were propagated and are wrong; real names below come from vtable slots,
FableWin.pdb symbols, PDB layouts and landed siblings.

## What matched (39)

- **CDynamicObjectHeap<N,M>::CreateMoreBlocks** (00af88a0 <24,853>, 00af8c00 <116,529>, 00b48f20 <12,16>,
  00b48f90 <72,16>, 00b49010 <56,16>): ++TotalAllocatorBlocks, `new` block chained after StartBlock, every entry
  pushed onto the free list. One template; the block geometry is read off the `push size` / loop count. Each
  entry's link is written twice (`entry->NextEntry = FreeList;` then the inline FreeObject push). Lesson: the loop
  must walk an entry POINTER (`++entry`). `&Blocks[i]` schedules the first two lea's of the 4x-unrolled
  <12,16>/<72,16>/<56,16> bodies differently. The pointer form matches all five.
- **CVtable_012b90d4_00c5ec50::Reset** (00c5e3f0): the handle owner of the landed destructor 0x00C5EC50. It does an inline
  Close() (`!= -1 ; CloseHandle ; = -1`), then the assignment. The dead `= -1` store survives.
- **CEdgeStipVertexArray<CWaterFanBaseVertex>::Resize** (00bfc140): the fourth member of the landed Resize family
  (stride 56 = PDB CWaterFanBaseVertex). It uses the catalog-majority callee/global symbols.
- **CEntityScriptBinding<CQ_EndGameFocalSitesScript> ctor** (00e95390): the class-allocator overload. The script class needs
  `struct __single_inheritance` so the member-function pointer `Func` is 4 bytes (an incomplete class gives 16).
- **CPKSerialisedShaderGroup_00c2ee80::Transfer** (00c2ee80): a libpk record. It checks the version (1), then makes three
  sub-record transfers at +0/+0x10/+0x20. The sub-records return bool and the record returns int (`mov eax,1`).
- **CGraphicInfoFrame::CFrameData** IsResourceLoaded (009fd7f0, slot 4) and ReleaseTexture (009fea00, slot 10):
  IsResourceLoaded needs `if (p && !p->IsFinished()) return false; return true;`. The `||` form gives 32-bit
  0/1.
- **std::basic_string<unsigned short>::resize** (0099cd60): STLport resize with the inline erase. Its move includes the
  terminator (`(finish-last)+1`), and the char_traits `n==0 ? s1 : memmove` gives `inc ; je`.
- **CIntelligentPointer holders**:
  - 006a2f00 NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser(CThing*, bool) (FableWin ctor symbol).
  - 0082c4c0 CMiniMapDisplay::CMarkerInfo(const CThing&, long, bool).
  - 007ca5a0 STLport pair<CIntelligentPointer<CThingObject>, bool>(const T1&, const T2&), the element type of the landed
    GiveItems.
  - All three use the pointer's `CIntelligentPointer(T*)` / copy ctor models.
- **CCombatAbilityAttackBase(long)** (0044c503): /Os def module. It runs the def base, the vtable store, then the
  CCombatAbilityMeleeAttackData member ctor.
- **CVolume(IDirect3DVolume9*)** (00a67da0): `m_pVolume(0)`, then InitialiseFromVolumeTextureMipmap.
- **CLipSyncDataBankEntry::CData::UnloadResource** (00a42a80, slot 2): MATCH. It needs inline `IsValid()` and a
  value-returning `GetRefCount()` (`Info ? Info->RefCount : 0`) for `mov eax,[eax] ; cmp eax,1`.
- **CLocalDetailCacheMap** (all CMovableResource overrides):
  - CQuadTreeElement::CanRelocateData (00bddd10, MATCH).
  - CObjectTypeCollection::RelocateData (00bddde0, MATCH).
  - CObjectCacheGroupCollection::RelocateData (00bddee0).
  - CQuadTreeElement::RelocateData (00bde0c0).
  - The re-base helper must be in place with integer arithmetic: `if (p) p = (T*)((long)p + ((long)new - (long)old));`.
- **CVtable_012B5870::SetSign8C** (00c3d310): a store-if-changed float sign plus dirty bits. It is a member of the static
  object 0x0143BB58, reached through a thunk.
- **CThingPhysicalSwitchDef ctor** (00454f7c): two 14-byte CEngineGraphic members (pack 2). It only matches under
  `#pragma optimize("s", on)`: /O2 folds the float zeros into integer stores.
- **CParticleUpdateParameters<ulong,ulong>**:
  - The ctor (006e24d0) and Update (0076c750) use the CVectorMap Sort() inline with a __fastcall STLport sort.
  - GetUpdateParam (00af3080) needs the LowerBound template body in the TU under `auto_inline(off)`. Then VC knows
    the call writes no memory and keeps the key in ebx.
- **CSymbolMap / CCRCSymbolMap ctors** (009e5200, 00a38500): CBase, a CVectorMap, an STLport set (malloc'ed header,
  `_M_empty_initialize`) and a bool.
- **std::vector<CThingTypeInfo>::resize(n)** (0052bf50): `resize(n, T())` with the string-only default ctor.
- **CPersistContext transferable components**:
  - CRGBColour TransferOut (006421f0).
  - C3DVector TransferOut/TransferIn (00485348/0048531e, /Os with frame pointers: `optimize("s",on)` plus `("y",off)`).
  - CFaction TransferOut (006bad20).
  - CIntelligentPointer<CThing> TransferOut/TransferIn (006c7c30/006c9b30) and the traits' out-of-line TransferOut
    (006c7b00).
  - All use the generic `CPersistTraits<T> traits; traits.TransferX(context, arg, value);` body of the landed
    CRGBColour/CFaction TransferIn rows. Vtable slot 0 is TransferOut and slot 1 is TransferIn.
- **CPersistContext::TransferBinary_0067b820**: a binary-only, unnamed ("" key) transfer of CReactionFrequencyTraitsArray
  (2x79 pointer rows zeroed in one loop). It matched first try with a plain `switch (Mode)` and cases 2 and 3.
- **C3DMeshFilePrimitiveChunk::SetMaterialIndex** (00abcf10): `Resize(1, 0); [0] = index;`, the else arm of the landed
  ReadFromFile.

## Parked for Fable 5.1 (1)

- 006c5110 CThingPointerHolder_006c5110::CThingPointerHolder_006c5110 DIFFER(67v69): only retail's final `mov eax,esi` is missing, and the temporary CIntelligentPointer<CThing> is built and copied without elision | tried: member `P(CIntelligentPointer<CThing>())` (copy elided, 18 bytes), novtable derived class with a base-from-temporary init (elided), ??_F default-ctor closure of `X(const CIntelligentPointer<CThing>& = CIntelligentPointer<CThing>())` (all bytes but the void closure has no `this` return) | next: a real constructor whose copy is not elidable (template converting ctor, or a reference-returning helper in the initialiser); no caller survives (xref 0), so the owning class is unknown.

## Skipped

None. No row spans two functions. Several rows' catalog lengths include trailing int3 padding, which the grader strips.

## Label corrections

- 00af8c00 "CDynamicObjectHeap<24" is really <116,529>. 00b48f20 and 00b48f90 "<56" are <12,16> and <72,16>.
- 00c5e3f0 is not CDynamicObjectHeap::Release: it is the handle owner's Reset(void*) (vtable 0x012B90D4 class).
- 00bfc140 is the CWaterFanBaseVertex instantiation, not CVertex.
- 00c2ee80 is not CFilter_Bind4<...>::operator(): it is a libpk serialiser record Transfer. Callee labels 0x00C31D40 /
  0x00C2F7D0 / 0x00C2FA20 / 0x00C2FC70 are also propagated.
- 0099cd60 is not CHistogram<long>::SetSize: it is STLport basic_string<unsigned short>::resize. 0x0099CB00 is its
  append(n, c) and 0x0099C990 the reserve.
- The CIntelligentPointer<const CThing> rows are holders:
  - 0044c503 CCombatAbilityAttackBase ctor.
  - 006a2f00 CThingGraphicArgPasser ctor.
  - 007ca5a0 pair<CIntelligentPointer<CThingObject>,bool> ctor.
  - 0082c4c0 CMiniMapDisplay::CMarkerInfo ctor.
  - 00a67da0 CVolume ctor. Its callee 0x00A67770 is CVolume::InitialiseFromVolumeTextureMipmap, not CSurface.
- 00a42a80 is CData::UnloadResource. 009fd7f0 / 009fea00 are IsResourceLoaded / ReleaseTexture.
- 00bddd10 / 00bddde0 / 00bddee0 / 00bde0c0 are CanRelocateData / RelocateData.
- 00c3d310 is not CParticleMorphs::CEntry::SetMorphScale: it is a member of the static CVtable_012B5870 object.
- The CParticleUpdateParameters<unsigned long rows:
  - 00454f7c is the CThingPhysicalSwitchDef ctor.
  - 009e5200 is the CSymbolMap ctor and 00a38500 the CCRCSymbolMap ctor.
  - 0x0076C000 / 0x0076C1A0 are the two STLport sorts, not "vector_Relocate".
- The CPersistContext_TransferableComponent rows:
  - 0052bf50 is vector<CThingTypeInfo>::resize(n).
  - 006bad20 is the CFaction TransferOut.
  - 006c7b00 is CPersistTraits<CIntelligentPointer<CThing>>::TransferOut.
  - 006421f0 is the CRGBColour TransferOut.
  - 00485348 / 0048531e are the C3DVector TransferOut / TransferIn.
  - 0067b820 is a CPersistContext binary transfer.
  - 00abcf10 is C3DMeshFilePrimitiveChunk::SetMaterialIndex.
- Landed 0x00BDD8E0 "CVertexBufferChainOwner_00bdd8e0::DestroyProceduralTextureVertexBuffers" is really
  CLocalDetailCacheMap::CObjectTypeCollection::OnPostRelocation (it walks the primitives at +8/+4 and rebinds
  each block owner). 00bddee0 binds it under that name, which the dry run reports as RELOC_NAME.
