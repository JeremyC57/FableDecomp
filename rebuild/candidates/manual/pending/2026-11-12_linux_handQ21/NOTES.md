# Chunk Q21 notes (2026-11-12)

Result: 1 RELOCATION_MATCH, 2 ported but still DIFFER, 11 not ported (1 baseline regrade each). Grade with
`export FABLE_HDR_OVERLAY=$S/w22/chunkQ21/hdr` (table in hdr/README.md). Grades per row in STATUS.tsv, one line per grade in ATTEMPTS.log.

## Matched (payload.json / oracle.tsv / src/ / tests/)
- 0x00B39080 CEngineInternalAttachedPrimitive::GetParentTransformAnimatedMesh -- RELOCATION_MATCH, no flags. Ported off the local
  model onto the shared headers: CEngineInternalAttachedPrimitive (GetParentPrimitive + static_cast to CEngineInternalPrimitiveAnimatedMesh,
  as the landed 0x00B395E0 sibling), C3DMeshInfo / CMeshDataBank (GetAnimationManager), C3DMeshStats (GetDummyObject + the C3DMesh2
  virtual slots), CCountedPointer (DTOR_DECL_ONLY: retail calls the dtor COMDAT 0x004C24C0), CCharString::GetCRC inline, CMatrix3x4
  SetMultiply over a named copy + PreScale. The prior draft's last residue (one `lea ecx,[esp+74h]` scheduled one store early) went away
  with the port alone; the two real fixes needed were the OcclusionBase slot-28 virtual (calls landed one slot low) and SetMultiply
  instead of PostMultiply (operand order). Needs 6 overlay opt-ins (hdr/README.md); all are additive, land them with the row.
  Test: tests/00b39080_test.cpp, PASS under VC7.1 (verify_and_land.behaviour_of, /Od link + Wine run) and g++ -std=c++03.

## DIFFER (best source in differ/<addr>.cpp, residue on line 2)
- 0x00540320 NUISystem::CEditBox::KeyPressed -- ported onto engine/NUISystem_CEditBox.h (the local 150-virtual CComponent list is gone).
  Residue 755v755: three vptr scratch registers eax/edx swapped (UpdateDisplayText in the delete branch, GetTextComponent, GetTextWindow)
  and `sub edi,5` v retail `add edi,-5`. `i - (len + 5)` stored straight into the member gives `lea eax,[edi-5]` (the add form in a fresh
  register); every in-place spelling (local, `i -=`, `-(len-i+5)`) gives `sub`. The port found a real header bug: see Coordinator.
- 0x007DAEF0 CTCTavernGameCardBlackjack::DealCard -- ported onto engine/card_sorting_types.h (shared CTCTavernGameCardBase/CCard,
  CDefPointer, CTCTavernGame retail). DealIndex of the old draft is the base's CurrentMovingCard (+0x310); the old HeightScale/HeightOffset
  are CTavernGameCardBaseDef CardSeparation/TableSeparation (+0x5c/+0x60); Placed/OldPosition/Target/Position are CCard
  Moving/LocalPos/NewLocalPos/CurrentPos. Same residue as before (flip-branch register rotation; retail's three PlaySound arg leas rotate
  eax, ecx, edx across the three branches, ours eax, eax, edx).
- Not ported (budget): 00a78b20, 00649e8b, 006c1250, 0096cd70, 0063f7c0, 00489d40, 00a1d290, 00814260, 00584b86, 00622a00, 00b4a700. Each
  draft is unchanged apart from the new line-2 note (prior drafts also in work/prior_drafts/). Port pointers:
  - 00a78b20: the node virtuals are AddToIndexTable (slot 20, +0x50) and SaveToFile (slot 21, +0x54), Ego_r MBE (the draft calls the first
    AddToIndexMap); CANavQuadTreeNode.h stops at slot 15. for_each is the /Gr fastcall COMDAT 0x00A7E440 (Ego_r spells the instance
    std::_For_each<const CNavigationPosition*, CSaveNavigationPositionToFile>); CSaveNavigationPositionToFile = {CAFile* File; C2DVector MapPos}
    (Ego_r 12 bytes). The shared C2DVector has a user copy ctor, which is what makes retail build the by-value functor in its argument slot.
  - 00b4a700: the per-primitive {0,0} temp is the CEngineSubPrimitiveChain& of RenderPrimitive (Ego_r CEnginePrimitiveManager*
    ::RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&) returns ERendered).
  - 0063f7c0: the draft's header comment describes the Bind2 sibling 0x0063F4B0; this row is the Bind3 (IsNot first) instance called once
    from the landed CWorldMap::GetNavigationLayerAt 0x0063E590 (call 0x0063E6CC).

## Names (two sources each)
- 0x00B39080 `?GetParentTransformAnimatedMesh@CEngineInternalAttachedPrimitive@@QAE_NAAVCMatrix3x4@@M@Z`: Ego_r.pdb + FableWin.pdb; retail
  callers 0x00B398ED (jmp) / 0x00B399CD are in the attached-primitive switch on ParentType, and the body reads the PDB members.
- 0x006C1250 is `?FrameUpdate@CTCBuyableHouse@@UAEXXZ` (CTCBuyableHouse vtable 0x0125D6A4 slot 9 = CTCBase slot 9 FrameUpdate; slot 8
  0x006BF7A0 is its HasFrameUpdate; Ego_r + FableWin both list ?FrameUpdate@CTCBuyableHouse@@UAEXXZ and no Update). The catalog row name
  "CTCBuyableHouse::Update" and the manifest "vfunc_9" are wrong; nothing landed uses it (no NAME_FIXES line).
- 0x00B4A700: the catalog name is the raw mangled leaf; it is `CEnginePrimitiveRenderer2D::Render` (vtable 0x012A2308 slot 4; Ego_r + FableWin
  ?Render@CEnginePrimitiveRenderer2D@@UAEXABVCEngineComponentRenderParams@@@Z).
- 0x0063F7C0: Ego_r ??$ForEachInRoughArea@VCThingPhysical@@V?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_HasNavigationInformation@@
  VCThingFilter_OverlappingBoundingBox@@@@VCThingFindNavigationLayerAt@@@CThingSearchTools@@QBE_N... (naming_stragglers/ego_r.exe.signatures.tsv)
  + the landed caller 0x0063E590's Bind3 branch.
- The other rows' names were checked present in both Ego_r.pdb and FableWin.pdb, with retail placement: 00540320 CEditBox vtable slot 146;
  0096cd70 CAIStateGroup_EatAtTable vtable slot 12 (MAEXXZ); 00814260 CTCBoastingPodium slot 6 (OnInitialActivate); 00622a00 CTCInventoryQuests
  slot 30 (OpenInventory); 00b4a700 above; direct callers for 00a78b20 (0x00A14E76), 00649e8b (0x00439455), 00489d40 (0x0048A0AF), 00a1d290
  (0x00A1F1F0), 00584b86 (5 calls), 007daef0 (3 calls).
- NAME_FIXES.tsv: 0x004C24C0 is ~CCountedPointer<const C3DMeshStats> (landed as a CUnprovenPointee stand-in).
- Made-up names: none in the matched row (all members, callees and locals other than `prim_base`, `parent_prim`, `stats`, `dummy`,
  `object_transform`, `dummy_transform`, `interp` are PDB spellings; those locals are descriptive). `g_CardLayerHeight` (0x01383C98) in the
  007daef0 draft is descriptive (no symbol found).

## Coordinator
- Land 0x00B39080 with the six overlay opt-ins it uses (hdr/README.md rows 1-6). The overlay copies were re-diffed against the repo at
  hand-back: additions only.
- engine/NUISystem_CText.h has CText slots 147..150 in the wrong order with mismatched addresses (it puts GetTextWindow at 150 / 0x0054FFF0,
  retail has it at 148 / 0x0054FFE0 = `lea eax,[ecx+178h] ; ret`; 0x0054FFF0 is GetAlignment, 0x0054FFB0 SetTextWindow, 0x0054FB10 ForceWidth;
  the catalog already has the right names). No landed row calls them; the overlay's FABLE_RETAIL_CTEXT_WINDOW_SLOTS order can become the default.
- Tests: the matched row's test was built and run under VC7.1 (verify_and_land.behaviour_of) and g++ -std=c++03: PASS both.
- 0x00B39080's dtor call relocates to `??1?$CCountedPointer@$$CBVC3DMeshStats@@@@QAE@XZ` (FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY); the landed
  0x004C24C0 row still instantiates the CUnprovenPointee stand-in, so apply NAME_FIXES.tsv (respell that row) when binding this row's relocations.
