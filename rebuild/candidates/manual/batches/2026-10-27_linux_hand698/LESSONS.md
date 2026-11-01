# Chunk 698 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 698) In a propagated-label destructor band, rows with NO callers are still nameable: Ego_r donor order (??1Base then
  ??1Derived, 0x0051D530 -> 0x0051D590 CNavigationAreaUpdate / CNavigationAreaInit), FableWin's compiler-generated ??0(const&) /
  ??1 pair next to an unreferenced copy ctor (0x006F3000 / 0x006F2FD0 CCombatDebugInfo), or the class's own ??_G expanding the
  same body inline (0x00AA7370 / 0x00AA7320 C3DMeshFileXUVTangentChunk).
- (chunk 698) Real STLport templates let VC7.1 pick the out-of-line split itself: `~list() {}` over `~_List_base() { clear();
  deallocate(); }` with clear() defined in the TU gives retail's out-of-line ??1?$_List_base (0x00B43BB0) from a user dtor, and
  ~vector<vector<T*> > stays a call (0x007DC890) while the flat vectors expand (0x007DCA90); with clear() declared only, the
  implicit dtor inlines ~_List_base around the clear() call (0x0072DA60).
- (chunk 698) A user dtor whose arms are same-TU members with their own guard (`if (ZlibInitialised) { if (Compressing)
  FinishCompressStream(); else FinishDecompressStream(); }`) folds the inner guards and merges the `flag = false` stores at the
  join (CCompressorZlib 0x00A3ACC0); a z_stream cleared through `SetupStream(z_stream_s&, 0, 0, 0, 0)` gives the esi-relative
  zero stores. zlib callees are `z_`-prefixed (Z_PREFIX; both PDBs spell _z_inflateEnd / _z_deflateEnd).
- (chunk 698) DirectShow BaseClasses dtors match verbatim from amfilter.cpp with wxutil.h's inline `~CCritSec()` added to the
  landed strmbase model; CDynamicOutputPin's four vptr stores land between the first load and its branch on their own
  (0x00CA9690, /EHsc).
- (chunk 698) A member CStaticMapBankFile whose implicit dtor adds nothing calls the base ~CBankFileAsync directly (0x00B42E70):
  model the derived class with no dtor and the base dtor declared virtual.

Review at landing:
- Six rows (0x0072DA60, 0x007DCA90, 0x004D1D50, 0x0051D590, 0x006F2FD0, 0x00AA7320; none of the user-dtor rows) carry the
  ungraded `DestroyOne` emission helper (`p->T::~T()`), the accepted chunk 656/657/696/697 pattern for out-of-line implicit `??1`.
- 0x00CA9690 needs `// Flags: /EHsc` (strmbase convention); the object grades the same leaf.
- Respell the landed rows / bindings in NAME_FIXES.tsv in the same landing: 0x007DC890, 0x00B40000, 0x00B42C70, 0x00B43BB0,
  0x0051D530, 0x006F3000, 0x00AA7370 (+ its QAE ~CChunkedFileChunk binding), 0x00A3ABB0, 0x00C038D0 / 0x00C00AE0 zlib
  bindings, 0x0072DA40 / 0x00A399E0 free __fastcall ??_G stand-ins, 0x009D6490 and 0x00CA7C80 invented bindings.
- The landed CEngineWorldMap rows (0x00B40000, 0x00B42C70) model MapList as a hand CArray {m_pBegin, m_pEnd, m_pCapacity}; the
  real STLport vector (this chunk's 0x00B42E70 model) is available.
- No row needed more than one probe; no retail_asm, no volatile, no pragmas.
