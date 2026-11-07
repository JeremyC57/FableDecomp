# Chunk 851 notes

Result: 4 of 6 RELOCATION_MATCH (0x0075E4C0, 0x006001E0, 0x00A9BBE0, 0x00BA1B00) and 2 parked (0x005FE840, 0x00402510).
Every matched row has bcheck "N N 0 []" (0 reloc-masked byte differences), passes vland read-only with the final overlay,
and its test passes under g++ and VC7.1. Every attempt is logged in STATUS.tsv.

## 0x0075E4C0 CTCCameraPointScriptedSpline::OnSerialseLeaf (RELOCATION_MATCH, 4 attempts)
- Name: the catalog placeholder is vfunc_33. Ego_r + FableWin both have `?OnSerialseLeaf@CTCCameraPointScriptedSpline@@UAEXAAVCPersistContext@@@Z`
  (sic, "Serialse"). It sits in slot 33 of vtable 0x0126A0BC, the slot where the landed CTCCameraPointScripted /
  GeneralCase / Track OnSerialseLeaf rows also sit. The strings ("TimeToPlay", "Tension", "ValidAnims", "NumKeyCameras",
  "KeyCameras[i].*") are this class's fields.
- Key spellings:
  - The Transfer<T> switch bodies sit above the row under auto_inline(off).
  - The load arm's AnimationSpeed / RollAngle are explicit 3-argument Transfers.
  - The anim index and the world are named locals.
  - GetEntrySymbol's result is a named CCharString.
  - The save index is `GFIntToCharString(index++)`.
- Local models: the row keeps a local CBankFile { GetEntrySymbol } / CMeshDataBank / CWorld { PMainMeshBank +0x3c } trio,
  the same model the landed 0x00692C80 uses. engine/CBankFile.h is HOLD and lacks GetEntrySymbol, which gives vland
  SHARED_HEADER_AVAILABLE warnings for CBankFile / CWorld. See Header debt.
- Callee names: NAME_FIXES.tsv covers 0x0075F180 / 0x0075F1F0 / 0x0075F860 (the nested CKeyCamera, misbound as
  SLipSyncSample / an unnested CKeyCamera).

## 0x006001E0 CTCInventoryMap::ConstructRegionDescriptionShops (RELOCATION_MATCH, 6 attempts)
- Levers (each is a same-TU body or a declaration order):
  - GetAllLeafThings (FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY).
  - GetDef<T>, as in 0x0047A9F0.
  - map<unsigned long,CUIState>::operator[], as in 0x0047A9F0.
  - Counter declaration order: shops, title, barber, tattoo, taverns.
  - CUIRegionInfo store order: Barber, Tatoo, Shops, Title, Taverns.
- New header CThingFilter_InMap.h. CTCShop::IsBarberShop / IsTattooShop / IsTitleShop are CTCShop members in both PDBs, not
  CTCShopkeeper (NAME_FIXES.tsv). New binding: GetAllLeafThings<...> 0x00617A90.

## 0x00A9BBE0 C3DMeshFileLoader::C3DMeshFileLoader (RELOCATION_MATCH, 3 attempts; Codex rebase)
- The Codex body is nearly unchanged. The chunk classes are FABLE_RETAIL_<CLASS> opt-in branches of our generated headers,
  plus 4 new headers. The Register* bodies are inline (nested opt-in in C3DMeshFileLoader.h).
- The last 2 diff lines (the EngineMaterial temporary's MapFilenames release) went away with the pointee Release that
  deletes itself (CIVCountedPointeeBase RELEASE_DELETES, the Codex model) plus CIVCountedPointer RELEASE_MEMBER.
- Regraded after chunk 850's landing: this row now uses the repo's chunk-850 C3DMeshFileEngineMaterialChunk retail branch
  (ctor 0x00AC56F0 now bound).
- New bindings (no conflicts):
  - the 21 chunk ctors / dtors listed in the source header;
  - CCountedPointer<C3DMeshFileRootChunk>::Reset 0x00A9CED0;
  - RegisterClothSubChunkType 0x00A9B930.

## 0x00BA1B00 CEnginePrimitiveManagerStaticMeshesBase::DisplayMirroredTextures (RELOCATION_MATCH, 3 attempts)
- CIndexBuffer::Lock takes its flags first (retail pushes 0, 0, flags).
- The end-of-pass reset is the inline CRenderManagerCore::FlushRender. That symbol is FableWin.pdb-only (0x02CABE80), with
  no Ego_r or retail copy. It is a same-TU __forceinline body; written as separate statements, the manager pointer is
  re-read each time.
- The other inline helpers (SetTextureFactor x2, GetPixelShadersEnabled, SelectVertexBuffer, DrawBuffersIndexed x2) are
  same-TU __forceinline bodies of the shared headers' declarations, as landed 0x00BA7DB0 writes them.
- C3DPrimitive2 / CEngineSubPrimitiveStaticMesh have no header: local models of the two members read (Ego_r offsets).
- Existing bindings use the wrong access for three private Update* members (QAE vs PDB AAE) and the wrong class for the
  CIndexBuffer members: see NAME_FIXES.tsv. The new binding is CIndexBuffer::Initialise 0x00AC1F70 with the PDB spelling.
- Label errors vland reported, for bindings made through the shared CShaderRenderManager.h: 0x0098A8E0 is UpdateBoneTransform
  (labelled UpdateBoneMatrices) and 0x00989700 is UpdateCameraPos (labelled UpdateAmbient). See LABELS.tsv.

## 0x005FE840 CTCInventoryMap::ConstructRegionExitMarkers (PARKED, DIFFER(2424v2379) md485, 4 attempts + diagnostic)
- Residue: VC7.1's function-wide inline budget, one of the two documented blockers. Retail calls the set<long> member-template
  find 0x00616580 three times and expands it once, at loop 2's core-route test (0x005FEE6D-0x005FEE9A). Ours:
  - out-of-line declared find (e2): call sequence identical to retail, but 4 calls (frame 0x84 v 0x80; md563);
  - inline find body (e3 / e4): 3 of the 4 expanded, loop 1's optional test called (frame 0x7C; md485).
- Tried something not in GOTCHAS:
  - e4: the STLport 4.6.1 member-template find body as a same-TU definition, which keeps the retail symbol for the
    out-of-line copies. Same split as e3.
  - Diagnostic only, never in a source: /d2inlS10/12/13 change nothing. /d2inlT30 md804, T50 md699, T65 md526, T68-72 md389
    (all four expanded), T75/T80 = default. No threshold expands exactly retail's one site, so the split depends on the
    calling context, not one callee-size knob.
- The row's call sequence, literals and every other inline decision match. Parked in differ/005fe840.cpp (+ test).
- Bindings: the new names are IsThingAsPosBeingDrawn 0x005FABA0 (the landed row calls it IsPositionInsideClipEdges:
  NAME_FIXES.tsv) and GetRelativePosOnMiniMap 0x005FBAA0 (not bound before).
- UIDefName assignments: `CCharString(..)` temporaries (retail operator= 0x0099EFB0). Declared-only specialisations of
  CCountedPointer<CComponent>::Reset (0x00429C6F) and vector<CUIMiniMapMarker>::push_back (0x00618CA0).

## 0x00402510 GFMain (PARKED, header debt, 1 attempt)
- The Codex pass-12 body (RELOCATION_MATCH in its own 88-header world) gives 102 errors on our tree. With 16 new headers
  (Codex models, string includes mapped to fable_string.h) plus the CCHARSTRING / CWIDESTRING / CDISPLAYMANAGER / CAFILE /
  CDEFINITIONMANAGER / CSTRINGPARSER / CPERSISTCONTEXT / CSYSTEMMANAGER / CMAINGAMECOMPONENT / CSTREAMINGFONTBANK opt-ins,
  the count falls to 79.
- The blocker is engine/CBankFile.h (HOLD), and it is not a compiler residue:
  - NGameText::CDataBank : CBankFileAsync : CBankFile needs CBankFile polymorphic. GFMain calls the text bank's OpenReadOnly
    through vtable slot 1 (`mov ebp,[esi] ; ... ; call [ebp+4]`, 0x00402EA5).
  - It calls `?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z` 0x009CC410 (0x00402EBD / 0x00402F52), which must be a CBankFile
    member.
  - Our CBankFile is the flat generated layout with a `__vftable` data member. A derived class that declares virtuals
    would put a second vptr in front of it, and a CBankFile-qualified symbol cannot be declared without editing the file.
- Parked as differ/00402510.cpp: the g1 source, SRC_FAIL. The test (Codex's) passes under g++ and VC7.1. The headers are
  kept in differ/00402510_hdr/engine/.
- Bindings note: 0x009CC410 is bound under 7 different wrong spellings (NAME_FIXES.tsv). Codex's own NAME_FIXES.tsv in
  verified_pending/00402510 lists about 80 more GFMain callee respellings.
- Landing note: the Codex TU carries four file-static helpers (GFUninitialise 0x00401B80, GFHandleSystemInitError 0x00401C00,
  GFGetBuildNumber2 0x00401F30, GFInitialise 0x004022B0). GFInitialise is already landed as its own row (with a
  FableEngineRoot local model), so the coordinator must keep one body per address.

## Header debt
1. engine/CBankFile.h (HOLD). Needs the polymorphic retail branch (CBaseClassNonCopyable base, the 20 virtual slots, sizeof
   0x110), as in Codex verified_pending/00402510/hdr/engine/CBankFile.h, as an opt-in, plus member declarations
   `unsigned long FindIndexBySymbolCRC(unsigned long) const;` (0x009CC410) and `CCharString GetEntrySymbol(unsigned long) const;`
   (0x009CC1D0). This unblocks GFMain, and lets 0x0075E4C0 / 0x00692C80 drop their local CBankFile models.
2. GFMain members still missing after the 16 new headers. They are additive GFMAIN opt-ins; the full error list is in
   differ/00402510_hdr/g1_errors.txt:
   - CSystemManager: Initialise(CSystemManagerInit*) -> ESystemCode, Uninitialise, DisplayCriticalMessage(const CCharString&),
     GetApplicationWinHandle (inline, +0x94), GetGraphicBankManager (inline).
   - CDisplayManager: IsTextureBitDepthSupported, GetRefreshRate (inline +0x1C8), GetRenderManager; the D3D member types
     (D3DCAPS9 / D3DDISPLAYMODE / D3DVIEWPORT9 / D3DPRESENT_PARAMETERS) under FABLE_RETAIL_CDISPLAYMANAGER need Direct3D9Types.h.
   - CRenderManager(Core): GetMaxTextureWidth / GetMaxTextureHeight (inline, D3DDeviceCaps +0x3A58+0x58/0x5C).
   - CAFile: SetErrorCallback / SetHardDiskErrorCallback / SetCurrentPath / GetProjectPath (statics).
   - CDefinitionManager: SetCreatingCompiledDefs / SetCreateChecksums / SetCompiledDefDirectory / SetUsingCompiledDefs (statics);
     its retail branch's static assert fails in this TU.
   - CDefStringTable: GDefStringTable, SetTableFilename, LoadTable.
   - CUserProfileManager: Release (0x00406DF0), SetProfileName (0x0040D400).
   - CStreamingFontBank: SetUseFixedFunction (0x009A0EF0); its retail branch needs C2DExtentsF.
   - CMainGameComponent: GenerateMetFilesFromLugFiles.
   - CCharString: the (CAFile&, long) ctor.
   - The CASoundManager forward declaration.

## Descriptive names (not recovered spellings)
None in the matched rows. GFMain's Codex source keeps 8 descriptive address-suffixed globals / one function, as Codex's
NAMING_EVIDENCE.md says (StartupConsoleEnabled_0137548f etc.).
