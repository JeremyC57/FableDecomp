# Hand chunk 794 (CThingObject .. CThingAICreature): 3 of 6 byte-exact

All three matches are RELOCATION_MATCH (bytes equal with relocations masked; every relocation checked symbol by symbol against the
retail call / data target, one symbol per target). Header changes: hdr/README.md (all opt-ins plus one new header).

## What matched (3)
- **CThingAICreature::Save 0x008304A0** -- appends the AI creature's level-script lines after CThingCreatureBase::Save (relation UIDs
  remapped through CThingManager::RemapUIDForSerialisation, brain name or NULL, ten flags, initial position). Shape: the CThingSwitch::Save
  idiom (`out += "Key " + value + ";\r\n";`); the remapped UID must be a named local first, else VC pushes ";\r\n" before the Remap call.
  The brain line builds its prefix as an explicit CCharString and picks `name == "" ? CCharString("NULL") : name`.
- **CThingObject::OnHit 0x007039A0** -- melee forward to the shot definition, vandalism / incomprehension deeds for a destroyed object,
  morality, BARREL fallback, wobble. Shape: a `posted` flag (retail `xor bl,bl` at 0x00703C87), `Contains("BARREL")` through the implicit
  conversion, CDefString::GetString forced inline (opt-in), Morality read as a public member. Constness matters per lookup: the BARREL
  branch's village-member lookup is a non-const GetTC (retail 0x00703D13 calls the non-const LowerBound 0x0040F020; a PeekTC grades
  RELOCATION_MATCH too but binds the const 0x004365B0 -- a relocation-identity error only the per-site target check catches).
- **CTCSummonSpell::CreateMinion 0x0079ACC0** -- creates and sets up the summoned creature. Shape keys: (1) same-TU
  `__declspec(noinline)` bodies for CDefClassBase::GetSubDef<CSpecialEffectsDef>, CSpecialEffectsDef::GetEffectFromString and
  CDefinitionManager::GetDef<CBrainDef> (retail 0x0079BD80 is in this TU): with the bodies visible VC knows the CDefPointer locals do not
  escape (retail's `dec [ecx+4]` release, effect/effects in the dead argument slots); (2) FableWin's inline CTCEffectOnDie setters
  (SetEffect / SetTimer / SetSoundEffect / SetAlphaFadeOutTimer): the reference parameter makes VC push the CCharString temporary with
  `lea` (0x0079B36E) instead of the constructor's eax; (3) the Normalise squared length as `X*X+Y*Y+Z*Z` (opt-in) for retail's Z, Y, X loads.

## Parked for Fable 5.1 (3)
- 007c4430 CTCHealLife::StartHealingEffect DIFFER(1980v1952): the loop's `HealthIncreaseAccum.push_back(0.0f)` is inline (retail calls the folded 4-byte push_back 0x00568850; the first site is inline in both), then the end pointer stays in a register (retail spills it to [esp+44h]) and esi/edi swap; retail also shares [esp+28h] between the target spill and the CThingFilter_IsNot temporary | tried: if-block vs continue, iterator outside the for, push_back(0), second continue, (flags & 8) && IS_THING_ALIVE outer test, same-TU noinline def bodies, pos by value | next: find the source form that halves the loop site's frequency estimate (probe/hl_p3.cpp: one extra `if` around the push_back outlines it exactly like retail)
- 006c6040 CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed DIFFER(2793v2377): retail tail-merges the vandalism (GetPOwner) and 0x35/9 (GetPThingInside) village-witness branches at 0x006C6228 (esi = the intelligent pointer), keeps the witnesses list node in ebx, compares 3 as an immediate (mine `mov ebx,3`), inlines IS_THING_ALIVE(villager) | tried: duplicated branches, shared hoisted locals, same-TU GetDeedFromHandle (now the handle copy matches), return informed | next: make the two branch tails allocate identically so VC cross-jumps them (or the real shared-tail spelling: POwner is private, so not a pointer-to-intelligent-pointer), then the list construction (retail stores the new node to two slots, no zero pre-store)
- 007de710 CTCWallMount::AttachObject DIFFER(2574v2578): only x87 order inside the inlined PostMultiply: retail emits each sum k3, k2, k1 with the memory operand first (`fld [a.E13] ; fmul st(2)`), mine the memory*memory product first; retail spills the inverse scale (`fstp [esp+14h]`) and schedules the copy's `rep movsd` before the `fdivr` | tried: PostMultiply sums in k1..k3, k3..k1 and k2,k3,k1 order (VC always emits the mem*mem product first), cached appearance pointers (fixed the prefix) | next: another SetInverseOfUniformScaleRotationTranslation / PostMultiply spelling (e.g. scale returned and reused, or a SetMultiply into a temporary)

## Skipped
None.

## Callee names (evidence) and descriptive names
- Proven (PDB spelling + retail use): CTCSummonSpell::GetValidTeleportPos 0x0079AA90, CDefinitionManager::GetDef<CBrainDef> 0x0079BD80,
  CHitParameters::IsMeleeStrike 0x00868860, CThingCreatureBase::Save 0x00661A60, GFFloatToCharString 0x009A08B0,
  CTCHeroOpinionDeedLog::GetDeedFromHandle 0x006C58F0 / PeekPDeedEffectsDef 0x006C5630, CTCCreatureOpinionOfHero::KnowsOfDeed 0x00681AE0 /
  InformOfDeed 0x00682810, CThingFilter_IsEligibleWitness ctor 0x006C8DE0 / operator() 0x006C8E90, CTCEnemy::IsEnemyOfFaction 0x0076C7D0 /
  IsAlliedWithFaction 0x0076C7F0, CTCParticleAttacher::DetachParticles 0x007F8860, CTCWallMountEffects::OnPutOnMount 0x007DF3E0,
  CTCTargeted::SetTargetable (inline, +0x10 Targetable), CTCEffectOnDie setters (inline), CMatrix3x4::PostMultiply /
  SetInverseOfUniformScaleRotationTranslation (inline).
- Descriptive (no PDB name found): GDeedUnknownThreatWitnesses (bool global 0x01375732, IRTOD), CGameDefinitionManager::PHeroStatsDef
  (+0xDC; no PDB layout for that class), CTCHeroStatsTrophies::NumMountedTrophies (+0xD4 of the TCI_HERO_STATS component, AttachObject local model).

## Label corrections
See NAME_FIXES.tsv: 0x0079AA90 (CTCSummonSpell_GetValidSummonPos -> CTCSummonSpell::GetValidTeleportPos), 0x00868860
(CStateMapOwner_00868860::HasStateZeroOrOne -> CHitParameters::IsMeleeStrike), 0x0079BD80 (GetDef_0079bd80 -> GetDef<CBrainDef>),
0x009A08B0 (CCharString::FormatDouble -> GFFloatToCharString), 0x007F8860 (CTCInterface82::ClearEntry_7f8860 ->
CTCParticleAttacher::DetachParticles), 0x007DF3E0 (unlabelled -> CTCWallMountEffects::OnPutOnMount).

## Header debt
- CTCHeroOpinionDeedLog.h is a new opt-in-only header (no default path); the members past OpinionDeedMap stay unmodelled.
- C3DVector::GetSquaredMagnitude now has two retail spellings selected per TU (X,Z,Y default retail branch; X,Y,Z opt-in). One of them
  is presumably the real source; the operand order of a one-line sum is context-sensitive in VC7.1, so a single body may exist that fits
  both TUs -- worth a recheck of the C3DVECTOR includers with the X,Y,Z body at some point.
- CMatrix3x4.h PREMULTIPLY / POSTMULTIPLY / SetInverse: all FableWin inline helpers; the landed out-of-line PostMultiply 0x00C1CF20 is
  __fastcall (math library), the inline header body is the same sums.
