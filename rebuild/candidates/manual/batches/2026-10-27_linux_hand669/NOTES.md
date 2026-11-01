# Hand chunk 669 (CNavQuadTreeNode .. CBufferedFileDataOutputStream): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH (relocations checked against the object's own relocation table: every call / data target is
the intended PDB symbol) and pass their behaviour tests under g++ and the lane's VC7.1 cl.exe. No row was parked, so the
12-probe parking rule never applied; nine rows matched on the first probe, 0x006E7550 on the fourth (5 logged).

## What matched (10)
- **CNavQuadTreeNode::~CNavQuadTreeNode 0x00A75C20** -- `if (p && p != &CNavQuadTree::BlockedNode) delete p; p = 0;` for TL, TR,
  BR, BL; CANavQuadTreeNode's inline user dtor gives the 0x0129CBBC store before ~CBaseClassNonCopyable. The null test comes
  before the blocked-node test, so the out-of-line Set?Node setters (blocked test first) are not what the dtor runs.
- **CTCTrophy::~CTCTrophy 0x006DDB80** (catalog: CTCActionUseBuyOrSteal) -- body `RemoveFromGlobalTCs();`; the two STLport
  list<CIntelligentPointer<CThingAICreature> > members from the real _list.c text keep clear() out of line (0x00510D40) and expand
  the sentinel free, as retail.
- **CScriptGameResourceObjectMovie::~CScriptGameResourceObjectMovie 0x006E7550** -- InternalEndMovieSequence, then
  `CPlayer& player = ScriptInterface->PlayerManager->GetPlayer(ScriptInterface->GetCurrentPlayer()); player.RemoveMode(0x1B);`
  under the bool; the named reference orders `push 1Bh` after GetPlayer, the inline PDB accessor GetCurrentPlayer gives the
  inherited PImp CCountedPointer release its eax reload (probe log).
- **CTCSpotLight::~CTCSpotLight 0x00731DB0** -- `LightPrimitive = 0;`; CTCBase + NGraphicAppearance::CAGraphicAppearanceComponent
  (+0x0C) with the component's inline user dtor (0x0125A170 store), as the landed ~CTCLight.
- **CTCOpinionSource::~CTCOpinionSource 0x00771A90** (catalog: CSharedData) -- compiler-generated (no vtable store); emitted out of
  line by defining the PDB constructor ??0CTCOpinionSource@@QAE@AAVCThing@@@Z in the TU (vtable -> ??_G -> implicit ??1).
- **CTCTurncoatSpell::~CTCTurncoatSpell 0x007C66F0** -- empty user body; CWillButtonHelperTimer's implicit dtor expands
  (vector<float> free, then PCreature).
- **CTCMultiStrike::~CTCMultiStrike 0x007CCA90** (catalog: CTCCoopSpirit) -- empty user body; STLport vector of
  CIntelligentPointer<CThing> (`push 0 ; call [eax]` per element via _Destroy).
- **CTCElectrocutedEffect::~CTCElectrocutedEffect 0x007CE730** (catalog: CTCActionUseBuyHouse) -- body
  `CTCSpecialAbilities::FadeOrKillEffect(PEffect);` (unconditional, unlike ~CTCBurntEffect's IS_THING_ALIVE loop).
- **NEnvironmentInternal::CEnvironmentThemePriorityStack::ProcessTransitions 0x00887B00** -- index loop over Ents (size()
  re-read), then `Ents[0].SetDefaultThemeWeight(0.0f, true);` with SetDefaultThemeWeight a plain member defined above: VC7.1 folds
  the clamp and leaves `mov [edi],0 ; mov byte [edi+4],1 ; jmp RebalanceWeights`.
- **CBufferedFileDataOutputStream::~CBufferedFileDataOutputStream 0x009947A0** -- `Flush(); delete[] Buffer;` with Flush defined
  above in the TU (retail keeps it out of line at 0x00994520 and expands it here); CDataStream's inline user dtor stores 0x0129A69C.

## Parked for Fable 5.1 (0)
(none)

## Skipped
(none)

## Proven
- 0x00A75C20 ??1CNavQuadTreeNode@@UAE@XZ -- first store vtable 0x0129DCD4 (RTTI CNavQuadTreeNode); sole caller 0x00A7D903 in the slot-0 ??_G 0x00A7D900; Ego_r.pdb name set
- 0x00A7D900 ??_GCNavQuadTreeNode@@UAEPAXI@Z -- slot 0 of 0x0129DCD4; Ego_r.pdb
- 0x006DDB80 ??1CTCTrophy@@UAE@XZ -- first store vtable 0x0125FC54 (RTTI .?AVCTCTrophy@@); sole caller 0x006DFB43 in slot-0 ??_G 0x006DFB40; Ego_r CTCTrophy members walked; Ego_r.pdb name set
- 0x006DFB40 ??_GCTCTrophy@@UAEPAXI@Z -- slot 0 of 0x0125FC54
- 0x006E7550 ??1CScriptGameResourceObjectMovie@@UAE@XZ -- first store vtable 0x01260F00 (RTTI); sole caller in slot-0 ??_G 0x006E7C90; Ego_r.pdb
- 0x006E7C90 ??_GCScriptGameResourceObjectMovie@@UAEPAXI@Z -- slot 0 of 0x01260F00; Ego_r.pdb
- 0x00731DB0 ??1CTCSpotLight@@UAE@XZ -- stores 0x01266FBC / 0x01266FA4 (RTTI CTCSpotLight : CTCBase, CAGraphicAppearanceComponent@12); sole caller in slot-0 ??_G 0x00732690; Ego_r.pdb
- 0x00732690 ??_GCTCSpotLight@@UAEPAXI@Z -- slot 0 of 0x01266FBC
- 0x00771A90 ??1CTCOpinionSource@@UAE@XZ -- no store (implicit); sole caller in slot-0 ??_G 0x00771A70 of 0x0126A81C (RTTI CTCOpinionSource); releases Ego_r POpinionProduced / RestrictToFactionAndAllies / DefStack; Ego_r.pdb
- 0x00771A70 ??_GCTCOpinionSource@@UAEPAXI@Z -- slot 0 of 0x0126A81C
- 0x00771360 ??0CTCOpinionSource@@QAE@AAVCThing@@@Z -- the only CTCOpinionSource ctor in Ego_r.pdb; stores 0x0126A81C (landed row spells CThing*)
- 0x007C66F0 ??1CTCTurncoatSpell@@UAE@XZ -- first store 0x0126E5C4 (RTTI); sole caller in slot-0 ??_G 0x007C77D0; Ego_r.pdb
- 0x007C77D0 ??_GCTCTurncoatSpell@@UAEPAXI@Z -- slot 0 of 0x0126E5C4
- 0x007CCA90 ??1CTCMultiStrike@@UAE@XZ -- first store 0x0126EE0C (RTTI .?AVCTCMultiStrike@@); sole caller in slot-0 ??_G 0x007CD810; Ego_r CTCMultiStrike members; Ego_r.pdb
- 0x007CD810 ??_GCTCMultiStrike@@UAEPAXI@Z -- slot 0 of 0x0126EE0C
- 0x007CE730 ??1CTCElectrocutedEffect@@UAE@XZ -- first store 0x0126F044 (RTTI .?AVCTCElectrocutedEffect@@); sole caller in slot-0 ??_G 0x007CED40; Ego_r members; Ego_r.pdb
- 0x007CED40 ??_GCTCElectrocutedEffect@@UAEPAXI@Z -- slot 0 of 0x0126F044
- 0x00887B00 ?ProcessTransitions@CEnvironmentThemePriorityStack@NEnvironmentInternal@@QAEXXZ -- Ego_r + FableWin name sets; sole caller 0x006B4045 walks CEnvironment::PriorityStackChannels (+0x48) per stack; calls the entry's ProcessTransitions 0x00887A30 per 0x30-byte entry
- 0x009947A0 ??1CBufferedFileDataOutputStream@@UAE@XZ -- first store 0x0129A710 (RTTI); sole caller in slot-0 ??_G 0x009971E0; Ego_r.pdb
- 0x009971E0 ??_GCBufferedFileDataOutputStream@@UAEPAXI@Z -- slot 0 of 0x0129A710
- 0x00510D40 ?clear@?$_List_base@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@QAEXXZ -- STLport _list.c body; all four callers (0x006DDB80, 0x006DDA20, 0x006D4D40, 0x006C0FD0) clear a list<CIntelligentPointer<CThingAICreature> > (Ego_r CTCTrophy / CTCFollowed::AIFollowers / HasBuildingAsHomeOrWork copy_if)
- 0x008EDFF0 ??0CWillButtonHelperTimer@@QAE@W4EHeroAbility@@AAVCThing@@@Z -- the only ctor in both PDB name sets
- 0x00A75CA0 / 0x00A75CE0 / 0x00A75D20 / 0x00A75D60 ?Set{TL,TR,BL,BR}Node@CNavQuadTreeNode@@IAEXPAVCANavQuadTreeNode@@@Z -- both PDB name sets spell them protected (IAE); landed as QAE
- Callees used under existing PDB spellings (no change): CTCBase::RemoveFromGlobalTCs 0x00686DF0, CTCBase::~CTCBase 0x00686830,
  CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CTCSpecialAbilities::FadeOrKillEffect(CIntelligentPointer<CThing>&)
  0x0072ABD0, CGameScriptInterface::InternalEndMovieSequence 0x0089B210, CPlayerManager::GetPlayer(long) 0x004498C0,
  CEnvironmentThemePriorityStackEntry::ProcessTransitions 0x00887A30 / RebalanceWeights 0x00886200, CCharString::~CCharString
  0x0099EAE0, CBaseObject::~CBaseObject 0x0099A430, CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300,
  CNavQuadTree::BlockedNode 0x013D2D2C.

Descriptive (not proven) names: none for symbols. Unproven details stated in the sources: CAGraphicAppearanceComponent's slot
order, CAFile slots 1-3 (placeholders, never called), the EPlayerMode enumerator for 0x1B (cast; no enumerator list in either PDB).

## Label corrections
- 0x006DDB80 catalog CTCActionUseBuyOrSteal::~CTCActionUseBuyOrSteal -> CTCTrophy::~CTCTrophy
- 0x00771A90 catalog CSharedData::~CSharedData -> CTCOpinionSource::~CTCOpinionSource (implicit)
- 0x007CCA90 catalog CTCCoopSpirit::~CTCCoopSpirit -> CTCMultiStrike::~CTCMultiStrike
- 0x007CE730 catalog CTCActionUseBuyHouse::~CTCActionUseBuyHouse -> CTCElectrocutedEffect::~CTCElectrocutedEffect
- the nine slot-0 ??_G rows (0x00A7D900, 0x006DFB40, 0x006E7C90, 0x00732690, 0x00771A70, 0x007C77D0, 0x007CD810, 0x007CED40,
  0x009971E0): manifest "`vector_deleting_destructor' CLandscapeBackgroundPatch" -> ??_G<Class>@@UAEPAXI@Z
- 0x00510D40 catalog CGuiControlTreePane::SortTreeRecursively -> std::_List_base<CIntelligentPointer<CThingAICreature> >::clear
- Labels right: 0x00A75C20, 0x006E7550, 0x00731DB0, 0x007C66F0, 0x00887B00, 0x009947A0 (6 of 10; destructor labels 5 of 9).

## Header debt
- engine/CTCBase.h, CNavQuadTreeNode.h, CTCSpotLight.h, CTCTurncoatSpell.h, CBufferedFileDataOutputStream.h are flattened
  generated headers (vptr word, padded bases, opaque byte arrays), so every row models its chain locally with the PDB names and a
  `// Layout:` line; CDefPointer, CCountedPointer, CBaseObject, CBaseClassNonCopyable come from the shared headers. No headers
  exist for CTCTrophy, CTCOpinionSource, CTCMultiStrike, CTCElectrocutedEffect, CScriptGameResourceObjectMovie(Base),
  CScriptGameResourceObjectBase, CGameScriptInterface (partial model), CDataStream, CAFile, CWillButtonHelperTimer,
  CANavQuadTreeNode (header exists but flattened), CEnvironmentThemePriorityStack(Entry).
- engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT) should gain `void InternalEndMovieSequence();` (0x0089B210, both PDBs
  QAEXXZ) and the inline `long GetCurrentPlayer() const { return CurrentPlayer; }` (both PDBs QBEJXZ; 0x006E7550 needs the inline
  form to match); engine/CPlayer.h should declare RemoveMode(EPlayerMode) 0x00634190. Until then 0x006E7550 models both locally
  (engine/CPlayerManager.h is used for GetPlayer).
- engine/CNavQuadTree.h should declare `static CNavBlockedQuadTreeNode BlockedNode;` (0x013D2D2C, established spelling).
