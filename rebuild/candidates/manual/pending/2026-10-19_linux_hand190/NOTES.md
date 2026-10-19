# Hand chunk 190 (CCreatureAction_ScorpionCharge .. CDW5_Balv): 27 of 40 byte-exact

All 27 sources grade RELOCATION_MATCH (gall.sh) and their tests print PASS under g++ (tall.sh) and the lane cl.exe (tall.py).
Engine headers for these action classes carry PDB offsets (PDB CCreatureActionBase 0xb4 / CCombatActionBase 0x12c; retail
0xa8 / 0x114), so every `this` is modelled locally with the retail offsets and the PDB member names (said in each header comment).

## What matched (27)
- CCreatureAction_ScorpionCharge ctor 0x008C4EA0: ScorpionChargeStrike family ctor plus two body assignments (float as an
  integer move, `StartPos = owner->GetPosition()` through a C3DVector user copy ctor = `lea ecx` copy).
- 0x00783F00 (label "ScorpionChargeInto::InsertIntoTree"): STLport `_M_insert` of a map keyed by unsigned __int64 with the node
  creation out of line (0x00783DD0); the landed CCombatantMap::Insert model with `_M_create_node` declared, not inlined.
- ScorpionPrepareTailPlunge / ScorpionTailPlunge FinishAction 0x008C4990 / 0x008C48A0: unchecked mode-manager GetInterface in an
  inline accessor, `new CMode..(owner)` into a per-type CCountedPointer whose ctor (0x008C5980 / 0x008C58F0) lives in the SAME TU:
  defining its body above the caller under `#pragma auto_inline(off)` keeps the block pointer in esi across AddMode (otherwise
  every field is re-read from the frame); inline converting copy into the argument area + inline release with operator delete.
- SendMessage 0x00982210, ShopkeeperGetOut 0x00842C70: CCreatureActionBase ctors with the implicit-conversion literal for the
  `__fastcall` animation lookup (built before all other arguments); init-list members after the vtable; 12/24-byte records copied
  dword by dword through memberwise user copy ctors.
- SheatheItemToInventory ctors 0x0069EF80 / 0x0069FFD0: `CAnimationEntry(GetAnimationFromName(owner, name), t)` temporary passed by
  const ref to the base ctor (its inline dtor = ReleaseTemporary on the ref-count base at +4); `CCountedPointer<CCreatureActionBase>`
  from a raw pointer (`if (p) Count = new Block(Object); else Count = 0;` = three duplicated exits). The second overload gets its
  name from the `__fastcall` GetAnimationToUse (hidden CCharString in the dead arg slot).
- ShowTrophy 0x00845850, Talk 0x00846660, SummonerFlameSlice 0x008CAFA0, LightningOrbOutOfGroundSmash
  0x008CB500, LightningOrbInto 0x008CB1A0, SummonerSummonInto 0x008CB060, SummonerUnsheatheStrike 0x008CB620: GetInterface
  variants -- checked (out-parameter template in the `if`/`&&`), unchecked (inline accessor returning the never-written out
  pointer from the key word), cached (`bits & mask ? *out = m_p5c`); BriarRose AddMode shape with `operator CThing*()` for the
  owner argument; landed ScorpionTailPlunge explosion shape.
- Tentacle rows 0x008CF610 / 0x008CF700 / 0x008CF460 / 0x008CF540: next action as a 0x114/0x120-byte temporary built inside the
  `SetCurrentAction(...)` call (inline ctor: base, vtable, zero-register init list); `StartHealth > health` (`test ah,41h ; je`);
  a thing fetched once for its null+dead test is `IsThingAlive(p.GetPItem())` (two separate GetPItem calls = the plain `&&`).
- TrollProjectileAttack 0x008C6430: the CDefPointer passed to CreateShot shares the def-name string's frame word only when it is
  declared in a SIBLING block of the string's block; the definition manager read into a local after the string is built.
- TrollThrowProjectile::FinishAction 0x008C6A50: two checked lookups in the body share one key word.
- WaspQueenDeath 0x008C52C0: cached particle-emitter accessor + the landed WaspQueenDie restart temporary with LoopNum 0.
- CCreatureGeneratorDef::Transfer 0x0074C5F0: CActivateQuestDef template; the vector transfer is `switch (mode) { case 2: In;
  default: Out; }` (an if/else puts the load arm first).
- CDBSummoner::Main 0x00D25D80 / 0x00D25F50 and CDW5_Balv::Main 0x00E047D0 / 0x00E04970: CSummonerToKill entity-script model,
  `Game->GiveThingBestEnemyTarget(Me, Game->GetHero())` nested (outer vtable fetched first), CScriptThing slot 16 IsAwareOfHero;
  each pair is a byte-identical copy (no ICF), same source.

## Parked for Fable 5.1 (13)
- 008c4170 CCreatureAction_ScorpionKingDeath::Initialise DIFFER(218v218): `sub esp,0Ch` v `sub esp,8` -- retail shares one word between the first name string, the GetSubDef out slot and the DEATH string | tried: holder after out, out in a block + found local, `__forceinline` GetSpecialEffectsDef helper, implicit SetAnimationName literal | next: the 0x008C6430 lesson -- put out/holder/rest of the body in a SIBLING block of the first string's block
- 008b8bc0 CCreatureAction_ScreamerDrainAttack::Initialise DIFFER(231v231): `push ebp ; mov ebp,ecx` v `push ebx ; mov ebx,ecx` (this in ebp, target in ebx swapped) | tried: inline predicate, `__forceinline` predicate, 12-byte delta with z unused (fixed the frame), static free predicate | next: lane gap: register colouring
- 00845270 CCreatureAction_ShowItemToThing::PerformAction DIFFER(219v212): retail `mov cl,[eax+91h] ; mov bl,1 ; test bl,cl` + `test [eax+91h],bl`, one push-ecx word, shrink-wrapped ebx/ebp; ours `sub esp,8`, spills this, immediate tests | tried: plain `&&`, IsAlive helper, 1-bit field, optimize("s") | next: owner argument through `operator CThing*()` (evaluated before the item, as 0x008CB1A0) -- not tried
- 008b7a20 CCreatureAction_SummonCreatures::Initialise DIFFER(236v236): this in edi / summon component in esi swapped | tried: plain `T* p = 0` + GetInterface, accessor, `self` local | next: lane gap: register colouring
- 00845b10 CCreatureAction_TavernGameMasterInteraction::Initialise DIFFER(165v165): retail `mov edi,ecx ; lea esi,[edi+8]` / `lea esi,[edi+0A8h]`, ours `mov esi,ecx ; lea edi,[esi+8]` / `add esi,0A8h` | tried: plain `&&`, IsAlive(const&) helper, nested ifs + reference local | next: lane gap: register colouring
- 008b9f20 CCreatureAction_TeleportIn::Initialise DIFFER(205v205): retail `add esi,8` (this becomes &PCreature) with the sub-def holder in edi, ours `lea edi,[esi+8]` with the holder in esi | tried: def through the out-slot chain, def local before/after the out pointer, implicit-conversion literal (fixed the conditional-temporary order), owner reference local | next: lane gap: register colouring
- 008b9e50 CCreatureAction_TeleportOut::Initialise DIFFER(205v205): same esi/edi swap as 0x008B9F20 | tried: the 0x008B9F20 final form | next: as 0x008B9F20
- 008c67a0 CCreatureAction_TrollThrowProjectile::FrameUpdate DIFFER(190v190): retail calls the third PShot.GetPItem() before `xor ecx,ecx ; mov cl,[edi+12Ch] ; push ecx`, ours pushes the alpha first | tried: plain chain, shot local, `operator->`, inline CThing::SetAlpha (the last three fix the order but swap esi/edi) | next: lane gap: scheduling vs colouring
- 008bf1c0 CCreatureAction_UnblockableMidAttack::Initialise DIFFER(178v178): &PTarget in ebx / owner+physics in edi swapped (instruction order identical) | tried: `&&` chain, inline CThing::GetAngleXYTo(CThing*), (const CBaseIntelligentPointer&) form, nested if with owner local + `__forceinline` | next: lane gap: register colouring
- 008ec320 CCreatureAction_UndeadReturnToEarth::Initialise DIFFER(205v206): this in ebp / sub-def holder in edi (retail the reverse; shifts one push) | tried: TeleportIn form, owner local for SetSuspendAnimation (fixed the order), raw pointer holder, bool declared first | next: lane gap: register colouring
- 008ec260 CCreatureAction_UndeadRiseUp::OnAnimationLoaded DIFFER(187v188): esi/ebp swap between &PCreature and the holder, twin of 0x008EC320 | tried: the 0x008EC320 form | next: as 0x008EC320
- 006a02b0 CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction DIFFER(240v240): `sub esp,14h` v `sub esp,10h` -- retail keeps the key/"CTCHaste" word apart from the {key,float} value and the insert result | tried: HasTC+GetTC (wrong), out-param lookup (fixed the code), named value local, implicit literal temp | next: an inline GetOrAddTC helper owning the string, or a 12-byte insert result
- 008b7ed0 CCreatureAction_UnsheatheWeapons::Initialise DIFFER(174v174): retail stores `sheathe = false` before the GetPItem call and uses ecx/edx for the flag/key addresses (ours eax/ecx), SetAnimationName's `mov ecx,edi` after the push | tried: direct, `__forceinline` GetAnimationName helper | next: lane gap: scheduling

## Skipped
- none (no row spans two functions).

## Label corrections
- 0x00783F00 "CCreatureAction_ScorpionChargeInto::InsertIntoTree" is STLport `_Rb_tree<unsigned __int64, pair<const unsigned __int64,
  CIntelligentPointer<CThing>>>::_M_insert`; 0x00783DD0 "Clone" is its `_M_create_node`.
- 0x008C5980 / 0x008C58F0 are CCountedPointer<CModeScorpionKingVulnerable / CModeScorpionKingTailPlunge>(T*) (manifest
  "CCountedPointer<CFileInstaller::CFileInfo>"); 0x008D73E0 "CVillageTaskCarryCrate::AssignCreatureToTask" is
  CModeScorpionKingVulnerable::SetTarget.
- 0x0084AEE0 "CEventDied" is NEntityEvents::CEventShownItem's ctor (vtable 0x012723D8); 0x0084AF90 "CEventFiredShot" is
  NEntityEvents::CEventTalkedTo's ctor (vtable 0x01274228).
- 0x006DD740 "$E2" is a CTCTrophy member taking the showing creature; 0x007355F0 is a CTCCombat def setter; 0x00800020 is a CTCHaste
  refresh; 0x00692910 / 0x00692940 are CCreatureActionBase::GetPDefinitionManager / GetPWorld (STL labels).
- 0x00466A75 / 0x00466AA7 serialise vector<long> (labels say <float>/<CRGBColour>); 0x0040FE60 is CPersistTraits<long>::TransferBinaryIn.
- 0x0062E900 "~CTCLightningOrb" is ~CCombatActionBase; 0x0069FC40's manifest parameter type (CCreatureAction_SheatheItemToInventory*) is
  wrong (it is CCreatureAction_UnsheatheItemFromInventory::FinishAction).
