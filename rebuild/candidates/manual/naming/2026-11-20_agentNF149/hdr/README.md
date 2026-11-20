# NF149 header overlay

Two overlay headers, each a copy of the current repo header (orig/ holds the base copies; both checked byte-equal to the working tree at
hand-back) plus one `#ifdef` opt-in. Without the opt-in macro each header is token-identical to the repo copy, apart from the
FABLE_STATIC_ASSERT line pins described below. No other agent overlays these headers (NF148 has no hdr/, RP1 overlays only
CVsyncCallbackTimer.h).

## engine/CVertexBufferWin32.h
- Opt-in `FABLE_RETAIL_CVERTEXBUFFERWIN32_CTOR_INITIALISE` (inside the FABLE_RETAIL_CVERTEXBUFFERWIN32 branch, right after the default
  constructor): the inline five-argument constructor
  `CVertexBufferWin32(EVertexType, long, long, unsigned long, EVertexBufferPool)`, which sets PVertexBuffer / Stride to 0 and calls Initialise.
  Source: FableWin.pdb ??0CVertexBufferWin32@@QAE@W4EVertexType@@JJKW4EVertexBufferPool@@@Z (0x030A3300; Ego_r.pdb has no out-of-line copy, so
  it is inline in release) plus the retail body of 0x009E2830 (vtable 0x0129BA20, then the five arguments passed on to Initialise 0x00A63150).
  Used by: rows/009e2830.cpp (emits the COMDAT, nf86 scaffolding).
- pin_asserts.py pinned 3 asserts to their repo lines: `FABLE_STATIC_ASSERT_AT(29, sizeof(CVertexBufferWin32) == 0x34)` and
  `FABLE_STATIC_ASSERT_AT(47, ...)` / `(48, offsetof(..., __vftable) == 0x0)` (the flat branch).
- Landed includers: 37 files in rebuild/src/compiled include engine/CVertexBufferWin32.h. None defines the new macro (grep of
  FABLE_RETAIL_CVERTEXBUFFERWIN32_CTOR_INITIALISE over rebuild/src, rebuild/include, rebuild/tests: 0 hits). The default path is unchanged,
  so the opt-in changes none of them.

## engine/CEngineLandscapeMemoryManager.h
- Opt-in `FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_PROCESSDISPOSALLIST` (right after the class head, before the CTOR block, so it does not
  depend on another opt-in): `private: void ProcessDisposalList(); public:`.
  Source: Ego_r.pdb + FableWin.pdb ?ProcessDisposalList@CEngineLandscapeMemoryManager@@AAEXXZ (retail 0x00BDA150, which
  AllocateVertexBuffer / AllocateIndexBuffer call first with `this` still in ecx).
  Used by: rows/00bda3d0.cpp and rows/00bda360.cpp. The binding spelling is the one the landed HEAD row already used.
- pin_asserts.py: pinned 0 (this header's asserts are already FABLE_STATIC_ASSERT_AT or unchanged).
- Landed includers: 20 files in rebuild/src/compiled include engine/CEngineLandscapeMemoryManager.h. None defines the new macro (0 hits), so
  the opt-in changes none of them.

Every FABLE_RETAIL_* macro used in rows/ resolves in rebuild/include or hdr/ (the only two new ones are above).
