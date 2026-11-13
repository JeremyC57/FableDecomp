## 0x007E1FC0

engine/CTCAnimatingObject.h: FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION_NAME declares public PlayAnimation(const CCharString&, long, bool) 0x007F90B0 (Ego_r.pdb mangling); declaration only.
engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_XYZ -- Normalise() as GFFastOneOverSqrt((X*X)+(Y*Y)+(Z*Z)) (the PAREN_SQUARES_ZYX body summed X, Y, Z); body only, default path unchanged (row 0x007E1FC0).
engine/C3DVector.h: line 380 rewritten -- repo `#if defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX)` became `#if defined(..._XYZ)` (new body) `#elif defined(..._ZYX)` (original body unchanged); the only non-added line in the overlay, default path and every existing flag compile the same.

## 0x004BD6E0

- engine/C3DAnimationBlendState.h + engine/C3DAnimationInterpolator.h: FABLE_RETAIL_C3DANIMATIONINTERPOLATOR_IMPLICIT (row 0x004BD6E0) -- drop the user-declared ctor/dtor so both are compiler-generated and expand in place, as retail does in SetPrimitive.
- engine/NGraphicAppearance_CTCGraphicAppearance.h: FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETPRIMITIVE (row 0x004BD6E0) -- protected SetPrimitive declaration + inline GetUpdatePrimitiveReasons() const (constant 4).
- engine/CTCHeroAttachableAppearanceModifiers.h: FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_HIDDENMATERIALS (row 0x004BD6E0) -- GetHiddenMaterialNames(std::set<CCharString>&) const, 0x00706860.
- stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE (row 0x004BD6E0) -- _Rb_tree::_M_erase declared __declspec(noinline) (retail calls the 0x004ABA70 COMDAT from the inline clear).

## 0x00A24520

# hdr overlay (row 0x00A24520 C3DMeshLODInfo::BuildMesh) -- full copies of the current repo headers plus these opt-ins only
- engine/C3DMeshLODInfo.h: FABLE_RETAIL_C3DMESHLODINFO -- real class (CResource base, CCountedPointer Mesh/FileData, MeshInfo, LOD, UsedMeshStats), private BuildMesh decl, inline Sizeof (body of landed 0x00A244E0).
- engine/C3DMesh2.h: FABLE_RETAIL_C3DMESH2_BUILDMESH -- decls of ctor 0x00A89190, LoadBinary 0x00A8AD40, CalculateTextureChangeFlags 0x00A89380, Lock/UnlockPrimitive 0x00A89250/0x00A892A0, Sizeof 0x00A893C0.
- engine/C3DMeshStats.h: FABLE_RETAIL_C3DMESHSTATS_MESHGENERATORS -- decl InitialiseMeshGenerators(const CMeshDataBank&, const CCharString&) 0x00A2EC40.
- engine/CBankFile.h: FABLE_RETAIL_CBANKFILE_ENTRYSYMBOL -- decl GetEntrySymbol(unsigned long) const 0x009CC1D0.
- engine/CResourceList.h: FABLE_RETAIL_CRESOURCELIST_MAKEFREESPACE -- decl bool MakeFreeSpace(unsigned long) 0x009FC4F0.
- engine/CBankFileAsyncData.h: FABLE_RETAIL_CBANKFILEASYNCDATA_SIZEOF -- decl Sizeof() const 0x009D5620.
- engine/CMeshDataBankEntry.h: FABLE_RETAIL_CMESHDATABANKENTRY_LODINFO_FRIEND -- `friend class C3DMeshLODInfo;` (BuildMesh reads Bank/Index in place; the BUILD path makes Bank private).
- engine/C3DPrimitive2.h: FABLE_RETAIL_C3DPRIMITIVE2_UPLOADBUFFERS -- decls UploadAnimatedBuffers 0x00A92870 / UploadRepeatedBuffers 0x00A932C0 / UploadStaticBuffers 0x00A93320.
- engine/CResource.h: FABLE_RETAIL_CRESOURCE_SETRESOURCESIZE -- decl SetResourceSize(unsigned long) 0x009FBF20 (skipped when the STATICBLOCKS+CGRAPHICINFOFRAME_FREE pair already declares it).

## 0x0054FBC0

engine/NUISystem_CText.h: FABLE_RETAIL_CTEXT_FITINWINDOW -- declares protected long FitInWindow(CWideString&, const C2DBoxF&) 0x0054F8E0 (inside FABLE_RETAIL_CTEXT_VSLOTS).
engine/CSubtitleRenderer.h: FABLE_RETAIL_CSUBTITLERENDERER_FORMATTEXTWITHGAMEACTIONNAMES -- declares static __fastcall FormatTextWithGameActionNames 0x004917B5.

## 0x004C0560

engine/C3DMeshObject.h (copy of current repo header): FABLE_RETAIL_C3DMESHOBJECT_INLINE -- inline ctor `: InversePositionMatrixSetup(false) {}`, inline empty virtual dtor `{}` (FableWin.pdb debug ~C3DMeshObject = 84-byte /EHsc `{}` body), inline IsAnimated() { return Mesh->IsAnimated(); }; includes C3DMeshStats.h with the opt-in below (row 0x004C0560).
engine/C3DMeshStats.h (copy of current repo header): FABLE_RETAIL_C3DMESHSTATS_ISANIMATED -- inline IsAnimated() const { return AnimatedFlag; } (FableWin.pdb ?IsAnimated@C3DMeshStats@@QBE_NXZ; retail reads [stats+7Ah]) (row 0x004C0560).

## 0x009C9280

# Header overlay for row 0x009C9280 (full copies of the current repo headers + #ifdef opt-ins; default paths unchanged)
- engine/NGameText_CDataBank.h: FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT -- CDataBank : CBankFileAsync with Entries +0x1E0 / Narrators +0x204 / mutable Seed +0x210, inline GetGroupInfo, BeginLoadingText + GetTextEntryFromGroup decls; CDataBankEntry / CDataBankTextEntry (PText, NarratorIndex, GetNarratorIndex, IsLoaded decl) / CDataBankGroupEntry; CTextEntry PFileData +0x38 (CCountedPointer<CBankFileAsyncData>); CGroupEntry::GetTextEntries (inline; FableWin.pdb name only). Needs FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS + FABLE_RETAIL_C3DANIMATIONINFO_BUILD in the row (CBankFileAsync / IsFinished).
- engine/CBankFileAsync.h: same opt-in also enables the class and declares AsyncUpdatePending() (0x009D68B0).
- engine/CIVCountedPointer.h: FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE -- FABLE_FORCEINLINE on the ASSIGN operator= (VC7.1 inline budget; /d2inlT100 does the same).
- engine/GFRandom.h: FABLE_RETAIL_GFRANDOM_RESULT_LOCAL -- same GFRandom body with a zero-initialised result local (`xor edx,edx` before the range test).
- fable_string.h: FABLE_RETAIL_CBASICSTRING_COMPARE_BODY -- the real template body of CBasicString<T>::Compare (landed 0x00411570) visible in the TU (still called).

## 0x00646F67 (not landed)

engine/CPlayerGuiDef.h: FABLE_RETAIL_CPLAYERGUIDEF_EXPERIENCE -- C2DVector/CRGBColour typed Experience* members (+0x3d4..+0x40c), same layout.
engine/CPlayerGui.h: FABLE_RETAIL_CPLAYERGUI_FRIEND_EXPERIENCE -- friend NPlayerGui::CDrawExperience (reads Alpha / PStatUpdates in place).
engine/NPlayerGui_Graphics.h: FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX -- inline GFToScreen(const C2DBoxF&) returning the box (FableWin 0x020D90AD).
engine/CRGBColour.h: FABLE_RETAIL_CRGBCOLOUR_COPY_FORCEINLINE -- FABLE_FORCEINLINE on the Initialise copy ctor (caller inline budget).
engine/C2DBoxF.h: FABLE_RETAIL_C2DBOXF_TRPOS -- Ego_r/FableWin ?GetTRPos@C2DBoxF@@QBE?AVC2DVector@@XZ, inline C2DVector(GetTLX() + GetWidth(), GetTLY()) (full copy of the current repo header + this opt-in).

## 0x00F3BA70 (not landed)

- engine/CTriangleStripifier2.h: FABLE_RETAIL_CTRIANGLESTRIPIFIER2_BUILDCACHE declares private BuildCacheOptimizedVertexList (Ego_r + FableWin mangled name).
- engine/CTriangleStripifier2.h: FABLE_RETAIL_CTRIANGLESTRIPIFIER2_STRIPUNUSED declares private StripUnusedVerts (probe only; not used by the best source).

