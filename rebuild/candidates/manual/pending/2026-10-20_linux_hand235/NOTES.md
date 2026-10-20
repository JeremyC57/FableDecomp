# Hand chunk 235 (CTCTavernGameCardBase .. CTCWife): 29 of 40 byte-exact

All 29 matches are RELOCATION_MATCH under the lane default RTM cl 13.10.3077 (no QFE row). Two rows need `/GS`
(`// Flags: /GS`: 007dfcf0, 007dff90). Frame-pointer size-TU rows use `#pragma optimize("y", off)` + `optimize("s", on)`
(0059eb0c, 00658620, 0065890b, 00659164, 00659efc, 0065a686). No jump tables. Every src/ row has a test that passes
under g++ and the lane VC7.1 (`tall.sh`, `tall.py`).

Stand-in callee bodies (same-TU / link-time knowledge, under `auto_inline(off)`, commented in each header):
007cff40/007d3700/007d8160/007d8290/007786f0 (the typed GetSubDef in the same TU), 007786f0 also
GFGetValPositionInLimitsLooping (only eax/ecx, so edx survives), 007fecb0 (vector<long> _M_insert_overflow as the
landed STLport body + operator=), 00659164 (GetDef 0x0042AF3C, GetSubDef 0x00512FE0, and a plain-function
IS_THING_ALIVE<const CThing*> body). The IS_THING_ALIVE stand-in is a non-template `__fastcall` function, so its symbol
does not carry the template mangling of 0x0043DBD0 (reloc-audit note).

Local models instead of engine headers (layout provably differs): CTCVillage (retail lists are one pointer, so
everything past +0x28 is below the PDB; the landed CTCVillage rows do the same), CTCTurncoatSpell (PDef/PTarget 4 below
the header), CTCWife (+0x91 ReceivedWeddingRing, 4 below the PDB past the GiftsReceived map, as landed 0x006CFEC0).

## What matched (29)
- CTCTavernGameCardSorting/CardPairs GetRequiredScore (007d5450, 007d6720) and GetBestScore (007d5290, 007d6560): the
  `dec eax ; je` game-type test is a one-case `switch`; GetBestScore needs `return text;` inside the case (with `break`
  the empty-string temporary gets its own slot instead of the dead hidden-result slot); CardPairs zero-initialises the
  stats out pointer, CardSorting does not.
- GetAdditionalInfo for ShoveHaPenny/CoinGolf/SpotTheAddition (007cff40, 007d3700, 007d8160): `a + b + L"/" + c`
  evaluated right to left; the typed sub-def stays in edi only with a same-TU GetSubDef stand-in that writes the out
  pointer; a sub-def read through vector `size()` keeps its release null test, a direct field read drops it.
- CTCTavernGameCardPairs NewRound (007d70e0, the landed CardSorting NewRound plus four trailing stores) and SelectCard
  (007d5fb0, first try: block-scoped sound strings; the match/mismatch arms cross-jump on the hoisted `push -1 ; lea ecx`).
- CTCTavernGameSpotTheAddition::UpdateHighlightedObject (007d8290): named float radius, `radius + radius`, fsqrt intrinsic,
  GetSubDef stand-in.
- CTCTavernTable ctor (007b8710): containers as member objects with inline zeroing ctors, flags in the init list, each
  Strlen the inline length of a temporary CCharString.
- CTCTeleporter TeleportHero/StartTeleporting (007dfcf0, 007dff90; /GS): inline by-value AddFloat/AddBool/AddULONG
  payloads; StartTeleporting's out-parameter GetInterface leaves the map pointer uninitialised and the key/out flag reuse
  the dead `hero` slot.
- CTCThingOwner::OnLevelUnload (0071b380): a malloc'd sentinel list<CThing*> local with inline push_back/pop_back/dtor;
  the component lookup is an inline Dinkumware lower_bound (`count / 2`, `count -= half + 1`).
- CTCTimeAppearanceFade::FrameUpdate (007786f0): SetAlpha written in every arm (cross-jumped), the def class read into a
  named local before the def pointer, stand-ins for GetSubDef and the looping-limits helper (keeps time in edx).
- CTCTrophy StartDialogBox (006dd7e0: implicit `L""` conversion temporary built before the pushes) and FrameUpdate
  (006df480: the text symbol as an implicit "..." conversion -- an explicit CCharString pushes the ctor's eax; by-value
  dialog strings built in their slots; `left / (initial * 2.0f)` folds to `fadd st,st`).
- CTCTurncoatSpell::SetPTarget (007c6940): `PTarget = CIntelligentPointer<T>(target)` through the inline operator=, bare
  GetTC after the presence test, def read into a named local just before the level call, vector `operator[]`.
- CTCUndeadSoul::FrameUpdate (0072c230): both arms written out, `T* p = 0; if (HasTC) p = GetTC;` for CTCTargeted.
- CTCUserActionPointBase::UpdateBlockedActionPoints (007fecb0): vector<long> stand-ins; the live owner fetched into a
  named CThing* before the virtual test and the blocked arm `continue`s (else GetPItem runs after the pushes).
- CTCInventoryTrade ctor (0059eb0c, labelled CTCVillage): init-list members in declaration order; the landed dtor's model.
- CTCVillage IsTimeBetween (00658620, the HHMM conversion written back into `duration`), ClearOutNullMembers (0065890b,
  `remove(0)`: the implicit conversion temporary is re-addressed with `lea`), OnCreate (00659164, three stand-ins),
  RemoveThingFromTask (00659efc, cases in source order 5,4,3,2,1,0), PlayerCanApologiseForCurrentCrimes (0065a686,
  the landed GetRegionLeavingDuration model plus a named refusal bool).
- CTCWife::SetAttractionIcon (006cfc90): named definitions local in the married arm, frame test as an inline bool helper,
  the clear arm's priority from a named bool (`married ? 3 : 4` gives setle/neg/sbb; the bare compare folds to setg+3).
- CTCWaterWader dtor (007b5ba0): `cleared(0); item = cleared.GetPItem(); member.SetPItem(item);` in sibling blocks.
- CTCWallMount::OnInitialActivate (007de5d0): an inline GetPhysicsTC() getter puts the component straight into ecx and
  fixes the whole register cascade.

## Parked for Fable 5.1 (11)
- 007da400 CTCTavernGameCardBlackjack::GetCurrentScore DIFFER(280v280): frame 0x14 v 0x10, the GetHandValue `fild` temporary gets its own slot (retail shares the GetText result slot) | tried: one expression, named local, assignment inside the expression, (float)/(double) casts, else-arm first | next: lane gap: temporary slot sharing
- 007da520 CTCTavernGameCardBlackjack::GetRequiredScore DIFFER(286v286): same frame-slot gap as 007da400 (dealer hand) | tried: the 007da400 forms | next: lane gap: temporary slot sharing
- 007da640 CTCTavernGameCardBlackjack::GetBet DIFFER(274v274): frame-slot permutation of text/inner/def/temp and `mov ecx,[esi+330h]` v retail `mov esi,[esi+330h]` | tried: ternary (two flagged temps), if/else (275v274), inline helper (not inlined), __forceinline helper returning either IntToWideString | next: declaration order / block scoping of text vs def; lane gap: frame-slot order
- 007d6150 CTCTavernGameCardPairs::EndRound DIFFER(262v262): scheduling only, def release `dec esi ; mov [eax],esi ; mov edx,esi` v ours copy-then-store | tried: ReleaseTemporary with/without the field clear, explicit count local | next: lane gap: release store/copy scheduling
- 007d4f40 CTCTavernGameCardSorting::EndRound DIFFER(295v295): the same release store/copy order; everything else exact | tried: the 007d6150 forms, explicit count pointer, unsigned count, holder local in the dtor | next: lane gap: release store/copy scheduling
- 008f1bd0 CTCTavernGameCardBase::UpdatePointerScale DIFFER(254v256): component load `mov eax,[eax+4] ; push ecx ; mov ecx,eax` v ours `push ecx ; mov ecx,[eax+4]` | tried: cast GetTC, named appearance local, named entry/component, out-parameter GetInterface | next: lane gap: lookup result merged into ecx across an argument push
- 008efa00 CTCTavernGameCoinBase::AddNewCoin DIFFER(250v250): frame 0x50 v 0x4c, the push_back overflow tag gets its own frame byte (retail homes it on the position argument slot) and `lea ecx,[eax+25h]` is scheduled after the pushes | tried: temporary coin through inline push_back + same-TU def lookup stand-in | next: lane gap: tag homed on an argument slot
- 007d2ef0 CTCTavernGameCoinGolf::SetupForNewRound DIFFER(242v242): `lea ecx,[eax+25h]` hoisted above the RHSet push in retail (RHSet through eax) and the zero-copy registers rotate | tried: plain expression, Coin inline GetMesh(), named held local | next: an inline helper shared with AddNewCoin placing the held coin's mesh; lane gap: member address computed before a temporary's GetPItem
- 007cf860 CTCTavernGameShoveHaPenny::CalcScores DIFFER(313v313): only the hoisted invariant `Radius + Pos.Y` x87 order (`fld [ebp+30h] ; fadd [ebp+8]` in retail) | tried: both operand orders, flipped compare, statement-order local, flat PosX/PosY | next: lane gap: single-term x87 commutation of a loop-invariant CSE
- 0076e150 CTCWeaponTrail::ClearAllWeaponTrails DIFFER(267v267): the first inline handle Reset loads P into eax then `mov ecx,eax` (ours ecx directly), loop filler `mov edi,edi` v `lea esp,[esp]` | tried: plain Reset, named handle pointer, local primitive in Reset, __forceinline Reset/GetRefCount | next: lane gap: entry register choice for the first inlined member call
- 00785b40 CTCVolumeContainmentTracker::FrameUpdate DIFFER(237v318): retail rounds both frame intervals with an inline-asm `fadd 0.5 ; fistp` helper (then `fild ; fsub 1.0` and an integer compare of the float bits) | tried: C model with an inline round helper | next: lane gap: asm_bake (inline-asm fistp rounding)

## Skipped
None (no row held a second function body).

## Label corrections
- 0x007CFF40 is CTCTavernGameShoveHaPenny::GetAdditionalInfo (vtable 0x0126F2F4 slot 46), not CoinGolf::GetAdditionalInfo.
- 0x007D5450 is CTCTavernGameCardSorting::GetRequiredScore (slot 45) and 0x007D6720 CTCTavernGameCardPairs::GetRequiredScore
  (slot 45), not GetBestScore; 0x007D5290/0x007D6560 are the slot-43 GetBestScore.
- 0x007DA400 / 0x007DA520 are CTCTavernGameCardBlackjack::GetCurrentScore / GetRequiredScore (vtable 0x0126F8F4 slots 44/45),
  not CoinGolf::GetRequiredScore.
- vfunc names: 0x007D5FB0 CardPairs::SelectCard (slot 59), 0x007D6150 CardPairs::EndRound (33), 0x007D3700 / 0x007D8160
  GetAdditionalInfo (46), 0x0071B380 CTCThingOwner::OnLevelUnload (15), 0x0072C230 CTCUndeadSoul::FrameUpdate (9).
- 0x0059EB0C is the CTCInventoryTrade constructor (vtables 0x01253034 / 0x0125301C), not CTCVillage::CTCVillage.
- CTCTavernGameCardBase +0x300 is GameType (PDB +0x324), not NumCards (NumCards is +0x2fc); the landed CardPairs ctor
  0x007D7520 stores `NumCards = 1` at +0x300 (it is GameType; the landed CardSorting NewRound 0x007D5A00 has NumCards at
  +0x2fc, and this chunk's GetBestScore/GetRequiredScore/EndRound test +0x300 against 1).
- Callees: 0x00405F00 is the out-of-line inline `__fastcall IntToWideString(long)` (label "CGuiControlNumber::SetSLongValue");
  0x0060C900 CTCInventoryMap::BuildMapTeleportUI takes `bool*` as its third argument (an out flag; the landed caller
  declares `bool keepSelection`); 0x007D1260 / 0x007D4BD0 / 0x007D9A80 / 0x00778900 / 0x008F00F0 are the typed GetSubDef
  for the ShoveHaPenny / CoinGolf / SpotTheAddition / TimeAppearanceFade / CoinBase defs (all labelled
  "<CAICreatureWillPowerIndicatorDef>"); 0x0065F440 GetSubDef<CVillageDef>; 0x00660AE2..0x00660D49 are the six
  RemoveThingFromTask<T> instantiations in member order Torch, Horn, Patrol, PickCrops, SecuritySweep, CarryCrate (all
  labelled "<CVillageTaskPickCrops>"); 0x0065DDDB..0x0065E09E list::remove instantiations (label
  "SetScriptBrushThingsToMove"); 0x005B2706/0x005B2724/0x004DF86C/0x005B2E6C/0x005B2747 STLport map default ctors
  (label "CDisplayManager::CopyBackBufferToTexture"); 0x0043DBD0 is IS_THING_ALIVE<const CThing*> (taking a converted
  temporary), labelled <CThingBuilding*>; 0x007F8860 (label "Unwind@00fbdab0") is the interface-0x82 component's entry clear.
