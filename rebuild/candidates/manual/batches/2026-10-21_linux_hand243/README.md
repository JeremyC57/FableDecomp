# Hand chunk 243 (CCreatureAction_Drink .. CDrawQuestInfo): 18 of 40 byte-exact

## What matched
- 008f3e70 DragonTakeOffIntoFlying::Initialise -- Dest at retail +0x120; hero CTCSolidToThing via `HasTC ? GetTC : AddTC`; `Dest = C3DVector(p.X, p.Y, p.Z + 35.148f)` as a body assignment.
- 008b6ee0 DuckAndCover::Initialise -- Direction +0x114 fast-rsqrt Normalise, `atan2(X,Y) * 0.159154f`, `fraction < 0.0f || fraction >= 1.0f` fmod wrap, physics slot 66.
- 00848db0 GetInBed::PerformAction -- inline CModeSleepBed ctor, counted pointer built in the argument area, bed via the ternary GetTC (component in ecx).
- 008c0d10 JackScream::Initialise -- CCountedPointer operator= / dtor inline, `PMode.m_pObject->SetSwordObject(...)`, ternary GetInterface.
- 008448e0 OpenChest::PerformAction -- three zero-preloaded lookups, each a named thing local in its own `{}`; CEventOpenedChest {novtable CEventBase, long 0, CIntelligentThingPointer}.
- 0084a4a0 PutDownJugFromFilling::PerformAction -- PickUpGoods model: `CThing* jug = PJug.GetPItem(); jug->m_pDef->GetSubDef(&carryDef)`; RemoveThingInCarrySlot(slot, true); RemoveMode(0x2b).
- 00845900 PlayerInteractionGreet::PerformAction -- two tested GetInterface(0x42) events (CEventTalkedTo 0x0084AF90 / CEventTalkedToAnyone 0x0084B100); AddMessage(0x14, 0x10, ..., 0) model.
- 008e6500 ReleaseStrengthBerserk::FrameUpdate -- four float locals, Clamp01, CRumbleParams(a, b, c, d, true), `GFGetSystemManager()->InputManager->GetJoystick(n)->Rumble(4, params, 0)`.
- 0069f4f0 SheatheItemToInventory::PerformAction -- ternary GetTC lookups; CTCCombat through the out-param GetInterface because of the `push 0`.
- 008cb780 SummonerSheatheWeapon::PerformAction -- by-value GetDef temp into a raw pointer; CDefPointer dtor via the pointee `Release()` long form.
- 0069fe60 UseObject::PerformAction -- CEventItemUsed vtable 0x125C820; `else PUsedThing.GetPItem();` dropped-result call; scripted hook early return.
- 006765a0 CCreatureDef::CCreatureDef -- novtable CDefClassBase over CResource; CRetailMap `_M_header(0)` then malloc; CWoundMorphs/CExpressionSet bool at +0xd; four `Reset(new ...)`.
- 00434e10 CDisplayEngine::CDisplayEngine (leaf ??0CDisplayEngine) -- CFadeInFadeOutBase as body assignments (Active, Duration, StartFrame, FadingOut, FadingIn, `Colour = CRGBColour(0,0,0,255)` with ctor body A,B,G,R); InitialFadeReadynessCount = 30, ScreenFadeDuration = -1.0f.
- 009b02e0 CDefinitionManager::~CDefinitionManager (manifest "Cleanup") -- vtable store, ClearInstantiatedDefs, every member dtor inline in reverse order: tree dtor (erase root, reset head, free), CArray / POD vector `free`, non-POD vector through `__fastcall __destroy_range(first, last, const __false_type&)` with a NAMED empty tag local, CCountedPointer dtor (dec, destroy+delete on zero, zero both words), CWideString vector loop with a CACHED end pointer; CResourceList dtor 0x9FC5C0.
- 00996390 CDataInputStream::ReadPresizedString -- inlined Read of the ReadPixelFormat family with a leading `size <= 0` guard and `size <= SourceChunkBytesRemaining`; the count comes from an inline `ReadLong()` helper (value stays in ebp); byte array = CByteArray(n) 0x411910 with the GetData "own address when empty" idiom; NAMED CCharString result (copy-ctor into the hidden pointer).
- 009967e0 CDataOutputStream::WriteZlibCompressed -- the placeholder and the final size both go through an inline `WriteLong(long)` helper (its by-value parameter is the frame temporary / the dead `bufferSize` slot); the clamp runs on the parameter itself; stack CCompressorZlib of the OpenMemorySource model; `bool finished` cleared before the do/while.
- 009d4ab0 CDefStringTable::ReadTable -- four inline ReadLong()s (first discarded), the helper's frame word lives in the dead argument slot; StringStream (+0xc) addressed once through a named pointer; global 0x013CA824 = PeekData() + 4.
- 0064401c NPlayerGui::CDrawMiniMap::Update (size TU) -- CRGBColour(0,0,0,255) with the B,G,R,A layout + user copy ctor built in the argument slot; literal 0.5f (0x0122F59C is a float-pool constant, not a global); the world pointer named inside the third `&&` term; FadeOut() inline helper (Opening = false; Closing = true).

## Parked for Fable 5.1
- 00847c90 CCreatureAction_Drink::FinishAction DIFFER(346v348): callee-saved pushes (ebx, edi) inside one branch with a merged tail | tried: early-return / nested if forms, named carrier local | next: lane gap: push-in-branch with merged tail
- 00844740 CCreatureAction_DropGift::PerformAction DIFFER(342v350): retail multiplies sin(angle) by 0.25f twice (`fld c ; fmul st(1) ; fstp X ; fmul c ; fstp Y`), ours CSEs | tried: named double, two temps, separate statements | next: lane gap: refused x87 CSE
- 008b6ad0 CCreatureAction_MidAttackSide::Initialise DIFFER(342v344): x87 product operand order for the three normalised components | tried: operand swaps, named length local | next: lane gap: x87 product order
- 008ca310 CCreatureAction_NymphGetHit::PerformAction DIFFER(386v387): an extra frame word for the copied def pointer and esi/edi roles swapped | tried: raw pointer / CDefPointer copy, scope moves | next: lane gap: register-role rotation
- 0084a350 CCreatureAction_PickUpJugToFill::PerformAction DIFFER(321v321): register rotation only (eax vs edx key temporaries) | tried: named slot local, reordered lookups | next: lane gap: register-role rotation
- 00843310 CCreatureAction_PerformExpression(Extended) ctor DIFFER(335v347): GetString temporary homed in the dead `def` argument slot | tried: scoped temp, inline helper param | next: lane gap: temporary over a dead argument slot
- 0096dbc0 CCreatureAction_ScreamerIntoAggressive::Initialise DIFFER(321v331): `this` in ebp / map in esi / &PTarget in ebx plus a second spill word | tried: local declaration orders | next: lane gap: register-role rotation + spill
- 0069fce0 CCreatureAction_UseObject::FrameUpdate DIFFER(380v369): `this` esi/edi swap | tried: declaration orders, named pointer local | next: lane gap: register-role rotation
- 008c4cb0 CCreatureAction_WaspQueenDeath::Initialise DIFFER(355v364): intelligent-pointer temporary overlaid on the dead effect-type word + folded release null test | tried: scoped temp, operator= forms | next: lane gap: temporary over a dead local
- 00e03d30 CDarkwoodRockTroll::Main DIFFER(335v335): vtable fetch order of nested virtual calls | tried: named temporaries for the inner call | next: lane gap: vtable-fetch scheduling
- 009b0470 CDefinitionManager::CDefinitionManager DIFFER(370v370): pure scheduling -- the ErrorCallback arg load and the `lea edi/ebp` pair sit a few stores later in retail | tried: UseSafeBinaryFormat in the init list / body, body-assignment vector and counted-pointer ctors, const-ref arg | next: lane gap: store/load scheduling inside a long zero-store run
- 0064be45 CDrawMeleeActionButtonEffects::Update DIFFER(338v373): two CDefPointer temporaries share one frame slot and VC merges the two Adopt arms; retail keeps three frame words and a register zero | tried: named temps, full-expression temps, inline IsAlive param | next: three-slot temporary allocation (lane gap: temporary slot assignment)
- 008d5300 CCreatureAction_ReloadMissileWeapon::PerformAction DIFFER(379v383): frame and every call identical; VC hoists &PCreature into ebp at entry where retail spills `this` and re-forms this+8 per GetPItem (ebp/edi pushed inside the branch) | tried: C3DVector local removal (found: the empty CCharString IS the 4th argument of 0x4C73D0), def-word declaration order, inlined CreateAmmo member helper | next: lane gap: register-role rotation / push-in-branch
- 009adc90 CDefinitionManager::InstantiateDef DIFFER(367v382): retail spills the class-info pointer and keeps both counts in ebx/edi before the factory call | tried: early return vs single exit, bitfield Recompiled flag | next: spill order of the found info (lane gap: spill scheduling)
- 006c7370 CDeedManager::TransferState DIFFER(332v342): this/context esi-edi rotation and an 8-byte wider frame | tried: layout fix (tree is 12 bytes, NextHandle +0x1c) | next: the flag byte and record vector positions (lane gap: register-role rotation)
- 006ea0c0 CDefendingCombatantInfo::InitializeDefenderFromCreature DIFFER(379v360): best effort inline map walk | tried: one form | next: model the attack-pattern lower_bound through the landed CRetailTree templates
- 006ecea0 CDefendingCombatantInfo::UpdateCombatState DIFFER(390v385): best effort | tried: one form | next: the local map value type and the wheel-centre virtual call signature
- 00ca15c0 "CDefinitionManager::ProcessPropertyDefinition" DIFFER(429v399): a free `__fastcall` property/matrix walker, label wrong | tried: one form | next: identify the table/context structs (lane gap: unknown types)
- 00645099 CDrawAbilityButtons::DrawButton DIFFER(343v324): x87 compare chain + __ftol2 + inferred sprite ctor | tried: one form | next: lane gap: fistp/ftol rounding + unknown CEnginePrimitive2DSprite ctor
- 00646580 CDrawMeleeActionButtonEffects::Draw DIFFER(425v393): inferred emitter/sprite ctor signatures | tried: one form | next: identify Create2DEmitter / sprite ctor argument lists
- 00646010 CCrimeStatBar::DrawStatBarOnLeft DIFFER(366v399): parked at once | tried: best-effort only | next: lane gap: fistp/fild frame-slot rounding helper over a dead argument slot
- 0064d404 CDrawQuestInfo::DrawMiniGameInfo DIFFER(129v399): parked at once (sprite tail behind a stand-in helper) | tried: best-effort only | next: lane gap: fistp/fild rounding helper

## Skipped
- none (every row is a single body).

## Label corrections
- 0x00843310 is the CCreatureAction_PerformExpressionExtended constructor (vtable 0x012744FC, `ret 0x20`), not CCreatureAction_PerformExpression.
- 0x009B02E0 is `CDefinitionManager::~CDefinitionManager` (vtable store + reverse member destruction + ~CResourceList 0x009FC5C0), not "Cleanup"; 0x009FC5C0 is the CResourceList destructor.
- 0x00CA15C0 is not a CDefinitionManager member: a free `__fastcall` property-table/matrix walker (manager in ecx, context in edx, three stack words, `ret 0xc`).
- 0x0048DB13 is a CWorld bool query (called on CDrawBase::World), not CPersistContext::IsBinary (propagated).
- 0x00733290 is a CTCCombat bool query, not CThingFilter_IsUnconscious::operator() (propagated).
- 0x004C73D0 has a 4-argument overload used by 0x008D5300: (long player, long, long, const CCharString& name) returning a position pointer; the manifest "GetPosition" label is the no-argument form.
- 0x00411910 is the zero-filling `vector<unsigned char>(n)` size constructor (as 0x00514E20), not a class-specific "Buffer_AllocateAndZero".
- 0x009B8070 is STLport `__destroy_range<CDefClassInfo>` (`__fastcall` first/last + tag reference), not a CDefinitionManager method.
- 0x0122F59C is a float-pool constant 0.5f (used as a literal by 0x0064401C and 0x00646010), not a named global.
- Propagated labels seen again: 0x449970 (GetMainPlayer), 0x487DD0 (GetControlledCreature), 0x49D870 (GFGetCurrentFrame), 0x4365B0 (const LowerBound), 0x996BC0 / 0x996970 (CMemoryDataOutputStream size / read-from-stream thunks; manifest "_Cons_val").
