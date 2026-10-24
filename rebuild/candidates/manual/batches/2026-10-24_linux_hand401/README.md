# Hand chunk 401 (CTCDNone .. CCombatActionBase): 36 of 40 byte-exact

All 36 sources grade MATCH (2) or RELOCATION_MATCH (34) under VC7.1 RTM with the base flags (no `// Flags:` line). Every test
prints PASS under g++ (tall.sh) and VC7.1 (tall.py). Parameter names are descriptive where the PDB has none (StartsWith,
SendRawData); every function name comes from Ego_r.pdb / FableWin.pdb (or their TU order for 0x008368D0, 0x0084F490,
0x00850530).

## What matched (36)
- **CTCDNone** -- 0x00835A20 `Create(long, const C3DVector&, bool)`: create a bare thing on the map under the position. The
  map is a zero-initialised local overwritten when the flag is clear (a `?:` argument). The result is a zero declared after
  the create call, then `if (thing && GetTCDriver(&driver) && Activate) result = thing;`.
- **CThingSoundEmitter**
  - 0x00836740 OnCreate: the new sound object is stored before its null test, with one `xor al,al` exit.
  - 0x008368D0 StopSpecificSample: one inline `g && !Dead` guard.
  - 0x00836910 ConstructFromParams: `if (base) { ...; return true; } return false;`. The intelligent-pointer temporaries come
    from the raw pointer (the landed 0x00830360 model).
  - 0x008369F0 Create: the landed CThingDeadCreature::Create 0x00835C80 form (named zero result, explicit class operator new,
    placement construction inside the null test).
- **CThingTrackNode** -- 0x00837C70 constructor. Every member other than the base and TrackName is cleared in the BODY, after
  the TrackName constructor, from one zero register.
- **CThingVillage / CThingHolySite / CThingSwitch / CThingPhysicalSwitch**
  - 0x0083B340 CThingVillage::ConstructFromParams: the empty script name is a named CCharString in its own block, and the
    base result goes to a bool local that survives its destructor.
  - Create 0x0083B7F0 / 0x0083BBF0 / 0x0083C5D0 / 0x0083DBE0:
    - `new (false) T` through an out-of-line constructor, or the class `Alloc()`.
    - The zero result is declared AFTER the allocation; declaring it first grows the body by 21 bytes.
    - Then `if (p && Construct) { physics->ConstructFromParams(CTCParamsPhysicsBase(pos, CRightHandedSet())); if (Activate) result = p; }`.
    - The by-value CCharString of CThingSwitch::Create is destroyed once, at the shared exit.
- **LHNetworkLib** (an STLport TU without EH; all strings are 12-byte STLport strings)
  - 0x0083EE10 `StartsWith(const string&, const string&)` (__fastcall):
    - Body: `return toLowerCase(text).substr(0, prefix.size()) == toLowerCase(prefix);`.
    - The free inlines (`operator==`, `min`) must be `__fastcall`, as STLport's `_STLP_CALL` is empty under /Gr. Only then
      does VC7.1 evaluate the LEFT operand of `==` first; the default convention calls toLowerCase(prefix) first.
    - The substr result is built through the `(const char* s, size_type n)` constructor, whose `s + n` shares the argument
      list with the iterator tag, so the add follows the tag push. The (first, last) form swaps one instruction.
    - The out-of-range thrower is `__declspec(noreturn)`; that lets substr inline.
  - 0x0083F870 `LHHttp2::SetProxySettings`, 0x0083FB20 `LHHttp2::SetConnectionDetails` and 0x00841EE0
    `LAddress(const string&, unsigned short)`:
    - Each is an inline `operator=(const string&)` with a self test.
    - Retail homes the empty `__false_type` tag on the LIVE port argument slot.
    - That needs a same-TU `__declspec(noinline)` `_M_assign_dispatch` body (0x00569140) that never touches the tag. A bare
      declaration homes it on the dead name slot.
  - 0x0083F8B0 `LHHttp2::SendRawData`:
    - Body: `if (!Host) return 3; vector<char> buffer; buffer.resize(size); memcpy(&buffer[0], data, size); Host->Send(buffer, LAddress(), LAddress()); *bytesSent = Host->NrLastDataSent; return 0;`.
    - `resize(n)` forwards `_Tp()` to a `const _Tp&` resize, so the zero char is a TEMPORARY at byte 3 of its slot. A
      by-value parameter sits at byte 0.
  - 0x0083FB60 `LHHttpHeaderStatus::Reset`: code -1, then clears the header-line vector, the location, host and URI, and the
    port.
  - 0x0083FC00 `LHHttpHeaders::AddHeader(name, value)`:
    - Body: `string header(name); if (name[name.size() - 1] != ':') header += ":"; header += " "; header += value; HdrDocumentHeader.push_back(header);`.
    - Retail keeps `header` unaliased: `_M_start` is cached in edi across the push_back branch, and the copy's loads sit
      above the placement zero stores.
    - Every callee that receives `&header` must be a NON-template same-TU noinline body that never stores the address:
      `_M_range_initialize`, the two `append` instantiations as overloads, a puts/abort `_M_throw_length_error`, and
      `_M_insert_overflow` with its helpers, defined out of class and explicitly instantiated above the caller.
    - `/Oa` over-caches the `name` reference.
  - 0x0083FF80 `LHHttp2::Reset`: `if (Host) { delete Host; Host = 0; }`, then the member resets in Ego_r order, the memset as
    `rep stosd`.
- **STLport basic_string<char>**
  - 0x00841790 `find(const char*, size_type) const`: the 2-arg find inlines the 3-arg one. The `search` template body must be
    same-TU `__declspec(noinline)` so start and finish stay cached across the call.
  - 0x00841B70 / 0x00841C50 `operator+` (string + C string, C string + string): the landed 0x00569690 model, with a
    `__forceinline` _Reserve_t constructor and no NRVO.
- **CCreatureAction_PlayAnimationWithLookTurning** -- the three constructors at 0x00842DB0 / 0x00842DF0 / 0x00842E40. Each
  passes its arguments straight through to the base, then stores the vtable. The class adds no data to its 0xB4 base.
- **NCameraModes / C3DFrustrum TU**
  - 0x0084CCC0 GetNewCameraPosFromOcclusionParams: a named C3DVector result, returned by copy (no NRVO).
  - 0x0084E2C0 CBase::AddIncidentalAttractorLookAt: the camera manager is a POINTER local taken first. `push_back` =
    `insert(end(), x)`, which reads the sentinel before the node malloc.
  - 0x0084F010 `std::__find_if<CCountedPointer<CSubComponentBase>*, CIsOfType>`: the STLport 4x-unrolled body verbatim.
  - 0x0084F490 CPhysicsState(): two list sentinel constructors, then the bool.
  - 0x00850530 `boost::scoped_ptr<CPhysicsForceModelBase>::~scoped_ptr`: inline checked_delete.
- **CHeroAbilityStatPackage / CHeroAbilityStatWeight**
  - 0x00851B50 IsValid: the threshold is read into a local BEFORE GetTrainableStatLevel.
  - 0x00851BD0 GetParameter: the weighted mean of level / max level. The epsilon is a named `extern const float` global
    (0x0129BA3C); with a literal, VC threads the empty-map path straight to `return 0`.
- **CCombatActionBase**
  - 0x00856250 / 0x00856290 Get[Last]TimeThroughCurrentAnimation: the fps global is read into a local before the virtual
    GetAnimDelayTime call.
  - 0x00856570 anonymous IsThingWithHelperPoints: the appearance pointer is a local, the loop is a do-while (no empty-range
    guard), and `it == end ? true : false` gives the bare `sete al`.
  - 0x00856E50 GetPActiveStrikeEvent: returns `for_each(begin, end, CGetActiveStrikeEventThatIsFirstToFinish(FramesRunning)).PStrikeEvent`.

## Parked for Fable 5.1 (4)
- 008365f0 std::for_each<const pair<unsigned long, CAnimationEntry>*, CDeadCreatureAnimPreloader> DIFFER(119v119): retail keeps the strcmp zero in eax (`xor eax,eax`, pushed as the BeginLoading false) and loads BankIndex into ecx; the lane picks edx/eax | tried: named name local, `!strcmp`, pragma s, /Gr, the QFE compiler | next: same-TU noinline stand-ins for PeekAnimation 0x00A242C0 / BeginLoading (callee knowledge moved register choice for 0x00841790 / 0x0083FC00), else lane gap: register allocation
- 0084cd10 NCameraModes::SetCameraRoll DIFFER(194v194): retail loads the forward Z (`mov edx,[esi+20h]`) after the first fmul and stores it after `fmul [esp+4]`; the lane hoists it with the X/Y loads | tried: k local, `<` compare, the float fabs overload (fixed the fxch pair), pack(1) C3DVector | next: a named `float z = forward.Z;` read between the two products, or same-TU noinline AccurateNormalise 0x00A14440 / Rotate 0x00A13C60 stand-ins
- 0084d7e0 NCameraModes::CBase::UpdateRaiseUp DIFFER(194v193): retail reads the focal Z through GetFocalPos's returned pointer (`fld [eax+8]`) and parks the height in the dead temporary's slot; the named-local form reads `fld [esp+14h]` (and `float h = GetFocalPos(1.0f).Z - z` gives h its own slot, frame 0x10 v 0xC) | tried: one expression, split tanh statement, appearance local, named focal vector, ctor'd C3DVector | next: a same-TU noinline GetFocalPos 0x004C1060 stand-in, or a block scope that lets h share the dead temporary's slot
- 0084e390 C3DFrustrum::AddPoint DIFFER(111v111): retail copies the pair's weight in place from the weight ARGUMENT slot (`[esp+18h]`) and homes the `__true_type` tag there (`[esp+20h]`); the lane reads the temporary (`[esp+0Ch]`) and homes the tag in the point slot | tried: make_pair, POD C3DVector, STLport _Construct | next: a same-TU noinline `_M_insert_overflow` body that never touches its tag (the 0x00841EE0 / 0x0083F870 fix, GOTCHAS 0x4ADB50) so the tag homes on the live weight slot

## Skipped
None. No row holds two functions. 0x00841B70 / 0x00841C50 end in `jmp abort` with no int3 inside the row, and no row needed
an int3-after-noreturn note.

## Label corrections
| Address | Manifest label | Real (PDB) |
|---|---|---|
| 0x00835A20 | Create (_global) | CTCDNone::Create(long, const C3DVector&, bool) |
| 0x008365F0 | CTCInventoryAbilities_PreloadDeadAnimation | std::for_each<..., CDeadCreatureAnimPreloader> |
| 0x00836740 | CThing_OnCreateWithInterface | CThingSoundEmitter::OnCreate |
| 0x008368D0 | CActiveEntity_KillOnLevelUnload | CThingSoundEmitter::StopSpecificSample |
| 0x00836910 | Sound_SoundEmitter_Constructor | CThingSoundEmitter::ConstructFromParams |
| 0x008369F0 | Sound_CreateAttachedEmitter | CThingSoundEmitter::Create |
| 0x00837C70 | CTCDPhysicalObstruction_Constructor | CThingTrackNode::CThingTrackNode() |
| 0x0083B340 | CThingGameObject_ConstructorEmpty | CThingVillage::ConstructFromParams |
| 0x0083B7F0 / 0x0083BBF0 / 0x0083C5D0 / 0x0083DBE0 | Create (_global) | CThingVillage / CThingHolySite / CThingSwitch / CThingPhysicalSwitch::Create |
| 0x0083EE10 | HTTP_CompareStrings | LHNetworkLib::StartsWith (__fastcall) |
| 0x0083F870 | LHNetworkLib_InsertStringRange | LHHttp2::SetProxySettings |
| 0x0083F8B0 | LHHttp2_SendDataPacket | LHHttp2::SendRawData |
| 0x0083FB20 | LHNetwork_BuildLoginRequestString | LHHttp2::SetConnectionDetails |
| 0x0083FB60 | CPatchTesselationEdgeStrip_Reset | LHHttpHeaderStatus::Reset |
| 0x0083FC00 | HTTP_Request_BuildPath | LHHttpHeaders::AddHeader |
| 0x0083FF80 | LHHttp2_Clear | LHHttp2::Reset |
| 0x00841790 | StringSearch_FindPatternInRange | basic_string<char>::find(const char*, size_type) const |
| 0x00841B70 / 0x00841C50 | String_Concatenate / String_PrependFromRange | std::operator+(const string&, const char*) / (const char*, const string&) |
| 0x00841EE0 | LHHttp_InitializeRequest | LAddress::LAddress(const string&, unsigned short) |
| 0x00842DB0 / 0x00842DF0 / 0x00842E40 | CCreatureAction_PlayAnimation_* | CCreatureAction_PlayAnimationWithLookTurning constructors (3 overloads) |
| 0x0084CD10 | SetCameraRoll | NCameraModes::SetCameraRoll(CCamera&, float) |
| 0x0084D7E0 | CTCGraphicAppearance_UpdateHeightInterpolation | NCameraModes::CBase::UpdateRaiseUp |
| 0x0084E2C0 | LinkedList_InsertIfLOS | NCameraModes::CBase::AddIncidentalAttractorLookAt |
| 0x0084E390 | StdVector_PushBack | C3DFrustrum::AddPoint |
| 0x0084F010 | StdVector_LinearSearch | std::__find_if<CCountedPointer<CSubComponentBase>*, NCameraModes::CIsOfType> |
| 0x0084F490 | DataStructure_InitListPair | NCameraModes::CPhysicsState::CPhysicsState() |
| 0x00850530 | DiscardAsyncData | boost::scoped_ptr<NCameraModes::CPhysicsForceModelBase>::~scoped_ptr |
| 0x00851B50 | CAbility_CheckRequirements | CHeroAbilityStatPackage::IsValid |
| 0x00851BD0 | CTCHeroExperience_GetAbilityMultiplier | CHeroAbilityStatWeight::GetParameter |
| 0x00856250 / 0x00856290 | CalcCooldownRemaining | CCombatActionBase::GetLastTimeThroughCurrentAnimation / GetTimeThroughCurrentAnimation |
| 0x00856E50 | CCombatActionBase_GetFirstActiveStrikeEvent | CCombatActionBase::GetPActiveStrikeEvent (non-const) |

Callees:

| Address | Real |
|---|---|
| 0x0083EBD0 | LHNetworkLib::toLowerCase (__fastcall, hidden return in ecx) |
| 0x008F8F50 | LSocket::Send(vector<char>&, const LAddress&, const LAddress&) |
| 0x004110D0 | vector<char>::_M_fill_insert (landed as "CStringFillRange") |
| 0x005299C0 | CThingVillage ctor |
| 0x00529B00 | CThingSwitch ctor |
| 0x00850360 | ~CPhysicsForceModelBase |
| 0x00A242C0 | PeekAnimation |
| 0x00A14440 | AccurateNormalise |
| 0x004C1060 | CTCGraphicAppearance::GetFocalPos(float) (forge "GetHelperPointOr") |
| 0x00877C40 | CTools::IsLineOfSight (manifest "IsLineOfSightFiltered") |
| 0x004BE170 | CTCGraphicAppearance::HasHelperPoint |
| 0x006D6520 | CTCHeroExperience::GetTrainableStatLevel |
