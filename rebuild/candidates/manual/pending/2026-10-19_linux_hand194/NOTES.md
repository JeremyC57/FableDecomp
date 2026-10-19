# Hand chunk 194 (CGameScriptInterface .. CKeyRedefiner): 26 of 40 byte-exact

All 26 graded RELOCATION_MATCH (gall.sh) with tests PASS under g++ and the lane VC7.1 (tall.sh / tall.py); the
relocation-identity check (bin/relocchk.py, a copy of agent191's) reports no HARD findings. 13 rows parked, 1 skipped.

## What matched (26)
- **CJackOfBladesBattleDef::Transfer** 004e39e6 -- persist of the 17 battle tuning members; a size TU (`optimize("s")`),
  TransferFloat 0x431061 / TransferLong 0x431102 per member; the def members sit at PDB - 4.
- **NParticleEngine::CPSCUpdateNormal ctor** 00b0c9c0 (label CHeroStatsDef) -- all initialisers: VC folds the whole
  bit-field dword into one immediate and the six bool bits into one `and al,0C0h`; list sentinel built last.
- **NParticleEngine::CParticleSystem ctor** 00af4e10 (label CHeroMorphDef) -- the CVectorMaps need the STLport
  `_Vector_base` + `_STLP_alloc_proxy` model to place the Name ctor's `lea ecx` after the last map word; the scalars
  after Name are body assignments; Scale is a C3DVector temporary copied through `lea` (user copy ctor, no operator=).
- **CQ_SecretPassageScript ctor** 00dec7f0 (label CHeroMorphDef; factory 0xDF1080, "S_QSP") -- the landed quest-script
  model (novtable argument base, STLport maps, 11-element entity array as a counted loop).
- **CFishCompOrganiser ctor** 00e7ef70 (label CGameScriptThing; "FishCompOrganiser") -- the landed CBriarRose entity-script
  model plus an STLport map and two vectors.
- **CGameScriptThingManager ctor** 00704a80 -- the counted pointer must be a NAMED local (a temporary is built straight
  into the by-value argument slot); `if (p) Info = new Block(p); else Info = 0;` lets VC thread both null arms onto the
  zero register.
- **CHitParameters ctor** 00789238 -- frame-pointer size TU (`optimize("y",off)` + `("s",on)`), `pack(1)` record, every
  member an initialiser in declaration order.
- **CInputProcessManager::RemoveAllInputProcesses** 00a0d530 + its unreferenced byte twin 00a0d5f0 -- the landed
  CLinkedList scan-info model (PreparePrimitivesForEngine 0xA0D6B0).
- **CHandleBasedMemoryPool** Validate 00a024c0, MoveArea 00a028f0, InternalAllocate 00a030d0 -- local model (retail
  Handles is a 12-byte CArray, so everything after +0x438 sits 4 below engine/CHandleBasedMemoryPool.h); PDB CHeader
  names. Validate: the bucket hit block inside the countdown loop ending in `break`; InternalAllocate: success path as
  the `if (area)` body; MoveArea: an inline CHeader::RemoveFromFreeList.
- **CGraphicBankManager::CloseGraphicBank** 009f8290 and **CMeshBankManager::CloseMeshBank** 00a09dc0 (label
  propagated) -- by-value counted bank, the two-level CCharString == of TerminateActionByName, a list iterator with a
  user copy ctor passed to the out-of-line erase, `break`.
- **CGameScriptInterface::RemoveAllCutSceneSkippedMessages** 0089af30 -- retail keeps the found list's sentinel in edi
  across FindAllMessages: only reproducible with that template's body (and push_back's) compiled earlier in the TU under
  `auto_inline(off)`; the list ctor links the node before storing it; the unlink reads next/prev into locals.
- **CGameScriptInterface::GetSleepingPositionAndOrientationFromBed** 008a1b10 -- nested ifs with one false exit; a
  `__forceinline` claim helper whose CRightHandedSet/index locals land in the frame / dead argument slot, with
  `user.GetThing()` inside the GetFreeActionPoint argument list; C3DVector without user copy/assign (three dword moves).
  IsThingABed is vtable slot 682 (cgsi_vtable_slots.tsv lists it 4 rows late).
- **CGameScriptThing::MsgWhoKilledMe** 008d0610 -- message manager read into a local before the filters; the
  quest manager's thing manager through an inline accessor; the CThing type-mask template with a constant-0 mask copied
  into a local keeps the dead `xor al,al ; jne`.
- **CInputProcessMain::HandleDebugCameraAction** 00687ed0 -- dense switch (byte-indexed jump table); case 1 is if/else
  whose else repeats the default body (VC cross-jumps it and hoists the input load); a fall-through does not.
- **CInputProcessSetRangedWeaponMode::ProcessInput** 0068f180 -- the out-parameter GetInterface (component in eax) and a
  `__forceinline CProcessedInput::AddGameEvent(long type)` helper (&event pushed before the input reload).
- **CInputProcessSpecialAbilities::Update** 0068ff50 -- `T* p = 0; if (HasTC(k)) p = GetTC(k);` then an unconditional call.
- **CInputProcessTargetLockCycleTargets::ProcessInput** 00689050 -- click flag in bl from the entry; the unset-component
  accessor duplicates the SetAsTargetNextThing call into both arms.
- **CIsVeryCloseMotionlessCreature::operator()** 00942270 -- `!(step > eps*eps)` gives retail's `test ah,5 ; jnp`; landed
  C3DVector inline operators; unset-component accessor.
- **CInputManager::IsEventProcessed** 009f4b10 / **IsEventInQueue** 009f4d40 -- the per-device compares must be
  `if (event.X() == stored.X()) return true;` statements (a `return a == b` materialises sete); IsEventInQueue has to
  index `EventStore[i]` in the loop body (a predicate taking the element by reference caches it); `case 3: return true;`
  shares the default's `dec eax ; je`.
- **CInputEventPackage::Save** 009f52a0 -- double local (aligned frame); `(float)GFGetTime() - start` sinks the `fstp`
  below the Write pushes where `(float)(now - start)` stores before them.
- **NUISystem::CKeyRedefiner::UpdateKeyText** 00557a10 -- function-scope text / tag flag / colour (R,G,B,A-assigning
  ctor), key name in the if block, white colour assigned from a temporary (self copy).

## Parked for Fable 5.1 (13)
- 0055bc30 CHoverable::~CHoverable DIFFER(203v203): retail pushes ebx/edi in the prologue, ours shrink-wraps them into the `if (cursor)` block | tried: base form, function-scope found/node locals, /EHsc, /Oy- | next: lane gap: shrink-wrapped callee-saved pushes
- 00a02b30 CHandleBasedMemoryPool::Free DIFFER(201v201): the final `mov [esi+444h],ebx` (FirstUnusedHandle) sinks below the MultiThreaded reload and the pops | tried: plain body, inline Begin/EndCritical, inline ReleaseHandle | next: lane gap: store scheduling
- 008f63c0 CHeightMap::CHeightMap DIFFER(207v229): `xor ecx,ecx` zero register for the vector + `adc` vs retail immediates, ebp taken for x+1, SizeY+1 not spilled into the dead argument slots | tried: x local, rows local, IsSafeProduct(cols, rows) helper, plain vector / no local (all identical code) | next: lane gap: register allocation around the 64-bit count guard
- 009fc950 CGraphicFrameData::InitialiseAsSurface DIFFER(224v230): source Data kept in ebx (retail spills it to the dead `texture` slot), folded `cmp ecx,[eax]` dimension compares, sete/bl ScreenGraphic | tried: && expression, MatchesTexture helper, early-return helper with GetX(), swapped operands | next: a GetDimensions(0) result held in one named local across both compares
- 009fca40 CGraphicFrameData::InitialiseAsSurfaceSection DIFFER(167v218): bare-dword `fistp` through a pointer for TLX/TLY | tried: C `(long)` conversions | next: lane gap: inline-asm FloatToLong helper (GOTCHAS 0xBE6790 family)
- 006cd890 CGroundNavigatorExternalInterface::CGroundNavigatorExternalInterface DIFFER(233v221): def release `mov esi,[eax+4] ; add eax,4 ; dec esi ; test ; mov [eax],esi` vs retail `dec [ecx+4] ; jne` | tried: holder with --count, holder calling Release(), pointer-local release (block scope fixed the frame) | next: lane gap: CDefPointer short release (retry 5 / chunk 161)
- 008a8010 CGameScriptInterface::SmashAllWindowsWithinRadiusOfPoint DIFFER(219v218): only the box arithmetic (`add esi,eax` in place vs `lea edx,[eax+esi]`) and its stores are scheduled differently | tried: ctor box, body-assigned box, TU-local query/push_back/Grow bodies (needed for the cached vector pointers), one-ternary find | next: C2DBoxI built from two C2DCoordI / an inline box-from-centre helper
- 008d0730 CGameScriptThing::IsBeingCarriedBy DIFFER(233v233): Compare argument loads swapped (`mov ecx,[ecx] ; mov edx,[eax]`) in the hand-split form; the natural `name == "" || name == carrier->ScriptName` is 236v233 with a `sete al` merge | tried: natural ||, `!strcmp`, hand-split null case | next: lane gap: jump threading through the inlined strcmp merge
- 00690c70 CInputProcessDigging::ProcessInput DIFFER(166v162) [/GS]: `input` cached in ebp at entry and AddLong/AddUBYTE arguments in their own frame words (retail re-reads the slot, homes both in the dead `event` slot) | tried: plain body, `__forceinline AddGameEvent(type,long,byte)` helper, HasTC byte form | next: lane gap: /GS parameter caching (GOTCHAS chunks 143/144)
- 00688c90 CInputProcessTargetLockCycleTargets::AddToInterface DIFFER(177v169): the def local's final release is long-form here, `dec [edi+4] ; jne` in retail, while the getter temporary needs the long form | tried: ReleaseTemporary dtor, direct-count dtor (local matches, temporary does not) | next: a distinct pointer type for the local (CDefPointer<const T> vs the getter's) or an explicit release path
- 009109b0 CIsSeatWithSpaceOwnedBy::operator() DIFFER(208v219): retail evaluates `POwner ? (...) : owned` as two flag-producing arms merged into one `jne` (`mov ecx,[ebx] ; cmp eax,ecx ; jmp`), ours folds `cmp eax,[ebx]` and gives each arm its own exit | tried: ternary with !owned, early-return if/else, mask local + `owned == false`, negated ternary | next: lane gap: cross-jumped ternary arms (or a bool member function for the owner arm)
- 00945e70 CIsSeatWithSpaceOwnedBy::operator() DIFFER(212v223): the item-pointer twin of 009109b0, same ownership-ternary difference | tried: the 009109b0 best form with the GetPItem + kind-5 entry | next: follows 009109b0
- 00bee070 __pop_heap<pair<long,CCompositeSurfaceInfo>> DIFFER(206v206): esi/edi swapped between `result` and `first` (and the following eax/ecx/edx rotation) | tried: implicit pair assignment, explicit inline pair operator= | next: lane gap: callee-saved register swap

## Skipped
- 009fd2b0 CGraphicDataBank::GetTextureType: the 240-byte row also holds the 16-aligned `ret 4` stub at 0x009FD390 after
  the jump table (two functions).

## Label corrections
- 0x00B0C9C0 "CHeroStatsDef" is NParticleEngine::CPSCUpdateNormal::CPSCUpdateNormal (vtable 0x0129FC1C).
- 0x00AF4E10 "CHeroMorphDef" is NParticleEngine::CParticleSystem::CParticleSystem (vtable 0x0129F4E8; dtor 0xAF5900 landed).
- 0x00DEC7F0 "CHeroMorphDef::CHeroMorphDef" is CQ_SecretPassageScript::CQ_SecretPassageScript (factory 0xDF1080, S_QSP).
- 0x00E7EF70 "CGameScriptThing" is CFishCompOrganiser::CFishCompOrganiser (factory 0xE7EEF0, "FishCompOrganiser").
- 0x00A09DC0 "CGraphicBankManager::CloseGraphicBank" is CMeshBankManager::CloseMeshBank (list +8, CMeshDataBankInfo).
- 0x00A0D5F0 is an unreferenced byte copy of RemoveAllInputProcesses 0x00A0D530.
- 0x00BEE070 "CItem::operator=" is SGI `__pop_heap<pair<long,CCompositeSurfaceInfo>>` (vtable 0x012A8154, adjust 0xBEDD70).
- Callee labels: 0x00453554 "CopyBackBufferToTexture" is map<EDamageAttribute,float>::map(); 0x007046D0 "SetAnimationMemoryPool"
  is CScriptThing's by-value counted-pointer setter; 0x00663340 "CEventKilledCreature" is GetHeroCombatDef; 0x00687D10
  "operator_new" is GetButtonPressureForEvent; 0x00625930 "GetCanItemBeQuickAssigned" is the kind-0xf6 component's
  IsActive; 0x00705170 "CGuiControlListBox::GetCurrentSelectedEntryData" is CTCCarryable::GetCarrier; 0x007FEF40
  "EditCyclePreviousKeyCamera" is the action-point "use" (push of the index); 0x00A03B90 "GetCanItemBeQuickAssigned" is
  CInputEvent::GetJoystickButton; 0x006CD2F0 "GetSubDef<CAICreatureWillPowerIndicatorDef>" is GetSubDef<CCreatureNavigationDef>;
  0x008D46D0's label names the CanBeSeenOrHeard instantiation (here IsTargettedAtThing); IsThingABed 0x008A0E80 is
  CGameScriptInterface slot 682 (cgsi_vtable_slots.tsv row 686).
- Relocation note: 00942270 binds CThing::GetPosition to 0x006A4D00 (a byte twin of 0x004C73D0, both used under that name
  by landed rows) -- the identity check reports it as a cross-row RELOC_CONFLICT warning, not HARD.
