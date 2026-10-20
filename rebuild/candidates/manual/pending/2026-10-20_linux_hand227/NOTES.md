# Hand chunk 227 (CPlayer .. CRandomAppearanceMorph): 31 of 40 byte-exact

## What matched (31)
- CPlayer (4 of 4 authored, 3 matched): UninitModeLightning 006366b0 (engine/CPlayer.h carrier; `T* p = 0; if (HasTC) p = GetTC;`
  for the `xor ecx,ecx` preload -- the out-parameter GetInterface form keeps the manager in edi); InitModeViewHero 00636e90
  (by-value CCountedPointer built in the argument area, CDefPointer released through a pointee DecRef/GetRefCount Release,
  `__forceinline` mode ctor inside the new-expression); InitCharacterAs 0048a070 (`long number = Number;` read before the
  GetMainPlayerID call gives the short ebx live range with a sunk push).
- CPlayerGui: GetButtonAlpha 00439ab0 (the creature fall-back leaves through `goto done` to the shared `return alpha`, so the
  float is a merged value spilled at entry and re-loaded; a direct `return alpha` is re-folded to `fld 1.0`).
- CPlayerManager: LoadGameState 00449e60 (GetPlayer's body as a `__forceinline` helper with the count cached before the loop,
  used at the two main-player sites; out-of-line GetPlayer in the first loop); DrawGetEnvironmentInfo 0044a2a0 (uninitialised
  TC local, wide-string temporaries in the dead argument slots).
- CPreallocTexturePool::Initialise 00a6a540: `__forceinline` ctors inside both new-expressions, the intrusive slot
  operator=(T*) releasing through the pointee's inline Release() (`dec [ecx+4] ; jne`).
- CProximityPodium::Main efb360, CPrisonKeyDoor::Main dd9180: entity scripts (landed CHTE_Prisoner model), rotated `while`
  wait loops; the predicate is a scoped frame object with an out-of-line dtor.
- CQR_EscortTraderScript: ctor e1db00 (landed CQ_ArenaScript ctor model: novtable script-object base, self-registering
  CTimer, STLport vectors, two vector[3] arrays); EndMission e1e670 (per-arm IsActiveThreadTerminating returns, L"" implicit
  CWideString temporary).
- CQ_AmbushTradersScript: SetBanditAllegiences f060d0 (the convoy vector used as the member, not through a reference local --
  the reference flips the first `[edi+edx]` operand order); AddBanditHealthBar f07a80 (CRGBColour temporary by const ref,
  three terminating exits cross-jumped onto one destructor call). Row e3a030 is really CV_BordelloScript::WatchForBordelloStatus
  (`if (!loaded) { if (term) return; while (!loaded) { yield; if (term) return; } if (term) return; ... }` in the outer loop).
- CQ_ArenaScript::InitialiseVariables f25840: vector operator= call + twenty CCharString literal assignments.
- CQ_BountyHuntScript::WatchForAllGruntsKilled d17340: local STLport vector with a `__forceinline` destructor expanded at all
  four returns (free tail shared).
- CQ_HangingTreeGoodScript: ctor d6ef20 (landed CV_BanditCampPathScript model); UpdateWayPoint d74e80 (CScriptThing::operator=
  forwarding to the counted pointer's operator=: the forwarded `&other.PImp` leaves retail's dead `add eax,4`);
  SetGuardsHealth d766c0 (reference local for the float vector, unsigned size() loop, clear() as erase(begin, end)).
- CQ_MinionCampScript::AddNewHeroAttacker da0b30: retail expanded CScriptThing::operator= only at the first-slot site; written
  through a `__forceinline Assign()` that forwards to the counted pointer's `__forceinline` operator=, the other site calls
  0x008AB980.
- CQ_NewOakValeIntroScript::AttackStuff dbe3c0: the objective strings as block-scoped named locals (share the earlier
  temporaries' frame word).
- CQ_OrchardFarmRaidScript::MakeTeamMemberComment dcda80: one `code + data + "_" + line` chain of `__fastcall` operator+;
  the last temporary lands in the dead `speaker` argument slot.
- CQ_SecretPassageScript: EndMission df0a50 (CRGBColour built in its argument slot by a ctor-body R,G,B,A store order; named
  CScriptThing marker with inline dtor); Init dc25b0 (three AddQuestRegion literal pairs; class doubtful, see below).
- CQ_SummoningTheShipScript::MakeBriarRoseComment df20f0: the conversation index as a named local (edi), vtable cached in ebx.
- CQ_WizardBattleScript::CleanUp e1d690: `__forceinline` vector dtor + named CScriptThing village (sunk `push ebx` reproduced).
- CQuadTreeElement::LoadHeader bde290 (the landed sibling CObjectCacheGroupCollection::LoadHeader model).
- CQuestManager: Update 4b4490 (inline STLport list clear); SetQuestCardObjective 4b0d30 (nested ifs + one trailing
  `return false`); IsActiveQuestBetweenQuestStartAndFinishScreensInRegion 4b1340 (nested ifs so the by-value region
  temporary is unconditional, and an inline CCharString== whose length+Compare tail is a nested inline IsEqual).
- CRandomAppearanceMorph::TransferBinaryOut 86b6d0: fixed three-part loops plus size() loops, `__fastcall` map-dump helper.

## Parked for Fable 5.1 (8)
- 00edd060 CV_TempleOfLightScript::WatchForAcolyteExit (catalog CQ_BountyHuntScript::WatchForAllGruntsKilled) DIFFER(230v270): retail inlines the local vector<CScriptThing> dtor at the wait-loop return (others merge onto out-of-line 0x008AC970), the lane calls the out-of-line copy everywhere | tried: plain inline dtor, forceinline _Destroy, STLport _Vector_base/vector split dtor, dead `_M_start = 0` store | next: lane gap: inline-once of a local vector destructor (retry10c open shape; forceinline expands at all sites)
- 00cfcca0 CPretendGuard::Main DIFFER(280v274): retail enters the final `while (!IsDone()) NewScriptFrame();` with `jmp` to its bottom test; the lane duplicates the IsDone call+test after the StartScriptingEntity loop | tried: while, for(;;)+break, loop nested in the start loop, goto into a do/while | next: lane gap: loop-entry tail duplication
- 004b1a80 CQuestManager::SetBoastAsMade DIFFER(277v277): this/list-head registers swapped (retail ebx=this, edi=head) and the zero C3DVector stores sit before the AddMessage pushes (retail: after `push 0x32`) | tried: hand push_back with n/p locals, STLport insert(end()) iterator form (fixes the registers, breaks the link order), prev-first links, named position local | next: insert(end()) form with `_Node_base* p` read after _M_create_node, and the position from an inline `C3DVector Zero()` returning a named local
- 004b2b70 CQuestManager::GetAllActiveQuestInfo DIFFER(290v290): frame packing only -- retail keeps an 8-byte frame (find key in the dead second-argument slot, def-name string and __false_type tag in the first); the lane keeps 0x10 | tried: named local, block-scoped local, temporary argument, GetQuestCard helper | next: lane gap: dead-argument-slot packing
- 004887c0 CPlayer::SetupInitialPlayerMode DIFFER(246v251): retail duplicates `xor ebx,ebx` into both front-end arms (zero for AddMode(0x1b,false)), stores the vector zeros after the pushes, keeps the vector start in edi and the spirit in the key frame word | tried: compare operand order, `long number` local, hoisted spirit local, out-parameter GetInterface | next: `bool frontEnd = GetCurrentMode() == 0x1b;` form and the STLport _Vector_base/_STLP_alloc_proxy vector model
- 00439e00 CPlayerGui::UpdateRenown DIFFER(276v278): one `mov eax,edi` register copy before the fabs(OldValue - CurrentValue) loads | tried: member access, inline GetValueChange, PRenown re-read, named bar local | next: lane gap: register copy (CSE'd reload)
- 00878be0 NCameraModes::CProjectileWeapon::GetPosFromParams (catalog CProjectileWeapon) DIFFER(222v244): x87 sum order, unfolded `* 0.5f * 0.0f + 1.0f`, split position copy (X/Y on the FPU, Z as an integer) | tried: one probe | next: an inline Lerp(1.0f, 1.0f, t) for the scale, a named rotated-position local, the retry-10 vector-operand forms
- 00bdda80 CLocalDetailCacheMap::CQuadTreeElement::RenderUpdate DIFFER(277v269): retail keeps the distance on the x87 stack after fsqrt (`fstp st(2) ; fstp st(0) ; fst [slot]`) where the lane stores and re-loads it, plus a `lea esp,[esp]` pad before the child loop | tried: statement-order sum, Length() helper, inline fade condition, pointer child loop | next: lane gap: fst vs fstp/fld spill (chunk 214 shape) and loop-head alignment

## Skipped
- 00d80ae0 CQ_HeroSoulsArenaScript::GetJackSpeech: the row holds GetJackSpeech (0x00D80AE0..0x00D80B7C), its 8-entry jump table and the script factory at 0x00D80BA0.

## Label corrections
- 0x00DC25B0 is not CQ_SecretPassageScript::Init: it sits in vtable 0x012DA11C (no RTTI, slot 3) of the class built by the
  ctor 0x00DC2570 (four CCharStrings at +0x58; slot-4 transfer names QuestStartScreenShown +0x71 / HaveOpenedDemonDoor +0x72),
  the graveyard part of Q_OpeningGraveyardSecretPassage; true class name unknown (catalog module kept).
- 0x00E3A030 is CV_BordelloScript::WatchForBordelloStatus (thread name 0x012E3918, started at 0x00E39E64), not
  CQ_AmbushTradersScript::WatchForAllTradersDead.
- 0x00EDD060 is CV_TempleOfLightScript::WatchForAcolyteExit (thread name 0x012F3E28, started from 0x00EDB190), not
  CQ_BountyHuntScript::WatchForAllGruntsKilled.
- 0x00878BE0 / 0x00878A80 belong to NCameraModes::CProjectileWeapon (packed camera-mode members), not CProjectileWeapon /
  CThing::GetRotatedPosition.
- 0x00487510 returns CPlayer +0x228 = JoystickDeviceNumber (PDB +0x230 less the retail 4-byte PlayerModes list): rename to
  CPlayer::GetJoystickDeviceNumber (manifest "CWorld::DrawGetEnvironment", landed sources call it "GetPlayerNumber").
- 0x00449700 CPlayerManager::GetMainPlayerID (returns +0x1c), not CWorld::DrawGetEnvironment.
- 0x0066FB20 CTCCoopSpirit::GetScore (+0x10), not CWorld::DrawGetEnvironment; 0x0066FA50 CTCCoopSpirit::SetMaster (+0x24),
  not CObservable::AddExclusiveObserver.
- 0x00487B70 CPlayer::GetPlayerCharacter (`PlayerCharacter.GetPItem()`), not `_Cons_val<...>`.
- 0x004378E0 CPlayerGui forwarder to PDrawSpiritsHealth (+0x160 -> 0x00643EA2), not `_Cons_val<...>`.
- 0x004AF610 CQuestManager::IsQuestActive(const CCharString&) (walks ActiveQuests), not CQuest_Array_FindByName.
- 0x00CB8220 a CScriptBase per-frame update (calls 0xCB7C40 then jumps to 0xCB8170), not `_Dest_val<...>`.
- 0x0099E900 CCharString::operator!=(const CCharString&), not less<CCharString>::operator().
- 0x007021E0 CTCQuestCard::SetTeleporterRegionName (beside Get/SetRegionName), not `_Cons_val<...>`.
- 0x00F25980 vector<CArenaRoundDef>::operator=, not std_vector_Conversation_Assign2.
- 0x00440550 vector<float>::erase(first, last) (memmove form), not Buffer_MoveData.
- 0x00A6C020 vector<CIVCountedPointer<CManagedTextureData>>::resize (byte twin of the vector<unsigned int> label).
- 0x005669C0 CCountedPointer<CTexture>::Reset here (label CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset; byte twin).
- 0x0084F1B0 GetObservedThing (label `_Cons_val<...>`).
