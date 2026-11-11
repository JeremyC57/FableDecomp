# Hand chunk 2027 (CTCTurncoatSpell .. NScript::CQ_OrchardFarmRaidScript::CTeamSpawn): 1 of 2 byte-exact

## What matched (1)
- **0x00DCD350 NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::Main** (1833 bytes, RELOCATION_MATCH on grade 2; vland RELOCATION_MATCH,
  behaviour PASS under g++ and VC7.1). The generated-script form (TRetVoid `_Fail_Val`, a terminate check at the head of every block
  and after every loop) over a new shared header engine/CQ_OrchardFarmRaidScript.h. Shape lesson: the dead `HeroTeam` / `TeamID` loads at
  0x00DCD4C5 and the dead `cmp edx,ecx` at 0x00DCD88A are `if (TeamID == ParentClass->HeroTeam) { check } else { check }` statements whose
  arms hold only the terminate check: c2 merges the identical arms and drops the branch but keeps the loads. Duplicating the real body
  in both arms (grade 1) gives 2850 bytes: cross-jumping does not merge large identical arms.

## Parked for Fable 5.1 (1)
- 007c6e50 CTCTurncoatSpell::FrameUpdate DIFFER(1804v1806): XP block only -- retail `mov edx,[esp+14h]` (0x007C741C, hoisted above
  `mov [esi+90h],ecx`), `mov eax,[edx+2Ch]`, `mov esi,edx` (0x007C7432) and the temp rotation after it (eax/ecx/edx at 0x007C7434..
  0x007C749E); ours `mov esi,[esp+14h]` after the store, `mov eax,[esi+2Ch]`, no copy, rotation shifted by one | tried: 4 grades (if/else
  chain orientation for the absorb branch, implicit CThing* -> CIntelligentPointer conversion, the `bool in_range = d2 <= r*r` form,
  FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY, FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY -- the last fixed the turncoat / PTarget
  colouring, 0.926 -> 0.950) plus 16 c2prio-checked spellings of the XP block / caster local (declaration position, `!--x`, split
  decrement, `= 0` init, explicit HasTC + GetTC(type), GetTC(type, &p), CThing& / CThing* views, cast at the creature calls, nested vs
  chained conditions, no MakeNoise, ParentThing at the stats / sound lookups, a word-0 interface at the XP lookup) -- all leave the
  deciding allocation unchanged; a c2 what-if forcing that piece's spill to 0 at list insertion did not change it either | next: dump c2's
  split-piece membership (0x1078B51C pass 2 bitsets) for the caster (#19) to see why the XP HasTC load lands in its own 2-ref piece
  (#73, prio 14, spill 2, coloured esi by the copy bonus with the GetTC map) while the absorb lookup's piece is skipped (spill 0, codegen
  temp + copy, as retail does at XP), then look for the spelling that gives the XP lookup the absorb's piece structure.

## Skipped
- none. Both rows start at a real function start and end where the catalog says (0x007C6E50 `sub esp,64h` .. `ret` 0x007C755D, FinalCleanup
  at 0x007C7560; 0x00DCD350 `sub esp,74h` .. `ret` 0x00DCDA78, MakeTeamMemberComment at 0x00DCDA80). No SPLITS.tsv.

## Names and evidence
- 0x007C6E50 ?FrameUpdate@CTCTurncoatSpell@@UAEXXZ: Ego_r.pdb + FableWin.pdb name sets; Ego_r donor order (StopTurncoatingEffects /
  FrameUpdate / FinalCleanup = retail 0x007C6DA0 / 0x007C6E50 / 0x007C7560); vtable 0x0126E5C4 slot 9 (CTCBase::FrameUpdate slot).
- 0x00DCD350 ?Main@CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ: both PDB name sets; CTeamSpawn vtable 0x012DB2D4 slot 1 (the landed
  ctor 0x00DCD160 stores it; slot 2 Init 0x00DCD1D0, slot 3 GetParentScript 0x00DCD1C0).
- Callees (all bound spellings reused except where noted): CTCSpecialAbilities::FadeOrKillEffect(CThing*) 0x0072AC40 and
  CreateParticleEffect 0x0072AB70 (static __fastcall; Ego_r donor order CreateParticleEffect, FadeOrKillEffect(CIntelligentPointer&) 0x0072ABD0,
  FadeOrKillEffect(CThing*) 0x0072AC40; the 0x0072AC40 body null / dead-tests ecx and fades its particle emitter); CTCTurncoat::
  SetTurncoatOriginator(CThing*) 0x007C76D0 (Ego_r / FableWin; the body assigns a CIntelligentPointer<CThing> temporary to +0x24
  TurncoatOriginator; an out-of-line COMDAT of the spell TU, FableWin emits it right after the spell's FrameUpdate); CTCHeroStats::
  ModifyStaminaBy 0x0057B1DB; CTCHeroExperience::AddTrainableStatExperience(EHeroExperienceStatCategory, long) 0x006D8300 with
  CHeroAbilityDef Stat +0x40 / StatExperienceGainedOnUse +0x7c (engine/CHeroAbilityDef.h, landed 0x005EEA9B reads the same);
  CDefinitionManager::GetDef<CHeroAbilityDef>(const CCharString&, CDefPointer<const CHeroAbilityDef>*) const 0x00792350 (Ego_r / FableWin
  ??$GetDef@VCHeroAbilityDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@Z; the landed 0x00792350
  row names only "GetDef<T>"); CQ_OrchardFarmRaidScript::MakeTeamMemberComment 0x00DCDA80 (Ego_r / FableWin, see NAME_FIXES).
- Descriptive: none. `ERaidComment` has no enumerator names in either name set (the row passes `(ERaidComment)0`); MissionFailed's
  `<unnamed-type-MissionFailed>` type is modelled as `long` (the row stores 3).

## Header debt
- engine/CTCTurncoatSpell.h, CTCTurncoat.h: the default (generated / flattened) branches stay; the retail classes are opt-ins.
- engine/CQ_OrchardFarmRaidScript.h: MissionFailed typed `long` (PDB unnamed type); the landed rows of this class (0x00DCC770 Main,
  0x00DCD1D0 Init, 0x00DCD160 ctor, 0x00DCDA80 MakeTeamMemberComment, ...) still use local models.
- The landed CTCTurncoatSpell rows (0x007C6AA0, 0x007C6DA0, 0x007C6A40, 0x007C6940 ...) use local models and invented callee spellings
  (NAME_FIXES.tsv); they can move to the CTCTurncoatSpell.h opt-in.
