# hdr/ overlay (agentC2). Each file = a copy of TODAY's rebuild/include header plus NEW opt-in blocks only (no default path changed).
# Opt-ins re-applied from the bwf1 draft of 0x00651B04 (their stale copies were not used). FABLE_STATIC_ASSERT lines after an inserted
# block move down by the block's line count (CInputManager.h: the size assert keeps its line; the extern block goes after it); run
# lane_merge/pin_asserts.py at landing for: CActionInputControl.h, CFontBank.h, CGamePlayerInterface.h, CIVCountedPointer.h, CInputManager.h,
# CPlayerGui.h, CTCInventoryAbilities.h, CTCQuickAccessMenu.h, CTCSpecialAbilities.h, CTCWeapon.h, CUserProfileManager.h, NCreatureMode.h.
engine/CActionInputControl.h	FABLE_RETAIL_CACTIONINPUTCONTROL_DEFAULT_CTOR	inline zeroing default ctor	row 0x00651B04 bytes 0x0065279C..0x006527BA
engine/CFontBank.h	FABLE_RETAIL_CFONTBANK_VSTRINGEXTENTS_OUTOFLINE	GetVStringExtents declared (call 0x00437210), the inline body kept under #else	row calls 0x00437210
engine/CGamePlayerInterface.h	FABLE_RETAIL_CGAMEPLAYERINTERFACE_GETACTIONNAME	static __fastcall GetActionName in the retail class branch (+ fwd decls)	Ego_r ?GetActionName@CGamePlayerInterface@@SI_N...; call 0x00446720
engine/CIVCountedPointer.h	FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_FORCEINLINE	default Release body FABLE_FORCEINLINE (new #elif before RELEASE_INNER)	expanded at 0x00651B85..0x00651B96 (size TU)
engine/CInputManager.h	FABLE_RETAIL_CINPUTMANAGER_JOYSTICKQUEUE	IsJoystickEventInQueue decl + extern PInputManager	Ego_r name; 0x009F4C60; global 0x013B8388
engine/CPlayerGui.h	FABLE_RETAIL_CPLAYERGUI_ABILITYBUTTONS_DRAW	GetButtonAlpha / GetAbilityButton / GetAbilityButtonScale decls + inline Set/Is/Reset accessors	Ego_r + FableWin names; 0x00439AB0 / 0x00438990 / 0x004375E0
engine/CTCInventoryAbilities.h	FABLE_RETAIL_CTCINVENTORYABILITIES_SPECIALBUTTONS	six out-of-line queries (two shared with _AVAILABILITY, declared only without it)	Ego_r names; 0x005EE31D .. 0x005EE24B
engine/CTCQuickAccessMenu.h	FABLE_RETAIL_CTCQUICKACCESSMENU_ISACTIVE	IsActive() const decl	Ego_r/FableWin; 0x00625930
engine/CTCSpecialAbilities.h	FABLE_RETAIL_CTCSPECIALABILITIES_ISABILITYAVAILABLE	IsAbilityAvailable decl	FableWin; 0x0072C430
engine/CTCWeapon.h	FABLE_RETAIL_CTCWEAPON_GETWEAPONTYPE	GetWeaponType decl + enum EWeaponType fwd	Ego_r/FableWin; 0x005DA7D0
engine/CUserProfileManager.h	FABLE_RETAIL_CUSERPROFILEMANAGER_HUDTOOLTIP	GetAssignedInputForAction decl + inline IsHUDTooltipEnabled with own pads	Ego_r/FableWin; 0x00408C90; +0x0C/+0x19/+0xD9 reads
engine/NCreatureMode.h	FABLE_RETAIL_NCREATUREMODE_HERORUN	IsAvailableForHeroRun decl	Ego_r; 0x0085F560
# CPlayerGuiDef: no overlay -- the row uses the existing repo opt-in FABLE_RETAIL_CPLAYERGUIDEF_CLASS (retail class, C2DVector members) instead of the draft's ABXY block.
# Row 0x00BB3E80 (unmatched, DIFFER 20 fg hunks) -- opt-ins re-applied from the bwf1 draft or new; all additive, defaults unchanged:
engine/C3DMeshMaterial.h	FABLE_RETAIL_C3DMESHMATERIAL_FLAGS	GetSelfIllumination / IsTwoSided / IsTransparent (shared ones only without _PARTICLEMESH / _FLAG_ACCESSORS)	FableWin inline, +0x24/+0x28/+0x29
engine/C3DPrimitive2.h	FABLE_RETAIL_C3DPRIMITIVE2_RENDER	BoundingSphere typed C3DBoundingSphere + GetMaterialIndex/GetBoundingSphere/GetAverageTextureStretch	Ego_r type, FableWin accessors (chunk 2060)
engine/C3DVector.h	FABLE_RETAIL_C3DVECTOR_NAMED_RESULT_SUB / _MAGNITUDE_NAMED_SQUARED	named-result operator- ; GetMagnitude squared X,Z,Y	chunk 2060 (FableWin 34i; retail 0x00BA547A order)
engine/CEngineLightingManager.h	FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT	CEngineLocalLightArray::GetCount + SetupShaderSpotLight decl 0x00B47390	FableWin 9i; Ego_r name
engine/CEngineOcclusionManager.h	FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ALLOCATE	COccluderEntry + forceinline AllocateOccluderHandle (landed 0x00B91140 text) + Start/EndOcclusionTest decls	Ego_r / FableWin
engine/CEnginePrimitiveManagerMeshBase.h	FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP	static fastcall GetAppropriateMipmap decl (body in row)	FableWin name
engine/CEnginePrimitiveOcclusionInfo.h	FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONINFO_ADDOCCLUSIONTEST	AddOcclusionTest decl + private ClearHandles	FableWin; Ego_r copy 0x009D01F0
engine/CEnginePrimitiveRenderUtil.h	_MESHPRIMITIVE (chunk 2060's former _MESHRENDER content, renamed: that name is now chunk 2028's), _CLOTHSHADERS (GetPixelShaderXxx getters, fwsize SAME), _CLOTHRENDER (outline set-up decls)	see NAME_FIXES for 0x00BF0B20
engine/CEnginePrimitiveRendererSharedData.h	FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH	nested in _MEMBERS: CurrentOcclusionLayer/MipmapSizeMultiplier/OcclusionEnable + outline/shadow pixel shaders	Ego_r offsets
engine/CEngineShadowRenderer.h	FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC	CColorBufferDepthPackParameters + SetupShadowFuncRenderStates decl 0x00B4FC40	Ego_r
engine/CEngineSubPrimitiveBase.h	FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS	PeekRenderFlags / PeekAlpha (+Get/PeekParentBase without _PARENTBASE), data protected	FableWin 9i each
engine/CEngineSubPrimitiveRepeatedStaticMesh.h	FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_ACCESSORS	GetScale / GetRenderMode / PeekParent on the EXISTING CEngineSubPrimitiveMeshBase model	FableWin
engine/CGraphicInfo.h	FABLE_RETAIL_CGRAPHICINFO_GETWIDTH	GetWidth = header FrameWidth (+6)	FableWin 9i; FITTED word
engine/CRGBFloatColour.h	FABLE_RETAIL_CRGBFLOATCOLOUR_GETRGBCOLOUR_NAMED_RESULT	channels into named bytes, R,G,B,A, then built	retail 0x00BB415C..; FITTED
engine/CEngineInternalPrimitiveMorphedStaticMesh.h	(new header, no repo header for the class)	chunk 2060 model, unchanged
# agentC3: no header changes so far (hdr/ = agentC2's overlay, unchanged). Probe-only header copies live in probe/hdrp (never hand in).
# agentC6 (2026-11-13):
fable_assert.h	FABLE_RETAIL_ASSERT_DEAD_ARM	NEW shared header (no repo file of that name): FABLE_RETAIL_ASSERT(c) = Lionhead's ASSERT in its release form, the debug control flow (FableWin: DoErrorMessage -> IsDebuggerPresent -> int 3) behind `if (0)` with plain-pointer arguments; ((void)0) without the opt-in or under FABLE_PORTABLE; GFAssertFailed is DESCRIPTIVE (never emitted)	row 0x00BB3E80 (SetRenderState test/or operand order of 16 expansions 0x00BB46E0..0x00BB4E80: matches with 9..10 such asserts; window measured 8 no / 9,10 yes / 11 no). No FABLE_STATIC_ASSERT moved (new file).
engine/CEngineClothRenderer.h	FABLE_RETAIL_CENGINECLOTHRENDERER_RENDER	nested in _MEMBERS: RenderClothPrimitive / Render2DClothPrimitive (public) + RenderPolygons 0x00BB2540 / inline ApplyTriVertexShader (private) declarations, forward decls + CEngineRenderEnums.h include	Ego_r/FableWin names; rows 0x00BB3E80 / 0x00BB30A0 (they used a LOCAL CEngineClothRenderer before: shared-header rule). The size assert MOVED (line 79 -> 101): pinned as FABLE_STATIC_ASSERT_AT(79, ...) in the overlay copy.
engine/C3DClothPrimitive.h	(new header, no repo header for the class)	Ego_r layout + FableWin inline accessors GetParticleProgram / GetMaterialIndex (the text the rows had locally)	struct_layouts_egor.tsv; rows 0x00BB3E80 / 0x00BB30A0
