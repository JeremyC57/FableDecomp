# agentNF9: CThing-family headers completed, definition-system binders moved onto them

## Method
I copied NF8's tools (`work/chk.py`, `g.sh`, `dis.sh`, `loc.py`, `find_binders.py`) and pointed them at this folder. `work/correct.tsv`
lists the expected symbol for every GetDef / GetSubDef instantiation these rows call. For each class I added the class's own pieces
behind new `FABLE_RETAIL_*` opt-ins in the shared header: the real member type of the def handle, the PDB methods the rows define or
call, and the inline PeekDefinition. New def headers give the members at their retail offsets (Ego_r - 4). Each row was then rewritten
as plain member code over those headers, with GetDef / GetSubDef coming from the shared noinline template bodies
(`FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY` / `FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY`). Almost every row matched on the
first grade with no shape tricks. The local stand-in GetDef bodies, CDefHolder / CHeldDef types and hand-written releases are gone.

`work/final.sh` regraded all 28 changed rows together (`work/final.log`):
- 28/28 RELOCATION_MATCH.
- g++ -std=c++11 PASS 28/28 and VC7.1 test PASS 28/28.
- No HARD findings.
- Every catalog PassPattern is present (`work/pp.py`).
- No RELOC_NAME / RELOC_CONFLICT warnings from the 27 rows other than 0x0041E6D3, apart from the PeekWorld label warning described
  under "Warnings seen".
- `hdrcheck2.py hdr`: `default=same` on every changed header, and `same` for every pre-existing macro. Every changed header is the
  current repo file with lines added only (`diff`: 0 removed lines).

## Rows changed (28)
- **CThingBuilding** (7):
  - 0082D2D0 PeekPSimDefinition
  - 0082D5C0 PeekPMaterial: now the protected virtual MBE returning `CDefPointer<const CMaterialDef>`; the old stand-in type was
    CWallMaterial
  - 0082D6B0 **GetMaterialOnHitByMelee** (was GetMaterial)
  - 0082D6F0 OnCreate (MAE)
  - 0082D7D0 GetFloorMaterial (UBE)
  - 0082D870 **UseHighDetailQuadTree** (was UsesHighDetailQuadTree)
  - 0082D8C0 **GetReverbSettings**(EReverbEnvironmentType&, float&) const (was GetReverbEnvironment(long*, long*))
- **CThingObject** (4):
  - 00703050 ConstructFromParams (IAE, `const C3DVector&`)
  - 007030C0 SerialiseGameState (MAE)
  - 00703110 ConstructFromLevelScript(const char*) (MAE; was a CLevelScriptThingInfo reference)
  - 00703150 PeekPMaterial (MBE)
- **CThingHolySite** (3):
  - 0083BAB0 ConstructFromParams (IAE)
  - 0083BB40 SerialiseGameState
  - 0083BBA0 ConstructFromLevelScript(const char*)
- **CThingCreatureBase** (7):
  - 00662880 ConstructFromParams (IAE)
  - 006628E0 SerialiseGameState (UAE)
  - 00662930 ConstructFromLevelScript (UAE)
  - 00662CC0 AnimExists
  - 00663140 PreloadAnims
  - 006631C0 PreloadAnimation(const CCharString&) (was `long`)
  - 00663610 PreloadBasicAnimations
- **CThingAICreature**: 00830F20 AnimExists (MBE override).
- **CThingDeadCreature**: 00835B90 PeekDefinition.
- **CThingPhysicalSwitch**: 0083D900 **PeekDefinition** (was "CThingDeadCreature::PeekDefinition"; see item 3 below).
- **CThingMarker**: 0083D190 GetCurrentGraphic (by value), 0083D280 GetProperties (UBE).
- **CThingVillage**: 0083B520 GetCurrentGraphic (returns `const CEngineGraphic*`, IBE).
- **NUISystem::CManager::Action 0041E6D3**: three def-handle types swapped back (item 3). The landed test is unchanged and still passes.

Self-symbol changes: every row above except 0041E6D3. Each one now takes the exact PDB spelling: access letter, const, virtual,
parameter types. Every row still grades against the retail bytes.

## Brief item 3: settled
- **0x0083E0D0 = GetDef<CThingPhysicalSwitchDef>.** Evidence:
  - FableWin.pdb has the spelling.
  - Its only callers are 0x0083D900, TriggerMe 0x0083DDC0 and UntriggerMe 0x0083DF20 (catalog scan).
  - 0x0083D900 sits in the CThingPhysicalSwitch block, and its only callers are TriggerMe and UntriggerMe. So 0x0083D900 is
    `?PeekDefinition@CThingPhysicalSwitch@@IBEABVCThingPhysicalSwitchDef@@XZ` (Ego_r), not CThingDeadCreature's (RENAMES.tsv).
- **0x0042B144 = GetDef<CDefClassBase>** (`??$GetDef@VCDefClassBase@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCDefClassBase@@@@@Z`).
  - FableWin.pdb keeps the NUISystem::CManager TU's COMDAT run in source order: GetDef<CThingBaseDef> 0x01B1FBFD,
    GetSubDef<CObjectAugmentationsDef>, GetDef<CThingObjectDef> 0x01B20714, GetSubDef<CAppearanceDef>, the CViewport cast that only
    UI_ACTION_LOAD_WEAPON_DESC uses, GetDef<CDefClassBase> 0x01B20A80, GetSubDef<CWeaponDef>, ..., GetSubDef<CAugmentationDef>.
  - That is retail's run 0x0042AF3C / AF9E / B040 / B0A2 / B144 / B1A6 / B248.
  - 0x0042B144 has exactly one caller in Fable.exe: 0x00423647, the LOAD_WEAPON_DESC site.
  - The same order proves 0x0042AF3C = GetDef<CThingBaseDef>.
  - The landed 0x0041E6D3 text had the two types swapped (CDefClassBase at the AUGMENT and augmentation-slot sites 0x0041E880 /
    0x004248BD, CThingBaseDef at LOAD_WEAPON_DESC). It is fixed and regraded.
- **Preload rows.** The spellings are now settled:
  - 0x004C7AB0 = `?PeekWorld@CThing@@IBEABVCWorld@@XZ`. Ego_r donor order GetWorld 0x004C9640 / PeekWorld 0x004C9650 = retail
    0x004C7AA0 / 0x004C7AB0. The callers are const chains.
  - The mesh bank comes through the inline `CWorld::PeekMeshBank() const` (+0x3C).
  - 0x00A242C0 = `?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z`: the catalog name, in the CMeshDataBank Peek* run.
  - 0x005DC290 = `?GetNoAnims@CAnimationSet@@...`: catalog name plus Ego_r.
  - 0x005DC340 = `GetAnim`.
  - C3DAnimationInfo TouchAnimation / BeginLoading.
  - All three Preload rows moved onto them.

## What is left and why (binders_left in STATUS.tsv)
- **0x004C7990 (16 left).**
  - 0082D3B0 PeekPCameraManagerSetDefinition: needs a CRegionDef header (CameraManagerSetBuildingIndex at retail +0x4C) and the
    CWorld / CWorldMap / CRegion chain. No attempt was made.
  - 00664A50 / 00665510: CTCCombat, the busy virtual slot 77, and the combat-type defs.
  - 00832E60 / 008338D0 / 00833A70 CThingAICreature: brain defs, the STLport `set<void*>` suspenders and CThingCreatureDef members.
  - 00836260 CThingDeadCreature::WorldUpdate.
  - 0083CC10 CThingSwitch::OnThingWithScriptNameApplied and 0083DDC0 / 0083DF20 CThingPhysicalSwitch TriggerMe / UntriggerMe: these
    expand PeekDefinition at one site and call it out of line at another (an inline-budget split), plus HasTC / GetTC and the
    sound-player types.
  - 006A9EF0 CThingPlayerCreature::OnCreate.
  - The script-interface rows 00890F40 / 0089E080 / 008CB780 / 008D08A0 / 008D1130 have deep local models.
- **0x0042B0A2 (21 left).** Mostly non-CThing users (CTCDoor, CTCChest, CTCMeleeTrap, inventory screens and similar).
  - 00835BD0 / 00835E30 CThingDeadCreature use an STLport for_each functor over the animation set.
  - 006A9DD0 CThingPlayerCreature::ConstructFromParams has globals and progress display; its GetSubDef there is CAppearanceDef.
  - 00667B30 needs the combat-animation queries.
- **0x0042AF3C (59 left) and 0x006869D0 (44 left).** These are CTC component rows (inventory, shop, door and so on), which NF8
  already listed; this pass worked on the CThing family.
  - 0x0072F170 CTCDummyVillager::SetupFromData binds `GetDef<CDefClassBase>` at 0x0042AF3C. Its handle must be
    `CDefPointer<const CThingBaseDef>`, and the change surfaces as a RELOC_CONFLICT against the fixed 0x0041E6D3 (NAME_FIXES.tsv
    0x0042AF3C lists it). It is a whole local-model row, so it was not changed here. A type swap plus the shared CTCBase header
    would do it.
- **CThing virtual slots 29-86.** Not declared in CThing.h, because a partial run would break slot order. The slot / name pairs
  this pass proved are in hdr/README.md. The derived headers declare the overrides with the PDB access, and the rows that define
  them grade.

## Header debt seen
- NGraphicAppearance_CTCGraphicAppearance.h `_MOVEMENT` defines CEngineGraphic itself through the quarantine header, which clashes
  with engine/CEngineGraphic.h. The new `_SETMAINGRAPHIC` opt-in switches it to the shared header.
- C3DAnimationInfo.h: the default (generated) branch has no methods. `_LOADING` adds the two the preload rows call.
- The def headers' sizeof asserts hold only with `FABLE_RETAIL_CTHINGBASEDEF`, because the opaque default CThingBaseDef is
  1-aligned.
- The enum types EReverbEnvironmentType / EPSwitchTriggerType / ESwitchTriggerType are declared without enumerators: no name set
  on disk has them. The landed 0x0083CD70 has SWITCH_TRIGGER_* names, but they are unproven.

## Descriptive names
None introduced. Every name is a PDB spelling.

## Warnings seen
- 0x0041E6D3 (unchanged from its landed state apart from the swap) still reports RELOC_NAME label notes and two pre-existing
  RELOC_CONFLICTs: SetCameraYZInverted vs 0x0059697A, and CCountedPointer<CList>::SetStaticCast vs 0x00558770.
- The PeekWorld rows report a RELOC_NAME note: the catalog label `_global::GetGlobal_013b8a1c_deref0x24_004c7ab0` is a placeholder.
