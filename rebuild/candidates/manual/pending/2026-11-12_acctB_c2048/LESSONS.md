# Chunk 2048 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00B64FA0) The per-block operand order of SetRenderState's `state.BookmarkMask |= CurrentBookmarkMask` (which register the `or` writes,
  `or bookmark,mask` v `or mask,bookmark`) is a c2 record-id tie: it moves only with the function's total IL / record count (dead locals anywhere,
  start or end, move it identically; spellings, names, TU context and header text of the setter do not). The header's "lane gap" note on
  bookmark test/or order is this. Diagnose with a dead-local sweep read from the object (free); a dead-local build that matches proves the
  row needs only the right IL amount.
- (0x00B64FA0) In a long straight block an inline `value` parameter copy gets priority 2K-2 and beats the bookmark temp unless the `or` result is
  coalesced with the bookmark temp (5 refs): one operand-order tie decides a whole block's registers (cmp [mem],reg + copy to edi).
- (0x00B64FA0) Code-neutral IL levers for record ties: `C4DVector(C3DVector(r, g, b), w)` instead of `C4DVector(r, g, b, w)`; named locals and
  references add no c2 record chunks. Chunk counter: hook c2.dll 0x107603EB (new record chunk) per function (bin/c2h7/chunks.py).
- (0x00B64FA0) FableWin lists ?GetZeroRGBATexture@CEngineResourceManager@@QAEPAVCTexture@@XZ right after RenderClouds: the missing-texture
  fallback is `GPResourceManager->GetZeroRGBATexture()` (+0x2AD4).
- (0x00B64FA0) VC7.1 does not unroll `for (stage = 0; stage < 4; ++stage) { SetTextureUVModeU(stage, m); SetTextureUVModeV(stage, m); }`:
  retail's 8 consecutive state blocks are 8 statements.

- (0x00B64FA0, session 2) FableWin debug instruction counts (signatures.tsv col 5) v VC7.1 /Od /RTC1 of our text (bin/dbg.py): VC7.1 adds 4
  per function; RenderOuterSky 219 = 223-4 exactly (its matched text), the CRenderStateManager setters equal. SetRenderState (213),
  AddStateToUpdateList (108), the scoped capture block (93 / 92), PeekGraphic (170) are FableWin debug bodies far beyond any release form.
- (0x00B64FA0, session 2) Code-neutral helper layering (TouchFrame / GetBank / IsLoading in GetFrame, GetR accessors, a `bool found`, if-return
  PeekGraphic) does not move the or-pattern; a scoped capture block derived from CRenderStateCaptureBlock with inline Open bodies moves
  only block 5. Combined layers interact non-additively (TouchFrame cancels the capture block's flip).
- (0x00B64550) The render-pass texture fetch that BeginLoading()s an unloaded frame is `PeekGraphicInfoFrame(id, 0)` + a two-step loaded test
  (`if (Data.IsNull()) return false; return Data->IsLoaded();` gives retail's je + setne), then CGraphicInfo::GetFrame(frame, 0, 0, flags)
  out of line on `PeekGraphic(id)`. `if (!(z > 0.01))` (double literal) is retail's `fcom qword ; test ah,41h ; jne`.
- (0x00B64550) CEngineCamera::TransformWorldToView must be FABLE_FORCEINLINE (plain inline is called; retail expands it).

- (0x00B64550) Two CGraphicFrame locals in sibling scopes share a frame slot only when the second sits in its own block (`if (id != 0) { .. }`),
  not after the loop in the enclosing block; that block also gives each early return its own RestoreCaptureBlock epilogue (retail 0x00B64DB7).
- (0x00B64550) `C2DVector(a - c, b - d).GetMagnitude()` on an unnamed temporary plus the same differences repeated later = retail's x87
  `fst [slot] ; .. fld st(0) ; fmul st(1) ; fld st(2) ; fmul st(3) ; faddp ; fsqrt ; fstp st(2) ; fstp st(0)`; named float dx/dy or named
  C2DVectors give memory operands (and named by-value C2DVectors grow the frame).
- (0x00B64550) A `continue` before a loop-body CGraphicFrame leaves the destructor's zero store live; if/else around the frame removes it.
- (0x00B64FA0) FableWin debug sizes (fwsize.py) are a text oracle for inline helpers, and FableWin is /EHsc: compare rows with /Od /EHsc.
  Helper texts change the c2 record-id ties even where the code is identical (IsLoading's folded flag-16 path moved RenderClouds' or-pattern).
- (0x00B64FA0) A plain-inline helper newly layered under a FABLE_FORCEINLINE one can be refused in other rows (ReleaseData called out of
  line in RenderSunFlare / RenderOuterSky): regrade every row of the TU after a helper refit.

- (0x00B64FA0, session 4) The SetRenderState bookmark test/or order (GOTCHAS "lane gap") has two transitions over the 12 blocks: the front
  one moves with the function's NAMED-SYMBOL count (any named local, dead or live, anywhere; also named locals inside expanded inline
  helpers: one in SetRenderState = one per expansion), the back one with the constants' spelling (C3DVector-based C4DVectors), never with
  dead locals. Ruler: append N `const long deadK = K;` (diagnostic only) and read the pattern with bin/orsweep.sh; then supply N names
  naturally. Closed by FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED (`const unsigned long desired_state = state.DesiredState;`).
- (0x00B64FA0) Helper layering without a named local (CastToBase, ~CCountedPointer -> ReleaseData, GetTexture through Get) adds records
  but not named symbols and did not move the front transition; a helper call layer in an inline (pointer Release through IsNull()) moved it
  by about one band.
- (0x00B64FA0) FableWin: CIVCountedPointer<CFrameData>::Release 17i (= `if (!IsNull()) { Object->Release(); Object = 0; }`), pointee
  Release 24i (= named `unsigned long count`, self-delete), private CastToBase 10i/24b (= `CIVCountedPointeeBase* base = object; return
  base;`), listed right after Release in the CFrameData pointer's COMDAT run.

Review at landing:
- 0x00B64FA0 RenderClouds RELOCATION_MATCH needs the session-4 opt-ins (RELEASE_ISNULL, RELEASE_COUNT, SETRENDERSTATE_DESIRED) and the
  hdr/ copy of CIVCountedPointeeBase.h; define the same three in 0x00B64550 (cand/s74_00b64550.cpp) for one body per helper.
- Rename the repo label g_textureUVModeToD3D (0x01396DC8) to CRenderStateManager::D3DTextureUVModeTable (vland RELOC_NAME warning).
- 0x00B64FA0 parked at 2688v2688 (needs the c2031/r3014 overlay + this chunk's two opt-ins); its diagnostic probe/v/se40.cpp (dead locals) is
  NOT deliverable.
- 0x00B64550 RenderSunFlare: RELOCATION_MATCH (session 3); needs the overlay incl. FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS.
- 0x00B64FA0 RenderClouds: 731/734 with the session-3 opt-ins (hdr/README.md); not landable yet.
- Overlay session-2 opt-ins are candidates/drafts; only RENDERSUNFLARE, BEGINLOADING, GETFRAME3, VIEWTOSCREEN, PEEKSUNVECTOR, FLARE and CSkyDef.h are used (by the draft).
