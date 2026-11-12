# Q21 header overlay (grade with `export FABLE_HDR_OVERLAY=$S/w22/chunkQ21/hdr`)

Every file is a full copy of the CURRENT repo header (re-diffed against rebuild/include at hand-back, 2026-11-12: additions only, 0 removed
lines) plus `#ifdef FABLE_RETAIL_*` opt-ins; the default path of every header is unchanged. No new headers. No overlap with the Q20
overlay ($S/w22/chunkQ20/hdr).

| header | opt-in | change | rows |
|---|---|---|---|
| engine/C3DMeshInfo.h | `FABLE_RETAIL_C3DMESHINFO_LOADSTATS` (inside the FABLE_RETAIL_CMESHDATABANKENTRY branch) | declares Ego_r.pdb `IsLoadedStats() const` 0x00A23E80 and `BeginLoadingStats(bool) const` 0x00A24A80; inline FableWin.pdb `C3DAnimationManager& GetAnimationManager() const { return PeekBank()->GetAnimationManager(); }` (retail reads Bank +4 then +0x3C0 in place) | 0x00B39080 (match) |
| engine/CMeshDataBank.h | `FABLE_RETAIL_CMESHDATABANK_GETANIMATIONMANAGER` | inline FableWin.pdb `C3DAnimationManager& GetAnimationManager() const { return *AnimationManager_Data; }` | 0x00B39080 (match) |
| engine/C3DMeshStats.h | `FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS` (#error without FABLE_RETAIL_CMOVABLERESOURCE) | declares `GetDummyObject(unsigned long,unsigned long) const` 0x00A2E920 and the six pure virtuals C3DMesh2 overrides at slots 4..9 (GetHelperPointPositionAnimated x2, GetDummyObjectPositionAnimated x2, GetDummyObjectTransformAnimated x2; one-state before two-state in each pair so VC's reversed overload order gives slot 8 = two-state GetDummyObjectTransformAnimated 0x00A8A5F0, called through +0x20) | 0x00B39080 (match) |
| engine/CEngineInternalAttachedPrimitive.h | `FABLE_RETAIL_CENGINEINTERNALATTACHEDPRIMITIVE_PARENTTRANSFORM` | declares `bool GetParentTransformAnimatedMesh(CMatrix3x4&,float)` (Ego_r / FableWin QAE) | 0x00B39080 (match) |
| engine/CEngineInternalPrimitiveOcclusionBase.h | `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO` (retail branch) | declares the class's own `virtual void ResetOcclusionInfo(EEngineOcclusionLayer)` = vtable 0x012A3ED4 slot 28 (0x00B90390, Ego_r UAEX). Without it every mesh-base virtual sits one slot low (CalcInterpolatedObjectTransform +0x80 instead of retail +0x84) | 0x00B39080 (match) |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY` | FABLE_FORCEINLINE FableWin.pdb `SetMultiply(const CMatrix3x4& a, const CMatrix3x4& b)` (this = a*b, the PostMultiply sums with both operands as parameters). PostMultiply's internal copy swaps the row-2..4 operand order (`fld b ; fmul a`) | 0x00B39080 (match) |
| engine/card_sorting_types.h | `FABLE_RETAIL_CCARD_POSITIONS` (#error without FABLE_RETAIL_C3DVECTOR_PACKED) | types CCard LocalPos / NewLocalPos / CurrentPos (+0x27/+0x33/+0x3F) as the Ego_r.pdb C3DVector (packed, same offsets / size) | 0x007DAEF0 (DIFFER) |
| fable_ime.h | `FABLE_RETAIL_CIME_ISIMEACTIVE` (retail CIME branch) | declares Ego_r.pdb `bool IsIMEActive() const` 0x00404C50 | 0x00540320 (DIFFER) |
| engine/NUISystem_CText.h | `FABLE_RETAIL_CTEXT_GETCHARWIDTHW` | declares Ego_r.pdb `static unsigned int __fastcall GetCharWidthW(wchar_t)` 0x0054F4B0 (needs /Zc:wchar_t) | 0x00540320 (DIFFER) |
| engine/NUISystem_CText.h | `FABLE_RETAIL_CTEXT_WINDOW_SLOTS` | retail order of slots 147..150: SetTextWindow 0x0054FFB0, GetTextWindow 0x0054FFE0, ForceWidth 0x0054FB10, GetAlignment 0x0054FFF0 (read from the bodies; the catalog agrees). **The default order in the repo header is wrong** (Alignment/ForceWidth/SetTextWindow/GetTextWindow with mismatched addresses); no landed row calls any of the four, so the coordinator can make the opt-in the default | 0x00540320 (DIFFER) |

Only the first six rows back the MATCH (0x00B39080). The matched source also needs these existing repo opt-ins (defined in the row):
CENGINEINTERNALPRIMITIVEBASE_ATTACHED, CENGINEPRIMITIVEHANDLE(_DTOR), CENGINEPERSISTENTPRIMITIVEBASE, DEF_CLASS_BASE, CIVCOUNTEDPOINTER(_CONVERT),
CCHARSTRING(_GETCRC_INLINE), C3DVECTOR, CENGINEINTERNALPRIMITIVEOCCLUSIONBASE, CMESHDATABANK_BASES, CBANKFILE_ENTRYQUERY, CMESHDATABANKENTRY,
C3DMESHINFO_GETMESHSTATS, CMOVABLERESOURCE, CCOUNTEDPOINTER_DTOR_DECL_ONLY, CMATRIX3X4, CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE.
