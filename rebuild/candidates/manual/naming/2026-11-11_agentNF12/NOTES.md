# NF12 notes

All 7 changed rows were regraded together at the end (`work/final.sh work/rows.txt`, --all): RELOCATION_MATCH, the target BIND OK,
no HARD / RELOC_CONFLICT, VC7.1 tests PASS. Tests are unchanged (no rows/<addr>_test.cpp), so the catalog PassPatterns are untouched.
g++ tests: 007884f5, 00788406, 008338d0, 0083ddc0 PASS; 0078809b, 0083df20, 007310c0 are PREFAIL -- their landed tests embed a copy
of the old source (`__declspec(noinline)`, `_snprintf`, a 32-bit pointer assert) and already failed g++ on the landed tree; they stay
PREFAIL. Each row turns on its opt-ins with `#define FABLE_RETAIL_...` lines at its top; none needs a `// Flags:` line.

## Rows changed
- GetSubDef<CAppearanceDef>: 007884f5 CTCMeleeTrap::ResetTrap, 00788406 TriggerTrap, 0078809b InitialiseAnimation (new
  engine/CTCMeleeTrap.h, CTCTrapBase retail opt-in), 007310c0 CTCDoor::UpdateOpenStatusTriggerTypeScript (CTCDoor opt-in).
- CThing::PeekDefinitionManager: 008338d0 CThingAICreature::InitialiseFromDef, 0083df20 CThingPhysicalSwitch::UntriggerMe,
  0083ddc0 TriggerMe.
- The stand-in bodies those rows carried (GetSubDef, GetDef, LowerBound, PlayAttachedSample) are gone; the real template bodies come
  from the shared headers' GETSUBDEF_BODY / GETDEF_GLOBAL_BODY / GETDEF_NAME_BODY opt-ins.

## Left, with reasons
- 005b37f7 CTCInventoryClothing::InitialiseMannequin: parked at DIFFER(759v760), best source in work/parked/005b37f7.cpp (7 grades;
  uses the new CInventoryDef DOLLS opt-in and CTCDNone.h). One residue: the TCI_SKELETAL_MORPH (0x78) lookup. Retail lays that one
  lookup out as `cmp [eax],ebx ; jg ; mov edi,eax` and loads the component into eax before `mov ecx,PMannequin` (the other three
  lookups in the same function are the `jle` GetTC form). HasTC + GetTC(id) with FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND +
  FORCEINLINE_FIND gives the `jg` layout but pushes `[edi+4]` straight; `&GetTC(id)` / GFDebugPDynamicCast / a reference local give
  the same. The landed source got it from an invented `GetInterfaceFound` (the conditional `(it!=end && !(k<it->first)) ? it : end`
  with a store through the out pointer); no PDB-named CThing member has that body. Next idea: a CTCSkeletalMorph header and
  GetTC<CTCSkeletalMorph> (FableWin.pdb has that instantiation) with a Find-based typed GetTC body under an opt-in.
- 005cd767 CTCInventoryStats::InitialiseMannequin: the same 0x78 residue (and the same CInventoryDef DOLLS opt-in,
  StatsPersonalityDoll +0x2E0); not attempted separately.
- 007306c0 / 00730c00 / 00730dd0 CTCDoor OnInitialActivate / Open / Close: each calls CTCPhysicsBase virtual slot 85 (+0x154,
  0x006B1340, a 3-byte `ret 4` with no name in the catalog) on CThing::PhysicsTC +0x60, plus CTCSwitchableNavigation /
  CTCObstacle / CDoorDef / PlaySoundW models. The slot needs a proven name and the full CTCPhysicsBase virtual order.
- 007502d0 CTCChest::InitClosed: needs COpenCloseHelper (Init / InitOpened / IsTransitioning), C3DAnimationInfo
  GetNonLoopingDuration, CAnimateThing main-set members on shared headers.
- 00779d20 CTCDExperienceOrb::Construct: virtual slot 3 call and the unidentified `or [+0x59],2` field (as NF11).
- 004bfd10 CatchMessage: no new idea (NF11 parked it).
- PeekDefinitionManager rows: 00665510 UseAbility (CThingCreatureBase virtual slot 72 SetActionForMelee, protected MAE; CThing's
  virtual slots up to 72 are not declared), 00664a50 UnsheatheDefaultWeapon (slot 77 virtual), 00832e60 OnDie (slot 77),
  0083cc10 CThingSwitch::OnThingWithScriptNameApplied (slot 62 virtual), 00833a70 CThingAICreature::OnCreate (expands SuspendAI
  0x00833A10 over the Suspenders std::set<void*> + insert_unique 0x00835560: needs the real STLport set on the shared header),
  00836260 CThingDeadCreature::WorldUpdate (body reorienter, particle emitter Create / GetTCDriver, CTCFadeOutAndIn),
  006a9ef0 CThingPlayerCreature::OnCreate (many members exist; the def-pointer clears / vector clears / g_HeroCreationState shape
  need work -- the most promising next row), 008cb780 (no CCreatureAction_SummonerSheatheWeapon / SheatheWeapons headers, CTCCarrying
  calls), 0082d3b0 CThingBuilding::PeekPCameraManagerSetDefinition (CWorldMap region chain), 008d1130 (event filters, quest manager).
- Item 3 (GetDef<CThingBaseDef> 0x0042AF3C / CTCBase::GetDefinitionManager 0x006869D0): all 52 rows of NF10's "left" lists were
  compile-checked (work/item3_pre.txt): every one still binds a wrong spelling (none stale this time). Not attempted: the small
  ones need container models first (005bf9e3: the two serialised CCountedPointer<vector<..>> members of CTCInventoryBase and
  their out-of-line ReleaseData; 005bd82e / 005bdfb1: CItem / Categories; 007366c0: std::remove).

## Header debt / naming notes
- CSoundInit now has two inline-ctor spellings (INLINE_CTOR: GainFaderBits in the body; INLINE_CTOR_TYPE_LAST: only PlaybackType
  in the body). Both have the landed defaults; which one is Lionhead's is unproven (VC7.1's expanded store order differs).
- 0x013BAD34 / 0x013BAD38 (DOOR_OPEN / DOOR_CLOSE CCharStrings) each have two landed spellings (NAME_FIXES); 0x007310C0 uses the
  Open / Close rows' const spelling.
- g_bForceStandAroundBrain (0x013B86EA): established descriptive spelling kept (no PDB data name on disk).
- CThingCreatureDef MEMBERS opt-in keeps enum and vector members opaque (no enum definitions on disk for ESex,
  EThingCreatureProperty, ECreatureInteractionType, EControlledMovementType; ENavigatorType vector).
- The trap anonymous-namespace strings (STrapReset / STrapTrigger) keep their established names; the ?A0x hash differs per TU
  and matches by prefix.
