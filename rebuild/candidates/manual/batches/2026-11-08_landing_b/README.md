# Landing 2026-11-08b: authoring chunk 851 (4 of 6 landed, 2 parked)

| Row | Grade | Note |
|---|---|---|
| 0x0075E4C0 CTCCameraPointScriptedSpline::OnSerialseLeaf | **RELOCATION_MATCH + PASS, landed** | was placeholder vfunc_33; PDB spelling `OnSerialseLeaf` (sic, Ego_r + FableWin); local CBankFile / CMeshDataBank / CWorld models as landed 0x00692C80 (CBankFile.h on HOLD, header debt) |
| 0x006001E0 CTCInventoryMap::ConstructRegionDescriptionShops | **RELOCATION_MATCH + PASS, landed** | same-file GetAllLeafThings / GetDef<T> / map::operator[] bodies, retail counter and store orders |
| 0x00A9BBE0 C3DMeshFileLoader::C3DMeshFileLoader | **RELOCATION_MATCH + PASS, landed** | Codex pass-12 rebase; chunk classes as opt-in branches of our generated headers (4 new headers); pointee Release deletes itself |
| 0x00BA1B00 CEnginePrimitiveManagerStaticMeshesBase::DisplayMirroredTextures | **RELOCATION_MATCH + PASS, landed** | CIndexBuffer::Lock flags first; the PDB inline members (FableWin-only CRenderManagerCore::FlushRender, ...) as same-file `__forceinline` bodies of the header declarations (EXTRA_RULES: retail header inlines) |
| 0x005FE840 CTCInventoryMap::ConstructRegionExitMarkers | DIFFER(2424v2379), 485 lines, 4 attempts | function-wide inline budget: retail expands the set<long> find at 1 of 4 sites; /d2inlS and /d2inlT (diagnostic only) cannot give 1 of 4 |
| 0x00402510 GFMain | SRC_FAIL (79 errors), 1 attempt | header debt: needs a polymorphic CBankFile and FindIndexBySymbolCRC as a member (CBankFile.h on HOLD); 16 new headers and the error list in `pending/2026-11-08_linux_hand851/differ/00402510_hdr/`; GFInitialise 0x004022B0 is already landed while the Codex file carries it as a file-static helper |

- **Headers landed:** 30 files from the agent's hdr/ (additive opt-ins; 6 new: C3DMeshFileRootChunk / MapChunk / BoneChunk /
  ConvexVolumeChunk, CPlane, C3DVertexBlend2, CThingFilter_InMap). The CThing / CWorldMap / CTCQuestCard / CTCDRegionExit /
  CTCActionUseScriptedHook opt-ins serve only the parked 0x005FE840 and stay in the pending dir. Every landed row that includes a changed
  header, directly or through another header (321 rows), was rechecked: `recheck/recheck.log`.
- **Names:** labels `labels_landing_2026-11-08b.tsv` (14: IsThingAsPosBeingDrawn 0x005FABA0, GetRelativePosOnMiniMap 0x005FBAA0,
  CBankFile::FindIndexBySymbolCRC 0x009CC410, the set<long> find 0x00616580, the nested CKeyCamera ctors, UpdateBoneTransform 0x0098A8E0,
  UpdateCameraPos 0x00989700, CTCShop::IsBarberShop 0x00772490, ...). Queue `land0811b` (15).
