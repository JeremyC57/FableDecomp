# Hand chunk 143 (CGuiVarTransferStruct .. CInputProcessControlCreature): 31 of 40 byte-exact

31 matched (src/ + test/, every test PASS under g++ and the lane's VC7.1), 8 parked (differ/), 1 skipped.
Six catalog labels in this chunk were other functions (see Label corrections); the sources carry the real names.

## What matched (31)
- **CHandleBasedMemoryPool** (0x00A0xxxx build; 6): CombineAreas, Uninitialise, EndShutdown, Initialise,
  InitialiseFromPreallocatedBuffer, InternalAllocateHandle. Local model (the engine header puts the critical section
  at +0x450 behind a 16-byte CArray; retail has it at +0x44c). The free-list unlink is an inline helper applied to
  both areas, and the physical successor must be a named local. GFFreeFromGlobalAllocator / GFAllocateFromGlobalAllocator
  are inlined: the free one repeats the caller's null test. Alignment is `(x + 15) / 16 * 16`, because the `>> 4 << 4`
  form folds into `and eax,-16` and retail keeps `shr/shl`. In the preallocated form the start and end live in unsigned
  locals that are modified in place, and the allocation arguments are read back from the members (`PoolSize + 16`,
  `MemoryFlags`) just after storing them. InternalAllocateHandle uses the PushBackConstant inline push_back (tag at frame
  byte 7, value at +8).
- **CInputEvent** (3): the copy constructor copies every union view in order (word 0 twice, eight words, the key word,
  the joystick C2DVector and button byte, the mouse word, then the scalars), and VC keeps all the aliasing stores.
  CompareEvents / LessThanComparator are `__fastcall` statics over a 16-byte key. The first-word getter 0x00C25C60 is
  defined out of line above them under `auto_inline(off)`, so edx survives both calls. The `return true` tail sits at
  the end only when written `if (index == 0) { ... } return true;`.
- **CInputManager** (3): IsJoystickEventInQueue uses the STLport `*(begin()+i)` element access, and the implicit
  assignment of CInputEvent (which has a copy constructor but no operator=) gives `rep movsd` x13. In ClearEvents and
  SetEventsFromEventPackage, `clear()` is the inline `erase(begin(), end())`. The joysticks are a vector with
  `operator[]` and `size()`; raw pointer indexing leaves a dead `lea`.
- **CInputManagerDX::~CInputManagerDX**: the scoped_ptr reset (store before the null test). The VC7.1 erase-all
  (`_Move(last,last,first,tag,false)` + `_Destroy(kept,last,tag)`) is written as an inline member of the vector.
  Written in the destructor body instead, the last-pointer store sinks below the DirectInput load. Release goes through
  COM slot 2.
- **Entity scripts** (CHHUndead::Main, CHeroSoulsEnemy::Main, CHTG_AmbushPoint::Main, CHS_Screamer::OnPredicateFail):
  these follow the landed CHTE_Prisoner::Main model, with CGameScriptInterface slot names taken from
  rtti-vtables.tsv + the manifest (7 NewScriptFrame, 70 GetHero, 89 SetTimer, 108 RemoveThing, 376 Pause,
  451 GiveThingBestEnemyTarget, 543 EntityEndScreamerSuperAttackThing). In CHHUndead the filter destructor is out of
  line (0x00E241B0), not the inline vtable-store form. The ambush point passes `SpawnEnemies(Me.GetPos(),
  Me.GetDataString(), 4)`, and the CCharString is returned straight into its argument slot (`push ecx ; push esp`).
- **CInputProcess\*** (CameraLookAround, Combat, ControlCreature):
  - The camera source is read into a local before the null-initialised out pointer, otherwise the zero store hoists.
  - The game-player interface is read into a local before the player-number call (edi).
  - A "clear when released" store of the call's zero `al` comes from `bool held = ...; if (!held) m = held;`.
- **Constructors / destructors**:
  - CHeroStatsDef: `#pragma optimize("s", on)` makes each vector past +0x80 a single `lea ecx`.
  - CMiniMapDisplay (labelled CHeroMorphDef): a VC6 list sentinel, plus a CFadeInFadeOutBase whose colour is a frame
    temporary `CRGBColour(R,G,B,A)`. The colour needs a user default constructor, or an init-list zero fill appears.
  - CWeaponDef (labelled CHeroExperienceDef): a novtable derived class over the inline CDefPointeeBase destructor.
  - CIcicleData copy ctor: C3DVector needs a memberwise user copy constructor (a POD member copies through `lea`).
  - The OnAppearanceDraw record (labelled ~CHitLocation): inline tree destructor + out-of-line member + IV counted
    pointer.
- **Misc**:
  - CGuiVarTransferStruct::Add: the prev link is read into a local before the list-link stores; the count block is
    {count, destroy fn, object}.
  - CHeightMap::Update: the cell pointer is computed per row and incremented, which gives the frame-spilled area index.
  - CHitLocation::SetEnabled and CHitThing::ModifyHeroStats use the unset-local component accessor, which reads the
    dead argument slot.
  - C3DPrimitive2::FreeData (labelled ~CInputManagerDX): the early-out POD copy helper gives `cmp eax,eax ; jne`.
  - The insertion-sort step (labelled CHitParameters::SetPHitMaterialDef): a `__fastcall` with a by-value pair, its
    tag homed in the dead predicate slot.

## Parked for Fable 5.1 (8)
- 00a01ed0 CHandleBasedMemoryPool::FindFreeArea DIFFER(122v122): retail `mov ebx,edx ; cmp ebx,100h`, lane `cmp edx,100h ; mov ebx,edx` before the outer loop | tried: if-clamp, ternary loop initialiser, if/else into a separate index, inline static clamp helper, split `for(;;){ if (best) break; }` | next: lane gap: copy-propagated loop-guard register (scheduling)
- 00c27fd0 CHandleBasedMemoryPool::FindFreeArea DIFFER(116v116): same `mov ebx,edx ; cmp ebx,100h` vs `cmp edx,100h ; mov ebx,edx` (second pool build, free lists at +0x3c, no flags test) | tried: ternary initialiser, const-ref Min template (adds a frame) | next: lane gap: same as 00a01ed0
- 008f6050 CHeightMap::Clear DIFFER(123v109): retail hoists 0 into ecx (`cmp eax,ecx` loop guards, `mov [eax],ecx`, `mov [eax+8],cl`) and stores 0xff immediate; lane hoists `mov cl,0FFh`, uses `test`, and forms the memset address in ebp | tried: plain stores + memset, inline cell Reset(), 0.0f + sizeof memset + outer-declared indices, Height as unsigned long | next: lane gap: constant-register choice (maybe a cell assignment from a const default cell)
- 0068c6e0 CInputProcessClickPastText::ProcessInput DIFFER(128v126): `push ebx ; mov ebx,[esp+38h]` (processed cached at entry) vs retail re-reading `[esp+3Ch]` | tried: named CGameEvent | next: lane gap: /GS parameter caching (GOTCHAS 23e)
- 0068c760 CInputProcessBButtonExitMode::ProcessInput DIFFER(135v131): same /GS caching of `processed` in ebx (frame +4) | tried: named event + inline AddLong | next: lane gap: /GS parameter caching
- 0068e2c0 CInputProcessBlock::ProcessInput DIFFER(107v103): same /GS caching in ebx | tried: named event, unnamed temporary (no caching but no cookie: 85v103), `__forceinline` QueueGameEvent helper | next: lane gap: /GS parameter caching
- 0068e330 CInputProcessBlock::Update DIFFER(111v103): `push esi ; mov esi,[esp+34h]` caching under /GS; without /GS the body matches retail minus the cookie | tried: ternary constructor argument (becomes `neg/sbb/and/add`), one event per if/else arm (right structure) | next: lane gap: /GS parameter caching
- 00691560 CInputProcessBetting::ProcessInput DIFFER(108v105): retail loads the component `mov ecx,[eax+4]` before the flag byte `mov al,[13BAE1Dh]`; lane loads the flag into cl first, then `mov ecx,eax` | tried: local + nested if, inline member holding the flag test, volatile flag | next: lane gap: load ordering across the flag test (try the out-parameter GetInterface form)

## Skipped
- 009f5d70 CInputManager::LoadAllEventPackages: the 121-byte row is only the prefix of an `/EHsc` try/catch function. The body runs past the catch funclet at 0x009F5DE9 (catalogued separately) to 0x009F5E0B, so the row cannot be graded as one function.

## Label corrections
- 0x004E3D4A "CHeroExperienceDef::~CHeroExperienceDef" is **CWeaponDef::~CWeaponDef** (only caller is the deleting destructor 0x004E3D2E, slot 0 of CWeaponDef's vtable 0x0124291C).
- 0x0082A460 "CHeroMorphDef::CHeroMorphDef" is **CMiniMapDisplay::CMiniMapDisplay** (stores vtable 0x01231768 = CMiniMapDisplay).
- 0x00755520 "CHitLocation::~CHitLocation" is the destructor of a stack record in **CTCLook::OnAppearanceDraw** (0x007542B0). CHitLocation is 24 bytes; its real destructor is the landed 0x00710900.
- 0x005E26E0 "CHitParameters::SetPHitMaterialDef" is an **insertion-sort step** of `_Insertion_sort1<pair<long, CDefPointer<CContextSensitiveItemDef const>>*, CContextSensitivePriorityCompare>` (0x005E2E00). The real SetPHitMaterialDef is the landed 0x00868640.
- 0x00A901E0 "CInputManagerDX::~CInputManagerDX" is **C3DPrimitive2::FreeData** (no vtable store; called by C3DPrimitive2's destructor 0x00A90790 after storing vtable 0x0129DF94).
- 0x00C25CE0 / 0x00C25D50 "CInputEvent::CompareEvents / LessThanComparator": `__fastcall` comparators over a 16-byte key ({block, ref, index, kind}). They are called by the `__stdcall` wrappers 0x00C24760..0x00C247C0 on `&x->key` (+0x18), so the CInputEvent attribution is doubtful (kept the catalog module).
- Callee labels:
  - 0x00687A90 is the kind-0x13 button test (labelled IsEventButtonHeld like 0x00687BD0, which tests kind 0x15).
  - 0x00687D00 is the game-action analogue-vector query, and 0x00A0D390 is CProcessedInput::SetHandled.
  - 0x00445B90 / 0x00445BB0 are the game player interface's mode-forced getters (labelled CTCShotDefinitionBase::CanBeHitByMeleeStrike).
  - 0x00485D30 is `vector<CEnginePrimitiveHandle>::vector(size_type)` (labelled map::operator[]).
  - 0x00A03B40..0x00A03B90 are CInputEvent getters (Type +0x28, DeviceType +0x20, DeviceNumber +0x24, joystick button byte +8), not CWorld::DrawGetEnvironment.
  - 0x009F6890 is `vector<CInputEvent>::operator=`, and 0x009E42C0 / 0x00A66C00 / 0x00A66860 are the joystick / mouse / keyboard Clear.
