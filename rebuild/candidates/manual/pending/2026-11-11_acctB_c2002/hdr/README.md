# Chunk 2002 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2002/hdr`)

Every change to an existing header is an additive opt-in block; the default path is unchanged (`diff rebuild/include/engine/<f> hdr/engine/<f>`
shows only the opt-in, plus the `#if/#elif` re-spelling of CRightHandedSet's GetRight selector). Four headers are new.

## New headers
- `engine/CTavernGameSpotTheAdditionDef.h` (row 0x007D8570): CTavernGameSpotTheAdditionDef : CSubDefClassBase, Ego_r.pdb members in order at
  retail offsets (two STLport vectors 12 bytes: ItemsPerRound +0x44, ImpulseScale +0x60, HighlightWidth +0x84, HighlightColour +0x88,
  HighlightColourError +0x8C). Evidence: the landed FrameUpdate 0x007D8E20's local model (same offsets) and ProcessInput's reads; the
  StatGetTypeName literal 0x01243D1C in GetSubDef<...> 0x007D9A80.
- `engine/CTCTavernGameSpotTheAddition.h` (row 0x007D8570): CTCTavernGameSpotTheAddition : CTCTavernGame (RTTI), Ego_r.pdb members at the
  landed FrameUpdate offsets (CurrentRound +0xF0 .. MaxY +0x184), GamePositionRHSet +0x14D / GamePositionPos +0x165 as real CRightHandedSet /
  C3DVector (needs FABLE_RETAIL_C3DVECTOR_PACKED: retail C3DVector is 1-aligned; ProcessInput reads Forward at +0x159..+0x161 in place).
  Methods: the Ego_r / FableWin name set with PDB access (FindTarget / PositionCover / UpdatePointerScale private).
- `engine/CTCTavernTankard.h` (row 0x007B90F0): CTCTavernTankard : CTCBase, Ego_r.pdb sizeof 0x3C (no containers): RHSet +0x10, Pos +0x28,
  PBarmaid +0x34. Evidence: OnInitialActivate's whole-set / whole-vector stores at +0x10 / +0x28 (0x007B938C..0x007B93CE) + PDB.
- `engine/CTCCreatedEntity.h` (row 0x007B90F0): CTCCreatedEntity : CTCBase, PCreator (CIntelligentPointer<const CThing>) +0x0C, sizeof 0x14;
  inline SetPCreator (FableWin.pdb name; no out-of-line copy: the row builds the <const CThing> temporary, vtable 0x01237F1C, and assigns).

## Opt-ins added to existing headers
- `engine/CRightHandedSet.h` FABLE_RETAIL_CRIGHTHANDEDSET_GETRIGHT_OUTOFLINE: GetRight declared only (retail 0x007D8640 calls 0x0049CE00).
- `engine/CInputProcessTavernGame.h` FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS: `using CInputProcessGameBase::IsEventButtonPressed;`
  so a tavern game can call the protected IBE member (0x00687A90) through CInputProcessTavernGame*; keeps the base symbol. The original
  access grant (friend or using) is unproven.
- `engine/CTCTavernGame.h` FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE: inline ?GetGameState@CTCTavernGame@@QAE?AW4ETavernGameStates@1@XZ
  (Ego_r / FableWin; no retail copy). It is the lever of 0x007D8570's last argument (see LESSONS.md).
- `engine/NInventory_CTCInventoryBase.h` FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX: Get/SetSelectedCategoryIndex (0x005BC4B2 /
  0x005BC4B6; Ego_r / FableWin QBE / QAE).
- `engine/CTCInventoryQuests.h` FABLE_RETAIL_CTCINVENTORYQUESTS_OPENPC (nested in _MEMBERS): Teleport +0x154 .. AccumFadeTime +0x184 (Ego_r
  names, retail offsets: PreviousScreenType +0x168 / AvailableQuests +0x16C / DisplayQuests +0x178 agree with hand882's model and the landed
  CloseInventoryPC), the GetInventoryGuildValidity / IsInventoryScreen / OpenInventoryPC overrides and InitialiseQuestCardList.
- `engine/CTCHeroCentre.h` FABLE_RETAIL_CTCHEROCENTRE_GETQUESTCARDS: GetQuestCards declaration (0x0077DBC0, landed).
- `engine/CTCTavernTable.h` FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT: the retail layout (landed ctor 0x007B8710 offsets; vector<bool> is the
  generic byte vector in retail), OnInitialActivate and GetTankardName.
- `engine/CTavernTableDef.h` FABLE_RETAIL_CTAVERNTABLEDEF: the polymorphic def (CSubDefClassBase, AddTankards +0x25) as the landed
  GetSubDef<CTavernTableDef> row 0x007B9700 declares it locally; the generated flattened layout stays the default.
- `engine/CTCNamedUserActionPoints.h` FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_FREEPOINT: GetFreeNamedActionPoint (reference PDB spelling) and
  GetName declarations.
