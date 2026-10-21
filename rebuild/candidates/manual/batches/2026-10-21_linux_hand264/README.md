# Hand chunk 264 (CFileInstaller .. CHighlightItemDef): 14 of 40 byte-exact

All 14 grade RELOCATION_MATCH on the RTM 3077 lane (gall.sh). Each has a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (14)
- **CFontManager** 009e2e30 `CFontManager(const CCharString&, const CCharString&)`: creates both data banks and symbol maps, opens a bank only when its name is non-empty, and allocates eight CVertexBufferWin32. Shape: CBaseObject base (ctor 0x99A2F0), four `CCountedPointer::Reset` members, a 12-byte CArray. CVertexBufferBase's ctor is `: Length(0), Type(0) {}`. The derived ctor sets `PVertexBuffer = 0; Stride = 0;` in its own body, so those stores come after the derived vptr store.
- **CFrontendGameComponent** 00496cf0 `Run(CGameComponent*&)`: pumps the engine until it reports quit, then hands over to a new frontend or a main game. Matched on the first try with a `while (!Quit)` pump and two `new` hand-off arms.
- **CGameCameraBase** 006fd6c0 `IsLandscapeCollision()`: marches from the camera towards the look-at point in quarter steps and lifts the camera above the highest ground sample. Shape: C3DVector needs a user copy ctor, and the length is `sqrt(x*x + (y*y + z*z))`. Without both, the copies and the square order differ.
- **CGameCameraManager** 0069a990 `ResetCamera()`: re-targets the hero, resets the height band and the modes, snapshots the camera, and sends the renderer message. Shape: the CCharString message and the renderer calls sit in their own `{}` block. Retail destroys the string before the second `OldCamera = Camera` rep-movsd, and the block also lets the 0x81 component key share the string's slot.
- **CGameScriptInterface** 0089e540 `GiveHeroYesNoQuestion`: closes the boast UI, optionally pauses, shows the subtitle in the two- or four-answer form, then reopens the UI. Shape: `CIntelligentPointer<T>` has no declared dtor, so the call goes to ~CBaseIntelligentPointer directly. 0089aa70 `GetFurthestWithDefName`: the best distance is a `long` (fild/_ftol), compared with `distance > best`.
- **CGameScriptThing** 008d0c50 `MsgIsTriggeredBy`, 008d0e00 `MsgIsUsedBy`, 008d29c0 `MsgIsHitBySpecialAbilityFrom`: event-list interval walks with a named CScriptThing temporary. In 008d29c0 the whole test is one bool expression.
- **CConsole** 009ecd80 `CConsole()` (catalogued as CHeroInformationScreens::Ctor): STLport `_Rb_tree` built in two halves, a base that zeroes the header proxy and then mallocs it, and a derived part.
- **CGameflowAssistanceScript** 00ceffb0 `Tutorial_LowHealth`: the Tutorial_LowMagic loop shape, with two cooldown counters in ebp/ebx.
- **CHeroBoastsScript** 00ce40d0 `BoastUseOnlyFryingPan`: the failure test is one `||` expression whose middle operand builds the literal conditionally.
- **CHeroMarriageDef** 00452398 `Transfer`: engine header layout, context in edi, `lea eax,[esi+N]` per member.
- **CHighlightItemDef** 0077ce00 `Transfer`: eight inline `CPersistContext::Transfer<T>` calls (the template from the landed 0x007B7D20).

## Parked for Fable 5.1 (26)
- 009c4f20 CFileInstaller::ProcessAlways DIFFER(485v490): `cmp [ebp+74h],esi ; je ; mov ecx,[ebp+74h]` re-read and NoTotalBytesWritten via load/add/store, front-node `lea esi,[eax+8]` order | tried: plain members, inline STLport list pop_front/dtor with free | next: the thread-shared members look cv-qualified for threading (the qualifier this brief forbids); retry at landing with it
- 009c5690 CFileInstaller::ThreadMain DIFFER(468v469): same PCompletedRead re-read, `Running` stored before the ExitThread test, shl temporaries renamed | tried: inline CheckCompletedRead helper | next: as 009c4f20 (thread-shared qualifier on the members), then re-grade
- 00e81d10 CFishCompOrganiser::Initialize DIFFER(421v412): retail uses two scaled LEAs `lea eax,[eax+edi*4]`, ours CSEs edi*4 | tried: index forms, operator[] types, const element pointers | next: pointer-increment loops
- 00697490 CGameCameraManager::IsPosCameraInsideThing DIFFER(475v475): `mov eax,[esp+5Ch]` (pos arg) scheduled before the last `fstp [esp+14h]` in retail | tried: double `along` + InRange(float,...) helper (fixes the x87 dup), inline NormaliseGetLength (fixes the swap, breaks 1/len) | next: combine both fixes, inv as a by-reference temp
- 00697f80 CGameCameraManager::Is2DAreaInViewingFrustrum DIFFER(492v490): retail `fxch st(1) ; fmul [fwd.z]` consumes cos, ours `fstp st(1)` later | tried: casts, float angle, const float& scale, s*X order, reordered sum | next: lane gap: x87 stack allocation
- 006992a0 CGameCameraManager::PreparePrimitivesForRendering DIFFER(485v506): frame 0x64 v 0x4c, debug-line primitive temporaries, register roles | tried: one draft | next: model the end point and colour copy through an 8-byte temporary, list in the dead info slot
- 00699e60 CGameCameraManager::SetScriptedCameraMode DIFFER(459v431): retail builds the counted pointer in dead argument slots | tried: several | next: lane gap: dead-slot reuse
- 006b75a0 CGameMap::Resize DIFFER(414v434): min/max-by-reference bounds, loop counters in dead argument slots, bottom-up row index | tried: one draft | next: `const long& Max/Min` returning references with locals taken by address, counters as the offset parameters
- 00894500 CGameScriptInterface::CreateCreatureOnEntity DIFFER(465v466): `push edi` sunk inside the IsAlive guard, temporaries in dead argument slots | tried: one | next: lane gap: callee-saved push sunk inside a guard
- 00896b60 CGameScriptInterface::KickOffQuestStartScreen DIFFER(437v445): epilogue placement (tail duplication) | tried: if/else orders, explicit returns, inline helper | next: lane gap: cross-jump/tail duplication
- 0089a880 CGameScriptInterface::GetNearestWithDefName DIFFER(481v481): register colouring eax/ecx v eax/edx | tried: comparison forms, forceinline, split ifs | next: lane gap: register colouring
- 0089b5b0 CGameScriptInterface::StartScriptingEntity DIFFER(454v454): `mov eax,[esp+1Ch] ; push eax` v ours `mov ecx,...; push ecx` before operator delete | tried: fastcall/cdecl/pmf deleter, delete v operator delete, Release() helper, store order | next: count block typed through a template helper, or accept as a register-colour lane gap
- 0089f300 CGameScriptInterface::CreateCreatureNearby DIFFER(395v403): this in ebx and handle in edi in retail | tried: inline IsAlive/GetPThing forms, merged condition | next: lane gap: register allocation
- 008a0cb0 CGameScriptInterface::DebugGetAllTextEntriesForTargetedThing DIFFER(460v456): edi pushed at entry in retail, text bank kept in edi for the `_1_5` retry with a separate operator+ temporary | tried: template GetComponent<T,TYPE>, inline set clear | next: text bank in a named local, `_1_5` name as a named CCharString
- 008a8260 CGameScriptInterface::GetHeroSummonedCreaturesList DIFFER(481v466): found flag homed in a frame byte, pushes sunk | tried: two | next: lane gap: callee-saved push sunk inside a guard
- 008a8c60 CGameScriptInterface::EntitySetAsSittingOnFloor DIFFER(472v472): temp slots, with retail's name at +14h shared with the counted pointer and the filter at +1Ch | tried: explicit/implicit name, block-scoped name/filter, filter sizes 8/12/16 | next: lane gap: temp slot packing
- 008d1130 CGameScriptThing::MsgIsHitByWithWeapon DIFFER(499v499): frame 0x20 v 0x1c, the GetEvent out pointer not packed with the key slot | tried: helper/by-ref events, block-scoped key | next: lane gap: dead-slot reuse
- 008d19a0 CGameScriptThing::MsgIsTalkedToBy DIFFER(468v468): join reloads edi before esi | tried: 23 advisor probes | next: lane gap: scheduling
- 008d1fd0 CGameScriptThing::MsgIsKnockedOutBy DIFFER(468v468): same join reload order as 008d19a0 | tried: advisor probes | next: lane gap: scheduling
- 008d2bb0 CGameScriptThing::MsgIsHitByAnySpecialAbilityFrom DIFFER(474v474): same as 008d19a0 | tried: advisor probes | next: lane gap: scheduling
- 008d2d90 CGameScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom DIFFER(485v485): same as 008d19a0 | tried: advisor probes | next: lane gap: scheduling
- 008d2f80 CGameScriptThing::MsgIsHitByAnyNonAggressiveSpecialAbilityFrom DIFFER(485v485): same as 008d19a0 | tried: advisor probes | next: lane gap: scheduling
- 009fd9b0 CGraphicDataBank::LoadVolumeGraphic DIFFER(455v462): counted texture copy, per-call GetDimensions temporaries, closing surface pair | tried: one draft | next: model GetVolumeLevel's second form and two named surfaces
- 00b66c10 CGraphicInfo::GetFrame DIFFER(419v417): dead `add eax,20h`, Data reload for BankFrame, dec-flag release | tried: six (inline helpers, goto-shared load, Release forms) | next: CGraphicInfoFrame-typed this
- 00c2da20 CGuiWindowManager::LoadDrawEnvironment DIFFER(443v448) (needs `/EHsc /GS`): `mov ecx,ebp` hoisted above the Read/strcmp failure branches (two error tails), Size loaded before Offset | tried: novtable class, base dtor for EH states, combined read test, pos forms | next: error count via a helper returning bool, segment fields swapped
- 00e1b750 CHKC_ScreamerPoint::Main DIFFER(503v499): `lea ecx,[esp+18h]` not hoisted above the jne | tried: inline IsThingAlive(const&) | next: lane gap: cross-jump

## Skipped
None. Every row was attempted at least once.

## Label corrections
- 009ecd80 is `CConsole::CConsole()`, not `CHeroInformationScreens::Ctor`.
- 0x0099A2F0 carries the label `NHeroInformationScreens::CBase` but is CBaseObject's constructor (a shared body).
- 00b66c10 GetFrame: `this` is `CGraphicInfoFrame` per the PDB.
- 00c2da20 is not `CGuiWindowManager::LoadDrawEnvironment`. It is a novtable "LiOnHeAd" segment-reader constructor `(void* source, long size)` over CBufferedReader 0x00C3AAC0, and returns `this`.
- 0x00A0BEB0 (label `DrawGetSeaLevel`) is CCamera's near-clip accessor (`fld [ecx+2Ch]`).
- 0x00A14440 is C3DVector::Normalise (catalogued as `GetScaled`).
- 0x006D5AD0 / 0x008AB960 carry propagated `_Cons_val<...>` labels. In 0089b5b0 they are the thing's script-priority accessor and the scripted-thing resource's pointer setter.
- 0x0073A6E0 (label `CountReferences`) is used as the action user's current-action accessor in 008a8c60.
- 0x00C3A9E0 / 0x00C3AA50 / 0x00C3AA30 are the buffered reader's Read / SetRange / GetCount.
- 00ceffb0 really is Tutorial_LowHealth (checked).
