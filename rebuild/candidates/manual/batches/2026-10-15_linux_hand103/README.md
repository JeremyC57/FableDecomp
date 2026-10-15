# Hand chunk 103 (CTCTavernGameCardBlackjack .. CTCThingOwner): 38 of 40 byte-exact

All 38 src/ rows grade MATCH/RELOCATION_MATCH (grades.txt) and their behaviour tests print PASS under
g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). No row needed a pragma except 004e1d20
(`optimize("y",off)` + `("s",on)`, in the source); no row needs `/EHsc`.

Retail layout note used throughout: the retail CTCTavernGame is smaller than Ego_r.pdb's (UIComponents
vector 0x0C vs 0x14, handle vectors 0x0C vs 0x10), so IsBeingPlayed +0x20, GameWon +0x21,
GameFinished +0x28, Score +0x2C, TargetTime +0x98, NewBestScore +0xA0 (PDB +0x28/+0x29/+0x30/+0x34/
+0xB4/+0xBC). engine/CTCTavernGame.h does not fit and is not used (local models). Vtable slot 30 of
every tavern game is LeaveGame, 31 NewGame, 34 QuitMidGame, 11 DrawPreparePrimitivesForRendering,
1 the persist hook.

## What matched (38)
- CTCTavernGameCardBlackjack (4): `HasGoneBust` 007da8b0 (ace-demoting blackjack total; `long` result
  via `xor eax,eax ; setg`; the hand pointer must be a NAMED local or ecx/edx swap); `vfunc_1` 007daa60
  (card-base persist + Transfer("MostMoneyWon", m, 0): the const& zero default is homed in the dead
  second-argument slot); `vfunc_30` 007daaa0 (LeaveGame + vector clear = Dinkumware erase(begin,end)
  with the early-out copy-down helper -> `cmp eax,eax ; jne` + memmove); constructor 007db280 (the
  vtable store precedes the nine zero stores -> three vector MEMBER OBJECTS with inline constructors).
- CTCTavernGameCardPairs (4): `LeaveGame` 007d5f60 (base LeaveGame + KillThing(m) + clear); `vfunc_57`
  007d5f90 (`dec edx ; jne` = one-case switch into a zero-initialised result local; a `return` in the
  case splits the exits); `vfunc_1` 007d6830 (persists the base TargetTime at retail +0x98, float
  Transfer); plus the mislabelled 00cacd00 (see corrections) under CUnknown.
- CTCTavernGameCardSorting (3): `LeaveGame` 007d4de0 (catalogued under CardPairs; slot 30 of the
  CardSorting vtable), `vfunc_57` 007d4e10, `vfunc_1` 007d56d0 (two Transfer calls, fresh const& zero
  default per call, second member address by advancing `this`).
- CTCTavernGameCoinBase (3): `LeaveGame` 008ef110 (three null-tested CThing::Kill(true) then the clears in
  reverse member order); `AtRest` 008ef1b0 (squared threshold written INSIDE the loop -> hoisted after the
  entry test into the dead argument slot; `x*x + y*y` through an inline GetLengthSquared;
  `test ah,41h ; je` = `>` early return false); `UpdateCoinSounds` 008ef910 (plain for over the 0x3C-byte
  coin vector, argument cached in ebx).
- CTCTavernGameCoinGolf (2 of 3 own + 2 mislabelled): `vfunc_11` 007d4a60 (base DrawPrepare +
  FadeOutTableOccupants(false) while IsBeingPlayed); `vfunc_30` 007d4a90 (LeaveGame: null-initialised
  camera-manager out pointer; the world's +0x34 object read through an INLINE ACCESSOR so it loads
  straight into ecx).
- CTCTavernGameShoveHaPenny (3): `vfunc_30` 007cf460 (LeaveGame with an UNinitialised out pointer, result
  ignored, re-read from the `push ecx` slot per call); `NewGame` 007cf4a0 (float `Score = 0.0f` and an
  integer zero share `xor eax,eax` -- both long and float forms match); `vfunc_33` 007cf4c0
  (`best == 0.0f || Score > best` -> `fld [0.0] ; fld best ; fucompp ; test ah,44h ; jnp` +
  `fld ; fcomp ; test ah,41h ; jne`).
- CTCTavernGameSpotTheAddition (4 of 5): `vfunc_30` 007d7b40 (as ShoveHaPenny's, base CTCTavernGame);
  `FindTarget` 007d7c40 (unsigned `index < size()` first then `index >= 0`; the element through the
  retail vector iterator object so `first` is re-loaded -- the landed CRetailVector model); `QuitMidGame`
  007d8140 (the constant push hoisted above two byte stores); 00c3a970 (mislabelled, see corrections).
- CTCTavernJug (1): `ReplaceJug` 00968090 (PDB layout is packed: `#pragma pack(1)` gives RHSet +0x11,
  Pos +0x29; physics component a named local, null-tested; `&RHSet` by advancing `this`).
- CTCTavernTable (2 own + 1 mislabelled): `GetTankard` 007b8590 / `GetFood` 007b85c0 (`size() > index` in
  that operand order, unsigned; element through the iterator object; Tankards +0x1C, Food +0x28).
- CTCTavernTankard (1): `ReplaceTankard` 00947c30 (as ReplaceJug, no null test, slot 32 for RHSet).
- CTCTeleporter (3): `SetAsActive` 007dfeb0 (`active != member`; the GUI refresh is the last statement ->
  tail jump reusing the dead argument slot for its 0); constructor 007dfee0 (pack(1) PDB layout;
  PDef / PParticle member objects, three init-list flag bytes, WaitingForTutorialToFinish a BODY
  assignment, GetSubDef(&PDef) on the re-read parent); `vfunc_1` 007dff40 (two bool Transfers under the
  persist flag).
- CTCTextureDecal (2 of 3): `ChangeDecal` 0070a3e0 (16-byte POD record assigned whole: dword copies through
  one register with `lea esi,[node+10h]`); `DeleteDecal` 0070a430 (`it = Erase(it)` with the STLport
  node unlink + free inlined, `found` byte flag, then Dirty + parent forwarder(4)).
- CTCThingOwner (1 of 2): `OnKill` 0071b200 (`for_each(begin(), end(), CKillThing(CallOnDie))` with a
  __fastcall for_each returning the non-POD functor through the hidden pointer; iterators with a user
  copy constructor are built in their reserved slots; then `clear()`).
- Mislabelled rows matched under their real identity: 00cacd00 `CUnknown::NonDelegatingAddRef`
  (InterlockedIncrement, then the windef.h `max` macro over a plain LONG m_cRef); 0086bde0
  `CEntryLayers_0086bde0::PushEntry` (a 24-byte entry temporary whose flag lives INSIDE its 16-byte list
  record at +0x0D); 00c59fc0 `CMaterialState_00c59fc0::SetAnimatedStage`; 00c3a970
  `CInterpolatedFloat::CInterpolatedFloat` (init-list flag between the hoisted `push 0` and the Reset
  call); 0095aa00 `CAIStateGroup_HoverHornetQueen::KillBossHelpers` (landed out-parameter GetInterface
  template, interface 0xAD); 00a3eb60 `CSoundManagerLH::vfunc_5` (`delete sound` after `sound->Stop()`
  has NO null test); 004e1d20 `_global::__pop_heap` (SGI, /Gr, frame pointer = `optimize("y",off)` +
  `("s",on)`).

## Parked for Fable 5.1 (2)
- 007d3510 CTCTavernGameCoinGolf::vfunc_32 DIFFER(80v80): retail `mov ecx,[esi+4] ; mov ecx,[ecx+70h] ; lea eax,[esp+8] ; xor edi,edi ; push eax ; mov [esp+0Ch],edi` vs lane `mov ecx,[esi+4] ; xor edi,edi ; lea eax ; mov [esp+8],edi ; mov ecx,[ecx+70h] ; push eax` (the null out-pointer store scheduled between the two loads) | tried: raw-pointer out local, inline GetDefinition accessor, CDefPointer class with null constructor, definition pointer taken into a local first | next: lane gap: scheduling of a zero store against an address-chain load (same family as the 24i `lea ecx` after zero stores gap)
- 007d7ec0 CTCTavernGameSpotTheAddition::vfunc_11 DIFFER(78v66): (1) the same null out-pointer store schedule as 007d3510; (2) the inlined CDefPointer release compiles to the 24h counted shape `mov esi,[eax+4] ; add eax,4 ; dec esi ; mov [eax],esi ; jne` where retail has `mov ecx,[esp+8] ; test ecx,ecx ; je ; dec [ecx+4] ; jne ; call [vt+4]` | tried: `m_p && --m_p->n` destructor, local copy with &&, Release() member on the pointee, nested ifs on a local copy | next: write the release in the body on a plain pointer copied from the out slot (`T* def = slot; if (def) { if (--def->m_refCount == 0) def->Destroy(); }` -- the open-coded form gave the clean `dec [edi+4] ; jne` in 007d3510); part (1) is probably a lane gap

## Skipped
- none (no row holds two functions).

## Label corrections
- 007d4de0 is CTCTavernGameCardSorting::LeaveGame (slot 30 of 0x0126F5B4), not CardPairs.
- 007daaa0 / 007d4a90 / 007cf460 / 007d7b40 ("vfunc_30") are LeaveGame overrides; 007d8140 slot 34 =
  QuitMidGame; 007cf4a0 slot 31 = NewGame; 007d4a60 / 007d7ec0 slot 11 = DrawPreparePrimitivesForRendering.
- 00cacd00 "CTCTavernGameCardPairs::GetCardOrder" -> CUnknown::NonDelegatingAddRef (CTextureRenderer
  vtable 0x0129D08C slot 1, DirectShow strmbase).
- 00c3a970 "CTCTavernGameSpotTheAddition::QuitMidGame" -> a smoothed-float constructor (descriptive name
  CInterpolatedFloat), member +0x138 of the object built at 0x00C39130; 0x00C0B510 is its Reset(float)
  (not "Matrix_SetIdentityRotation").
- 0086bde0 / 00c59fc0 "CTCTavernGameCoinGolf::DrawPreparePrimitivesForRendering" -> a layered 24-byte
  entry push (no direct callers) and a render material-stage setter (caller 0x00C5BD00).
- 007b8590 "CTCTavernTable::GetFood" reads Tankards (+0x1C): GetTankard; 007b85c0 is GetFood (+0x28).
- 0095aa00 "CTCTavernTable::FreeServingPoint" -> a CAIStateGroup_HoverHornetQueen helper (this->PBrain->
  Creature, interface 0xAD, CTCBoss::KillHelpers), called only from its OnReplace 0x0095AB30.
- 00a3eb60 "CTCTextureDecal::DeleteDecal" -> CSoundManagerLH vtable 0x0129D2C4 slot 5.
- 004e1d20 "CTCThingOwner::SetOwnedObjectsSerialisationSectionName" -> SGI __pop_heap for
  pair<CCharString, CTCBase::CRegisteredTCInfo>; 0x004DA56A / 0x004D788D are that pair's operator= /
  copy constructor (not "pair<CCharString,CCharString>").
- 0x008F0920 is CTCTavernGameCardBase slot 1 (not "CQR_EscortTrader_ManagerScript::OnPersist");
  0x00686A70 is CTCBase::GetWorld; 0x00686800 is CTCBase::CTCBase.
