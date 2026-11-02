# Chunk 709: lessons

## GOTCHAS one-liners
- **PMF dispatch tables.** Declare the dispatch class before the class that owns the member-function pointer is complete. That
  makes the PMF the 16-byte unspecified-inheritance kind, which gives retail's stride. Then test the PMF through a named local
  (`const T* d = ...; return d->Fn != NULL;`), so VC7.1 tests only the function word instead of copying the PMF to a frame
  temp.
- **/Gr TUs and byte twins.** STLport `_Destroy<T>` is `__fastcall` under /Gr, so its body is byte-identical to `T::~T`. Lay
  out a pair twin set by emission order: non-const pair `??1` (the make_pair temporary), const pair `??1`, `??_G`, then
  `_Destroy` after the user function. COMDAT section order in the lane object reproduces retail address order.
- **Overloaded virtuals.** VC7.1 lays them out in reverse declaration order (CWorld::GetCurrentRenderCamera: slot 5 is the
  const& overload, slot 6 the no-argument one).
- **STLport list.** With the `_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> > _M_node` base and `this->`-qualified
  `_list.c` code, the 4.6 `clear()` and `erase(iterator)` inline shapes matched first time.
- **Catalog label `_Dest_val<..C3DSkeletalMorphTarget..>`.** It is propagated across byte-identical counted-pointer destructor
  bodies. Name each copy from its own `??_G` vtable slot or its ctor's vtable store.
- **0x00826380.** It is CCachedFile, not CChunkedFile: the stored vtable's RTTI decides.
- **Implicit destructors.** A local object or a `p->T::~T()` harness makes VC7.1 emit the out-of-line `??1`. Add
  `<stddef.h>` before using `offsetof`, otherwise you get C2275.

## Review at landing
- 0x0049E270: the creature chain sizes 0xC0, 0xD0 and 0x160 are opaque restatements. Check them against the
  CThingPhysical, CThingGameObject and CThingCreatureBase headers when those land.
- 0x00953470: vland WARN SHARED_HEADER_AVAILABLE CCreatureActionBase. See the NOTES header debt; strict mode rejects it.
- 0x009EEA50: the console TU flag is `/Gr`. It is the only row with Flags.
- No `#pragma pack` is needed for C3DVector or CBaseIntelligentPointer restatements: natural alignment gives the same bytes (rows 0x0071CD30, 0x00873DB0, 0x00953470 re-graded without it).
