# hdr/ overlay of row 0x00601310 (full copies of the current repo headers plus opt-ins; default text unchanged)
- engine/CTCInventoryMap.h: FABLE_RETAIL_CTCINVENTORYMAP_QUESTSSCREENPC -- public declaration of ConstructQuestsScreenPC (Ego_r 0x00655BC0, FableWin 0x020ABAB0). USED.
- engine/NUISystem_CAction.h: FABLE_RETAIL_CACTION_INLINE_CTOR (copied from agent4007's overlay, chunk 882 text). USED.
- engine/CMainGameComponent.h: FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS (agent4007's block re-based onto HEAD). Probe only (+15 g).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND (chunk 883 text), probe-only new blocks FABLE_RETAIL_STLP_LIST_BASE_CTOR_46 /
  _ALLOC / _STORES, FABLE_RETAIL_STLP_MALLOC_ALLOC_LAYER (+ _RESULT, FABLE_RETAIL_STLP_ALLOCATE_IF / _NAMED), FABLE_RETAIL_STLP_LIST_PUSHBACK_TEXT 2..6.
  None is used by differ/; they are inline-budget probes (see NOTES.md).
- engine/NUISystem_CActionParamULONG.h: FABLE_RETAIL_CACTIONPARAMULONG_BODY_ASSIGN (probe, no effect).
- engine/CCountedPointer.h: FABLE_RETAIL_CCOUNTEDPOINTER_CTOR_TERNARY / _CTOR_INFO0 (probes; INFO0 changes code, TERNARY no budget effect).
