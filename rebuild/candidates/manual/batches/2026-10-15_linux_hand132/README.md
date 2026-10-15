# Hand chunk 132 (CDiskFileWin32 .. CDrawRenown): 32 of 40 byte-exact

All 32 sources grade RELOCATION_MATCH under the Wine lane (grades.txt) and their behaviour tests print PASS under g++
(tall.sh) and the lane's VC7.1 cl.exe (tall.py).

## What matched (32)

- **CDisplayManager** (8 + 1 init):
  - 009c0020 SetExclusiveMode / 009bfe70 ChangePresentationInterval: copy LastDeviceCreationParameters (+0x268: the retail
    DeviceResetCallbacks list is 8 bytes narrower than the PDB) and ResetDevice. Shape: an implicit copy is `rep movsd`; an
    inline memberwise user copy constructor gives the retail field-by-field copy (dead fields dropped); the interval equality is an
    inline `operator==` on PresentImmediate + VSyncCount; SetExclusiveMode wants `if (changed) {...return Reset;} return 2;`.
  - 009be590 GetBestSignedTextureFormat / 009be6c0 GetBestAlphaTextureFormat: walk the 36-byte format table @ 0x0129BA40 and
    CheckDeviceFormat (COM slot 10). Shape: an INDEXED `for (long i = 0; table[i].BitDepth != -1; ++i)` with a reference to the
    record -- the pointer-cursor loop compiles differently (VC strength-reduces the indexed one to &BitDepth for the alpha picker).
  - 009be980 InitVirtualPixels: statement-by-statement double globals with the floor thunk (`#pragma function(floor)`).
  - 009bed10 GetCurrentWindowedSettings: COM GetAdapterDisplayMode + dllimport GetWindowRect; straightforward.
  - 009be490 ResizeWindowToBackBufferSize: dllimport GetSystemMetrics/ClientToScreen/MoveWindow; CSystemManager
    AttachedToExternalWindow is +0xec (PDB +228, retail +8 like the landed IsApplicationActive).
  - 009bf2d0 FlipToGDISurface: `if (ExclusiveMode) while (index != 0) {...}`, Present = device slot 17, D3DERR_DEVICELOST.
  - 009a6a00 CDisplayManagerInit copy constructor: user memberwise copy, CWideString copy ctor last.
- **CDisplayEngine** (engine/CDisplayEngine.h fits): 00434f60 WorldUpdate (`--count == 0` nested in `> 0`, re-read for the second
  test; CIEngine slot 19 GetPrimitiveDrawDistanceFactor). 006977f0 is not CDisplayEngine (see Label corrections): the camera
  view width `2 * dist * tan(fov/2 turns)` -- matches only with `(fov * 0.5f) * (double)TWO_PI` (an all-float product folds the
  two constants), `#pragma intrinsic(sqrt, tan)`, and the camera position copied through a C2DVector-style memberwise copy ctor.
- **CDraw* constructors** (size TU, CDrawStamina family): 0064e1e4 CDrawHealth, 00651904 CDrawGuildSeal, 0064e83f CDrawMiniMap
  (local model: engine/CDrawMiniMap.h is PDB-shaped and 0x20 off after the display), 00651a7e CDrawAbilityButtons. Shape: all
  members in the initialiser list in declaration order; vectors are inline-constructed member objects (zeroed through edi when the
  address is reused as `this` of a Resize); CEnginePrimitiveHandle members need a `__forceinline` constructor AND the counted Data
  as a member object with its own inline constructor (so the shared zero register is set after the handle's vtable store).
- **Stat drawers**: 006441f6 (CDrawMoney::Draw), 006442ae CDrawCrime::Draw, 0064483d CDrawRenown::Draw: PeekGuiDef, the GUI's
  counted stat bar (PMoney +0x2cc / PRenown +0x2d4 / PCrime +0x2ec), bar->Draw slot 0. Shape: the alpha argument must be an
  `unsigned char` parameter (a long parameter moves `mov ecx,ebx` to the last push).
- **CDrawQuestInfo** (9): 00647357 RemoveElement, 00644d91 UpdateBar, 00644efd UpdateMiniGame(float), 00643f13 DisplayQuestInfo,
  00654593 DisplayMiniGameInfo, 00643eb2 DrawBox, 0064f7fc AddBar, 0064f93f AddTimer (catalog AddCounter), 0064f9a7 AddCounter.
  Frame-pointer size TU (`optimize("y",off)` + `("s",on)`) except where noted. Shapes: list through a pointer local + `break`
  after erase; `percentage = Min(percentage, 1.0f)` Dinkumware min over const refs (1.0f temp in the frame); `size() == 1` folded
  to `(last-first) & ~3 == 4`; a long frame-rate local before the def call is homed in the dead bool slot for `fild`; the box copy
  needs `__forceinline` memberwise copy constructors (POD -> movsd x4, plain inline -> out-of-line call in this size TU); the
  adders take the sprite lookups into locals before `new`, wrap the element in CCountedPointer<Derived> and convert it to the
  by-value CCountedPointer<CQuestInfoElement> in the argument area through a member-template converting constructor.
- **CDrawQuickAccessIcons**: 0064d62f ClearPrimitivesIfNecessary (inlined CEnginePrimitiveHandle::Clear: null-safe refcount
  accessor, slot-3 notify, assign a default-constructed counted pointer whose release goes through a LOCAL pointer copy -- all
  helpers `__forceinline`; for_each(mem_fun_ref(&Clear)) x2 with the member pointer hoisted into edi); 0064b668 ClearSoundEffects
  (`ChargingSoundID > 0` on an unsigned = `jbe`; `IS_THING_ALIVE<CThing*>(hero)` on a CThingCreatureBase* binds the reference to a
  converted temporary so the hero stays in esi; presence test and lookup are separate: inline `HasInterface` over the out-of-line
  bitset::test plus a `__forceinline` lookup RETURNING the component, which lands straight in ecx).
- **CDraw2DText** 006541d1 Draw: list size by walking, count local BEFORE the handle-vector pointer local, `*(begin()+i)`.
- 00c689b0 CSkillTable_00c67470::SetSkillPropertiesEx: SetSkillIndexProperty inlined, then GUID (16-byte by value) / value / flag
  setters as nested `if (result >= 0)` blocks (early returns swap esi/edi).
- 005236f0 NThingManager::CEntityRuntimePersistence::Clear: two inlined VC `erase(begin(), end())` (+ DeallocateMemory) and the
  SGI-style tree clear (leftmost, root = 0, rightmost, count).

## Parked for Fable 5.1 (8)

- 0064a3ed CDrawQuestInfo::AddElement DIFFER(99v99): register assignment only -- retail argument object/max/loop id in ebx/edi/esi, ours edi/esi/ebx | tried: size pragma (no-pragma has a 16-byte loop alignment, plain "s" a frame), cached object local, declaration orders, re-read expression | next: lane gap: register allocation (try a `__forceinline` CCountedPointer operator-> returning the object)
- 00652f0a CDrawQuickAccessIcons::CDrawQuickAccessIcons DIFFER(154v154): `push 0Dh` for Effects.Resize scheduled between the 2nd and 3rd EffectPositions zero store in retail, before the 1st in ours | tried: initialiser-list ctor, shared count local | next: lane gap: scheduling (24i family)
- 0064f659 CDrawQuestInfo::CDrawQuestInfo DIFFER(131v131): `lea ecx,[esi+50h]` (Name's constructor) after two of the three OraclePrimitiveHandles zero stores in retail, before them in ours | tried: initialiser list with allocator-ref lists | next: lane gap: lea after register-zero stores (wave 24i)
- 009bee40 CDisplayManager::SetupDepthAndBackBufferTextures DIFFER(101v101): retail loads RenderTargetDimensions.Y into ecx (ours edx) and stores the sunk X before `push 1` with &size in eax | tried: X-first, Y-first (closest), C2DExtentsI(x,y) temporary | next: try the extents as two longs passed to an inline C2DExtentsI builder; else lane gap: scheduling
- 00434c00 CDisplayEngine::StartScreenFadeOut DIFFER(142v144): retail copies the colour's low half (G, B) through ebx after the FadeInTime/FadeOutTime stores; ours copies both halves through edx first | tried: RGBA/BGRA layouts, member-order and body-order copy constructors, two Start orders (CloseTimer reload now matches) | next: model Start out of line? (retail 0x00434840 is the same fade Start) or lane gap: scheduling
- 0099abf0 CDiskFileWin32::SetPosition DIFFER(142v159): the lane drops the dead LARGE_INTEGER high-half store (`cdq ; mov [esp+18h],edx`) and shrinks the 12-byte frame, moving the CBBBFileException object into the dead argument slot | tried: LARGE_INTEGER local, __int64 local, inline FlushAndSeek(__int64) and (LARGE_INTEGER) helpers | next: lane gap: dead-store elimination of a union member (QFE vs RTM)
- 0055a510 NUISystem::CDraggable::ProcessEvent DIFFER(157v157): frame layout only -- retail gives the two GetUIScale result temporaries separate 8-byte slots (sub esp,18h), ours share one (sub esp,10h) | tried: named float scale locals (fixed the code order), empty C2DVector destructor, inline helper temporaries | next: lane gap: temporary slot sharing (QFE/RTM frame-slot gap)
- 0064e019 NPlayerGui::CDrawMiniMap::ToggleCentering DIFFER(135v136): retail this/fade/world in esi/ebp/edi with World loaded into ecx then copied to edi; ours ebp/edi/esi loading World straight into esi | tried: fade pointer local, declaration orders, __forceinline CWorld::GetCurrentRegion accessor (5 grades) | next: lane gap: register allocation

## Skipped

None: no row spans two functions.

## Label corrections

- 006977f0 "CDisplayEngine::CDisplayEngine::GetSeaLevel" -> CGameCameraManager view-width helper (named GetViewWidthAtPosition):
  0x00A0BEB0 is a shared `fld [ecx+2Ch] ; ret` getter (ICF: CMap::DrawGetSeaLevel == CCamera::GetHorizontalFOV), called on the
  manager's Camera at +4; the body sits among the CGameCameraManager methods.
- 006441f6 "CDrawCrime::Draw" -> CDrawMoney::Draw (bar = CPlayerGui PMoney +0x2cc, def records = PDB Money*); 006442ae is the
  real CDrawCrime::Draw (PCrime +0x2ec).
- 0064f93f "CDrawQuestInfo::AddCounter" -> AddTimer (allocates 0x24 bytes and builds CQuestInfoElementTimer, vtable 0x0125A458).
- 005236f0 "CDrawQuestInfo::OracleSymbolPopBack" -> NThingManager::CEntityRuntimePersistence::Clear (callers
  LoadFromGameStateScript 0x005248B0 etc.; clears two CArray<CEntityScriptInfo> and the dead-entity map).
- 00c689b0 "CDrawGuildSeal::SetSkillPropertiesEx" -> skill table (class named by address as the landed 0x00C67470).
- Callees: 0x00656197 "ListPushFrontWrapper" is list push_back (inserts before the head); 0x0064A4E9 "_Pair_base<...>" is the
  CQuestInfoElementTimer constructor; 0x00656BA2 "CTCAtmosPlayer" is CQuestInfoElementCounter's constructor; 0x0082A460
  "CHeroMorphDef" is CMiniMapDisplay's constructor; 0x00434840 "CEngine::StartNewScene" is CFadeInFadeOutBase::Start(in, out,
  colour); 0x0043F360 is SGI for_each (mem_fun_ref Clear); 0x0099EAE0 "_Cons_val<...>" is ~CCharString; 0x0066AD90
  "CGameScriptThing::SetToKillOnLevelUnload" is a sound component StopSound(id); 0x004FB150 "CWorld::DrawGetEnvironment" is a
  +0x9c getter used as the world map's current region index; 0x0052BD70 "_Copy_backward<CPixelFormat*>" is the 24-byte
  element _Destroy.
