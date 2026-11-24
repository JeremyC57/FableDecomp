# NF279 header overlay (2026-11-22)

One header: `engine/CEngineWaterBackgroundSubPatch.h` (base copy in `orig/engine/`, taken from the repo working tree at the start of the round).

- **Opt-in `FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_VIRTUALS`** (new, add-only, 13 lines right after the class's `public:`; no existing line
  edited): the retail virtuals of vtable 0x012A8334 -- `virtual ~CEngineWaterBackgroundSubPatch()` (0x00BFD480, `??1...UAE@XZ`, Ego_r 00a4bba0 +
  FableWin 02e05150), the class delete `static void FABLE_FASTCALL operator delete(void*)` (FableWin `??3CEngineWaterBackgroundSubPatch@@SIXPAX@Z`
  02e05130; declared only, the destructor's TU defines it inline), and the two private overrides `RelocateData` (slot 1, 0x00BFD5D0, Ego_r
  00a4bd00 + FableWin 02e06160) and `CanRelocateData` (slot 2, 0x00BFD390, Ego_r 00a4bac0 + FableWin 02e06380), then `public:` again. Used
  with `FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22` (the existing `: public CMovableResource` path) and `FABLE_RETAIL_CMOVABLERESOURCE`
  (the polymorphic retail base). Users: rows 0x00BFD480, 0x00BFD5D0, 0x00BFD390 (and the fold 0x00BFDC90 through 0x00BFD480's file), test
  0x00BFD480.
- **Default path token-identical:** no landed row or test defines the new macro; whole-tree equiv_check (repo rebuild/include v the repo tree with
  this file swapped in, exact mode): see `evidence/e1_equiv.txt`.
- **Static asserts:** every assert in this header was already `FABLE_STATIC_ASSERT_AT(<line>, ...)`; `pin_asserts.py` (repo tree v overlay)
  pinned 0. The 13 added lines are above them, so their typedef names do not change.
- **Landed includers of the header** (source grep of rebuild/src/compiled, rebuild/include): rows 0x00BE6090, 0x00BE7BE0, 0x00B70230, 0x00BFD5D0,
  0x00BFD290, 0x00BFD3B0, 0x00BFD670, 0x00B6E010, 0x00B6FA40 and engine/CEngineWaterRenderer.h (+ its includers); none defines the new macro, so
  the opt-in changes none of them (equiv_check). 0x00BFD480's HEAD source mentions the header only in a comment.
- No other agent's overlay touches this header (NF278's hdr/ has no copy).
- Note for later users: the opt-in declares the class delete without a body. A TU that constructs or destroys the class under the opt-in (so emits
  its ??_G) must also see the inline class delete (as 0x00BFD480 defines it), or its ??_G calls an owner-less out-of-line `??3...SIXPAX@Z`.
