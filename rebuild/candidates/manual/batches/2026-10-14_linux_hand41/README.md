# 2026-10-14_linux_hand41 — agent-authored chunk 41 of the class-labelled 17–96-byte pool

Chunk 41 (`mkchunk.py 41`, 40 rows, CGameCameraManager .. CGameScriptInterface) was authored by the session's single
sub-agent from `lane_tools/chunk_pool/BRIEF.md`; the lander re-graded every row: **35 of 40** landed as MATCH/RELOCATION_MATCH
with behaviour tests (compiled with the lane's cl.exe). Parked sources stay in the lane's `differ/`. The agent's notes follow.


Result: **35 of 40** graded MATCH/RELOCATION_MATCH on the Linux VC7.1 lane (q.py), every one with a behaviour test that
PASSes under the lane's cl.exe (`exp/runtests.py` = `verify_and_land.behaviour_of`) and g++. 5 parked in `differ/`.

## Label corrections (manifest labels propagated over byte-identical shapes)
- 0x00711FA0 "CGameJoystickManager::CGameJoystickManager" is `CTCScriptedControl::CActionBase::CActionBase` (vtable 0x01265504).
- 0x00903660 (same label) is `NTCScriptedControl::CActionStopScripting::CActionStopScripting` (vtable 0x0128D788).
- 0x00905130 (same label) is `NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::ctor(const CScriptThing&)` (vtable 0x0128D7F0).
- 0x00855B50 (same label) is `CCreatureAction_BlockRespond::ctor(CThingCreatureBase*, CThingCreatureBase*, long)` (vtable 0x01276084, action id 0x96).
- 0x00B1B350 (same label) is `NParticleEngine::CPSCSpline::CPSCSpline` (vtable 0x012A0934).
- 0x00C77440 (same label) is a no-RTTI class: `CVtable_012bf0c8::CVtable_012bf0c8` (base vtable 0x012BF050 via 0x00C74420).
- 0x00A0D300 "CGameEventPackage::CGameEventPackage" is `CProcessedInput::CProcessedInput` (0xb0 bytes: Player, Type, CGameEvent[4], NoGameEvents, Priority) — parked, see below.
- 0x0069CEF0 "SetProjectileWeaponFired" is the bool/out-pointer twin of 0x00698470: named `GetCurrentModeIfProjectileWeapon(CBase** out)`.
- 0x00A9BA10 / 0x00A9BA40 "CGameCameraManager::HasCameraMode" are a by-value CCountedPointer<const CIPhysicsFacesSegmentBase> factory pair
  in the C3DMeshFile loader module (members +0x88 / +0x98, helper 0x00A9AD00): attributed to `C3DMeshFileLoader` (provisional).
- 0x00ED43D0 "CGameCameraManager::HasCameraMode" is a quest-script `GetTimeOfDayFraction` ((HHMM%100)/60 + HHMM/100)/24 via interface slot 647;
  attributed to `CV_StatueMasterScript` (neighbour 0x00ED4420) — provisional.
- 0x0055C600 "CGameScriptInterface::ClearGossip" is `CMouseCursor::GetScaledSize` (CSprite::GetSize scaled by CFrontEndManager UI scale) — provisional class.
- 0x00628E80 "CGameEvent::AddUBYTE" is a static `__fastcall` 8-byte reader (`ReadData8`, edx:eax return) over the buffered reader 0x009F18F0.
- 0x008200B0 / 0x00820140 "CGameScriptInterface::MsgIsActionModeButtonPressed" are `CMap::GetThemeMin/MaxCameraHeightAtPosition(const C2DVector&)`
  (0x0081FA20 / 0x0081FA90 + InterpolateHeightAtPosition 0x0081EA50).
- 0x007D7B80 / 0x007F81E0 / 0x0082FE00 "CGameScriptInterface::EndCutFade" are three unrelated two-call forwarders; attributed to
  CTCTavernGame / CTCInterface / CThingCreature from their callees' labels — provisional, names kept.
- 0x006E73E0 (second "FreezeControls") calls 0x00449A50 instead of 0x00449A30: named `UnfreezeControls` (inferred twin).
- 0x00449970 "Catch@0040d959" is `CPlayerManager::GetMainPlayer` (`GetPlayer(MainPlayer)`); 0x00487DD0 is `CPlayer::GetControlledCreature`.
- Retail `CGameCameraManager` is 0x18 bytes narrower than the PDB from CamTools on (CTools 0x20, 12-byte vectors, 4-byte STLport list):
  CurrentMode +0x98, PendingMode +0xa0, flags +0xa8..+0xaa, UsingCameraManually +0x12c, PCameraManagerSet +0x130, PDef +0x134.
- Retail `CGameScriptInterface::ScriptTimers` (+0x3c) is an 8-byte STLport tree {header*, count}, not the PDB's 16-byte std::map.

## Lessons
- A `ret` + edx:eax class return with `this` in ecx is a static `__fastcall` (VC7.1 returns classes from every non-static member through the hidden pointer).
- `cmp byte ptr [eax],dl ; je ; mov [eax],dl` inside a counted pointer walk is an inline element constructor `if (Valid) Valid = false` (array construction loop).
- The remainder next to a reciprocal-multiply quotient is `t - q * 100` (writing `%` makes VC emit one `idiv` for both).
- A base constructor argument that retail re-addresses with `lea` is an implicit conversion (`CDefinitionManager(L"game.bin", cb)`), not an explicit `CWideString(...)` temporary.
- The by-value counted-pointer parameter of a __thiscall is destroyed by the callee itself at its end (flag store scheduled between the null test and the `dec`).
- A member read whose load retail schedules before the frame (`mov eax,[ecx+98h] ; sub esp`) is a named local at the top of the body.
- An out-pointer local that retail addresses as `lea edx,[esp+28h]` (the argument slot) is an uninitialised pointer local VC homes in the dead argument slot — declare it as a plain local.
- `push 2Fh` before a `GetMainPlayer` call is the argument of the following `HasMode` (VC evaluates arguments before the object expression).
- A VC bitfield assignment is `mov cl,[flags] ; xor cl,arg ; and cl,MASK ; xor [flags],cl`; `sete ; shl` before it is an inverted bit (`bit = !flag`).
- A float call result parked in the dead argument slot across a second call is a named float local (`float offset = ...; return offset + Interp(pos);`).

## Lane gaps (parked, sources in differ/)
- 0x00A0D300 and 0x006B74E0: the lane compiler (DDK 13.10.4035) unrolls constant-trip loops of 2 and 4 that retail keeps rolled
  (both VC's own member-array construction loop and a source loop); `/O1`/optimize("s") keeps them rolled but changes the exit test
  to `jl`, merges tails and turns the array into a `??_L` call with `or/and` zero stores; optimize("t",off) still unrolls (probe: `exp/probe.py`).
- 0x006B1900: a stack CDefPointer temporary's release — retail folds to `dec [ecx+4] ; jne ; call [edx+4]` with `pop esi` hoisted above it;
  the lane keeps the decremented count in esi (`mov esi,[eax+4] ; add eax,4 ; dec esi ; mov edx,esi ; test ; mov [eax],esi`) for every
  release form tried (destructor with/without the dead null store, local pointer copy, Release() on the pointee, explicit end-of-block call).
- 0x006E7690: only the order of the two loads before the `it != end()` compare (retail iterator slot first, then the header).
- 0x0044C6C2: only the placement of `push 80000h ; mov ecx,esi` (retail between the last two zero stores; body/init-list/member-object forms tried).
