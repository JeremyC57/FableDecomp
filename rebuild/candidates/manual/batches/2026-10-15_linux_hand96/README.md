# Hand chunk 96 (CTCKraken .. CTCMeleeTrap): 38 of 40 byte-exact

All 38 sources in `src/` grade MATCH / RELOCATION_MATCH (`grades.txt`) and their tests print PASS under g++ (`tall.sh`)
and the lane's VC7.1 cl.exe (`tall.py`). 12 rows carried propagated manifest labels and are renamed (see the last section).

## What matched (38)

### CTCKraken (3)
- 007fb4a0 `CTCKraken::UpdateTentaclesOnly` -- GetClosestTentacleToTarget into AttackingTentacleIterator (+0x48), then the
  tentacle thing's interface 227 (CTCKrakenTentacle) gets Tracking = true. SHAPE: the out-parameter `GetInterface(type, &out)`
  template (0x007504E0 family) must be `__forceinline` here -- behind the GetPItem call VC7.1 emitted it out of line; the
  Tracking store is duplicated into the absent path that stores through the uninitialised out word.
- 007fbc30 `CTCKraken::StartHeadEffects` -- `CCharString name("MEETS_WATER", -1); CreateHeadEffect(name);` (named local in the push-ecx slot).
- 008f9e60 -> `CTimedTrigger_008f9e60::Start(long)` -- `m_startTick = GetTickCount(); m_duration = d; CheckAndTrigger();` (dllimport GetTickCount).

### CTCKrakenTentacle (3)
- 007f0770 `SetIsStriking(bool)` -- `if (IsStriking) LastStrikeTick = frame; IsStriking = s;` (the bool store tail-duplicated by VC).
- 007f1170 `QueueStrikeDamage()` -- QueuedStrikeTick = frame + (long)(fps * def delay). SHAPE: the frame-rate global and the def
  pointer are named locals read BEFORE the frame call (the int parked in the push-ecx slot for `fild`); a third GetPItem call
  whose result is never used is an unused local (`CThing* kraken = PKraken.Get();`).
- 007f15d0 `SetFacingAngleAndDummy(float, const CCharString&)` -- float dword copy, CCharString operator=, ReturnToDesiredPos(true).

### CTCLifeTimer (1)
- 007e5970 `CTCLifeTimer::CTCLifeTimer(CThing*)` -- init list FramesLeft -1 / FadeOutBeforeKill false / PerformOnDie true, then
  `m_pThing->SetForceFrameUpdate(true)` (owner load and `push 1` hoisted above the initialiser stores).

### CTCLight (3)
- 00641210 `CTCLight::OnAppearanceClear()` -- clears Lights then FlareSprites (landed STLport `CopyTrivial` clear). SHAPE: slot 1 of the
  SECOND vtable: the override of a virtual introduced by a secondary base at +0x0c is compiled with `this` at that subobject, so
  every offset is 0x0c low (+0x20 / +0x2c) -- model `struct CTCLight : CTCBase, CAppearanceListener`.
- 00641480 (vtable slot 6) -> `CTCLight::ApplyDefActiveState()` -- `if (PLightDef && !Initialised) SetActive(def->+0x40)`; retail keeps `call ; ret`.
- 00640d00 (vtable slot 9) -> `CTCLight::RequestAppearanceUpdate()` -- `if (Active && def->+0x42) m_pThing->Forward(4)`.

### CTCLightningNode (2) / CTCLightningOrb (3) / relatives (2)
- 004e6d17 `CTCLightningNode` ctor, 004db4d7 `CTCLightningOrb` ctor -- inline `CIntelligentPointer<T>` constructors (out-of-line base
  ctor 0x00A01B10, then their own vtable store); initialisers in declaration order. SHAPE: `#pragma optimize("s", on)` (pass-through
  `push dword ptr [esp+10h]`, the IP vtable constant cached in ebx, `or [..],-1`).
- 004e769f `~CTCLightningNode`, 004dcedd `~CTCLightningOrb` -- plain `??1` of a class that declares NO destructor (no vptr reset),
  members in reverse order. SHAPE: size TU (`and dword ptr [esi+N],0` clear) with `__forceinline` on the def pointer's destructor and
  Release (else out of line); `??1` emitted through a `new T` factory (the `??_G` calls it).
- 004e33bb (label "~CTCLightningOrb") -> `CTCNymph::~CTCNymph` -- same shape plus the FlightSpline vector's `if (p) free(p)`
  (`pop ecx`) and three intelligent pointers at the PACKED offsets +0x41/+0x49/+0x68.
- 004e3401 (label "CTCLightningOrb ctor") -> `CTCDExplosiveTrail::CTCDExplosiveTrail` -- CTCDriverBase base; the VC6 list
  constructor's default allocator temporary homed at `[ebp+0Bh]`. SHAPE: `optimize("y",off)` + `("s",on)`.
- 007f23d0 (vtable slot 6) -> `CTCLightningOrb::LoadDefAndFlagAppearance()` -- counted copy of the owner's PBaseDef (inline copy
  ctor `inc [esi+4]`), GetSubDef into PDef, release through the def object, then `PTCGraphicAppearance->+0x59 |= 2`.

### CTCLook (15)
- 007517b0 `SetHeadTurnState` -- `TimeToStopLooking = -1; if (HeadTurnState != s) { HeadTurnState = s; OneFrameDelay = true; }`.
  Retail CTCLook offsets after the 12-byte TurningAnims vector are PDB - 4 (+0xb4 / +0xb8; CachedHeadPos +0xa1).
- 00751b80 `StopLooking(long)` and 00753080 `FrameUpdateOverriddenByParent()` -- inline change-tested setters with the -1 store AFTER
  the head-state change (a separate statement, not SetHeadTurnState); the constant 1 CSE'd in eax / ebx (`test bl,al`).
- 00751ad0 `UpdateLookTimer()` -- LookTimer = frame + 90 through the CTCBase accessor 0x00686D60.
- 00751b30 `GetHeadPos()` -- C3DVector by hidden pointer; SHAPE: the cached copy needs a memberwise user copy constructor
  (implicit copy of the packed member = block copy through esi); pack(1) for CachedHeadPos at +0xa1.
- 00751fc0 `IsWithinYZHeadRange(float)` -- `long` 0/1 result of `a <= up*K && a >= -(down*K)` (K = 1/360).
- 00753020 `IsTurningBody()` -- the genuine CTCLook one: two float out-locals for GetTurningAnglesAndCheckForTurningBody.
- 00753040 `OnCreate()` -- GetSubDef(PDef), GetSubDef(PAppearanceDef) on the owner's PBaseDef (+0x70), CentreHead, ChangeEyeAngles(0,0).
- 00753b40 `IsCurrentlyTurningViaAnimation()` -- inline GetInterface(90) with the ignored bool (garbage out word on the absent path).
- 00851cc0 -> `CRetailMap_00851cc0::Add(long, long)` -- stack value_type + discarded pair<iterator,bool> for the landed tree insert 0x00463E2E.
- 00c0a8c0 -> `CBitMaskPair_00c0a8c0::SetFlag(unsigned long, bool)` -- thiscall forwarding to the `__fastcall` bit helper 0x00C0AF00.
- 00c2d100 -> `CRandomDelay_00c2d100::PickDelay(float)` -- geometric draw with `rand` (dllimport) and intrinsic log10. SHAPE: the
  argument rescale must be two statements (`rate *= 10.0f; rate /= m_period;`); one expression is re-associated to fdiv-then-fmul.
- 00c33920 -> `SetOrientationVectorsFromCopies` (free `__stdcall`) -- `set->SetOrientationVectors(C3DVector(*f), C3DVector(*u))`.
  SHAPE: C3DVector with a memberwise user copy constructor (z stored first, eax reused for the second source).
- 00c377b0 -> `CFloatMaximum_00c377b0::Accumulate(float)` -- `if (v > m_max) m_max = v;`.
- 00c44880 -> `CRenderSetting_00c44880::SetValue(long)` -- `if (m != v) { m = v; m_flags |= 0x4000; }` (sibling of landed 0x00C445C0/0x00C44860).

### CTCMapwho (5)
- 0071a670 `CTCMapwho` ctor -- CMapwhoNode members with inline zeroing constructors, SearchID(0), `InMapwho = 0` (bitfield `and ..,0FEh`),
  back pointers TL, BR, TR, BL as body statements.
- 0071a6d0 `IsToBePlacedInMapwho` -- manager taken into a local before the argument pushes; `if (!f()) return false; return true;` = `setne`.
- 0071a750 `MoveInMapwho` -- both accessor results as named locals (call before pushes); inline PlaceInMapwho.
- 0071a810 `??_GCTCMapwho` -- `__forceinline virtual ~CTCMapwho() { RemoveFromMapwho(); }` (+ `__forceinline` helper) and a `new` factory.
- 008ebdd0 (label "CTCMapwho::OnDie") -> `SCountedNamedRecord_008ebdd0::DeleteObject` -- the counted-pointer deleter stored by
  0x008EBE90: `__fastcall`, explicit `if (p) { p->~T(); operator delete(p); }`.

### CTCMeleeTrap (1)
- 007883c3 `InitTrap()` -- clears PlayingContinuous/StrikeAnimation; type 3 -> 0x00788198, else `InitialiseAnimation(CCharString("DEFAULT", -1), false)`
  as a named local in the else block. SHAPE: `optimize("y",off)` + `("s",on)` (leave + free esi).

## Parked for Fable 5.1 (2)
- 00751da0 CTCLook::IsAbleToTurnDaBody DIFFER(57v59): retail `mov eax,[esi+10h] ; test eax,eax ; ... test byte ptr [eax+91h],1 ; ... mov ecx,eax ; call IsPerformingAnyAction`, ours loads PParentCreature straight into ecx | tried: member reads, a creature local, a static inline helper taking the pointer, a Dead bitfield | next: an inline CTCLook accessor returning the creature through an out parameter (the GetInterface eax pattern), or lane gap: register choice
- 007884f5 CTCMeleeTrap::ResetTrap DIFFER(98v96): retail `mov ebx,[ebp-4]` straight after GetSubDef and releases through ebx; ours `mov eax,[ebp-4] ... mov ebx,eax` (best form) / re-reads `[ebp-4]` for the release (honest CDefPointer-destructor form, 102v96) | tried: CDefPointer destructor (plain and `__forceinline` Release), local-copy release, raw-pointer explicit release | next: lane gap: escaped CDefPointer local cached in a register by retail (wave 24k "CDefPointer local re-read from its slot")

## Skipped
- none (no row spans two functions).

## Label corrections
- 004e33bb "CTCLightningOrb::~CTCLightningOrb" is `CTCNymph::~CTCNymph` (only caller 0x004E339F = slot 0 of CTCNymph's vtable 0x01242AD4).
- 004e3401 "CTCLightningOrb::CTCLightningOrb" is `CTCDExplosiveTrail::CTCDExplosiveTrail` (stores vtable 0x01242B4C; CTCDriverBase base 0x00771200).
- 008f9e60 "CTCKraken::StartHighAttackBeam" is not CTCKraken (members at +0x100..+0x108, CTCKraken is 0x78 bytes; no direct callers).
- 008ebdd0 "CTCMapwho::OnDie" is a counted-pointer deleter (referenced only by the CCountedPointer constructor 0x008EBE90).
- 00851cc0 / 00c0a8c0 / 00c33920 "CTCLook::IsTurningBody", 00c2d100 "CTCLook::UpdateLookTimer", 00c377b0 / 00c44880
  "CTCLook::SetHeadTurnState" are propagated over unrelated bodies (tree insert wrapper, bit-mask setter, free __stdcall
  orientation helper, random delay, float max, engine render setting); the landed 0x00C44860 "CTCLook::SetHeadTurnState" and
  0x00C0A8A0 "CTCLook::IsTurningBody" carry the same wrong label.
- Callee labels used as hints only: 0x00686800 is CTCBase::CTCBase (not CEngineSubPrimitive2DClockSprite), 0x00686830 ~CTCBase
  (not ~CChunkedFileChunk), 0x0049D870 the frame getter (not GFGetPreMainMemoryUsed), 0x0099EAE0 ~CCharString, 0x004C7590 a
  CThing forwarder to PTCGraphicAppearance (not CRenderTarget::DetachTarget), 0x004E40A0 a vector destructor (not
  CActiveFile::OnReadFinished), 0x00788198 a CTCMeleeTrap routine (not CEngine::ChangeCameraAngle).
- vfunc rows renamed from what they do: 00640d00 RequestAppearanceUpdate, 00641480 ApplyDefActiveState, 007f23d0 LoadDefAndFlagAppearance.
