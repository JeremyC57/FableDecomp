# Hand chunk 489 (std::__pop_heap_aux .. std::__pop_heap): 39 of 40 byte-exact

11 MATCH + 28 RELOCATION_MATCH, 0 retail_asm, 0 parked, 1 skipped (007891f2, two functions in one row).
All 39 tests PASS under tall.sh (g++) and tall.py (VC7.1/Wine). vland dry run: 39 wins, no RELOC_IDENTITY, no DIFFER.

## What matched (39)
- STLport heap/sort (std): 00758d10 `__pop_heap_aux` over pair<EBodyLocation, CWoundMorphs::CEntry> (0x2C stride). It matched only once
  `__pop_heap` was `__forceinline`; with plain `inline` it is emitted out of line as a cdecl call (54 v 113).
  008178e0 is `__pop_heap` over the 32-byte THitLocationIntersectionElement. 00809f30 is `__insertion_sort` over CIntelligentPointer<CThing>
  with the anon CDoNotUseBinaryPredicate: `ThingPointer(*__i)` is built straight into __linear_insert's by-value argument slot
  (base ctor, vtable store, SetPItem(GetPItem())), and the callee destroys it.
- STLport small helpers (std):
  - 0076dae0 `_List_iterator::operator--`.
  - 0076dd10 `pair<C3DVector,C3DVector>` copy ctor. It needs the full mangled leaf, because the grader otherwise picks the larger 2-arg ctor.
  - 007771e0 `__lower_bound<long*>`.
  - 007776a0 `_Rb_tree::equal_range`. The iterator needs STLport's user copy ctor; without it the pair stores sink below the pops.
  - 00806800 / 00806850 `__copy_ptrs(.., __true_type)`: two byte-identical memmove instantiations.
  - 00806840 `__distance` (random access, cdecl).
- CTCHeroRangedAttacks:
  - 007899d0 AttemptToLoadRangedWeapon (two bool stores).
  - 00789a60 GetShotBuildupTime, `(float)AimingTimer / g_framesPerSecond`.
  - 00789a80 GetShotAccuracyFraction, `Accuracy * 0.01f`.
  - All three were named by the Ego_r donor TU order.
- CTCTrapBase:
  - 00787bf1 Reset (protected, `#pragma optimize("s")`). `TrapOpener.Close()` is an inline member of the CFadeInFadeOutBase sub-object, which keeps
    retail's store-before-vptr-load order; written as direct member stores, VC7.1 hoists the vptr load. Then it calls the virtual ResetTrap (slot 31).
  - 00787bfe ManualDeactivate: `Active = false; Reset();`.
- CTCBase virtual overrides (named by vtable slot + the CTCBase slot order from the Ego_r donor):
  - 00779420 CTCDRegionExit::OnInitialActivate.
  - 007c9760 CTCCreatureGeneratorCreator::OnCreate, setting the LevelPersistent bitfield bit 4 at +0x92.
  - 007e3690 CTCAtmosPlayer::OnCreate.
  - 007e5b30 CTCFireRangedWeaponOnLeavingStateGroup::OnInitialActivate (SuspendAI(true, this)).
  - 007ef6f0 CTCDecapitation::Copy.
  - 007f8b50 CTCParticleAttacher::OnInitialActivate (`if (ParticleList.empty()) AttachParticles();` tail jump).
- 007bbe24 NThingShot::CTCShotDefinitionTearInGround::OnPassingTarget: vtable slot 41, `SetDieInSeconds(2.0f)`, protected, size TU.
- 00810e50 NEntityEvents::CEventHitBase::GetLifeTime: `2 * g_framesPerSecond` (add eax,eax).
- 00780400 CTCFlammable::IsOnFire, `FireResistance <= 0`. The FirePoints placeholder is a byte array, not pointers.
- 00780800 NFire::CFirePoint::AddFireDamage: `Resistance -= d; if (Resistance > 0) SetAsSmoldering();`.
- CTCBerserk, both through the "unset local" GetModeManager accessor of the dead-frame-slot component:
  - 0078c780 Pause.
  - 0078c7f0 UnPause.
- 0079ec10 CTCPhysicalShieldSpell::UnPause: the sound CCharString is scoped in its own block, so its destructor runs before PlayShieldEffects.
  It calls PlaySoundW, which is protected per the PDB.
- 007a0950 CTCWillLightning::FinalCleanup: GetInterface 0xa7, then if CurrentAbility == 0xb it clears the lock and the ability, then StopLoopingSound.
- 007a6b20 NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::ReplaceMeshWithNext (wrap at the def's mesh count).
- Implicit destructors (no vptr store; emitted out of line through a helper `p->T::~T()`):
  - 0079a4d0 CTCShopKeeper.
  - 007d1aa0 CTCTavernGameCoinBase: `vector<Coin>` at retail +0xfc with 0x3c-byte elements whose dtor is virtual (Ego_r `??_GCoin`), then a
    tail jump to ~CTCTavernGame 0x005EDA50. The local CTCTavernGame base is 0xf0 bytes in retail.
- Filters:
  - 007fda20 (anon) CThingFilter_IsAffectedByObstructions::operator(): HasTC(0x31) && current mode GetType() == 0x5f → false.
  - 0080e400 (anon) CThingFilter_IsThrownByExplosion::operator(): mode == 0x42 → true.
  - Both use the landed CTCWaterWader inline HasTC/GetTC model (const LowerBound 0x004365B0, CTCCreatureModeManager::GetCurrentMode
    0x0073A6E0). The manager must derive from a polymorphic CTCBase for the static_cast.
- 008093d0 CThingSearchTools::GetAllThings<CThing, vector<CThing*>, CThingFilter_None>: the landed 0x0052B270 model with mask 0
  (gives the dead `xor al,al ; jne`) and vector<CThing*>::_M_insert_overflow 0x00436AC0.
- 0080ff20 CThing::GetInterface<CTCAISummon>: the out-parameter interface lookup for 0xFA as an explicitly instantiated member template.
  The kind comes from a traits enum.
- 00806820 CIsNotThing_00806820::operator(): retail reads the member before the GetPItem call. That needs a named local copy
  (`CBaseObject* thing = Thing; return p.GetPItem() != thing;`); the direct form reloads it after the call (27 v 25).
- 00802370 InitTargetingPlayerFlags_00802370: the two flag globals 0x013BAF20 = true and 0x013BAF21 = false.

## Names
### Proven
00760000
00779420
00780400
00780800
00787bf1
00787bfe
007899d0
00789a60
00789a80
0078c780
0078c7f0
0079a4d0
0079ec10
007a0950
007a6b20
007bbe24
007c9760
007d1aa0
007e3690
007e5b30
007ef6f0
007f8b50
007fda20
008093d0
00810e50

### Probable
00758d10
008178e0
0076dae0
0076dd10
007771e0
007776a0
00806800
00806840
00806850
00809f30
0080e400

Notes on the probable rows:
- STLport algorithm names come from the body shape plus the callers/callees.
- 0076dae0, 007776a0, 00806800, 00806840 and 00806850 have unproven element types (per-row CUnproven* placeholders).
- 0080e400: the PDB has no out-of-line IsThrownByExplosion operator. The evidence is its CThing* overload 0x0080E460 and the
  inlined copies in ForEachIf<.., CThingFilter_IsThrownByExplosion, ..>.

### Descriptive
00802370
00806820
0080ff20

Notes on the descriptive rows:
- 00802370 is a module initialiser, with no PDB name.
- 00806820 is an unreferenced functor with no PDB counterpart.
- 0080ff20 is an unreferenced GetInterface<CTCAISummon> COMDAT; the template's real name is unknown.

## Parked for Fable 5.1 (0)
(none)

## Skipped
- 007891f2: one catalog row holds two functions. It is CTCTrapBase::BulletTimeFrameUpdate (`mov eax,[ecx] ; jmp [eax+24h]`, 5 bytes) plus,
  at 0x007891F7, the separate `ret` that is the CTCTrapBase::InitTrap base body (vtable slot 28). The row needs splitting.

## Label corrections
| Address | Catalog / manifest label | Correct name |
|---|---|---|
| 007899d0 | CMeleeApprentice::Init | CTCHeroRangedAttacks::AttemptToLoadRangedWeapon |
| 00789a60 | CTCTalk::GetCurrentSpeechDuration | CTCHeroRangedAttacks::GetShotBuildupTime |
| 00789a80 | CMap::DrawGetSeaLevel | CTCHeroRangedAttacks::GetShotAccuracyFraction |
| 00787bf1 | Cleanup | CTCTrapBase::Reset (protected) |
| 00787bfe | AddToInterface | CTCTrapBase::ManualDeactivate |
| 00802370 | $E2 | module initialiser (descriptive InitTargetingPlayerFlags_00802370) |
| 0079a4d0 | Catch@0047e8cc (propagated) | CTCShopKeeper::~CTCShopKeeper |
| 007d1aa0 | Catch@0047e8cc (propagated) | CTCTavernGameCoinBase::~CTCTavernGameCoinBase |
| 005eda50 | CChunkedFileChunk::DestructorCleanup (landed) | CTCTavernGame::~CTCTavernGame (Ego_r `??1CTCTavernGame@@UAE@XZ`; the tail jump of 007d1aa0) |
| 0080e480 | CThingFilter_IsAffectedByObstructions::operator() (manifest) | (anon) CThingFilter_IsAffectedByExplosions::operator() |
| 007bed90 | SetTimeToDie (landed) | CTCShotDefinitionBase::SetDieInSeconds (Ego_r `?SetDieInSeconds@CTCShotDefinitionBase@NThingShot@@IAEXM@Z`) |
| 007a0d80 | FinalCleanup (landed) | CTCWillLightning::OnRemove |
| 00787c0c | ManualReset (landed) | calls 0x00787BF1 as extern "C" `Cleanup`; that callee is CTCTrapBase::Reset |

Notes:
- 0080e480 is byte-identical to 007fda20 (the obstruction filter), but it is the explosion TU's Bind5 filter: the CFilter_Bind5<..,
  CThingFilter_IsAffectedByExplosions> of GetAllThingsInRoughArea 0x0080ECC0 calls it.
- 0x00617D30, 0x006AE5C0 and 0x007512C0 are labelled "GetTCDriver<CTCDExperienceOrb>" but test different bitset bits from
  0x004ACB50, so those labels are propagated.

## Callee names
- PlaySoundW (CTCBase, protected per Ego_r.pdb) is used in 0079ec10. Landed code spells it PlaySound.
- 0079ec10 uses the const `CDefStringTable::GetString`.
- 0x0073A6E0 is CTCCreatureModeManager::GetCurrentMode (returns CModeBase&). Slot 1 of the mode is `GetType() const` (unsigned long; Ego_r
  `?GetType@CMode*@NCreatureMode@@UBEKXZ`). Mode 0x5f = CModeHitByEpicSpell (GetType 0x00741CC0), 0x42 = CModeExplosiveForcePushed
  (GetType 0x008680E0).
- Interface 0xFA = CTCAISummon. Interface 0x31 = CTCCreatureModeManager.
- 0x00809650 is STLport __linear_insert<CIntelligentPointer<CThing>*, .., CDoNotUseBinaryPredicate> (still unlanded).

## Header debt
These rows use a local model although vland reports that a shared header exists (SHARED_HEADER_AVAILABLE):
- CTCBase: 007899d0, 00789a60, 00789a80, 0079a4d0, 007ef6f0, 007fda20, 0080e400.
- CFadeInFadeOutBase / CTCTrapBase: 00787bf1, 00787bfe. engine/CTCTrapBase.h models the vptr as data, so the virtual ResetTrap call
  needed a local model.
- CTCBerserk: 0078c780, 0078c7f0.
- CSpecialAbilitiesPhysicalShieldDef: 0079ec10.
- CTCWillLightning: 007a0950.
- CTCTavernGame: 007d1aa0. The retail base is 0xf0 bytes; the PDB has 12-vs-16-byte vector differences.

Retail layouts differ from the PDB where STLport containers are smaller (vector 0xC). Each source carries a `// Layout:` line.
