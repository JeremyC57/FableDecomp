# Hand chunk 144 (CInputProcessControlCreatureActivateZTarget .. CLODDynamicBuildInfo): 27 of 40 byte-exact

## What matched (27)
- **CInputProcessGameBase** (0x687A90 / 0x687B30 / 0x687BD0): IsEventButtonPressed / Released / Held -- device 1 kind 0x13/0x14/0x15, device 2 kind 1/2/3, device 3 three kinds or a range. Shape: each device arm `return a == b` / `return a || b || (c && d)` (full-register bool), only the unknown-device fall-through gives `xor al,al`; out-of-line getters re-called per test.
- **CInputProcessCreditsUI::ProcessInput** (0x691470): hero's credits component (kind 0x101, bit 1 of +0x40) active and not quitting + press of action 0x22 -> Quit. CTCCreditsUI Quitting at +0x1c (12-byte retail vector).
- **CInputProcessFirstPerson::ProcessInput** (0x68CAC0): `!(sqrt(x*x+y*y) > 0.0001f)` (the `<=` form adds a `jnp`), C2DVector returned by hidden pointer from 0xA043A0.
- **CInputProcessFirstPersonLookAround::ProcessInput** (0x68D700): agent143's CameraLookAround model; `v.x += v.x` gives `fadd st(0),st(0)`; PreProcessStickVector is `__fastcall`.
- **CInputProcessManager::RemoveInputProcess** (0xA0D780): cursor chain at +0x10 (+4 forward, +8 backward, +0x10 next); `pos == node && pos` keeps the redundant null test.
- **CInputProcessParalysed::ProcessInput** (0x690D30): `++m_pressCount > 10`, then `hero->SetCurrentAction(&CCombatAction_GenericBreakOffDrain(hero))` -- temporary in the 0x114-byte frame destroyed through the CCombatActionBase dtor (JackFall shape).
- **CInputProcessQuickAccessItems** (0x68E3C0 CancelQuickAccessCharge, /GS, no params -> matches; 0x68E450 Update): CGameEvent with AddLong/AddUBYTE homed in frame words; slot read once into a local, Cancel tail-duplicated.
- **CInputProcessSetRangedWeaponThirdPersonMode::ProcessInput** (0x68EA60): no /GS in this TU; two identical `if/else if` arms each queueing event 0x3a cross-jump after the `push`.
- **CInputProcessWatchForWillChargeUpThirdPersonModeTermination::Update** (0x68F2A0): `T* c = 0; if (HasTC(k)) c = GetTC(k);` gives the preloaded `xor ecx,ecx` (a ternary materialises in eax); `--QuitModeTimer < 0`.
- **CIsImposterHit::operator()** (0x963810): three GetPItem calls; out-parameter GetInterface template (kind 0x42) then CTCCreatureModeManager::IsModeActive(0x25).
- **CIsNearCampfireWithSpace::operator()** (0x939A50): `(unsigned char)(flags >> 14) & 1`; three float locals summed `x*x+y*y+z*z`; `GetTC(k)->IsActionPointFree()` tail call.
- **CVtable012b2ab0 ctor** (0xC165A0, /EHsc): initialisers then `m_pHelper = new CHelper(m_pSource)` in the body.
- **CItem** (0x5BD8A0 GetItemDescription, 0x5BDDE4 AddInventoryItem; frame-pointer size TU): description = the GetMaximumNumberOfItems counted-temporary model plus a text-bank local read before the pushes; AddInventoryItem = `std::min` by reference over a room local, member re-read (no cached count local), `NumberOfItems = added + NumberOfItems` before the divide.
- **_global::__linear_insert** (0x4D1000, label CJoystick::EventScanStart): the landed 0x0081BDA0 skeleton with a 28-byte float-keyed record (key + 24-byte POD, memberwise user copy ctor).
- **CVtable012b1e84 dtor** (0xC0A9A0, /EHsc, label ~CJoystick): two embedded 4-byte handle arrays destroyed through `eh vector destructor iterator`, empty body.
- **CJoystick::CJoystick** (0x9E43B0, label CJoystickDX::Constructor): 64-event array (`dec ebx ; jne` loop), STLport list member with inline ctor (null, malloc 0x40, self-link), ExclusiveMode assigned in the body (stored last).
- **CJoystickDX::CJoystickDX** (0xAB7850): retail members 8 below engine/CJoystickDX.h; BODY assignments in retail store order (manager, device, ranges, axes) -- initialisers store in declaration order.
- **CKG_Sheriff::Init** (0xD1DFF0) / **CM_HiddenBooty::Init** (0xE93080): landed CGuardToHelpDefend::Init model (named CCharString in its own block, CScriptThing temporary with inline pimpl release); GSI slots 72 / 492 / 578 need the full filler vtable.
- **CKeyCamera copy ctor** (0x75F1F0): EventName default-constructed, body assigns in retail store order (vectors as struct copies, name via operator=).
- **CKeyRedefiner::GetSubTypeForAction** (0x557CA0): switch on GameAction (0x37/0x38 pre-increment counters, 0x3c -> dense direction switch with jump table).
- **CKeyboardDX** (0xAB5FF0 UpdateKeyStates, 0xAB6060 SetHasInputFocus): `return r == 0 ? true : false` gives `sete al` (plain `== 0` goes through ecx); drain loop `do { if (!count) break; r = ...; } while (r >= 0);` keeps the top test.

## Parked for Fable 5.1 (13)
- 00688260 CInputProcessControlCreatureActivateZTarget::ProcessInput DIFFER(141v137): `push ebp ; mov ebp,[esp+38h]` / edi = event at entry vs retail late `mov ecx,[esp+3Ch]`; zero bytes immediates vs `xor al,al` | tried: nested if, `bool` local + default-arg ctor, temporary event (no cookie -> shape right but no /GS frame) | next: lane gap: /GS parameter shadowing (combine with the __forceinline AddGameEvent(long) helper of 006882f0)
- 006882f0 CInputProcessControlCreatureActivateZTargetOnPress::ProcessInput DIFFER(107v103): `push ebx ; mov ebx,[esp+38h]` (processed shadowed) vs retail `mov ecx,[esp+3Ch]` before the call | tried: inline AddGameEvent(long) helper, __forceinline helper (fixed the `xor al,al` zero bytes) | next: lane gap: /GS parameter shadowing
- 006a4040 CInputProcessJumpingAndRolling::ProcessJumping DIFFER(116v112): ebp/edi shadow loads at entry vs retail late loads | tried: named CGameEvent local | next: lane gap: /GS parameter shadowing
- 0068fee0 CInputProcessLightning::Update DIFFER(103v98): `push esi ; mov esi,[esp+34h]` (processed) vs `mov ecx,[esp+30h]` before AddGameEvent; everything else identical | tried: named event local | next: lane gap: /GS parameter shadowing
- 0068c7f0 CInputProcessToggleViewHeroMode::ProcessInput DIFFER(133v129): processed in ebx, event in esi at entry vs retail event in edi, processed re-read | tried: __forceinline AddGameEvent(long) helper | next: lane gap: /GS parameter shadowing
- 00688ba0 CInputProcessZTarget::DeactivateZTarget DIFFER(149v149): `mov edi,[esp+64h]` hoisted above the GetHeroCreature call | tried: two named event locals | next: lane gap: /GS parameter shadowing
- 00811100 CIsHitEventHitByThingWithScriptName::operator() DIFFER(131v130): `sete al ; ... test al,al ; jne` after `repe cmpsb` vs retail `je true ; jmp` (no materialised bool) | tried: plain inline ops, __forceinline ops + accessor (fixed the `lea ecx,[eax+50h]`), ternary operator==, single `||` expression | next: an `IsEmpty()`-style inline that returns straight from the strcmp intrinsic, or lane gap: branch threading of an inlined bool
- 00875990 CIsSwitchValid::operator() DIFFER(154v157): ground length computed after the virtual radius call (`fst ; fcompp ; test ah,5`) vs retail `fstp [esp+28h] ; call ; fcomp [esp+28h]` | tried: C3DVector locals, C2DVector helper, user copy ctor, `C3DVector operator-` (fixed the frame and load order) | next: compute the length through an expression that reads a member (gotcha wave 25 chunk 139) or an inline `float GetLength2D() const` on the vector
- 005bd82e CItem::GetInventoryItemDef DIFFER(126v114): `xor esi,esi` shared zero register vs `and [ebp-4],0`; base def at [ebp-8] not cached in esi | tried: result copy + hand release, CDefClassBasePointer class with dtor, raw pointer local + local-caching copy ctor/dtor | next: declare the sub pointer before the def pointer, or `optimize("y", off)` + `("s", on)` variants with the base pointer as a CDefPointer member of a wrapper
- 009e42e0 CJoystick::UpdateMaintainedEventPositions DIFFER(122v122): `mov ecx,ebx ; push eax` vs retail `push eax ; mov ecx,ebx` before the last call | tried: event reference local, node re-addressing, forwarding inline overload | next: lane gap: call-argument scheduling
- 00ab6180 CKeyboardDX::GetKeyName DIFFER(154v146): `name` shadowed into esi at entry; failure arm first | tried: DIPROPSTRING local | next: lane gap: /GS parameter shadowing (also write the success arm first)
- 00ab6450 CKeyboardDX::SetExclusiveMode DIFFER(131v131): window in eax / device in ecx vs retail window ecx / device eax | tried: window local, unsigned-long window, device local | next: lane gap: register choice
- 0051d870 CRecord24Vector::resize DIFFER(104v104): `mov ecx,[esp+14h] ... mov edi,ecx` vs `mov edi,[esp+14h] ... mov ecx,edi` | tried: member re-read, first local, first+last locals | next: lane gap: register copy direction

## Skipped
- none (no row spans two functions).

## Label corrections
- 0x00687A90 / 0x00687B30 / 0x00687BD0 all labelled IsEventButtonHeld: they are IsEventButtonPressed (kind 0x13) / IsEventButtonReleased (0x14) / IsEventButtonHeld (0x15).
- 0x004D1000 "CJoystick::EventScanStart" is an STLport `__linear_insert` over 28-byte float-keyed records (callees 0x004CF5C0 __copy_backward, 0x004CFD40 __unguarded_linear_insert).
- 0x00C165A0 "CIsVariableFalse::Clone" is the constructor of a 12-byte class with vtable 0x012B2AB0 (CIsVariableFalse is 8 bytes).
- 0x00C0A9A0 "CJoystick::~CJoystick" is the destructor of the vtable-0x012B1E84 class (CJoystick's vtable is 0x0129C0EC).
- 0x009E43B0 "CJoystickDX::Constructor" is CJoystick::CJoystick(CInputManager*, bool) (stores 0x0129C0EC; called by CJoystickDX's ctor 0x00AB7850).
- 0x00E93080 "CKG_Sheriff::Init" is CM_HiddenBooty::Init (manifest neighbours, "M_HiddenBooty" literal); 0x00D1DFF0 is the real CKG_Sheriff::Init.
- 0x0051D870 "CLODDynamicBuildInfo::~CLODDynamicBuildInfo" is a one-argument `vector::resize(n)` forwarding to 0x0051D800 resize(n, T()) with a 24-byte record temporary.
- Callees: 0x00A03B50 / 0x00A03B40 / 0x00A03B70 / 0x00A03B90 / 0x00A03BA0 are CInputEvent getters (DeviceType +0x20, Type +0x28, data +0, button byte +8, value +0xc), not CWorld/CMap/CTCInventoryBase; 0x00A043A0 is CInputEvent's analogue C2DVector getter (not GetDefaultVal); 0x00445B80 is a CGamePlayerInterface flag getter (not CTCShotDefinitionBase); 0x00690DB0 is CCombatAction_GenericBreakOffDrain's ctor (vtable 0x0125BF6C); 0x00811340 is CTCCreatureModeManager::IsModeActive (not CPlayer::HasMode); 0x009C7A40 is CGroupEntry's text getter (not CBankFile::GetBankHandle); 0x009E4710 is list::clear (not SortTreeRecursively); 0x00519EF0 is a range destroy (not _Move_backward); 0x00A04410 is CInputEvent::CInputEvent (not CDefaultEngineMemoryInit).
