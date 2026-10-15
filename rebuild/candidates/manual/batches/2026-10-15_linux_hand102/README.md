# Hand chunk 102 (CTCSwitchableNavigation .. CTCTavernGameCardBlackjack): 33 of 40 byte-exact

All 33 matched rows grade MATCH / RELOCATION_MATCH from the file in src/ (grades.txt) and their behaviour tests
print PASS under g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). 6 rows are parked in differ/, 1 row skipped.
Several catalog labels in this chunk are propagated over other classes' bodies; the sources carry the real identity
(see "Label corrections").

## What matched (33)

- **CTCSwitchableNavigation** -- 007876d0 `~CTCSwitchableNavigation`: when the nav map is active, un-blocks the parent's
  switchable nodes (40-bit id = `m_id & 0xFFFFFFFFFF`), then `jmp ~CTCBase`. User destructor (stores its own vtable); the
  navigator manager is a named local right after `GetWorld()` (0x00686A70).
- **CTCTalk** (Ego_r.pdb layout, 0x3c bytes)
  - 006e27e0 constructor: init list in declaration order; LastNarrator `("")` with the defaulted `-1`; scalars share ebx.
  - 006e28a0 `IsTalking`: `return frame - started < duration;` (plain bool return = `xor ecx,ecx ; setl cl ; mov al,cl`).
  - 006e28d0 `StopCurrentSpeech(emitter)`: stop sample, cut the window to `frame - started - 1`, reset; float 0.0f body
    assignment shares the eax zero, -1.0f immediate.
  - 006e2910 `DoesSpeechExist(id)`: the data bank is a named local taken FIRST (retail loads it into ecx before any push,
    `mov eax,ecx` parks this); the `found` bool lives in the high byte of the `push ecx` slot.
  - 006e2cb0 `Persist` (vtable slot 1, catalogued vfunc_1): `Transfer("IsPureAISpeaker", b, true)` homes the `true`
    default in the dead flag slot; then `Transfer("DialogueVoice", &voice)`.
  - 006e3180 `GetAttitude`: inline out-parameter `GetInterface(&comp, 0x31)` template (0x007504E0 family) then the mode's
    virtual type == 0x56 -> 10, else Attitude.
  - 006e3b10 `PlaySpeech(id, layer, b1, b2)`: forwards with `DialogueVoice`; 006e3b30 / 006e3b60 `PlayAISpeech` overloads:
    `if (!IsPureAISpeaker && !force) return 1;` then forward with layer 1 (the second one tail-jumps with slot reuse).
- **CBaseVideoRenderer** -- 00ca5380 `OnStopStreaming` (DirectShow renbase.cpp body; catalogued CTCTalk::IsTalking):
  `m_tStreamingStart = timeGetTime() - m_tStreamingStart; return NOERROR;` with an `extern "C" __declspec(dllimport)` import.
- **CTCTargeted**
  - 004da8f7 constructor: `#pragma optimize("s",on)` (pass-through `push dword ptr [esp+8]`); local polymorphic model with
    the engine header's member names (the header is a flattened POD).
  - 007ea510 `OnCreate`: `GetSubDef(&PTargetingDef)` -> glow (default true), flags (default 0) through the null-tested def.
- **NTCScriptedControl::CActionFireProjectileWeapon** -- 00716790 constructor (catalogued CTCTargeted::CTCTargeted): the
  landed CActionPickUpGenericBox model (novtable CAction, ebx zeros, vtable sunk before GetScriptThing) plus the inline
  sentinel-list constructor (head = 0, node linked to itself, head = node); CurrentActionIndex = -1 at retail +0x1c.
- **CTCTargetingBase**
  - 004da8b7 `~CTCTargetingBase`: no vtable store = `__declspec(novtable)`; size TU (`and dword ptr [..],0` in the
    `__forceinline` def release); intelligent pointers with no own destructor -> base destructor at each unaligned member.
  - 007f5860 `GetPNearestInterestingThing`: ignore -> 0, else override (if set) else nearest; two sibling-call jumps.
  - 007f5890 `SetPScriptOverrideInterestingThing(thing)`: `if (ParentThing != thing) { CIntelligentPointer p(thing);
    member.SetPItem(p.GetPItem()); }` -- the converting constructor is INLINE in this speed TU (base ctor, vtable, SetPItem).
- **CTCDExplosiveTrail** -- 004e4045 destructor (catalogued ~CTCTargetingBase): same novtable/size shape; retail list is
  4 bytes and vector 12, so PDef is at +0x2c.
- **CTCTrapBase** -- 007891f8 destructor (catalogued ~CTCTargetingBase): same shape; engine/CTCTrapBase.h is 0xc high
  after its three 16-byte vectors (retail 12), local model with the header names at retail offsets.
- **CTCTargetingPlayer**
  - 008023e0 `UpdateNearestInterestingThing` (slot 39): clear, then `if (GetPTargetedThing()) Set(GetPTargetedThing());
    else CTCTargetingBase::UpdateNearestInterestingThing();` (virtual getter twice, base call as the sibling jump).
  - 00803b20 `PostProcessTargetedThing`: old threatened pointer read into a named local before the virtual call; returns
    the parent's bool check (tail jump).
- **CTCTavernBar**
  - 007b9f60 `GetJug(index)`: by-value CIntelligentPointer result; the element through the retail vector's iterator OBJECT
    (`*(begin()+n)`, landed 0x004AF3C0 model) -- a raw `first[index]` hoists the element address (94v94).
  - 007b9fc0 constructor: the STLport vector modelled with its real structure (_Vector_base initialisers +
    _STLP_alloc_proxy end pointer over an `allocator()` temporary) puts `mov ecx,edi` between the 2nd and 3rd zero store.
- **CTCWallMount** -- 007de4a0 constructor (catalogued CTCTavernBar::CTCTavernBar): init list, Trophy pointer, 64-bit id 0.
- **CTCTavernGame**
  - 005e9b70 `SetStateAfterTime(state, time)`: quit-request global -> LeaveGame (slot 30); else prev = cur, cur = 14,
    next = state, timer = time (float copied with integer moves).
  - 005eb4b0 `GetHeroStats(out)`: hero via `world->PlayerManager->GetMainPlayer()->GetPlayerCreature()`, alive check, then
    the GetInterface template writing *out -- it must be `__forceinline` (unused bool result keeps it out of line, 50v92).
- **C2DVector** -- 00c46dc0 `operator==` (catalogued CTCTavernGame::IsBetting): free `__fastcall`, `return a.X == b.X &&
  a.Y == b.Y;` -> full-register 1/0 exits, `// Leaf: ??8`.
- **CTCTavernGameCardBase** -- 008f0930 `NewGame`: base NewGame, five source-order zero stores from eax, SetState(7);
  008f0960 `NewRound` (catalogued NewGame): base NewRound, bool then MaxY..MinX zeros, SetState(18).
- **CTCTavernGameCardBlackjack** -- 007da020 `EnterGame`, 007da050 `NewGame` (base call + zero stores); 007da1a0 `CanSplit`:
  ONE `if (a && b && c && d) return true; return false;` (early returns duplicate the exits, a nested if CSEs `first`);
  007da230 `GetHandValue(cards, includeFaceDown)`: visible-card sum then `while (total > 21 && aces > 0)`.

## Parked for Fable 5.1 (6)

- 006e2cf0 CTCTalk::SetAsUsingNarrator DIFFER(66v64): retail `mov edx,[esp+0Ch] ; mov esi,[eax+104h] ; lea ecx,[esp+0Ch] ; push ecx ; mov ecx,[eax+100h]`, lane loads last first and first through eax (`mov ecx,eax`) | tried: `last` local, no local, result local, begin()/end() accessors + template front end | next: model the narrator list as a real STLport vector member with an iterator-object begin()/end() and the SGI find front end taking iterators by value
- 00802590 CTCTargetingPlayer::SetAsTargetNextThing DIFFER(64v64): block order -- retail places the method-0 arm right after the flag test (`je` to the method-1 arm), lane places method 1 first | tried: `locked && !flag` if/else, `!locked || flag` if/else, nested if + early return, ternary argument | next: lane gap: branch layout of a tail-duplicated if/else (try a `bool` helper `IsLiveLockTarget()` returning the test)
- 005e9b20 CTCTavernGame::SetState DIFFER(74v74): state-47 arm `mov eax,[ecx] ; call [eax+78h]` (retail) vs `mov edx,[ecx] ; call [edx+78h]` | tried: if/else-if + return, switch, three-arm with inline ChangeState, two sequential ifs | next: lane gap: register choice for a dead-argument vtable fetch
- 007bd2a7 NThingShot::CTCShotDefinitionMultiArrow::GetWeaponTrailGraphic DIFFER(49v42): retail release `dec dword ptr [ecx+4] ; jne`, lane `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0` | tried: member `--Object->count`, local pointer copy, pointee Release(), speed TU with frame pointer | next: drop the `const` sub-def / `mutable` count, or a non-template def pointer; check whether the TU is speed with `and [ebp-4],0` from an EH-like store
- 008f0b80 CTCTavernGameCardBase::GetPackPosition DIFFER(75v73): retail loads the def object straight into ecx, lane `mov eax,[esp+4]` + `mov ecx,eax` (and edx for the count) | tried: member-form release (82v73), local pointer copy (78v73), pointee Release (78v73), local pointer + local count (75v73) | next: read the def object into a named local in the body (`CTavernGameCardBaseDef* d = def.Object;`) so it lands in the dead `this` register
- 008f0b20 CTCTavernGameCardBase::GetCardDef DIFFER(112v92): retail keeps the local def's object in esi across the old object's virtual Destroy() and into the local's destructor; lane reloads the address-taken local after every call, and orders the zero store / `[thing+70h]` load differently | tried: member-form operator= + destructor, operator= reading both objects into locals + local-copy destructor | next: lane gap: address-taken local CSE'd across a call by retail; maybe a raw `T**` GetSubDef out-parameter

## Skipped

- 004debf7 CTCTargetingSpirit::vfunc_10: the 37-byte row holds six tiny functions back to back (`mov eax,[ecx] ; jmp
  [eax+24h]`, `xor eax,eax ; ret`, `xor eax,eax ; ret 0Ch`, `xor eax,eax ; ret 4`, a call forwarder, `mov al,1 ; ret`).

## Label corrections

- 00ca5380 "CTCTalk::IsTalking" -> CBaseVideoRenderer::OnStopStreaming (vtable 0x0129D08C slot 17, CTextureRenderer).
- 00716790 "CTCTargeted::CTCTargeted" -> NTCScriptedControl::CActionFireProjectileWeapon::CActionFireProjectileWeapon (vtable 0x01265960).
- 004e4045 "CTCTargetingBase::~CTCTargetingBase" -> CTCDExplosiveTrail::~CTCDExplosiveTrail (called by ??_G 0x004E4029, vtable 0x01242B4C slot 0).
- 007891f8 "CTCTargetingBase::~CTCTargetingBase" -> CTCTrapBase::~CTCTrapBase (called by the ??_G of CTCTrapBase/CTCMeleeTrap/CTCProjectileTrap).
- 007de4a0 "CTCTavernBar::CTCTavernBar" -> CTCWallMount::CTCWallMount (vtable 0x0126FB7C).
- 007bd2a7 "CTCTavernGame::GetBanter" -> NThingShot::CTCShotDefinitionMultiArrow::GetWeaponTrailGraphic (vtable 0x0126DF34 slot 32).
- 008f0b80 "CTCTavernGame::GetBanter" -> CTCTavernGameCardBase::GetPackPosition (C3DVector by hidden pointer).
- 00c46dc0 "CTCTavernGame::IsBetting" -> `bool __fastcall operator==(const C2DVector&, const C2DVector&)`.
- 008f0960 "CTCTavernGameCardBase::NewGame" -> CTCTavernGameCardBase::NewRound (slot 32).
- 006e2cb0 "CTCTalk::vfunc_1" -> CTCTalk::Persist; 008023e0 "CTCTargetingPlayer::vfunc_39" -> UpdateNearestInterestingThing.
- Callee labels used descriptively: 0x00686A70 is CTCBase::GetWorld (not CViewBase::GetDisplayEngine); 0x007F5810 is
  CTCTargetingBase::SetPNearestInterestingThing (not CVillageTaskCarryCrate::AssignCreatureToTask); 0x005E9A90 is
  CTCTavernGame::NewRound (not CXMVPlaybackInfo); 0x004DFB47 / 0x004E1B08 are container destructors (not
  CActiveFile::OnReadFinished); 0x0049D870 is the frame getter (not GFGetPreMainMemoryUsed); 0x0073A6E0 is a mode getter
  (not CPixelShader::CountReferences); 0x0066ADD0 is a speech-stop member (not CGameScriptThing::SetToKillOnLevelUnload).
