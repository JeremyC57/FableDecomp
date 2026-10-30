# Chunk 591 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU statics.** At plain /O2, VC7.1 gives a same-TU static function a private register convention. When the
  static's real body is in the TU, VC picks retail's registers itself:
  - LoadBank 0xC17260: `log` in esi.
  - seed_loop 0xC92E20: p in ebx.
  - max_seeds 0xC93080: p in edi.
  - `_preextrapolate_helper` 0xC8F960: v in esi.

  A residual `add esp,N` after the call means retail's static is callee-pop, so declare it `static __stdcall`
  (0xC172F0, 0xC93E30). Try this before calling a static-register row a lane gap.
- **Inline methods on member sub-objects.** An inline method on a member sub-object (`m_ring.Forward(n)`) keeps
  `&m_ring` in a callee-saved register across a virtual call. Retail reloads through `this`, so write the update on the
  members (`m_ring.m_pos = ...`), as in 0xC75FF0 (307v303 -> 303v303).
- **Per-TU inlining of `_M_clear`.** STLport `vector::_M_insert_overflow(__false_type)` inlines `_M_clear` (null-tested
  free) in some TUs, as in 0xAAFE50 / 0xB7C870. The landed 0xB7FFC0 calls it out of line, so model each TU by its retail
  call graph.
- **`map::operator[]` with a nested map value.** It matches only with the full STLport `_Rb_tree` model:
  `less : binary_function`, a struct `_Rb_tree_iterator` with a user copy ctor, a 3-arg `_STLP_alloc_proxy`, and an inline
  `_Rb_tree` default ctor and dtor for the `mapped_type()` temporary (0xB988E0).
- **retail_asm with a scalar fallback.** When the scalar fallback keeps a register (edx) live across a FtoL call, FtoL's
  own `__asm` body must be in the same TU; an extern FtoL clobbers the assumption (0xC6B490).
- **strmbase `new` expressions.** A strmbase `new CRendererPosPassThru(NAME(...), ...)` matched with `/EHsc`, the landed
  0xCAD810 model, `#define NAME(x) 0` and `#define NULL 0` (0xCA6450).
- **Diff tooling.** g.sh output is cut by `head`, so a diff of its stdout can lose the tail. Write the full listing to a
  file before diffing.

Review at landing:
- **Land only the graded leaf of each source.** Several sources carry same-TU bodies of other functions: FtoL in
  retail_asm/00c6b490.cpp, LoadBank 0xC17260 in 00c172f0, `_preextrapolate_helper` 0xC8F960 in 00c8fa60, and
  seed_curve / seed_loop / seed_chase / max_seeds in 00c93e30.
- **seed_chase also graded here.** It has an oracle and graded DIFFER 360v362; it was not a target.
- **Parked rows that may close.** The chunk-447 parked seed_loop 0xC92E20 and max_seeds 0xC93080 likely close with the
  `static __stdcall` same-TU form. Retry the chunk 587/590 "static register lane gap" rows (e.g. `_fetch_headers`
  0xC8A0A0) the same way.
- **CUnknown ctor bindings look swapped.** 0xCACC80 is bound `PBD` but is called by CBasePin and CRendererPosPassThru,
  which pass wide names, so it is likely `PBG`. 0xCACB80 is bound `PBG` and is likely the char overload. Check this before
  respelling.
- **Two allocator spellings.** 0x143E514 has two spellings (`ImeUiCallback_Malloc` / `g_ImeMalloc`), and 0x143CF08 is bound
  `g_ImeFree` while this source uses the DXUT name `ImeUiCallback_Free`. Pick one per address. These are not in
  NAME_FIXES because the DXUT name is the source name, but the binding owner should decide.
- **Placeholder bindings on 0xB97580.** It carries placeholder bindings (CRetailTree_erase00b97580,
  CRetailEntryList_00b98230) next to the correct `_M_erase`.
- **Respell landed callers.** Apply NAME_FIXES.tsv to the landed callers:
  - 0xAAF800 AddRecord -> AddMatrix.
  - The InsertGrow stand-ins in 0xAAF800 / 0xAAF850.
  - global_EHepilog3_00c31d80, CChangingStateComponent::AddChildDynamically 0xC31D90 and CThingGraphicByPassFlagState
    0xC308F0.
  - CTypedContainer_00c30940 and EntitySetAsOpinionSource 0xC308A0.
  - The CRendererPosPassThru `PBD` spelling and CCombFilter `CDelayLine`.

  0xC76120 Blend is respelled by analogy only; its body was not graded here.
- **No row needed more than 4 grades.** No local base model sits next to an existing engine header.
