## 0x008A7A70

# Header overlay, row 0x008A7A70
- `engine/CTCSummonSpell.h` -- full copy of the current repo header + new opt-in `FABLE_RETAIL_CTCSUMMONSPELL_GETSUMMONEDCREATURE_FORCEINLINE` (marks the inline `GetSummonedCreature() const` FABLE_FORCEINLINE and includes fable_compiler.h; per-TU inline opt-in: CGameScriptInterface.cpp expands the getter at 0x008A7B2F (and 0x008A82CA in GetHeroSummonedCreaturesList) where a plain inline is called). Default path unchanged.

## 0x00724630

# Overlay opt-ins (row 0x00724630), each a full copy of the current repo header plus one #ifdef block (CCircle.h is new)
- engine/CTCPhysicsStandard.h: FABLE_RETAIL_CTCPHYSICSSTANDARD_SETPHYSICSMESHBANKINDEX -- slot-85 override decl `virtual void SetPhysicsMeshBankIndex(long)` (Ego_r/FableWin UAEXJ).
- engine/CTCPhysicsBase.h: FABLE_RETAIL_CTCPHYSICSBASE_CALCULATERADIUS -- protected `float CalculateRadius() const` (0x006AFE20) + out-of-line `C3DVector GetMapwhoPosition() const` (0x006AFCB0) unless _MAPWHOPOSITION.
- engine/CPhysicsMesh.h: FABLE_RETAIL_CPHYSICSMESH_BOUNDS -- BoundingBox typed C3DBoundingBox, BoundingCircle typed CCircle, inline GetBoundingBox()/GetBoundingCircle() (FableWin inline accessors). No layout change.
- engine/CCircle.h: NEW shared header -- Ego_r CCircle (12 bytes): `C2DVector Centre` +0, `float Radius` +8, inline GetRadius() (FableWin ?GetRadius@CCircle@@QBEMXZ). Unconditional (no opt-in macro; nothing else in the repo includes it yet).
- engine/CMeshDataBank.h: FABLE_RETAIL_CMESHDATABANK_PEEKPHYSICSMESHINFO -- `const CPhysicsMeshInfo* PeekPhysicsMeshInfo(unsigned long) const` (0x00A24250).
- engine/NGraphicAppearance_CTCGraphicAppearance.h: FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE -- `float GetFullScale() const` (0x004BBFF0).
- Uses the repo's existing FABLE_RETAIL_C3DMESHOBJECT_POSITIONMATRIXFROMRHSET (wf17, row 0x0075B000) unchanged: no C3DMeshObject.h overlay.
- (repair) removed: stale C3DVector.h / C3DMeshObject.h overlays and the probe-only C3DVector PLAIN_INLINE, CIVCountedPointer ARROW_VIA_*,
  C3DBoundingBox GETCENTER_*, CTCPhysicsBase SETRADIUS opt-ins (row matches without them).

## 0x009CE5E0

engine/CBankFile.h: FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES (with FABLE_RETAIL_CBANKFILE_FILENAMEINDICES) types Checksums/UpdateData/PackedValid/OldSymbolIndices, inline EntryExists (0x009CC190 body), private InitEntry 0x009CE050 decl (row 0x009CE5E0).
engine/CBankFileEntryUpdateData.h: FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL = retail 0x18-byte record (no StateBlock), Filenames as minimal CSmallVector<CCharString,8>, inline private ctor (0x009CC6A0), Equals 0x009CC750 / SetFilenames 0x009CC840 decls (row 0x009CE5E0).

## 0x00627400

- engine/CTCQuickAccessMenu.h: FABLE_RETAIL_CTCQUICKACCESSMENU_EXPRESSIONS declares private ConstructExpressionCategoryList (PDB AAEX...PAVCComponent@4@@Z) for row 0x627400.

## 0x007EB6A0

- engine/CTCActionUseBase.h: FABLE_RETAIL_CTCACTIONUSEBASE_ISTHISTHEFT -- declares bool IsThisTheft() const (0x007EACB0).
- engine/CPhysicsMeshObject.h: FABLE_RETAIL_CPHYSICSMESHOBJECT_ZEDGE -- declares C3DVector GetBoundingBoxZEdge() const (0x00A11BA0).
- engine/CTCContainerRewardHero.h: FABLE_RETAIL_CTCCONTAINERREWARDHERO_HASREWARD -- declares public bool HasReward() const (0x0074E500).

## 0x00959F80

engine/CAIStateGroup_EnemyCreatureBase.h: FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS -- declares vtable slots 14..21 (row 0x00959BF0).
engine/CAIStateGroup_HoverMovementBase.h: FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS -- protected virtual slot 22 AIAction_MoveToPosAndFacePos (needs EAIAction.h/EMovementType.h first; row 0x00959BF0).
engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_CROSSWITHUP -- FableWin ?CrossWithUp@C3DVector@@QBE?AV1@XZ inline body C3DVector(Y,-X,0.0f) without the rest of ANGLE_BODIES (row 0x00959BF0).
engine/CAIStateGroup_HoverKeepInArea.h: FABLE_RETAIL_CAISTATEGROUP_HOVERKEEPINAREA -- polymorphic class over HoverMovementBase, StartPos C3DVector +0x1c, size 0x28 (row 0x00959F80).
(C3DVector.h / CAIStateGroup_EnemyCreatureBase.h / CAIStateGroup_HoverMovementBase.h copied unchanged from wf18/00959bf0/hdr.)

## 0x0094F180

- engine/CAIStateGroup_BalverineCornered.h: FABLE_RETAIL_CAISTATEGROUP_BALVERINECORNERED -- the real class over CAIStateGroupBase (PTarget, PLastJumpTarget, TimeOfLastJump, LandPos; GetLandPos decl).
- engine/NEntityEvents_CEventHitBase.h: FABLE_RETAIL_NENTITYEVENTS_CEVENTHITBASE_GETHITPARAMS -- declares const CHitParameters& GetHitParams() const (0x00810F40).
- engine/CIntelligentPointer.h: FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE -- FABLE_FORCEINLINE on the ASSIGN operator= (retail expands both assignments in place past VC7.1's inline budget); its own `#ifdef ..._ASSIGN_FORCEINLINE` / `#include "fable_compiler.h"` block added after the untouched FORCEINLINE_CTOR include block (no repo line rewritten).

## 0x00AC53F0

- engine/C3DMeshFileMaterialChunk.h: FABLE_RETAIL_C3DMESHFILEMATERIALCHUNK_METHODS -- inline GetMaterialIndex/GetName/GetTwoSided + decls HasExtendedProperties/GetExtendedProperties/FindFirstMap/FindNextMap (row 0x00AC53F0)
- engine/C3DMeshFileMapChunk.h: FABLE_RETAIL_C3DMESHFILEMAPCHUNK_METHODS -- inline GetType/GetFilename (row 0x00AC53F0)
- engine/C3DMeshFileMaterialExtChunk.h: FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK_METHODS -- decl GetSelfIllumination (row 0x00AC53F0)
- engine/C3DMeshFileEngineMaterialChunk.h: FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK_FROM_MATERIAL -- declares the (const C3DMeshFileMaterialChunk&) ctor 0x00AC53F0

## 0x007E6450

engine/NGraphicAppearance_CTCGraphicAppearance.h: FABLE_RETAIL_CTCGRAPHICAPPEARANCE_REMOVETINT -- declares RemoveTintColour(void*) 0x004BFAE0 (Ego_r ?RemoveTintColour@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAX@Z)

## 0x00959BF0 (not landed)

engine/CAIStateGroup_EnemyCreatureBase.h: FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS -- declares vtable slots 14..21 (row 0x00959BF0); slot 20 IsCombatStateGroup is public (UBE in Ego_r + FableWin), the rest protected.
engine/CAIStateGroup_HoverMovementBase.h: FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS -- protected virtual slot 22 AIAction_MoveToPosAndFacePos (needs EAIAction.h/EMovementType.h first; row 0x00959BF0).
engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_CROSSWITHUP -- FableWin ?CrossWithUp@C3DVector@@QBE?AV1@XZ inline body C3DVector(Y,-X,0.0f) without the rest of ANGLE_BODIES (row 0x00959BF0).
engine/CAIStateGroup_HoverWander.h: FABLE_RETAIL_CAISTATEGROUP_HOVERWANDER -- class : CAIStateGroup_HoverMovementBase with public virtual Update (Ego_r UAEXXZ); default flat 0x1c layout unchanged (row 0x00959BF0).

