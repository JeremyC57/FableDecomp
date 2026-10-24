# Hand chunk 431 (CAudioOwner .. CPanel): 34 of 40 byte-exact

All 40 rows are `_global` rows in the 0x00C0xxxx-0x00C1xxxx middleware library: an MFC-style `CString`, an audio
parameter block, a script host, typed-list visitors and a UI panel. No PDB covers this library, so every class and
function name is descriptive. Every manifest label in the chunk is propagated from a byte-identical body somewhere
else, and every one is wrong (see Label corrections). The library TUs that carry EH frames build under `/EHsc`
(`// Flags: /EHsc`). All 34 sources in src/ grade MATCH/RELOCATION_MATCH (gall.sh), and all 34 tests print PASS under
g++ (tall.sh) and the lane's VC7.1 (tall.py).

## What matched (34)
- **CAudioOwner** (0x00C08AE0 WaitForSprite, /EHsc): a do/while that polls the sprite, with the `CScopedLock` in an
  inner scope so that its state goes -1 before `Sleep(100)`. The done flag stays in bl.
- **Bit-mask helpers** (0x00C0AE70 SetMaskBit, 0x00C0AEE0 SetMaskBit16, 0x00C0AF00 SetMaskBit8):
  - SetMaskBit is `__fastcall (index, set, pMask, pTouched, bitCount)`. It ORs the touched mask first, then sets or
    clears the bit in the mask.
  - The 16/8 wrappers re-push the two stack words under the constant.
- **CRampedParam / CAudioParamBlock / free audio helpers**:
  - 0x00C0B4C0 WrapAngle (`__stdcall`) needs early returns: each return reloads the argument.
  - 0x00C0B510 SetImmediate inlines the wrap in register form (`fcom`, no reload), and its nine zero stores keep the
    retail source order.
  - 0x00C0B180 MultiplyMaskedParams (`__fastcall`) inlines GetValue (out-of-line copy at 0x00C0B730). Its loop is
    `while (value != 0 && mask && count)`.
  - 0x00C0AF20 Update tests `m_time != m_duration`; in the compare, retail loads the right operand first.
- **CString** (MFC text, all with the out-of-line `LIB_ASSERT` = hook @ 0x0143BB2C + `CDebugBreak()` @ 0x00C16750):
  - Rows: 0x00C0B8C0 GetData, 0x00C0B910 Release, 0x00C0B9F0 static Release(CStringData*), 0x00C0BC20 ReverseFind,
    0x00C0BD60 CompareNoCase, 0x00C0BDA0 AllocBuffer, 0x00C0BE20 CopyBeforeWrite, 0x00C0BFE0 CString(LPCTSTR),
    0x00C0C060 AssignCopy, 0x00C0C970 SetAt, 0x00C0D740 TrimRight(LPCTSTR), 0x00C0DCF0 Append(const CString&) and
    0x00C0E440 Mid(int) (/EHsc).
  - Shape: the empty CDebugBreak temporary is homed in a dead argument slot when the argument lives in a register.
    Otherwise it goes in the `push ecx` frame word (`lea ecx,[esp+7]`).
  - ReverseFind and TrimRight need tchar.h's `_MBCS` inline wrappers (`_tcsrchr`, `_tcschr`, `_tcsinc` over
    `_mbsrchr`, `_mbschr`, `_mbsinc`). Calling the imports directly picks other registers.
- **CScriptHost** (vtable 0x012B25A8):
  - 0x00C0E9E0 QueueEvent: POD `new` with no null test. The source order is time, data, type, and the second store
    is held in edx and stored last.
  - 0x00C0EA10 ClearEvents, 0x00C0EAF0 DeleteAllScripts and 0x00C0EB40 DeleteAllWatchers: rotated
    GetFirst/handle loops.
  - 0x00C0EC10 DestroyOwner: a `Find(owner)` loop, then `delete owner` through the virtual dtor.
  - 0x00C0EC90 PurgeUnreferencedOwners: written as `for (;;) { found = 0; for (i = 0; !found && i < n; ++i) ...;
    if (!found) break; DestroyOwner(found); }`. This form duplicates the destroy arm the way retail does.
- **Visitors**: 0x00C10150 CCollectFreeMatchesA::Visit and 0x00C10190 CCollectFreeMatchesB::Visit are byte-identical
  bodies (base vtable 0x012B260C, slot 0 `_purecall`).
- **CNamedSet** (0x00C101D0 Find, /EHsc): needs MFC's three `operator!=` overloads together with the inline
  `operator LPCTSTR`, and an explicit `!= CString("")`. The temporary's text is then read through the ctor's eax.
- **CItemDispatcher** (0x00C10400 RunAllItems, /EHsc):
  - It builds a local typed list (vtable 0x012B1B78, modelled on the landed 0x00C0A610 / 0x00C09B70 ctor/dtor) and a
    collect-all visitor (0x012B2624), enumerates the container at +0x10, then drains the list with a stack
    {dispatcher, item} action.
  - The 8-byte action whose address is taken inside the loop gives retail's `and esp,-8` frame for free.
- **CPanel**:
  - 0x00C10CD0 AssignPendingItems: the second, top-tested loop needs the `__forceinline` early-return helper
    `IsEntry`.
  - 0x00C11CB0 UpdateItems: the count is re-read on every pass.
  - 0x00C11F00 OnKey: keeps the trailing `IsBusy()` call that retail makes.

## Parked for Fable 5.1 (6)
- 00c0acc0 _global::MultiplyMaskedFactors DIFFER(74v74): retail `mov esi,ecx ; mov ecx,[esp+0Ch]` (pointer in esi, count in ecx), ours keeps the pointer in ecx and the count in esi | tried: named pointer local, index loop (73 bytes), separate down-counter | next: lane gap: register allocation (the sibling 0x00C0B180 has our allocation in retail)
- 00c0b130 CAudioParamBlock::GetPitch DIFFER(71v71): retail `push 8 ; add ecx,0x344 ; push eax`, ours `push 8 ; push eax ; add ecx,0x344` | tried: the twin 0x00C0B420's forms (named array pointer, /Oa, optimize("s")) | next: lane gap: scheduling
- 00c0b420 CAudioParamBlock::GetGain DIFFER(68v68): retail `push 10h ; add ecx,4 ; push eax`, ours `push 10h ; push eax ; add ecx,4` | tried: named pointer local for the array, /Oa, #pragma optimize("s") (70 bytes) | next: lane gap: scheduling
- 00c0b7b0 CRampedParam::Update DIFFER(171v167): retail evaluates the rate sum as (h*c3 + t*c2) + c*c4 + c1 and accumulates the value into the q slot (`fxch st3 ; faddp st3`); ours emits t*c2 first and folds top-down, and the finished-path temporaries swap eax/edx | tried: bool-flag forms, early-return `>=` (kept), local t vs CSE'd m_time (kept m_time), three term orders | next: lane gap: x87 sum canonicalisation
- 00c11520 CPanel::DispatchNavigation DIFFER(163v163): retail stores the direction's first word (`mov [esp+10h],ecx`) before `push eax`, ours after it | tried: struct assignment, memberwise x/y/z, z-first through locals | next: a same-TU noinline CNavEvent ctor body (0x00C20390), else lane gap: scheduling
- 00c12300 CPanel::GetItemStatistics DIFFER(165v165): retail keeps the loop index in edi and the item in ebx (ebx pushed late, the +0x370 value read into the dead ebp), ours picks ebx/edi the other way and reads the value into eax | tried: pointer local for the item, named local for the +0x370 value, inline Reset() for the five zero stores | next: lane gap: register colouring

## Skipped
None. No row holds more than one function.

## Label corrections
Every manifest label in this chunk is propagated. The descriptive names used here are:

| Address | Manifest label | Name used here |
|---|---|---|
| 0x00C08AE0 | EventQueue_ProcessBlockParrySuccessesWithTimeout | CAudioOwner::WaitForSprite |
| 0x00C0ACC0 | Graphics_MultiplyFloatByBitMask | MultiplyMaskedFactors |
| 0x00C0AE70 | ConstructString_SetCharMask | SetMaskBit |
| 0x00C0AEE0 | ConstructString | SetMaskBit16 |
| 0x00C0AF00 | ConstructString | SetMaskBit8 |
| 0x00C0AF20 | NUISystem_CComponent_UpdateAnimation | CAudioParamBlock::Update |
| 0x00C0B130 | Graphics_ClampFloatParam | CAudioParamBlock::GetPitch |
| 0x00C0B180 | Render_ApplyAlphaValueMask | MultiplyMaskedParams |
| 0x00C0B420 | Render_ClampAlphaValue | CAudioParamBlock::GetGain |
| 0x00C0B4C0 | FloatThreshold_Clamp | WrapAngle |
| 0x00C0B510 | Matrix_SetIdentityRotation | CRampedParam::SetImmediate |
| 0x00C0B7B0 | AnimationCurve_UpdateWithDelta | CRampedParam::Update |
| 0x00C0B8C0 | CCharStringData_GetOffset | CString::GetData |
| 0x00C0B910 | CCharString_ReleaseRefcount | CString::Release() |
| 0x00C0B9F0 | CCharStringData_ReleaseWithRefcount | static CString::Release(CStringData*) |
| 0x00C0BC20 | CCharString_ReverseFind | CString::ReverseFind (library CString, not the game's CCharString) |
| 0x00C0BD60 | MessageEvent_Validate | CString::CompareNoCase |
| 0x00C0BDA0 | CCharString_AllocFromSize | CString::AllocBuffer |
| 0x00C0BE20 | RefcountedData_Copy | CString::CopyBeforeWrite |
| 0x00C0BFE0 | Std_Allocator_Construct | CString::CString(const char*) |
| 0x00C0C060 | CCharString_SetData | CString::AssignCopy |
| 0x00C0C970 | CCharString_SetCharAt | CString::SetAt |
| 0x00C0D740 | CCharString_TrimRight | CString::TrimRight(LPCTSTR) |
| 0x00C0DCF0 | CCharString_InsertDataWrapper | CString::Append(const CString&) |
| 0x00C0E440 | CCharString_SubstringRange | CString::Mid(int) |
| 0x00C0E9E0 | CEngineVSConstantLayout_QueueEventWithData | CScriptHost::QueueEvent |
| 0x00C0EA10 | CEngineVSConstantLayout_ClearAllEvents | CScriptHost::ClearEvents |
| 0x00C0EAF0 | CTCMapwho_ClearAllMappedActorsFromUI | CScriptHost::DeleteAllScripts |
| 0x00C0EB40 | CTCMapwho_ClearAllInventoryItemsFromUI | CScriptHost::DeleteAllWatchers |
| 0x00C0EC10 | CTCMapwho_ClearMessageQueue | CScriptHost::DestroyOwner |
| 0x00C0EC90 | UISystem_ProcessInputEventLoop | CScriptHost::PurgeUnreferencedOwners |
| 0x00C10150 | Inventory_CheckAllItems | CCollectFreeMatchesA::Visit |
| 0x00C10190 | Inventory_IterateAndValidate | CCollectFreeMatchesB::Visit |
| 0x00C101D0 | CAnimationSet_FindAnimation | CNamedSet::Find |
| 0x00C10400 | Combat_InitializeStrafeDef | CItemDispatcher::RunAllItems |
| 0x00C10CD0 | NUISystem_CSpellContainerList_PopulateSpells | CPanel::AssignPendingItems |
| 0x00C11520 | Inventory_ProcessNavigationItem | CPanel::DispatchNavigation |
| 0x00C11CB0 | CTCInventory_ProcessKeyPress | CPanel::UpdateItems |
| 0x00C11F00 | CTCHero_OnInventoryKeyPressed | CPanel::OnKey |
| 0x00C12300 | Inventory_PopulateItemArray | CPanel::GetItemStatistics |

Callee labels that are also propagated:

| Address | Manifest label | What it is |
|---|---|---|
| 0x00C185B0 | CCombatAbilityStrafeDef::Initialize | the typed list's impl constructor |
| 0x00C17B10 | ~CPixelShaderData | the typed list's impl destructor |
| 0x00C1EE00 | CDateAndTime | the {dispatcher, item} action's constructor |
| 0x00C1F480 | ActionHandler_ReplaceWithUnsheatheWeapons | the action's Run |
| 0x00C104F0 | CTCHero_ProcessBlockParryStrafe | the sibling drain of the same list type |
| 0x00C0DBB0 | landed as CNamedRecord_00c0dbb0::JenkinsHashOfName | CString::GetHash |
