# Hand chunk 87 (CTCElectrocutedEffect .. CTCForcePushed): 36 of 40 byte-exact

All 36 matched sources re-graded together by gall.sh (grades.txt: 7 MATCH, 29 RELOCATION_MATCH); every test prints
`<addr>_TEST PASS` under g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). Only CTCBase, CTCPerceiveThing and
CTCDoor have engine headers among the classes touched; the other CTC classes are modelled over `engine/CTCBase.h`
with the PDB member names (or locally where a polymorphic base is needed, see LESSONS).

## What matched (36)
- **Audio-library profile-lock bodies (labelled CTCElectrocutedEffect::OnInitialActivate, 5 rows; propagated label)** --
  00c0aa10 GetElementCount (returns 16), 00c0ac00 GetSecondTableCount (returns 8), 00c11150 GetValue (float at +0x7c0),
  00c15f10 SetUnused(long) (stub), 00c1e4d0 GetShortValue (unsigned short at +0x14 of the object at +4). All take the
  8-byte `CProfileLock` on the section @ 0x0143BB14 around the read. Shape: a float result is copied as a dword into a
  frame slot before the lock's destructor and `fld`ed after it; an `unsigned short` result rides in esi (`movzx esi`).
  Classes named by address (no RTTI).
- **CTCEmoteIcon (5)** -- 0071aa50 SetActiveTimer (-1 forever / 0 off / else SecondsToFrames through the 0x00686D60
  accessor, whose result is discarded: a static `__stdcall` helper called through an object), 0071aab0 SetEmoteIcon
  (signed priority early-out, five stack args; clearing arm pushes 0.0f as `push 0`), 0071ac20 SetAsDrawable,
  0071af30 FrameUpdate (vtable slot 9), 0071b040 HandleMessage. The three handle-release bodies share the
  CEnginePrimitiveHandle model: `if (Data.IsUnique()) Data->OnLastHandleReleased(this); Data = 0;` where `= 0` is the
  counted release plus a second unconditional zero store (two `mov [esi+4],0`).
- **CTCExplodingObject (4)** -- 00750b80 OnCreate (slot 4; bool member guard, `call ; ret` kept), 00750ba0
  SetCountdownTimer (const-reference `Min` template: the address of the member or of the argument slot is selected,
  then dereferenced), 00750cd0 ~CTCExplodingObject (leaf `??1`; kill the proximity particle if alive, out-of-line
  intelligent-pointer dtor, inline def-pointer release), 00750dd0 Explode (def read into a local before the position
  call; the inlined GetTCDriver out-param frame word is read uninitialised on the absent path).
- **CTCFadeOutAndIn (3) + relatives (2)** -- 0049d310 IsFading and 00661600 CTCPerceiveThing::CanSeeOrSmellPerceivedThing
  (`return a || b;` gives full-register 0/1 exits), 0082cf40 CTCDoor::IsClosed (`!a && !b && !c`, shared `xor eax,eax`
  exit last), 006c3b50 FinalCleanup (slot 2; the virtual FrameUpdate on `this` as the last statement becomes
  `jmp [eax+24h]`; polymorphic base modelled locally because engine/CTCBase.h is flat).
- **CTCFineDialogue (1)** -- 007e2790 IsAllowed: early `return false`s share one `xor al,al`; the closing
  `if (mode == 0 || mode == 9) return true; return false;` keeps byte exits.
- **CTCFireRangedWeaponOnLeavingStateGroup (1)** -- 007e5cc0 FinalCleanup (slot 2): ParentThing in a local, unregister,
  then the creature-side helper as a tail `jmp`.
- **CTCFireballSpell (1)** -- 007c8660 SetWeaponState: 3-case switch (`sub eax,0 ; je / dec ; je / dec ; jne`), each
  case's CCharString assignment a tail jump that reuses the argument slot for the source address.
- **CTCFishing (1)** -- 007b1eb0 GoFishing: getter chain, three early exits, `push 0 ; push 1Dh` before the
  `GetPlayer()` object call (arguments before the object expression), inline SetStage.
- **CTCFishingRod row / CTCAISummon (1)** -- 004df2d4 is really CTCAISummon::CTCAISummon (vtable 0x01241C8C), matched
  only under `#pragma optimize("y", off)` + `optimize("s", on)` (frame-pointer size TU), leaf `??0CTCAISummon`.
- **CTCFishingRod (1)** -- 007e2450 constructor (leaf `??0CTCFishingRod`): def pointer address formed once for the
  inline zero store and the GetSubDef<CFishingRodDef> argument.
- **CTCFishingSpot row (2)** -- 007b0330 OnSerialise (slot 1; the `true` default is a const-reference temporary homed
  in the dead bool argument slot); 00c3caa0 is really the destructor of the landed 0x00C3CB30's 0x14-byte slot member
  (`/EHsc`, leaf `??1CSlot_00c3cb30`; non-virtual dtor keeps `??_G` out of the TU, inline empty polymorphic base keeps
  its vtable store).
- **CTCFlammable (1)** -- 00780d20 OnInitialActivate: `if (!GetSubDef(&PDef)) PDef = 0;` then member-order resets,
  InitialiseFirePoints, SetForceFrameUpdate(true); the zero register (ebx) is set right after the GetSubDef call.
- **CTCFlourishTarget (2)** -- 007e8a00 OnInitialActivate (slot 6) / 007e8a20 FinalCleanup (slot 2): `mov eax,ecx`
  first, type-1 test, register/unregister with `call ; ret`.
- **CTCFollowed (3) + list-walk relatives (3)** -- 006d4d40 RemoveAllFollowers (two out-of-line list clears + GUI
  refresh), 006d4d70 IsFollower (list at +0xc; the catalog says IsAIFollower), 006d4db0 IsAIFollower (+0x10),
  0072a8d0 CTCSpecialAbilities::IsUndeadFollower (+0x14, pointer at node+0xc), 0072a910
  CTCSpecialAbilities::GetUndeadFollower (returns the record: a reference to the node value kept in a local),
  007f6170 CTCTargetingAI::IsThingAttackable (+0x48, exits swapped). Shape: an explicit sentinel-list walk with the
  head re-read from `this` at each back edge and byte exits.
- **CTCForcePushed (1)** -- 00798510 OnKill (slot 14): ParentThing null test, dead-bit `& 1` and `type == 1`
  sharing one materialised 1, unregister with a null component.

## Parked for Fable 5.1 (4)
- 00567d00 CNetString_Retail::RangeInitialize DIFFER(90v89): one extra `int3` after the `__declspec(noreturn)` ThrowLengthError call at the end of the body (retail's next byte is int3 padding, trimmed by the oracle) | tried: STLport _M_range_initialize with inline _M_allocate_block/__copy_trivial (1 grade) | next: lane gap: trailing int3 after a noreturn call (grader should trim it, or compare ignoring trailing int3)
- 00569330 CNetString_Retail::Substr DIFFER(84v86): lane computes `m_start+pos` once and adds min to it; retail recomputes `min + start + pos` (`add eax,esi ; add eax,edx`) | tried: `m_start + pos + Min(n, size()-pos)`, `m_start + (pos + Min(...))`, `&m_start[pos]` as the first argument (4 grades in all, probe/00569330_b/_d) | next: `Min(...) + m_start + pos` as the second constructor argument
- 006c3b70 CTCFadeOutAndIn::FadeOut DIFFER(88v90): retail rounds with `fstp [esp+8] ; fld [esp+8] ; fistp dword ptr [esp]` into a `push ecx` frame word; /QIfist emits `fistp qword ptr` into an 8-byte frame, plain C++ calls __ftol2 | tried: /QIfist cast (1 grade) | next: lane gap: asm_bake (inline-asm rounding helper)
- 006c3bd0 CTCFadeOutAndIn::FadeIn DIFFER(83v85): same inline-asm `fistp dword` rounding helper as FadeOut | tried: /QIfist cast (1 grade) | next: lane gap: asm_bake (inline-asm rounding helper)

## Skipped
none (no row holds two functions).

## Label corrections
- 00567d00 "CTCElectrocutedEffect::InitFromStringRange" -> STLport `basic_string<char>::_M_range_initialize` of the
  networking module's 12-byte string (callers: CHttpUrl accessors 0x0083F5C0/0x0083F600, substr 0x00569330).
- 00569330 "CTCElectrocutedEffect::CopyDataRange" -> STLport `basic_string<char>::substr`.
- 00c0aa10 / 00c0ac00 / 00c11150 / 00c15f10 / 00c1e4d0 "CTCElectrocutedEffect::OnInitialActivate" -> unrelated
  audio-library vtable slots (profile-locked getters/stub setter); classes named by address.
- 0071af30 "CTCEmoteIcon::vfunc_9" -> FrameUpdate; 00750b80 "CTCExplodingObject::vfunc_4" -> OnCreate;
  006c3b50 "CTCFadeOutAndIn::vfunc_2" -> FinalCleanup; 007e5cc0 vfunc_2 -> FinalCleanup; 007e8a00 vfunc_6 ->
  OnInitialActivate; 007e8a20 vfunc_2 -> FinalCleanup; 00798510 vfunc_14 -> OnKill; 007b0330 vfunc_1 -> OnSerialise.
- 00661600 "CTCFadeOutAndIn::IsFading" -> CTCPerceiveThing::CanSeeOrSmellPerceivedThing.
- 0082cf40 "CTCFadeOutAndIn::IsFading" -> CTCDoor::IsClosed.
- 006c3b70 "CTCFadeOutAndIn::FadeIn" -> FadeOut (caller CGameScriptInterface::EntityFadeOut 0x008A0720); 006c3bd0 is FadeIn.
- 004df2d4 "CTCFishingRod::CTCFishingRod" -> CTCAISummon::CTCAISummon (vtable 0x01241C8C); the LANDED factory
  0x004E181D "Create_CTCFishingRod" really builds CTCAISummon (allocates 0x18 bytes for it).
- 00c3caa0 "CTCFishingSpot::Destructor_UnwindHelper" -> destructor of the 0x14-byte slot member of the landed
  0x00C3CB30 (no-RTTI audio-library module).
- 006d4d70 "CTCFollowed::IsAIFollower" -> CTCFollowed::IsFollower (list +0xc; 006d4db0 at +0x10 is the real IsAIFollower).
- 0072a8d0 / 0072a910 "CTCFollowed::IsAIFollower" -> CTCSpecialAbilities::IsUndeadFollower / GetUndeadFollower.
- 007f6170 "CTCFollowed::IsAIFollower" -> CTCTargetingAI::IsThingAttackable.
- The CTCBase accessor 0x00686D60 (`[0x013B8A1C]+0x1c`) is labelled CViewBase::GetDisplayEngine in the manifest
  (propagated); landed 0x007E1F70 calls it GetSubsystem, landed 0x007497C0 GetWorld. Named neutrally here
  (GetGameSubsystem1C) in 0071aa50 and 007e2790.
