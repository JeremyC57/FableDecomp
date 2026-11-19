# hand4f lessons (2026-11-19)
- 8 of 10 rows closed in 1-3 grades each by porting the parked local-model draft onto the shared headers (stlport/fable_map.h,
  NUISystem_CList.h, CEngineWaterRenderer.h) with a LANDED sibling's opt-in set. Look for the landed sibling of the same template /
  TU first (0x00553E60 for the map operator[] family, 0x005383DD for the ui_list pragmas, 0x00BF5C60 for the shader getters).
- A dead `mov edi,ecx` (this saved across a call, never read) + a loop with no exit test = an unreachable END after a loop whose
  condition existed at register assignment: the FableWin loop + `__assume(0)` after it matched (0x005DC340); dead-arm asserts, empty ifs,
  folded conditions and fall-off-the-end do not. Accepted as an unreachable-END marker (EXTRA_RULES 2026-11-19, with the __GNUC__ twin).
- A new object kept in a callee-saved register with the constructor's return ignored (`mov eax,edi` after the call) = the called
  constructor is a BASE constructor from an inline derived ctor: STLport list's inline default ctor + out-of-line _List_base ctor
  (FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE). Rename the callee accordingly (0x00535140).
- Retail STL COMDAT copies of helpers can come from an /O2 TU while the row's TU is /Os: grade helper bodies at their own address
  without the row's size pragmas before calling them non-retail.
- VC7.1 checks access in explicit instantiations: a protected nested value type needs a derived-class typedef to name it.
- Map operator[] tests need STLport's out-of-line _Rb_global<bool>::_Rebalance: a test-only no-rotation stand-in keeps a valid BST.
