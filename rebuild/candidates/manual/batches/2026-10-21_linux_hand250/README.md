# Hand chunk 250 (CStreamingFontBank .. CTCContainerRewardHero): 28 of 40 byte-exact

## What matched (28)
- CStreamingFontBank::GetStringExtents 0x009A14E0 -- wide-string extents; the maxima are one C2DExtentsF local declared before the line height, the loop walks the parameter (`while (*str) { c = *str++; }`), `Advance * (Scale * 0.01f)`, both exits through the two-float ctor. Leaf `GetStringExtents` (lane has no native wchar_t).
- NCameraModes::CStringAndCageBase::Update 0x008728C0 -- C3DVector memberwise user copy ctor; observed thing as a named local so GetObservedThing precedes the focal-point pushes.
- CSubtitleRenderer::CSubtitleRenderer 0x0049166E -- size TU (`#pragma optimize("s", on)`, odd address); STLport `_Vector_base`/`_STLP_alloc_proxy` empty vectors.
- vector<CActionInputControl>::operator= 0x00719540 (label CSubtitledMessage::Assign_CActionInputControl) -- STLport operator= with same-TU copy-helper stand-ins (landed 0x006F3C20 model), no _Destroy (trivial element).
- CSummoner1::Main 0x00D7ECB0 / CSummoner2::Main 0x00D7EED0 -- `while (!(hit || (anySpecial && !special14)))` with conditional CCharString temporaries; the twin stores PlayerHitSummoner2.
- CSummonerMinion::Main 0x00DF2AB0 -- the landed entity-script wait-loop shape throughout.
- CSwordChest::Main 0x00EF2290 -- CScriptThing's forwarding IsBeingCarriedBy needs `__forceinline virtual`; ~CIsThingAlive out of line in this TU.
- vector<SSymbolEntry>::_M_fill_insert 0x0052C300 (label CSymbolMap::Insert) -- STLport text with four auto_inline(off) helper stand-ins.
- CSymbolMap::ParseBuffer 0x009E5AD0 -- CStringParser built by an inline ctor calling Init (explicit `CCharString("")` temporary first); an unused 12-byte token local explains two dead frame words.
- CSystemManager::InitWindow 0x009A64B0 -- Win32 imports; the doubled border height inline in the y expression.
- CTB_Bard::GetRandomSpeech 0x00ED99D0 -- `found = true; break;` out of the retry loop, found block after it.
- CInstance_00c388a0::CheckFrameStreamer 0x00C388A0 (/EHsc; label CTCActivationReceptor::OnStateTransitionCheck) -- landed AI-library assert-stream model.
- CTCBattleCharge::UpdateDirectionOverride 0x007A7000 / CTCAssassinRush::UpdateDirectionOverride 0x007A9960 -- CInputEvent is 0x34 bytes; `sqrt(Y*Y + X*X)`; members stored one by one; angle as a named float.
- CTCAssassinRush::OnInitialActivate 0x007AA6B0 -- out-parameter GetInterface for the mode manager, ternary GetTC into a named local before the zeroed vector, cached end.
- CTCBattleCharge::CleanUpEffects 0x007A74A0 -- one function-scope emitter local for both GetInterface calls.
- CTCBerserk::CreateStartingEffect 0x0078C4C0 (advisor) / CancelMove 0x0078C610 -- one TU model: same-TU stand-ins for GetSubDef and the effects def's GetEffect; named berserk-def local; `max * (t / released)`; IS_THING_ALIVE `__forceinline`.
- CTCBoss::SetHelperCreature 0x007AF310 -- STLport `operator[]` as `*(begin() + n)`; `creature == v[i].GetPItem()`; placement-new push_back.
- CTCBuyableHouse::SetupDressingAtLevel 0x006C0330 -- named list-head local; CCharString inline GetPString/Length.
- CTCBuyableHouse::OnSerialise 0x006C0810 -- defaults in dead argument slots; NAMED bool for the mode test (coordinator hint).
- CTCBuyableHouse::Evict 0x006C0FD0 -- copy_if into back_inserter (non-POD inserter → hidden result); named 0x23 component local.
- CTCCarriedActionUseGiveBeer::vfunc_28 0x007ED2C0 / CTCCarriedActionUseShow::vfunc_28 0x007ED870 -- landed CTCCarriedActionUseGive model; `__forceinline` HasTC; named drink local; GetSubDef stand-in + GetInterface for the stats.
- CTCCarrying::RemoveThingFromPrimarySlot 0x006A1E00 -- whole body inside `if (!IsPrimarySlotEmpty())`.
- CTCChest::Open 0x00750540 -- GetSubDef stand-in; opener flags through an inline COpenCloseHelper member; one GetPItem for the emitter.
- CTCCombat::UpdateCombatTimers 0x007339E0 -- STLport inline `_M_increment`; ability data pointer read at the top of each pass.

## Parked for Fable 5.1 (11)
- 00760a70 NShape::CSubdivisionTriangle::GetUpDirection DIFFER(332v332): `pop esi` two x87 instructions later than retail | tried: corner copy forms, iterator, dot/cross variants, flip forms, /G5 /G7 /Oa /GS, QFE | next: lane gap: x87 epilogue scheduling
- 00493281 CSubtitleRenderer::ClickPast DIFFER(370v357): return-false epilogue placed last (retail after the TextCharactersShowing block) | tried: if/else, goto next/clear, single return-false, result local, inverted top test | next: named-bool conditions (chunk 247 lesson) on the DisplayingText test
- 007ee8f0 CTCActionUseChest::OnActionUse (vfunc_28) DIFFER(346v346): eax/ecx/edx rotation in the PostDeed/morality tail | tried: ternary vs if GetTC, unnamed log, operand order, QFE | next: named ParentThing local before the PostDeed pushes
- 00783a30 CTCBoastingCrowdControl::GetCrowd DIFFER(393v393): insert tail loads the UID into ecx/edx instead of eax/esi | tried: entry ctor forms (by value, ref, member, swapped args, make-function) | next: real STLport pair/map insert model
- 006bffd0 CTCBuyableHouse::GetValueOfTrophies DIFFER(353v328): value spilled to the frame, out-def re-read after dec | tried: holder, scoped copy, GetDef by value, inline helper | next: lane gap: CDefPointer release re-read (needs GetSubDef<CTrophyDef> visibility)
- 006c0580 CTCBuyableHouse::DestroyMoneyBags DIFFER(329v329): one extra key slot (frame 10h vs 0Ch) | tried: GetInterface/HasTC+GetTC mixes, named bag/stats, all-GetInterface | next: key-slot sharing form
- 006c0b20 CTCBuyableHouse::FindPosToCreateNewVillager DIFFER(383v368): vector start cached in ebx in retail | tried: GetAllThings stand-in, named thing manager, filter order | next: begin() local + explicit end
- 0075cad0 CTCCameraPointTrack::UpdateActiveCameraPosition DIFFER(388v388): `fld Y ; fmul st(2)` vs ours `fld st(1) ; fmul Y` | tried: operator* forms, ref, reciprocal forms | next: lane gap: x87 operand order
- 007ed560 CTCCarriedActionUseGoldBag::vfunc_28 DIFFER(354v354): first (interface 8) key not shared (frame BCh vs B8h) | tried: key order, GetInterface for targeting/stats, key as param | next: key-slot sharing form
- 0074e560 CTCContainerRewardHero::CreateNewObject DIFFER(334v330): 0x103 key homed in the `position` arg slot (retail: `scriptName` slot) | tried: GetInterface, named string, pointer params | next: lane gap: dead-argument slot choice
- 009e5c90 CSymbolMap::LoadFromFile DIFFER(373v373): `[eax+esi]` vs `[esi+eax]` for buffer[size] | tried: all index forms, GetSize(), alias local, QFE | next: lane gap: base/index operand order (same as 009bb0d0, advisor-confirmed)

## Skipped
- 0064dd9a CTCActionUseScriptedHook::PopulateRegionExits: the catalog row (321 bytes) truncates the function -- it ends in `jmp 0x64dfbe` into the rest of the body past the row end, so no single compiled function can match the row.

## Label corrections
- 0x00D7EED0 is CSummoner2::Main (stores PlayerHitSummoner2, CSummoner2 vtable 0x012D3ED0), not CSummoner1::Main.
- 0x007A7000 is CTCBattleCharge::UpdateDirectionOverride (CTCBattleCharge fields +0x2E/+0x2F, called from CTCBattleCharge slot 6), not CTCAssassinRush.
- 0x00719540 is vector<CActionInputControl>::operator= (catalogued as CSubtitledMessage::Assign_CActionInputControl).
- 0x0052C300 is vector<SSymbolEntry>::_M_fill_insert (catalogued as CSymbolMap::Insert).
- 0x00C388A0 is AI-library ".\CInstance.cpp" code (catalogued as CTCActivationReceptor::OnStateTransitionCheck).
- 0x007EE8F0 is CTCActionUseChest::OnActionUse; the vfunc_28 rows 0x007ED2C0/0x007ED560/0x007ED870 are the carried-action OnActionUse.
- Callee labels: 0x00487510 is CPlayer's player-number accessor (+0x228), 0x0099AF00 is the CWideString character-pointer conversion, 0x00494168 the CSubtitleRenderer text-info queue ctor, 0x00436560 a CFadeInFadeOutBase-derived timer ctor, 0x009B9BA0 CStringParser's keyword search, 0x007ED780 the show-item action ctor, 0x006A1270 CTCCarrying's primary-slot empty test, 0x007338A0 a CTCCombat member (block-hold expiry), 0x00628EE0/0x007ED420 action destructors.
