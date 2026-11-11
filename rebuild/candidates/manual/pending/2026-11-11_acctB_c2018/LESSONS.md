# Chunk 2018 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A table index written `GFGetTopBit(x) - GFGetTopBit(32)` keeps retail's `lea r,[y+x*4+1DFh]` member addressing: c2 folds the
  constant site only after address CSE. A literal `- 5` lets c2 share `x*4+y` in a register instead (0x00B2CB40).
- An inline header helper that retail expands everywhere and whose out-of-line COMDAT has no caller (GFGetTopBit 0x0099FDB0,
  CGraphicFrameData::Release 0x0041C090, CGraphicFrame::operator= 0x0041C0E0, CGraphicInfoFrame::GetFrame) is FABLE_FORCEINLINE; the c2
  inline trace shows the cascade -- forcing only the outer helper leaves an inner one called (Release c1 75, score 85 inside the
  expanded operator=), so force bottom-up (0x00B2D330).
- A large caller that passes `&local` to a same-TU callee retail compiled earlier (GetDimensionsAndAdjustMipmapLevel 0x00B2AEF0) needs
  that callee's body visible (`__declspec(noinline)` under `_MSC_VER`) or the frame and register assignment differ (0x00B2D330 frame
  0x48). The callee RenderToTexture (same chunk) was also kept visible: pair-landing.
- `std::pair<long,long> h = cond ? A[x][y] : B[x][y];` gives retail's single address ternary + two dword loads (0x00B2D646); naming
  `h.second` before StartTextureUse keeps the pushed handle.
- A C2DBoxF local filled field by field with two constant corners: c2 folds the constants into the users and leaves the two frame words
  unused, as retail does (0x00B2B180 frame); member order of the stores (TLY before BRX) decides the x87 order.
- Header include completeness: a `CArray<T>` member of an incomplete T compiles in one TU and fails C2036 in another as soon as the
  class's implicit destructor is instantiated; the branch header now includes the record's header after the class (both
  include-guarded) -- a TU that matched earlier can SRC_FAIL after an overlay change, so regrade every row at the end.

Review at landing:
- 0x00B2CB40 took 7 grades and 0x00B2D330 8 (each including one final regrade); 0x00B2B180 took 4.
- Pair-landing: land 0x00B2B180 and 0x00B2D330 together (src/00b2d330.cpp also defines RenderToTexture and GetDimensionsAndAdjustMipmapLevel).
- Already-landed sources with local models / non-PDB signatures used by these rows: 0x00B2AEF0 (long* / CTextureEntry_00b2aef0* /
  unsigned long*, public), 0x00B2B050 (CEngineSurfaceCompositionManager_Methods, const), 0x00B2B790 (non-const CGraphicInfoFrame*
  return, public), 0x00F38990 (CSurface*), 0x00F39A50 (CTexture*, SCompositeVertex*) -- NAME_FIXES.tsv.
- The SURFACECOMPOSITION branches of CGraphicDataBank.h / CGraphicInfo.h / CGraphicInfoFrame.h overlap the STATICBLOCKS models; merge them.
- reloc_bindings.tsv binds several names to 0x00B59710 (CEngineComponent / CEngineBaseGenerator / CEnvironmentBase ctors) and
  0x0099B7D0 (CWideString::operator= plus local aliases); not changed here (possibly folded bodies), worth a label_trust check.
