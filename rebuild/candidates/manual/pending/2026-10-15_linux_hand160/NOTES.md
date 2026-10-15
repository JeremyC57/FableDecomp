# Hand chunk 160 (CSurface .. CTCBuyableHouse): 30 of 40 byte-exact

All 30 sources in src/ grade RELOCATION_MATCH (grades.txt) and their tests PASS under g++ (tall.sh) and VC7.1 (tall.py).
9 rows parked in differ/, 1 row skipped.

## What matched (30)
- **CSurface** (0x9F3230 InitialiseAsImage, 0x9F32A0 InitialiseAsDepthBuffer, 0x9F3310 InitialiseAsRenderTarget, 0x9F4120 SetPixel):
  release + Create*Surface on the system manager's device. Shape: the inline Uninitialise leaves &PD3DSurface in edi for the
  out pointer; the D3D format lookup is a named local after the device fetch; Image/RenderTarget read the extents into locals
  BEFORE the device call, DepthBuffer reads them inside the call. SetPixel needs the destination pointer as a named local
  (inline it folds PMem into the add and drops the `mov ecx,eax`). Accessors GetPixelFormat as `return CPixelFormat(desc.Format)`
  with an inline ctor calling Initialise (a local + Initialise copies the format to a second slot).
- **CSurfaceCompositor** ctor (0xF38990): init list StateCaptureBlock (inline zeroing ctor), DestSurface, CCountedPointer; body
  `RenderTarget.Reset(new CRenderTarget)` with inline CRenderTarget/CSurface default ctors; CurrentBlend/LayerCount stored last.
- **CSuspendableProcessMicrothread::Restart** (0xA447D0): `#pragma optimize("", off)` TU like the landed ctor/dtor.
- **CTCTavernGameCardBlackjack::EndRound** (0x7DAAE0, label fix): two `clear()` = erase(begin,end) with the early-out
  copy-down (landed 0x567AC0 shape); CTCTavernGame must be the polymorphic root or `this` is adjusted by +4 for SetState.
- **CSystemManager** (0x9A59A0 SetExclusiveMode, 0x9A7440 ctor): two-pass `for` with a frame-spilled old mode; the ctor is
  member ctors only (retail has one extra counted pointer at +0x84, 8 bytes above the PDB from there on).
- **~NCameraModes::CBase** (0x56E0E0, label fix): CDefPointer dtor `if (D) { D->ReleaseTemporary(); D = 0; }` gives
  `dec [ecx+4] ; jne` (writing the decrement in the dtor re-reads the pointer).
- **~NInventory::CTCInventoryBase** (0x5C00A0, label fix): size TU, `__forceinline` POD-vector and CDefPointer dtors,
  out-of-line container dtors, RemoveFromGlobalTCs() as the dtor body.
- **CTCActionUseReadable::OnActionUse** (0x7E80C0): the creature action is a TEMPORARY passed by reference straight from the
  ctor's eax (`SetCurrentAction(CCreatureAction_Read(*creature, TextTag))`), its class `__declspec(novtable)` so the inline dtor
  stores no vtable; `typedef unsigned short wchar_t` for the L"" literal.
- **AddCameraKey** (0xB317A0, label fix): `__fastcall` free helper; `key.V = C3DVector(a,b,c)` with ONLY the 3-float ctor gives
  retail's frame temporary copied through integer registers (a user copy ctor/operator= turns it into x87 code).
- **CTCActionUseScriptedHook** ctor (0x729A90): one init list, CIntelligentPointer(T*) inline ctor with SetPItem(0).
- **CTCActivationTrigger::OnInitialActivate** (0x77B1C0) and **CTCBattleCharge::OnInitialActivate** (0x7A7370): slot 6 is the
  CTCBase OnInitialActivate slot; GetInterface results used straight; `ReceptorUID > 0` on the unsigned 64-bit UID; the UID
  pushes precede the accessor call (arguments before the object expression).
- **~CHeroCombatDef** (0x458B9D, label fix): the landed def-dtor recipe (size TU, `__forceinline` POD vectors, out-of-line maps).
- **~CEnginePrimitiveMorphedAnimatingMesh** (0x709B10, label fix): inline STLport set dtor (`if (count) { _M_erase(root);
  leftmost = header; root = 0; rightmost = header; count = 0; } if (header) free(header);`), novtable CBase-derived interpolators.
- **CTCAvoidRegionExit::GetPosition** (0x92BD80): `HasPosition() ? m->GetPosition() : C3DVector(0,0,0)` joins on an address;
  the memberwise user copy ctor gives the edx copy (implicit copy uses esi).
- **CTCBase** (0x6869E0 EndSerialising, 0x686FA0 Clone): EndSerialising's sum is a named local (inner temporaries die first);
  Clone passes the name temporary into an inline static `AllocTC(const CCharString&, CThing&)` (VC keeps the returned pointer
  in esi), owner read into a local first, CCharString operator< written `o.PString == PString` first.
- **CTCBoastingArea** (0x7827D0 / 0x782860), **CTCBoastingCrowdControl::GetCrowdGroup** (0x783730), **CTCBoastingPosition**
  (0x781FE0 / 0x782060): GetInterface template; GetCrowdGroup needs nested ifs with one shared `return false` (an early return
  opens a separate exit before the pushes); BoastingPosition is a `#pragma pack(1)` model (pointer at +0x2E) with an inline
  KillParticleEffect helper.
- **CTCBoss** (0x7AEF60 ClearEffects, 0x7AEFD0 ctor, 0x7AF050 FrameUpdate): `IsThingAlive(Effects[i])` helper; the ctor's
  vectors as `_Vector_base` + `_STLP_alloc_proxy` (fixes the `lea ecx` placement); FrameUpdate's Bind2 predicates must be
  argument temporaries (named locals swap their frame slots); C3DVector declared packed for the unaligned PhaseStartPos.
- **CTCBuyableHouse** ctor (0x6BF8A0): one init list, default CIntelligentPointer ctor (no SetPItem).

## Parked for Fable 5.1 (9)
- 009f33e0 CSurface::Lock DIFFER(139v141): retail keeps desc.Width/Height in ebx/ebp loaded after `mov esi,[esi+4]` and stores rect bottom/right from them after the pushes; failure arm `xor eax,eax ; mov edx,eax` | tried: direct desc fields, width/height locals, rect stores top/left/bottom/right, C2DExtentsI local | next: build the result through a `CSurfaceLockedRect` default ctor / a C2DExtentsI(0,0) temporary bound by reference
- 009f40a0 CSurface::Clear DIFFER(120v120): scheduling only, retail does both imuls before `mov edi,[locked.PMem]` | tried: X*Y*depth order, named byte count | next: the destination as a named pointer local (the fix that matched SetPixel 0x9F4120)
- 009a4f20 CSystemManager::ProcessWindowMessages DIFFER(141v141): register rotation only (retail eax/ecx/edx for the lea temporaries, ours ecx/edx/eax) | tried: if/continue form, MSG declared first | next: lane gap: register rotation (compiler build)
- 007eb110 CTCActionUsePickUp::OnActionUse DIFFER(150v138): local def release, retail `mov ecx,[esp+8] ; test ; je ; dec dword ptr [ecx+4] ; jne`, ours the register form `mov esi,[eax+4] ; ... ; dec esi ; mov [eax],esi`; retail zeroes the def after pushing its address | tried: dtor with/without zero store, GetSubDef by pointer/reference, action named vs temporary | next: `T* p = Data; if (p && --p->RefCount == 0) p->Destroy();` (0x7EB1A0's last try got the memory-form dec)
- 007eb1a0 CTCActionUsePickUpGenericBox::OnActionUse DIFFER(147v138): same release; best form `if (Data && --Data->RefCount == 0)` gives `dec [eax+4]` then a reload | tried: ReleaseTemporary helper, __forceinline dtor, const def + mutable count, direct decrement | next: the decrement through a local pointer copy in ecx, or GetSubDef wrapped in an inline helper that returns the def pointer by value
- 00729b20 CTCActionUseScriptedHook::OnActionUse DIFFER(125v150): our two SetText arms cross-jump into one tail; retail keeps two complete calls with their own `ret 4` (arm 1 font in eax + UI before the pushes, arm 2 font in ecx, UI after) | tried: inline font, font locals, explicit return, UI local | next: write the two arms through different inline helpers (e.g. a `ShowConfirmation()` member for arm 1) so they cannot merge
- 007ebab0 CTCActionUseWindow::OnActionUse DIFFER(124v122): retail this=esi/user=edi and the camera vcall `mov ecx,[eax+34h] ; mov eax,[ecx] ; lea edx`; ours swapped registers and `mov eax,[eax+34h] ; mov edx,[eax] ; lea ecx ; mov ecx,eax` | tried: camera local, creature for the physics call, block 2 as an inline member | next: lane gap: register allocation (or the camera reached through an inline accessor)
- 007f9010 CTCAnimatingObject::PlayAnimation DIFFER(131v131): priority in ebp where retail uses ebx, nothing else | tried: CAnimInit temporary vs named (named fixed the store order), default ctor + Set, early return | next: lane gap: callee-saved register choice
- 0056a8c0 CTCBoastUI::DrawPreparePrimitivesForRendering DIFFER(104v104): retail spills the hoisted info.Renderer into the dead `info` argument slot (`mov [esp+14h],eax`) and reads each element at the loop top; ours keeps it only in ebp and rotates the first element load before the loop | tried: renderer local, per-call read, 4-byte struct by value, const member | next: lane gap: a short-lived local kept in a dead argument slot

## Skipped
- 0080aad1 CTCActionUseBuyHouse::OnCreate: not a function entry. It is the else branch of 0x0080AA60 (`je 0x80AAD1` at 0x80AA77),
  starts with no prologue right after that function's `ret 50h` and pops edi/esi it never pushed.

## Label corrections
- 0x007DAAE0 "CSwappingStateComponent::Initialise" is CTCTavernGameCardBlackjack::EndRound (vtable 0x0126F8F4 slot 33; base 0x008F1720).
- 0x0056E0E0 "~CTCActionUseBed" is ~NCameraModes::CBase (deleting dtor 0x0056E1F0 = slot 0 of 0x0124DD74).
- 0x005C00A0 "~CTCActionUseBuyOrSteal" is ~NInventory::CTCInventoryBase (vtable store 0x01253DBC).
- 0x00B317A0 "CTCActionUseReadable::OnCreate" is an unreferenced `__fastcall` CEngineCamera debug-script helper (named AddCameraKey).
- 0x00458B9D "CTCAssassinRush::FinishMove" is ~CHeroCombatDef (deleting dtor 0x00458B81, Sizeof 0x00458B7B = 0x148, vtable 0x01234CF4).
- 0x00709B10 "~CTCAssassinRush" is ~CEnginePrimitiveMorphedAnimatingMesh (callers destroy the object built by ctor 0x00709A50).
- 0x0077B1C0 / 0x007A7370 "vfunc_6" are OnInitialActivate (CTCBase slot 6); 0x007EB110 / 0x007EB1A0 / 0x007E80C0 / 0x007EBAB0
  "vfunc_28" are OnActionUse(CThing&).
- 0x0080AAD1 "CTCActionUseBuyHouse::OnCreate" is a branch target inside 0x0080AA60, not a function.
- Callees: 0x00A14440 "C3DVector::GetScaled" is C3DVector::Normalise() (in place); 0x00686A70 CTCBase::GetWorld; 0x00686AE0
  "GFGetPreMainMemoryUsed" is a CTCBase global accessor (`mov eax,[0x13B8A1C]`); 0x0051ED80 "CStopWatch::GetTicks" is the
  thing-by-UID lookup; 0x007F7810 is the activation receptor's trigger registration; 0x00693EF0 "~CPhysicsMeshInfo" is
  ~CCreatureActionBase; 0x007EB030 / 0x00843700 are the AddRealObjectToInventory / PickUpGenericBox action ctors (0x00843700
  carries the other's label); 0x007EBA20 "CCreatureAction_AlertToPlayer" is the CCreatureAction_LookThroughWindow ctor;
  0x004FAC70 "CFourierAnalysis" is the fibre exception ctor; 0x0099AED0 "CCharString::CCharString" is CWideString's default
  ctor; 0x00686ED0 "std::list::push_back" is CTCBase::AddToGlobalUpdateTCs; 0x00686D60 "CViewBase::GetDisplayEngine" is the
  CTCBase main-game-component accessor (+0x1614C font bank); 0x00686D40 the player-manager accessor; 0x00451763 / 0x004517A4 /
  0x005C1E30 / 0x005C2C1F "CActiveFile::OnReadFinished" and 0x005C0511 / 0x005C0563 "_Dest_val" are container dtors;
  0x0056CF20 "SortTreeRecursively" is ~list<pair<C3DVector,float>>; 0x00B31C10 "OnInitLeaf" is a push_back.
