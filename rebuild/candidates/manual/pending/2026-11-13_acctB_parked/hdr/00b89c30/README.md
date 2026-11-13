engine/CRenderTarget.h: NEW overlay-only header (no repo header), copied from chunk 2008 (agent3004/hdr): CBaseClass base, RenderToSurface +4 / ZBufferSurface +0x14, ctor(CSurface&,CSurface&) 0x00A10DC0, AttachTarget 0x00A10E60, Uninitialise 0x00A10E30.
FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET (engine/CDisplayManager.h): chunk 2008's opt-in, bool SetRenderTarget(CRenderTarget&) 0x009BF160.
FABLE_RETAIL_CDISPLAYMANAGER_GETRENDERTARGET (engine/CDisplayManager.h): inline GetRenderTarget() by value (FableWin debug-only), includes engine/CRenderTarget.h.
FABLE_RETAIL_CSURFACE_ASSIGN (engine/CSurface.h): CSurface& operator=(const CSurface&) 0x009F2E70 declaration.
FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS (engine/CEngineScreenEffectOutlineGlow.h): typed own members in Ego_r order + private ApplyOutline decl.
engine/CRenderStateManager.h: full copy of the current repo header + FABLE_RETAIL_CRENDERSTATEMANAGER_LOCAL_MASK (probe only: SetRenderState with a local copy of CurrentBookmarkMask; NOT used by the best source -- it makes every block mask-first).
