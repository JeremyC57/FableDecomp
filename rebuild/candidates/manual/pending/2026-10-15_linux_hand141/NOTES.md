# Hand chunk 141 (CGameScriptInterface .. CGameScriptInterface): 36 of 40 byte-exact

All 40 rows are CGameScriptInterface script-API entry points. Every source is generated from one shared
model (`prelude.txt` / `prelude2.txt` / `gsi.txt` + `body/<addr>.txt`, built by `mk.py`), so the sources are
self-contained but share one vocabulary: the thing's component presence bitset (+0x20) tested through an
inline `HasTC` that returns the masked word, the CVectorMap at +0x44 searched by the out-of-line LowerBound
(0x0040F020, const twin 0x004365B0) with the inlined `find` tail, `CScriptThing` slots 11 (GetThing) and 75
(IsValid), and the hero chain PlayerManager(+0x14) -> GetMainPlayer 0x00449970 -> GetHeroCreature 0x00487DC0 /
GetControlledCreature 0x00487DD0. Tests use `tprelude.txt` + `tbody/` (built by `mkt.py`).

## What matched (36)
- CGameScriptInterface hero-component calls (GetNumberOfItemsOfTypeInInventory, IsPlayerWieldingWeapon,
  ApplyHeroPenaltyForDeath, SetHeroHandLampAsLit, ChangeHeroHairstyle, CancelUsingAbility, CancelHeroTeleportEffects,
  InternalEndMovieSequence, SetCutsceneMode): the component lookup shape decides the match --
  plain `GetTC(t,k)` used as `this` loads `mov ecx,[eax+4]`; the out-parameter `GetTCOut(t,k,&p)` gives
  `mov eax,[eax+4] ; push ; mov ecx,eax`; a named local used later gives `mov esi,[eax+4]`. When retail reads a
  dead stack slot as the component on the "absent" path and duplicates the call (0x00897190, 0x00898EC0), the
  accessor is an inline with an UNSET local (`CTC* tc; if (HasTC(k)) tc = GetTC(k); return tc;`).
  `return w && w->HasTC(0x48)` gives the inner full-register 0/1 while the outer `return false` stays `xor al,al`.
- Script-thing component calls (EntityForceMarriageToHero, GiveThingHeroRewardItem, IsWearingClothingItem,
  IsWearingHairstyle, EntityGetStandingInsideBuilding, EntitySetPerceptionVariables, ImpSetEntityAsAllyOfThing,
  EntityPostOpinionDeed{ToRecipient,ToRecipientVillage,KeepSearchingForWitnesses}, EntitySetAsAwareOfThing,
  EntityAttachToVillage, EntityForceToLookAtThing, EntityDropGenericBox, Add/RemoveItemFromContainer,
  CreatureGenerator{Trigger,SetAlwaysCreateCreaturesOnTrigger,IsDestroyed,SetGeneratedCreatureScriptName,SetNumTriggers},
  IsCreatureFlying, EntitySetRangedTarget, PlayCriteriaSoundOnThing): validity is either `IsValid()` (slot 75) or an
  inline `IsThingAlive(thing)` (its own GetThing + null + dead bit) -- retail's second GetThing call before the
  mask test is the visible marker. A definition index used as an argument after another argument is a named local
  (retail resolves it before the pushes). The opinion-deed range flag `deed >= 3 && deed <= 10` is homed in the dead
  `thing` argument slot by the compiler on its own. AddItemToContainer's empty-name CCharString shares the lookup
  key's frame word (the dead `thing` slot) only as a named local in its own `{}` block. If/else with one call per
  arm (EntitySetRangedTarget) -- a ternary argument would merge the call.
- Create{Explosion,PhysicalBarrier,Rumble} (CScriptThing returned by hidden pointer, a by-value CCharString script
  name destroyed by the callee): `return t->GetScriptThing();` passes the hidden pointer straight through; the
  null result is an INLINE `CScriptThing()` over the out-of-line `CBase::CBase` @ 0x0099A2D0 whose counted-pointer
  member has an inline constructor zeroing both words after the vtable store (the zeroes come from the shared
  `xor edi,edi` register). The factories are `__fastcall`: Rumble's trailing `bool` travels in dl after the two
  stack floats; Explosion's direction overload is picked by a `dir ? f(i,p,*dir) : f(i,p)` ternary;
  `CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name)` pushes the name before the getter call.

## Parked for Fable 5.1 (4)
- 0089bb30 CGameScriptInterface::EntitySetThingAsWantingMoney DIFFER(141v142): retail keeps the lookup key and the "CTCMoneyReceiver" CCharString in the dead `thing` slot ([esp+8], no frame); ours adds `push ecx` and puts the string (or the key) in that new word | tried: temporary argument, named local in the else block, __forceinline GetOrAddTC helper | next: the EntityFadeOut form (`CTCBase* added; { CCharString name(..); added = AddTC(name,0,0); }` then `receiver = added`), which shared the slot correctly there
- 0089bbc0 CGameScriptInterface::EntityPlayObjectAnimation DIFFER(113v130): retail loads the component into ecx (`mov ecx,[eax+4] ; mov al,[ecx+0Ch]`) and tail-duplicates `inc ; push loop ; push id ; push anim ; call PlayAnimation` into both arms of `m_hasId ? m_id : -1` (`or eax,-1 ; inc eax` unfolded); ours joins with a jmp | tried: local + ternary, inline PlayNextAnimation member, if/return GetId, __forceinline | next: an out-of-line-defined (auto_inline off) GetId above the caller, or the id expression as a separate inline returning `GetId() + 1`
- 0089fe20 CGameScriptInterface::SetVillageAttitude DIFFER(133v133): register choice only -- the attitude switch runs in eax (`mov eax,[esp+0Ch] ; sub eax,0 ; dec eax`) in retail, edx in ours | tried: switch in the body, switch in a (__forceinline) component member | next: lane gap: register allocation
- 008a0720 CGameScriptInterface::EntityFadeOut DIFFER(157v155): register choice only -- retail keeps the fade component in ecx (`mov ecx,[eax+4]` / `mov ecx,esi ; test ecx,ecx`), ours in eax plus a `mov ecx,eax` before the call | tried: temporary name, named local, name in its own block before the null test, inline FadeOut member | next: make the GetTC path produce the component as `this` of an inline (`GetTC(...)->...` chain) so it lands in ecx; else lane gap: register allocation

## Skipped
None (no row spans two functions).

## Label corrections
- 0x00744470 / 0x007444A0 (manifest "CTCPerceiveThing::SetSightRadius", propagated): SetSoundRadius (+0x3c SoundRadius) and SetSmellRadius (+0x48 SmellRadius); only 0x00744540 is SetSightRadius.
- 0x004C7E50 (manifest "GFPredicateAnd<...CIsThingAlive...>"): called as `thing->GetScriptThing()` returning CScriptThing by hidden pointer.
- 0x008CFF40 (manifest "operator&&<...CIsThingAlive...>"): CScriptThing::CScriptThing() (the null script thing, out of line).
- 0x0099EAE0 (manifest "_Cons_val<...EHeroMorphType...>"): CCharString::~CCharString.
- 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof"): CThing::GetPosition (`mov eax,[ecx+60h] ; add eax,0Ch`), as landed elsewhere.
- 0x007446A0 (manifest "_Cons_val<...>"): CTCPerceiveThing perceived-thing getter.
- 0x0073A8A0 (manifest "CPlayer::HasMode"): called on the creature mode manager component (kind 0x31): CTCCreatureModeManager::HasMode.
- 0x0065BFBC (manifest "CInputProcessGameBase::AddToInterface"): the village component's (kind 0x22) attitude setter.
- 0x008ABB50 (manifest "CVillageTaskCarryCrate::AssignCreatureToTask"): the targeting component's (kind 8) ranged-target setter.
- 0x007338A0 (manifest "CAIStateGroup_MinionReactToNoise::Cleanup"): a reset on the hero's kind-0x30 component during SetCutsceneMode.
- 0x008ED040 (manifest "std_vector_erase_single<CTCSkeletalMorph>"): CTCContainer remove-item (by definition index).
- 0x0076C850 (manifest "CCreatureActionBase::AddDependentThing"): the ally-removal twin of 0x0076D270 on component kind 0x49.
- 0x006C3B70 (manifest "CTCFadeOutAndIn::FadeIn"): EntityFadeOut calls it with `false`; it is a generic Fade(time, bool fadeIn).
- 0x00894DD0 is vtable slot 206 of CGameScriptInterface (EntitySetAsMarryable); slot 400 is 0x00890B90.
