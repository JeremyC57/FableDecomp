# Hand chunk 97 (CTCMeleeTrap .. CTCParticleAttachmentTracker): 38 of 40 byte-exact

All 38 matched sources grade MATCH/RELOCATION_MATCH in `grades.txt` (gall.sh), and every behaviour test prints PASS under
g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). No pragma/flag beyond `#pragma optimize("s", on)` in the three
size-optimised constructor/factory rows (004d49dd, 004d7e46, 004df365). No row needed /EHsc.

## What matched (38)
- **C3DAnimationBlendState** (catalogued CTCMeleeTrap::UpdateTrap) -- 00a3c330 ClearCombinationBlends: MakeOwnCopyOfData then the
  inlined `clear()` (`erase(first, last)` @ 0x00A3D3E0) of the shared data's blend vector; the `ret 8` callee blocks a tail jump.
- **CTCMeshAutomaticEntityCreator** -- 0072e560 HandleMessage: `if (msg->GetType() == 5) CatchMessage(msg);` (argument loaded first).
- **CTCMoneyReceiver** -- 004d49dd ctor: size TU, base ctor, `AmountOfMoneyWanted = GetDefaultAmountOfMoneyWanted()` (a static
  returning 50: nothing in ecx). 00788d68 is really the STL converting constructor
  `pair<CIntelligentPointer<const CThing>,long>(const pair<..., float>&)` (`first(p.first), second(p.second)` via __ftol2), written as a
  member template with an explicit instantiation (leaf `pair`).
- **CTCMultiArrow** -- 007cbf20 SetAICast: shots/volley from the def's per-level tables, the stored member re-read for the divide.
  007cc410 FinalCleanup: the out-parameter GetInterface template on the parent (type 0x13) whose result is NOT tested -- the absent
  path reads the uninitialised out word shared with the lookup key, as retail. 007cc490 vfunc_9: nested ifs with
  `--timer == 0` (`dec [ecx+2Ch] ; jne`), tail jump into CreateAmmoEffect, else RemoveTC(0x88). 007cc7d0 ctor: needed the real
  STLport `_Vector_base`/`_STLP_alloc_proxy` model for AmmoEffects (else `mov ecx,edi` for BowEffect is scheduled before the vector
  stores); BowEffect is `CIntelligentPointer(T* p) { SetPItem(p); }` built from 0; Init() in the body.
- **CTCNoise** -- 00749860 vfunc_9: timer countdown, Kill(true) at zero (re-read at the join).
- **CTCNymph** -- 0080f130 GetRandomFlightHeight: one inlined world-random step + `% 1000` as unsigned, and the step
  `(max - min) * 0.001f` must be a NAMED float local (inline, VC7.1 multiplies by r first, the constant last). 0080f210
  CheckIfCanAttack: the fps global read into a named local before the frame call (as an operand it folds into `imul eax,[g]`).
  0080fd30 GetShouldSummon: out-parameter GetInterface (type 0xfa) inside the ReadyToSummon test, `return count > 0;`.
- **CTCObjectAugmentations** -- 00765d50 GetFreeSlot: unsigned index loop against a re-read size(), strength-reduced element pointer.
  00766480 GetOnHitExplodingEffect: zero-initialised result local (edi) in an if/else-if chain over HasCombination(0xc) /
  HasType(4) / HasType(8). 007681d0 RemoveAugmentationFromSlot: CDefPointer `operator=(0)` = release through a local copy plus a
  second unconditional null store, then the two refresh calls.
- **CTCObstacle** -- 004df365 vfunc_6: size TU; `new CObstacleNavigatorExternalInterface(ParentThing)` whose three zero words come
  AFTER its vtable/thing stores -> an STLport vector member (plain initialisers are scheduled before the vtable store); then
  `new CSteeringAgent(PInterface, true)`. 00cb1e50 is really DirectShow `CMediaType::Set(const CMediaType&)`: `&rt != this`,
  FreeMediaType inlined (CoTaskMemFree, pUnk->Release), `CopyMediaType` (WINAPI) failure -> E_OUTOFMEMORY.
- **CTCOnHeroPush** -- 007bba50 IsAngry: `if (DistPushed > g) return 1; return 0;` (long). 007bba70 vfunc_9: a function-local static
  `g_GameFramesPerSecond * 5`, `++FrameSincePushed > limit` -> RemoveTC(0xc4). 00c384e0 (class unknown, label propagated): an inline
  long `m_20 == m_24` negated into a bool (`test eax,eax ; sete cl ; mov al,cl` in both arms).
- **CTCOpinionSource** -- 00771290 FrameUpdate: static GetWorldFrame, unsigned `>=`, clear the deed. 00771360 ctor: the 12-byte DefStack
  vector first, so every store follows the vtable in declaration order; VirtualDeedID = -1. 00771620 PushNewSourceDef: the world's
  DefinitionManager in a named local (`mov eax,[eax+10h] ; mov ecx,eax`), `if (index > 0 && Push(index)) return 1; return 0;`.
  008640c0 / 008640f0: `if (flag && AppliesToWitness(w)) return 1; return 0;` on +0x19 / +0x1a. 0075c180 is really the
  CTCCameraPointDefinitionBase ctor (pack(1) layout, identity CRightHandedSet, FOV = 72.0f as a BODY assignment -- stored last).
  00baa250 is really the CEngineInternalPrimitiveMorphInfo ctor: two CVectorMaps, an STLport set<CCharString> (header node malloc'd,
  `_M_empty_initialize` re-reading the header member), a CArray -- all vectors as `_Vector_base` + `_STLP_alloc_proxy` (else the
  `add esp,4` of the malloc moves).
- **CTCOverheadDisplay** -- 007b7de0 vfunc_11: the inlined clear of PrimitiveHandles (+0x1c), argument unused.
- **CTCOwnedEntity** -- 004d7e46 ctor: size TU, POwner (pack(1) at +0x0d) = base ctor + vtable with no SetPItem, OwnerUID = 0.
  007e8370 OnForceRecreateUID: IS_THING_ALIVE (two fetches) + third fetch, `OwnerUID = owner->UID`. 007e8420 / 007e8440
  vfunc_23 / vfunc_19: one GetPItem into a local, `if (p && !dead) return true; return false;`. 007e8820 InitialActivate:
  `CTCBase::InitialActivate();` then `if (OwnerUID != 0) SetPOwner(GetGame()->GetThingFromUID(OwnerUID));`. 007e1ac0 is really the
  CTCInfoDisplay ctor (vtable 0x01270044; two CCharString default ctors between the byte and float zero stores).
- **CTCParamsPhysicsBase** -- 006a06e0 ctor: Position/Rhs copied word by word = C3DVector with a MEMBERWISE user copy constructor
  (the implicit copy goes through `lea eax,[esi+4]`).
- **CTCParticleAttacher** -- 004a93c0 is really `CBasicString<char>::operator==` (PDB `??8?$CBasicString@D@@QBE_NABV0@@Z`):
  length test then `Compare(PData, other.PData) == 0`. 007f8b60 vfunc_27: on message type 5 `for_each(first, last, CKillParticle())`
  (SGI /Gr fastcall, hidden functor result) -- the functor is an EMPTY class with a user ctor homed in the dead message slot, which is
  why retail "reloads" the message to push it; then clear() and AttachParticles(). 00c2a5c0 (class unknown, label propagated):
  owner read first, `entry->~T(); operator delete(entry);`, then the same for the owner when its embedded list's slot-15 call is 0.

## Parked for Fable 5.1 (2)
- 00749810 CTCNoise::CTCNoise DIFFER(74v79): retail `mov cl,[edi+24h] ; xor eax,eax ; and cl,0F8h ; mov [edi+24h],cl` vs lane `and byte ptr [edi+24h],0F8h ; xor eax,eax` (the inlined CDefClassBase bit-field clear; everything else identical) | tried: unsigned char / bool / unsigned long bit-fields, three separate and one chained assignment | next: lane gap: split read-modify-write of a byte bit-field (retail scheduling of the RMW around `xor eax,eax`); maybe a CDefClassBase ctor that assigns the flags through a byte local
- 0071a850 CTCParticleAttachmentTracker::vfunc_16 DIFFER(78v78): retail node in edi / value address in esi (push edi, ebx, esi), lane node in esi / value in edi | tried: reference local, pointer locals in both declaration orders, direct member path, VC6 list iterator object | next: lane gap: callee-saved register colouring in a list walk

## Skipped
- none (no row spans two functions).

## Label corrections
- 00a3c330 "CTCMeleeTrap::UpdateTrap" -> C3DAnimationBlendState::ClearCombinationBlends (between AddCombinationBlend 0x00A3C310 and
  SetSkeletalMorph 0x00A3C350; calls MakeOwnCopyOfData).
- 00788d68 "CTCMoneyReceiver::CTCMoneyReceiver" -> pair<CIntelligentPointer<const CThing>,long> converting constructor (CTCMeleeTrap
  region caller 0x00788783).
- 0075c180 "CTCOpinionSource::CTCOpinionSource" -> CTCCameraPointDefinitionBase::CTCCameraPointDefinitionBase (vtable 0x01269B04).
- 00baa250 "CTCOpinionSource::CTCOpinionSource" -> CEngineInternalPrimitiveMorphInfo::CEngineInternalPrimitiveMorphInfo (Morph member
  at +0xac of CEngineInternalPrimitiveMorphedStaticMesh).
- 007e1ac0 "CTCOwnedEntity::CTCOwnedEntity" -> CTCInfoDisplay::CTCInfoDisplay (vtable 0x01270044).
- 004a93c0 "CTCParticleAttacher::OnInitialActivate" -> CBasicString<char>::operator==(const CBasicString<char>&) const.
- 00cb1e50 "CTCObstacle::OnKill" -> CMediaType::Set(const CMediaType&) (DirectShow base classes).
- 008640f0 "CTCOpinionSource::PeekWitnessWillNotAttack" (twin label) -> PeekWitnessWillOverrideFactionEnemy (tests +0x1a).
- 00c384e0 "CTCOnHeroPush::IsAngry" and 00c2a5c0 "CTCParticleAttacher::HandleMessage": propagated; the classes are not those
  (offsets past CTCOnHeroPush's 0x20 bytes; `this` unused); kept the catalogue names with local descriptive models.
- Callees: 0x00686AE0 = CTCBase::GetGame() (returns the global @ 0x013B8A1C; "GFGetPreMainMemoryUsed"), 0x0051ED80 = the game's
  GetThingFromUID (map at +0x58; "CStopWatch::GetTicks"), 0x004C9840 = CThing::RemoveTC(type) ("CBaseObject::OnPropertyChanged"),
  0x007F8760 = for_each(..., CKillParticle) ("_Destroy_range"), 0x0044C0C0 = CDefClassBase::CDefClassBase
  ("CShoveHaPennyRegionDef"), 0x007898F0 = CTCMoneyReceiver's default amount (returns 50; "AddChildPrimitive"), 0x00CB1CC0 =
  FreeMediaType(AM_MEDIA_TYPE&), 0x00CB1BD0 = CopyMediaType.
