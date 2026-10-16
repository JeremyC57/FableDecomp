# Hand chunk 170 (CTCSmashableBase .. CTCTavernGameCardPairs): 28 of 40 byte-exact

## What matched (28)
- CTCSolidToThing 0x00770E80 FrameUpdate: `erase(remove(begin, end, CIntelligentPointer<const CThing>(0)), end())` with STLport's inline remove (find + tag, remove_copy); world and UID as named locals in retail order.
- CTCSoundPlayer 0x0066B300 PlayAttachedSample: inline generator `seed = GFROR13(seed*0x24A1+0x24DF)`, `n ? r % n : 0`, generator reference fetched first, index a named local.
- CTCSpecialAbilities 0x0072B0D0 SetupPhysicalShieldSpell (CCharString named local in its own block shares the key slot); 0x0072C3C0 vfunc_16 (follower intelligent pointer as a reference local, GetPItem per use).
- CTCSpotLight 0x00731CA0 GetDirection (user C3DVector copy ctor gives `fld X ; fstp` copy-out); 0x00731D40 ctor (body stores in retail order, CDefPointer operator= releasing through a pointer local); 0x00731E20 OnCreate.
- CTCStealth 0x00747110 vfunc_16 (null-safe test copy + non-null-safe dereferenced copy of the region def); 0x00747790 IsAvailableToPerformStealthAttack (GetInterface twice shares the key slot); 0x00747820 ctor (init list).
- CTCStockItem 0x007F4FC0 GetShopTC (const GetInterface for the link, `__forceinline` GetTC for the shop, preloaded `xor edi,edi`).
- CTCSummonSpell 0x0079A820 CancelMove (pack(1) Minion at +0x11, inline IsThingAlive); 0x0079B650 SetSummonCreatureGlobalDefNum.
- CTCOracleMinigame 0x007E5F00 ctor (catalog CTCSummonableCreature).
- CTCTalk 0x006E2DC0 IsSpeechSampleLoadedAndPlaying (`!(elapsed > limit)` gives `test ah,5 ; jnp`; `__forceinline` GetTC).
- CTCOverheadDisplay 0x007B80B0 dtor (catalog ~CTCTargetingBase; novtable over ~CTCBase, member dtors in reverse order).
- CTCTargetingBase 0x007F58E0 ctor (pack(1) intelligent pointers at odd offsets); 0x007F5950 GetTargetedPos (C3DVector made non-POD so the __fastcall NTargeting::GetTargetingPositionOnThing gets its hidden pointer in ecx); 0x007F5A70 IsTargetedThingOfType.
- CTCTargetingSpirit 0x007F9B90 FrameUpdate (UID named local before the frame call).
- CTCTavernGame 0x005EB510 PlayerHasBeenHit (as the landed CheckForHits); 0x005EC5C0 ClearBox (null-safe GetRefCount()==1 keeps the immediate; inline STLport erase with __copy + tag + (int*)0).
- NUISystem::CKeyRedefiner 0x00558540 ctor (catalog CTCTavernGameCardBase; three bases, input definition with its own zeroing ctor).
- CTCTavernGameCardBase 0x008F1CD0 ctor (vectors + card pointers in the init list, PointerScale/Pointer body stores).
- CTCTavernGameCardBlackjack 0x007DA120 vfunc_53 (`*(begin()+n)`); 0x007DA810 ReorderCards (`++index, ++hand` header order).
- CTCTavernGameCardPairs 0x007D60C0 vfunc_29 (EnterGame override, C3DVector(0,0,h) temporary); 0x007D6260 vfunc_53 (def object local, CWideString built in the argument area).

## Parked for Fable 5.1 (10)
- 0072b330 CTCSpecialAbilities::SetupSummonSpell DIFFER(116v118): component `mov ecx,[eax+4]` where retail has `mov eax,[eax+4] ; mov ecx,eax` before ResetMinionPos | tried: GetTC()->F(), named local, HasTC+GetTC GetInterface, single-body GetInterface | next: lane gap: looked-up component register
- 0072c930 CTCSpecialAbilities::SetupFireballSpell DIFFER(143v143): HasTC test through ecx and AddTC's this loaded before `lea edx`, retail eax and `lea ecx ; push ecx ; mov ecx,[esi+18h]` | tried: nested ifs, early return false, direct if, CThing base class | next: lane gap: register choice
- 00747480 CTCStealth::UpdateViewability DIFFER(128v119): def release re-reads the escaped local after `dec [ecx+4]`, retail branches on the dec flags | tried: hand release (lea form), null-safe dtor (best), member Release(), pointer-local dtor | next: lane gap: CDefPointer release re-read
- 00787620 CTCSwitchableNavigation::WouldBlockPos DIFFER(107v107): UID push registers (retail thing edx, vtable esi; ours thing ecx, vtable edx) | tried: __int64, two-dword struct, thing local | next: lane gap: register allocation
- 006e2e60 CTCTalk::StopCurrentSpeech DIFFER(109v109): `mov edx,[esi+28h] ; mov ecx,[eax+4] ; push edx` vs ours `mov ecx,[esi+28h] ; push ecx ; mov ecx,[eax+4]` | tried: inline overload, emitter local, body written out | next: lane gap: scheduling
- 007ea490 CTCTargeted::GetTargetPoint DIFFER(116v116): fallback pushes `mov ecx,[radius] ; mov eax,[esi+4]` with eax/ecx swapped in ours | tried: plain call, appearance local (fixed the first arm), non-const thing, NTargeting reference signature | next: lane gap: register swap
- 008031e0 CTCTargetingPlayer::vfunc_28 DIFFER(150v152): target esi / cursor edi and `mov ebx,[ebx+4] ; mov edx,esi ; mov ecx,ebx` vs ours edi/esi and `mov ecx,[ebx+4]` | tried: early returns (branchless `?:`), merged else arm (best), reference AreThingsEnemies | next: lane gap: register allocation
- 007f98e0 CTCTargetingSpirit::OnInitialActivate DIFFER(114v114): retail 8-byte frame reuses the def slot for the find() iterator (zero store after the push), ours 12 bytes | tried: def in a block, __forceinline def getter, __forceinline find helper | next: lane gap: frame slot reuse
- 007f50b0 CTCSoundAtmosphereVillage::vfunc_6 DIFFER(160v156): retail one frame dword shared by key, unset village and the by-value def, ours gives the def a second slot | tried: plain accessor, __forceinline accessor + GetTC, inline AddLivingSpaces helper, out-pointer village | next: lane gap: frame slot reuse
- 005ea100 CTCTavernGame::vfunc_42 DIFFER(184v158): retail null-tests the def release only on the unfinished arm and keeps def.Object in edi on the finished arm (no test), zero store after the push | tried: null-safe dtor (best), pointer-local dtor + def local | next: two separately scoped def holders, or a GetDef accessor returning a reference

## Skipped
- 007eec50 CTCSmashableBase::OnDie: two functions in the row (OnDie ends at +0x34, another body at +0x40).
- 007da360 CTCTavernGameCardBlackjack::FrameUpdate: two functions in the row (FrameUpdate ends at +0x31, another body at +0x40).

## Label corrections
- 0x007E5F00 "CTCSummonableCreature::CTCSummonableCreature" is CTCOracleMinigame::CTCOracleMinigame (vtable 0x012707B4).
- 0x007B80B0 "~CTCTargetingBase" is CTCOverheadDisplay::~CTCOverheadDisplay (caller 0x007B8090 = slot 0 of 0x0126DA94).
- 0x00558540 "CTCTavernGameCardBase::CTCTavernGameCardBase" is NUISystem::CKeyRedefiner::CKeyRedefiner (vtables 0x0124ADBC/0x0124AD98/0x0124AD90).
- 0x007F59C0 "CTCTargetingAI::OnInitialActivate" is called as CTCTargetingBase::OnInitialActivate by CTCTargetingSpirit (fills +0x30 PTargetingDef).
- 0x007F53A0 "CTCInventoryAbilities::AddLearnedExpression" is CTCSoundAtmosphere::OnInitialActivate (slot 6 of 0x01271DFC).
- 0x00A14440 "C3DVector::GetScaled" is C3DVector::Normalise() (in place, void); 0x007EA2F0 "Creature::GetTargetFirePoint" is NTargeting::GetTargetingPositionOnThing.
- 0x0079DCB0 "C3DMeshInfo::HasPhysicsMesh" is called on CTCPhysicalShieldSpell (`[+0x1c] != 0`); 0x0085EE40 "CThingFilter_IsUnconscious::operator()" is called as a CThing member.
- 0x008BCD10 "CCreatureAction_CastBattleCharge" ctor stores CCreatureAction_CastWillFireballSpell's vtable 0x0127DF4C; 0x0062E900 "~CTCLightningOrb" is that action's dtor.
- 0x007379D0 / 0x00737AD0 / 0x00738D70 are STLport find / remove_copy / vector::erase for CIntelligentPointer<const CThing>; 0x00440260 "_Move" is STLport __copy(first,last,result,tag,int*).
- 0x0066BD00 "CSoundInfo::PlaySound" and 0x0066B080 "Sound_PlayAttached" are CTCSoundPlayer members; 0x005E9E70/0x005EA950 "CTCBetting::GetText/PlaySoundW" are CTCTavernGame members.
- 0x005EA000 "_Cons_val<...>" is CTCTavernGame::GetTavernGameDef; 0x0082D2D0 "CThingBuilding::PeekPMaterial" returns the building def used for living spaces (+0x44).
- Retail offsets: CThing::PhysicsTC +0x60 (PDB +0x70), CTCSpecialAbilities::Creature +0x18 (PDB +0x20), CTCTavernGame 0xF0 / CTCTavernGameCardBase 0x318 in retail, CRegionDef::MaxNumAttackersOnHero +0x98 (PDB +0xA4).
