# Hand chunk 564 -- 12 never-tried rows: 10 RELOCATION_MATCH, 2 parked (0 MATCH, 0 retail_asm)

All 10 src/ rows pass the read-only vland dry run (RELOCATION_MATCH + behaviour PASS, no RELOC_CONFLICT /
RELOC_DUPLICATE / RELOC_IDENTITY). Tests pass under g++ (tall.sh) and VC7.1 (tall.py); the two differ/ tests
(`differ/<addr>.test.cpp`) pass under both too.

## What matched
| addr | function | grade | notes |
|---|---|---|---|
| 0x0072ACF0 | CTCSpecialAbilities::CancelUsingAbility(EHeroAbility) | RELOCATION_MATCH | switch; jump table 0x0072AF1C checked entry by entry |
| 0x00743120 | NCreatureMode::CModeBriarRoseConcentrate::~CModeBriarRoseConcentrate (implicit) | RELOCATION_MATCH | emitted via `p->...::~CModeBriarRoseConcentrate()` helper |
| 0x00750850 | CTCChest::~CTCChest | RELOCATION_MATCH | implicit CTCContainerRewardHero / CTCContainer dtors inlined |
| 0x00753290 | CTCLook::~CTCLook | RELOCATION_MATCH | two bases (CTCBase, NGraphicAppearance::CAGraphicAppearanceComponent) |
| 0x0078C080 | CTCBerserk::~CTCBerserk | RELOCATION_MATCH | |
| 0x0078F5A0 | CTCDivineWrath::~CTCDivineWrath | RELOCATION_MATCH | out-of-line map `_M_erase` 0x00792300 |
| 0x00794AC0 | CTCUnholyPower::~CTCUnholyPower | RELOCATION_MATCH | out-of-line map `_M_erase` 0x00797840 |
| 0x00798620 | CTCForcePushed::~CTCForcePushed | RELOCATION_MATCH | |
| 0x007A0770 | CTCWillLightning::CLightningBeam::~CLightningBeam | RELOCATION_MATCH | non-virtual; out-of-line `_List_base::clear` 0x007A3910 |
| 0x007A73F0 | CTCBattleCharge::~CTCBattleCharge | RELOCATION_MATCH | three static FadeOrKillEffect calls |

## Parked for Fable 5.1
- 0x0072c430 CTCSpecialAbilities::IsAbilityAvailable DIFFER(584v584): same length, prologue, guards, PeekTC blocks, jump table and epilogues byte-exact; in each of the 15 `!HasTC` arms ours loads ParentThing into a rotating scratch register (`mov ecx|edx|eax,[esi+4]; mov eax,[reg+30h]`) where retail reuses esi (`mov esi,[esi+4]; mov eax,[esi+30h]`) | tried: const operator[] / _Unchecked_test / test() / direct word mask / bitset-reference HasTC (gets esi but leaves a dead `add esi,30h`, 46 vs 60 ndiff, longer), STLport-faithful reference ctor (_M_getword&), no ~reference, initializer-list reference, `== false` / ternary / if-return arms, GetParentThing(), `this->`, local `const CThing*` (hoists the load), CTCSpecialAbilities helper inline (forceinline and plain), member-template HasTC<T>(), plain inline HasTC | next: try a reference type whose operator bool is non-const or whose _M_wp is formed from `&_M_w[0] + word`, or a free inline `IsUsingTC(const CThing*, type)`; 3 of 4 grades used.
- 0x0077fde0 CPersistContext::Transfer<EHeroCentreDoorType> DIFFER(208v240): over-spanned catalog row; the true function (sub esp,8 .. end of its jump table, 208 bytes) is byte-exact with relocations masked; bytes 208..240 are the 2-argument Transfer<EHeroCentreDoorType> at 0x0077FEB0 | tried: chunk 561's Transfer<T> template with T = EHeroCentreDoorType (graded once) | next: re-split the row at 0x0077FEB0 and land differ/0077fde0.cpp as is.

## Skipped
None.

## Label corrections
- 0x00798620 `CTCTurncoat::~CTCTurncoat` -> `CTCForcePushed::~CTCForcePushed` (first store vtable 0x0126C09C, RTTI `.?AVCTCForcePushed@@`; CTCTurncoat has PDef at +0x14, this body uses +0x14 as a thing pointer).
- 0x007A73F0 `CTCActionUseBed::~CTCActionUseBed` -> `CTCBattleCharge::~CTCBattleCharge` (vtable 0x0126CBFC, RTTI `.?AVCTCBattleCharge@@`).
- 0x00750850 `CModeOutOfActionBase::~CModeOutOfActionBase` -> `CTCChest::~CTCChest` (vtable 0x0126957C, RTTI `.?AVCTCChest@@`).
- 0x00743120 `CModeBriarRoseConcentrate::~...` -> `NCreatureMode::CModeBriarRoseConcentrate::~...` (implicit; RTTI of the caller's vtable 0x01268548 carries the namespace).
- 0x007A0770 `CLightningBeam::~CLightningBeam` -> `CTCWillLightning::CLightningBeam::~CLightningBeam` (nested class, Ego_r.pdb).
- 0x0072C430 `CTCSpecialAbilities::GetAbilityAvailability` -> `CTCSpecialAbilities::IsAbilityAvailable(EHeroAbility) const` (no PDB name GetAbilityAvailability; PDB has IsAbilityAvailable; it calls IsAbleToUseAnyAbility first; UseSpecialAbility 0x0072CF52 calls it).
- 0x0077FDE0 `CPersistContext::Transfer<signed_char>` -> `Transfer<EHeroCentreDoorType>` (text-arm wrapper vtable 0x0126B038 RTTI); 0x0077FD10 manifest `CPersistTraits<float>` -> `CPersistTraits<EHeroCentreDoorType>::TransferBinaryIn`.
- Callee catalog labels that are propagated across identical bodies (vland RELOC_NAME warnings): 0x0079DCB0 `C3DMeshInfo::HasPhysicsMesh` is CTCPhysicalShieldSpell::IsShieldActive; 0x007CC9B0 / 0x007CBEE0 `_Dest_val<...CFlash>` are CTCMultiStrike::CancelMove / CTCMultiArrow::CancelMove.
- 22 spelling fixes in NAME_FIXES.tsv.

## Header debt
- engine/CTCBase.h, CTCBerserk.h, CTCWillLightning.h, CIVCountedPointeeBase.h, CWorld.h, CEnvironment.h (vland SHARED_HEADER_AVAILABLE): the generated headers flatten the vptr into `void* __vftable` and declare no virtual functions, so they cannot drive a virtual destructor chain, the CDefPointer `Delete()` slot call or the CTCBase / CTCTargetingBase slot calls; local models with retail offsets are used (`// Layout:` lines). CTCWillLightning.h has no nested CLightningBeam. CWorld.h models `scoped_ptr<CEnvironment>` as a raw `PEnvironment_ptr`.
- No engine header exists for CThing (retail TC bitset +0x20 / TC map +0x44 / Dead bit +0x91 / PTCGraphicAppearance +0x64), CTCSpecialAbilities, CTCForcePushed, CTCBattleCharge, CTCChest, CTCLook, CTCDivineWrath, CTCUnholyPower, CModeBriarRoseConcentrate.
- CIntelligentPointer and C3DVector are pack(1) in these objects; the headers that embed them must keep that.

## Names -- Proven (two sources: PDB name set + RTTI / caller / donor order / offsets; spelling checked with objdump -t)
- 0x0072ACF0 `?CancelUsingAbility@CTCSpecialAbilities@@QAE_NW4EHeroAbility@@@Z`
- 0x0072C430 `?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z`
- 0x00743120 `??1CModeBriarRoseConcentrate@NCreatureMode@@UAE@XZ`
- 0x00750850 `??1CTCChest@@UAE@XZ`
- 0x00753290 `??1CTCLook@@UAE@XZ`
- 0x0077FDE0 `??$Transfer@W4EHeroCentreDoorType@@@CPersistContext@@QAEXPBDAAW4EHeroCentreDoorType@@ABW41@@Z`
- 0x0078C080 `??1CTCBerserk@@UAE@XZ`
- 0x0078F5A0 `??1CTCDivineWrath@@UAE@XZ`
- 0x00794AC0 `??1CTCUnholyPower@@UAE@XZ`
- 0x00798620 `??1CTCForcePushed@@UAE@XZ`
- 0x007A0770 `??1CLightningBeam@CTCWillLightning@@QAE@XZ`
- 0x007A73F0 `??1CTCBattleCharge@@UAE@XZ`
- Callees: 0x0072ABD0 `?FadeOrKillEffect@CTCSpecialAbilities@@SIXAAV?$CIntelligentPointer@VCThing@@@@@Z`;
  0x0072AF30 `?IsAbleToUseAnyAbility@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z`;
  0x0079DCB0 `?IsShieldActive@CTCPhysicalShieldSpell@@QAE_NXZ`; CancelMove (`?CancelMove@<Class>@@QAEXXZ`):
  CTCPhysicalShieldSpell 0x0079E300, CTCSummonSpell 0x0079A820, CTCGhostSword 0x007C5710, CTCBulletTime 0x0079C640,
  CTCBerserk 0x0078C610, CTCMultiStrike 0x007CC9B0, CTCMultiArrow 0x007CBEE0;
  0x005EE49A `?IsAbleToUseAbility@CTCInventoryAbilities@@QBE_NW4EHeroAbility@@_N@Z`;
  0x0040F020 / 0x004365B0 CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound non-const / const;
  0x00686830 `??1CTCBase@@UAE@XZ`; 0x00686A70 `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ`;
  0x006B3950 `?TransitionToDefaultThemeAllChannels@CEnvironment@@QAEXJM@Z`;
  0x004C0000 `?UnregisterComponent@CTCGraphicAppearance@NGraphicAppearance@@QAEXAAVCAGraphicAppearanceComponent@2@@Z`;
  0x004C9B80 `?Kill@CThing@@QAEX_N@Z`; 0x00A01B50 `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`;
  0x00A01C10 `??1CBaseIntelligentPointer@@UAE@XZ`; 0x0099EAE0 `??1CCharString@@QAE@XZ`; 0x0073D860 `??1CModeStandBase@@UAE@XZ`;
  0x0099A2E0 `??1CBaseClass@@UAE@XZ`; 0x0099A300 `??1CBaseClassNonCopyable@@MAE@XZ`;
  0x00792300 / 0x00797840 the `_Rb_tree<...PerCreatureInfo@CTCDivineWrath / @CTCUnholyPower...>::_M_erase` (STLport 4.6);
  0x007A3910 `_List_base<CCountedPointer<CTCWillLightning::CLightningBeam>>::clear`;
  0x0077FD10 `CPersistTraits<EHeroCentreDoorType>::TransferBinaryIn`; 0x00993EB0 CDataOutputStream::WriteSLONG (inlined call).
- free 0x00BFEA14 (the STLport allocator deallocate in the vector / list / tree teardowns).

## Names -- Probable
- CTCTargetingBase virtual slots 28-30 (GetPBestThingForMeleeAttack, GetPBestThingForProjectileTargetingLock,
  GetPBestEnemyCreatureForAttack): PDB definition order only; slots 31-39 are confirmed by their bodies.
- EHeroAbility values 1..19 (ForcePush, SlowTime, Enflame, PhysicalShield, Turncoat, DrainLife, RaiseDead, Berserk,
  MultiStrike, Summon, Lightning, BattleCharge, AssassinRush, HealLife, GhostSword, Fireball, MultiArrow, DivineWrath,
  UnholyPower): from the UseSpecialAbility 0x0072CF40 jump table (value -> Setup* call) and the IsAbilityAvailable table;
  HEAL_LIFE = 14 also in STS_BriarRose.lua. Sources use numeric case labels with comments (no enumerator names invented).
- CTCSpecialAbilities::AbilityAvailability (+0x1C) modelled as std::vector<bool>; retail indexes it bytewise
  (`cmp byte ptr [ebx+eax],0`), so the retail type may be a byte vector / CArray<bool>.

## Names -- Descriptive
- `EmitImplicitDestructor` (0x00743120 source): emission device only, not a retail function.

## Review at landing
- 0x0077FDE0: re-split at 0x0077FEB0 before landing (row extent 240 vs true 208).
- 0x0072C430: register-only DIFFER; three of four grades used.
- RELOC_NAME warnings on 0x0072ACF0 (0x0079DCB0, 0x007CC9B0, 0x007CBEE0) are wrong propagated catalog labels; the
  PDB spellings above are correct.
