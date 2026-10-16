# Hand chunk 171 (CTCTavernGameCardPairs .. CTCWeapon): 30 of 40 byte-exact

## What matched (30)
- CTCTavernGameCardPairs 007d7520 ctor: card-base ctor + vtable, TargetTime = PrizeScores.back().Score (`end[-8]`), three body stores; CDefPointer dtor calls the def's inline ReleaseTemporary() with no null test.
- CTCTavernGameCardSorting 007d4e30 EnterGame: NumCards = 1 before the def temporary; offset C3DVector(0,0,h) temporary into the __fastcall PositionMesh; null-tested ReleaseTemporary dtor.
- CTCTavernGameCardSorting 007d4ed0 NewGame (slot 31): `size()` of 72-byte CardOrders; same dtor.
- CTCTavernGameCardSorting 007d5ad0 ctor: `Times[i] = (float)((i+1)*2)` strength-reduced with the counter in the dead owner slot; no-null-test release.
- CTCTavernGameCardBase 008f09e0 slot 53 (TurnAllCards, descriptive) and 008f1250 OnInitialActivate: sound name as a named CCharString local in a `{}` block; `T* p = 0; GetInterface(&p, 46)`; by-value CCharString built in its arg slot.
- CTCTavernGameCoinBase 008ef2f0 UpdateMeshes: the mesh's GetPItem taken into a named local before the call (fixes both call order and the loop's strength-reduced registers).
- CTCTavernGameShoveHaPenny 007cf740 / CTCTavernGameCoinGolf 007d2c20 GetRequiredScore: one expression `GetText(id) + GFIntToCharString(n).ToWideString()`.
- CTCTavernGameShoveHaPenny 007d09c0 / CTCTavernGameCoinGolf 007d49e0 ctors: inline novtable CTCTavernGameCoinBase ctor, Held in the init list, Arrow a body assignment.
- CTCTavernGameCoinGolf 007d34a0 EnterGame (slot 29), CTCTavernGameShoveHaPenny 007cf530 EnterGame: zero-initialised camera out pointer tested; camera name as a named CCharString in its own block.
- CTCTavernGameSpotTheAddition 007d8da0 ctor (vector member after vtable store, body scalars in retail order) and 007d7ba0 vfunc_33 (uninitialised stats out pointer, `GameWon == true`).
- CTCTrapBase 007888b4 FrameUpdate (size TU): ParentThing as a named local read before GetWorld; erase loop advancing only on the kept arm; tail-jumped slot 29.
- CTCTrespasser 007bae70 IsCurrentlyTrespassing: HasTC+GetTC for the first lookup, the out-pointer GetInterface for the second (a returning GetTC loads the component into edx).
- CTCWife 006cebf0 / CTCTrophy 006dd760 SendPauseEvent (/GS): flag normalised in the `pause` arg slot through an inline AddBool(bool&).
- CTCTrophy 006dda50 EndMinigame: implicit CCharString conversion temp; by-value CWideString args built in place (wchar_t is `unsigned short` without /Zc:wchar_t).
- CTCTurncoat 007c78c0 ctor: init list in declaration order, CIntelligentPointer(0) built by SetPItem, body WillTurncoatTimer = 1 after GetSubDef, ftol / fdivr against g_framesPerSecond.
- CTCVillage (size TU) 00658f41, 006592e2, 0065a1fd, 0065a852, 0065ac0e, 0065b0b4, 0065b2c4: `T* p = 0` declared before the GetPItem it precedes; inline map `find(&key, key)` returning `last`; uninitialised GetInterface out local sharing the key slot; IS_THING_ALIVE bound through a derived-to-base temporary (keeps the thing in esi); GetWorld into a named local before the argument set-up; flag through a `bool&` local.
- CTCVillageMember 007ee4d0 vfunc_14: uninitialised GetInterface out local, call on both arms.
- CTCWallMount 007de530 OnSerialise: derive from a 0xc-byte CTCBase so the vptr does not pad to 8 before the __int64 TrophyID.

## Parked for Fable 5.1 (9)
- 008ef390 CTCTavernGameCoinBase::EnterGame DIFFER(108v105): ours keeps the def in ebx and CSEs &def->ArrowDefIndex into edi (extra push ebx); out-slot zero before the push | tried: member-form dtor, holder copy, plain local + hand release, holder + ctor zero | next: def accessor through a non-inline this-call wrapper to move the zero store
- 007cf7e0 CTCTavernGameShoveHaPenny::NewRound DIFFER(127v127): out-slot zero before `mov ecx,[ecx+70h] ; push eax`; start vector Y copied through edx after X (retail fld Y first) | tried: CDefPointer with/without dtor, direct holder call, `0.5f*x`, C3DVector copy ctor, body-assigned ctor | next: lane gap: inline def-accessor zero placement
- 007d80c0 CTCTavernGameSpotTheAddition::NewGame DIFFER(122v115): accessor zero store before the push; release reloads/retests instead of `mov ecx,[esp+8] ; cmp ecx,ebx ; dec [ecx+4]` | tried: member-form, local-copy and Release() dtors, bool accessor, named def-class local (fixes the zero placement but rotates eax/edx) | next: named def-class local + a dtor that copies Object once
- 007b8680 CTCTavernTable::GetTankardName DIFFER(156v144): frame 0xc v 8, the dead GetSubDef out slot is not reused by the village-arm CCharString temporary; HQ byte via al | tried: member dtor, holder, forceinline pointer helper, helper returning the holder | next: ?: arms inside a __forceinline helper taking the def by value
- 007dfe20 CTCTeleporter::~CTCTeleporter DIFFER(141v141): this/&PParticle in esi/edi swapped | tried: pointee Release() dtor (fixed the release), named reference local | next: lane gap: register assignment
- 007bb150 CTCTrespasser::SeeIfAnyOccupantsKnowAboutDeed DIFFER(147v145): component loaded into eax then `mov ecx,eax` (retail straight into ecx) | tried: GetInterface out form, HasTC + GetTC expression | next: lane gap: looked-up component used as `this`
- 00658ec3 CTCVillage::BuildingContainsCorrectVillagers DIFFER(126v126): &villager / home in edi/ebx swapped | tried: template Get(), typed GetPItem, unrelated pointer type, inline GetHome() (fixed the lea) | next: lane gap: register assignment
- 0065bffa CTCVillage::IsReportedOrUnreportedCrimeKnown DIFFER(146v144): component loaded into eax then `mov ecx,eax` before FindMostRecentDeed | tried: named local, GetTC as `this` with the filter first, GetInterface form | next: lane gap: looked-up component used as `this`
- 005da6e0 CTCWeapon::OnCreate DIFFER(99v99): `mov ecx,[esp+0Ch] ... mov ebx,ecx` where retail loads ebx first | tried: holder, plain local + hand release, call through out.Object | next: lane gap: register rotation

## Skipped
- 00c8a4b0 "CTCVillageMember::IsAttachedToVillage": a library/LTCG body that receives its object in esi (never set inside; callers at 0xC8A6BD/0xC8B4B2/0xC8B670 preload it), returns HRESULT-style 0/0xFFFFFF7F/0xFFFFFF77; not reproducible without a register-parameter convention, and the label is a propagation.

## Label corrections
- 007d09c0 is CTCTavernGameShoveHaPenny::CTCTavernGameShoveHaPenny (vtable 0x0126F2F4), not CoinGolf.
- 007cf740 is CTCTavernGameShoveHaPenny::GetRequiredScore (vtable 0x0126F2F4 slot 45), byte twin of CoinGolf's 007d2c20.
- 008f09e0 (slot 53) and 008f1250 (slot 6, OnInitialActivate) are CTCTavernGameCardBase's, inherited by CardSorting (CardPairs overrides 53, shares 6).
- 006cebf0 is CTCWife::SendPauseEvent (called only from CTCWife::GlobalUpdate), byte twin of CTCTrophy's 006dd760.
- Callee labels: 0x00686A70/0x00686A80 GetWorld (not CViewBase::GetDisplayEngine), 0x00686D40 GetPlayerManager, 0x0049D9D0 CWorld::GetCurrentPlayer, 0x0065F01D is a component method (not CTCVillage::InitialiseSecuritySweeps), 0x00789085 vector erase, 0x004292C0 _Rb_global::_M_increment, 0x00661FF0 / 0x007E2780 / 0x008EF180 / 0x005E9AA0 propagated labels.
