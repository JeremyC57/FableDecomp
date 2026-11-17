# NF51 header overlays (FABLE_HDR_OVERLAY=$S/w22/agentNF51/hdr)

Every overlay is a fresh copy of the repo header with `#ifdef` opt-in blocks added; without the opt-in macros each file is
token-identical to the repo header (checked with `diff`: added lines only).

- `engine/CAIStateGroupBase.h` (copy of rebuild/include/engine/CAIStateGroupBase.h)
  - `FABLE_RETAIL_CAISTATEGROUPBASE_MINIONIDLE`: `EAIActionReturn AIAction_MinionIdle();` 0x0097FA00 (Ego_r ?AIAction_MinionIdle@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ). Rows 0x0090BED0 / 0x0090F320 / 0x0092C070.
  - `FABLE_RETAIL_CAISTATEGROUPBASE_PRECALCROUTEPROXIMITY`: protected `C3DVector SetUpPreCalcNav(const CThing&)` 0x0097F5B0 and `AIAction_MoveToPreCalcRouteProximity(float, const CThing&, bool&, EMovementType, const CAIFilterBase&, bool)` 0x0097F780 (Ego_r.pdb). Needs `_FILTER`. Row 0x0097F780.
  - `FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSSIBLYBLOCKED`: protected `AIAction_MoveToPossiblyBlockedObjectAndFace(CIntelligentPointer<CThing>, long&, float, float, const CAIFilterBase&, const CThing*, const CThing*, float)` 0x00980770 (Ego_r.pdb). Needs `_FILTER` + `_AIACTIONS`. Row 0x00980770.
- `engine/CAIStateGroup_ApprenticeHero.h` (copy of the repo header)
  - `FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_LOOKATLEADERBOARD`: protected `bool LookAtLeaderBoard();` 0x0092AE70 (Ego_r / FableWin ?LookAtLeaderBoard@CAIStateGroup_ApprenticeHero@@IAE_NXZ). Row 0x0092AE70.
  - `FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_LOOKATMAP`: protected `bool LookAtMap();` 0x0092AFE0 (?LookAtMap@CAIStateGroup_ApprenticeHero@@IAE_NXZ). Row 0x0092AFE0.
- `engine/CFrontEndManager.h` (copy of the repo header)
  - `FABLE_RETAIL_CFRONTENDMANAGER_UPDATE` (in the `FABLE_RETAIL_CFRONTENDMANAGER` branch, public section): `void Update(float dt);` 0x00599E3F (Ego_r ?Update@CFrontEndManager@NUISystem@@QAEXM@Z). Row 0x0042DC94.
- `engine/CTCInventory.h` (copy of the repo header)
  - `FABLE_RETAIL_CTCINVENTORY_STYLEENTRY` (in the `FABLE_RETAIL_CTCINVENTORY` branch, before the QUICKACCESS block; opens with its own
    `public:`, which is already in effect there): `void ConstructStyleEntry(NUISystem::CComponent*, std::vector<std::pair<long,long> >&,
    const CCharString&, NUISystem::CComponent*);` 0x00582ACE and `void ConstructStyleEntryPC(NUISystem::CComponent*,
    std::vector<std::pair<long,long> >&, NUISystem::CComponent*, const CCharString&, NUISystem::CComponent*);` 0x0058315B (Ego_r.pdb
    ?ConstructStyleEntry@CTCInventory@@QAEXPAVCComponent@NUISystem@@AAV?$vector@U?$pair@JJ@std@@...@std@@ABVCCharString@@0@Z /
    ?ConstructStyleEntryPC@..@@QAEXPAVCComponent@NUISystem@@AAV?$vector@..@std@@0ABVCCharString@@0@Z, public). Declarations only; needs
    `_QUICKACCESS` for the vector include. Rows 0x00582ACE / 0x0058315B.
- `engine/CTCInventoryAbilities.h` (copy of the repo header)
  - `FABLE_RETAIL_CTCINVENTORYABILITIES_HUDLEVEL` (after the `_PRIORITISED` block, public section): `long GetHUDAssignedAbilityLevel() const
    { return CurrentAssignedAbilityLevel; }` (FableWin.pdb ?GetHUDAssignedAbilityLevel@CTCInventoryAbilities@@QBEJXZ, public const; no
    Ego_r.pdb copy, so inline in the release build; retail 0x0064DB2B reads the private CurrentAssignedAbilityLevel +0x144 in place from
    NPlayerGui::CDrawAbilityButtons, which this inline reader explains). Row 0x0064DB2B. The body is inferred from that one in-place
    read (the debug body at FableWin 0x020F62E8 was not disassembled).

Withdrawn (no row uses them; not in this overlay): `FABLE_RETAIL_CFRONTENDMANAGER_REFRESHPROFILES` /
`_REFRESHPROFILESFORDELETE` and `FABLE_RETAIL_CUSERPROFILEMANAGER_PROFILEDIR` (tried for the blocked 0x00596CCE / 0x00597006).
