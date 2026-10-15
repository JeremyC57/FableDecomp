# Hand chunk 74 (CScriptThing .. CShaderRenderManager): 35 of 40 byte-exact

All 35 authored rows grade MATCH / RELOCATION_MATCH from the exact file in `src/` (`grades.txt`), and every
behaviour test prints `<addr>_TEST PASS` under g++ (`tall.sh`) and under the lane's VC7.1 cl.exe (`tall.py`).
The other 5 rows span two or more functions and are skipped. No row was parked.

## What matched (35)

- **CScriptThing** (pimpl handle: vptr + `CCountedPointer<CScriptThing> PImp {Data +4, Info +8}`; every virtual
  forwards to the SAME slot of the CScriptThing in `PImp.Data`)
  - 004aaa40 `GetAngleXY` (slot 10): 0.0f when there is no implementation, else forward. Needs a named `float`
    local for the result; returning the call directly makes VC7.1 emit a sibling-call `jmp` where retail has `call ; ret`.
  - 004abe90 copy constructor: the vtable store comes first because the copied words belong to the PImp
    member's inline copy constructor (the CBase vtable store is dead and dropped).
  - 004abf80 `MsgWhoKnockedMeOut` (slot 33, CScriptThing returned through the hidden pointer): `return *this`
    for a null handle, with the copy constructor inlined. Read the implementation pointer into a local so it
    stays in eax (`mov ecx,eax`); reading the member again reloads `[ecx+4]`.
  - 008ab980 `operator=`: inlined `CCountedPointer::operator=`. Both source words are read into locals first,
    in load order (object, then block). The count block is `{refs, __fastcall destroy fn, object}`.
  - 00704730 is really `CGameScriptThingManager::GetNullScriptThing()` (PDB name): it returns a copy of
    `NullThing` at +4 (the retail list head is 4 bytes) with the copy constructor inlined.
  - 0050eef0 is really a thing filter's `operator()(CThing*)`, the second filter bound by two GetAllThings
    instances: `physics = thing->PhysicsTC; if (physics && physics->Flag()) return thing->CanBlockNavigatingCreature();
    return thing->CanBlockNavigatingCreature();`. VC7.1 keeps both identical tails (no tail merge). The
    component pointer must be a named local to load straight into ecx.
  - 00a0a550 / 00a0a610 (byte-identical twins): Sizeof of a `{CCharString, CCountedPointer<T>}` record,
    `return Pointer.Sizeof() + Name.Sizeof();`, where the pointer's inline Sizeof is
    `Data ? Data->Sizeof() + sizeof(*this) : sizeof(*this)`.
- **CScriptedMapBrush** (retail layout = PDB minus 0x10: GameArea `C2DBoxI` +0x38, Cells first +8, 20-byte cells)
  - 0082e290 `GetGameHeight`, 0082e2e0 `SetGameHeight`, 0082e380 `SetTheme`: the bounds test is an inline
    `C2DBoxI::Contains(x, y)` and the row width an inline `Width()`, indexing
    `Cells[(y - TLY) * Width() - TLX + x]`. That gives retail's memory-operand compares, the re-read of TLY
    and `mov esi,edi ; sub eax,esi`. Testing the members directly makes the lane cache TLX/TLY and push ebx.
    I calibrated the model on the landed sibling GetThemeBlend 0x0082E3D0, which then MATCHes (its landed
    plain-member source does not).
  - 0082ea50 `SetThingToMove`: `ThingsToMove[id].Position = pos; ThingsToMove[id].FacingAngles = angles;`.
    That is two map `operator[]` calls (0x0082FB90), and the 12-byte PODs are copied word by word.
- **NUISystem components** (retail = PDB minus 0x30; three vtables; members in the init list are stored after the vtable stores)
  - 00559830 `CScrollBar(long)` and 00559360 `CScrollBarOutside(long)`: base constructor, zeroed members in
    the init list, then `GetDataFromDef()` in the body.
  - 00546b00 is `CZoomingComponent(long)`: the float zeros share the integer zero register.
  - 00547600 is `CSwappingStateComponent(long)`: the SwappingStates member is an **STLport** vector.
    `_M_start(0), _M_finish(0)` are plain initialisers and the end pointer is an `_STLP_alloc_proxy` member
    object built from (allocator, 0). That member constructor is what places `mov ecx,esi` between the 2nd and
    3rd zero store. One three-pointer init list, with or without a defaulted allocator argument, puts it first.
  - 00559600 `CScrollBar::Update(float)`: `if (WasDragged && !WasLeftClicked) { WasDragged = false;
    ChangePosition(PositionBeforeDrag, 0.0f, false); ++Layer; } CDraggable::Update(dt);`. Layer is the
    CComponent char at +0x12F (PDB +0x157).
- **CScrollingViewport / CTC constructors**
  - 004e0c61 is the `CTCHaste(CThing*)` constructor and 004e5c2f the `CTCCreatureGroup(CThing*)` constructor.
    Both are size TUs (`#pragma optimize("s", on)`: `push dword ptr [esp+8]`, `or [..],-1`, `and [..],0`); the
    map member's constructor is called out of line last.
  - 0053ed40 `CScrollingViewport::GetDataFromDef()`: `CDefPointer<const CUIDef> def = LoadAndReturnDef();`
    (virtual slot 108), then a dword copy of ScrollingSpeed and a struct copy of TextWindow. The release goes
    through the inline DecRef/GetRefCount pair (wave 18 shape), and the destructor's `if (Data)` folds away.
  - 0053fc30 the scalar deleting destructor: an inline `__forceinline virtual ~CScrollingViewport()`
    (`GetInstance()->RemoveObserver(this); if (g_active == this) g_active = 0;`), plus a `new` factory with the
    constructor inline (the CMouseCursor 0x0055C8D0 recipe).
- **Entity scripts** (PDB: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8})
  - 00dc3e50 `CSecretPassageSkeleton::Init`: `for (i < 4) returned[i] = false;` compiles to ONE dword zero
    store; then four `skeleton_object[i] = "OBJECT_CEREMONIAL_SKELETON_..."` and `collected_all = false`.
  - 00ec0b00 `CSecretWhisperingFriend::Init`: `Game->Slot477(Me, 1); StartPos = Me.GetPos();`. The call on
    the polymorphic member object stays virtual.
- **Unattributed**
  - 00c208e0 (no-RTTI 0x00C2 module): `ChangeState(4); owner->m_observers.Register(this);
    m_link.Bind(owner, &m_record); m_state = 1;`. Both embedded objects are member OBJECTS.
  - 00a35330: a sorted container's `Sort()`. It calls the /Gr SGI sort over `[data, data + count)` with the
    empty comparator passed by value (`xor eax,eax ; mov al,[esi] ; push eax`), then clears Dirty.
  - 0059b4fe: the **STLport `deque<T*>` copy constructor**, a frame-pointer size TU (`optimize("y",off)` +
    `("s",on)`). The allocator and `__false_type` temporaries both sit at `[ebp+0Bh]`. The iterator has a user
    default constructor, so it returns through the hidden pointer in ecx (`__fastcall`), and an implicit copy,
    so the three by-value iterators are `movsd` block copies.
- **CSetTextInfo**
  - 0048ddf6 value constructor: frame-pointer size TU (`optimize("y",off)` + `("s",on)`), with pass-through
    `push dword ptr [ebp+N]` into four CWideString copy constructors.
  - 004935a7 copy constructor: memberwise in declaration order. It needs `optimize("s",on)`; /O2 gives the same
    length in a different order.
- **Shader classes**
  - 00a61f20 `CShaderBankStateBlock::GetDefault`: `memset(&state, 0, 8)` is the source of
    `lea eax ; mov esi,eax ; mov [esi],edx ; mov [esi+4],edx`. `state = CStateBlock()` stores through the
    argument register instead.
  - 00f39d40 / 00f39d90 `ConvertToGenericLight` (directional / point): plain field stores in source order.
    The 0.0f W and the zero type share edx, and `1.0f / Attenuation.Attenuation2` has its `fld` hoisted to the top.
  - 00987e50 is the out-of-line `CPixelShader` handle assignment: release the old data through a LOCAL copy
    (`delete` folds its null test into the enclosing `if`), adopt and `++ReferenceCount` with no null test.
  - 00988140 `ApplyPixelShader(const CPixelShader&)`: `if (shader.Data != Current.Data) { Current.Assign(shader); UpdateFlags |= 3; }`.
  - 00988540 `SetProjectionTransform`: a 64-byte POD copy (`rep movsd`), then `CMatrix4x4::Transpose()` in place, then `UpdateFlags |= 0xC990`.
  - 00988680 is really `SetAmbientColour`: `AmbientColour = c; AmbientColour.A = 1.0f; UpdateFlags |= 4;`.
    The dead first store of A is kept.

## Parked for Fable 5.1 (0)

None.

## Skipped

- 0088b7e0: the row covers 0x0088B7E0 (null-tested slot-6 forward) plus padding plus 0x0088B800 (`mov eax,[ecx+3Ch] ; ret`).
- 008aae20: the row covers four bodies (0x008AAE20 slot-78 tail forward, 0x008AAE40 negated compare, 0x008AAE60 and 0x008AAE70 getters).
- 00c3eb60: the row covers two bodies (0x00C3EB60 slot-8 forward, 0x00C3EB80 slot-9 forward).
- 00c40e20: the row covers four bodies (a slot-6 forward, the `fpatan` / `fyl2x` math helpers, and a `call` helper).
- 00ca5510: the row covers four bodies (a `[eax-8]` forward, a byte getter, a `lea` getter, and an import call with `/ 10000`).

## Label corrections

| Address | Manifest label (propagated) | Actual |
|---|---|---|
| 0x0050EEF0 | CScriptThing::IsAlive | operator()(CThing*) of a thing filter, the second filter bound in GetAllThings 0x00823720 / 0x008266E0 |
| 0x00704730 | CScriptThing::CScriptThing | CGameScriptThingManager::GetNullScriptThing (`?GetNullScriptThing@CGameScriptThingManager@@QAE?AVCScriptThing@@XZ`) |
| 0x00A0A550, 0x00A0A610 | CScriptThing::GetCurrentScriptPriority | Sizeof of a `{CCharString, CCountedPointer<T>}` record (pair-like) |
| 0x004E0C61 | CScrollingViewport::CScrollingViewport | CTCHaste::CTCHaste (vtable 0x012423CC) |
| 0x004E5C2F | CScrollingViewport::CScrollingViewport | CTCCreatureGroup::CTCCreatureGroup (vtable 0x0124326C) |
| 0x00546B00 | CScrollBarOutside::CScrollBarOutside | NUISystem::CZoomingComponent::CZoomingComponent |
| 0x00547600 | CScrollBarOutside::CScrollBarOutside | NUISystem::CSwappingStateComponent::CSwappingStateComponent |
| 0x00C208E0 | CScrollableList::OnDeactivate | state-machine body in the no-RTTI 0x00C2 module (class unknown) |
| 0x00A35330 | CSetIllustratorAsDrawing::operator() | sorted-container Sort() over 56-byte records (twin 0x00A35310: 20-byte records) |
| 0x0059B4FE | CShaderRenderManager::SetD3DMatrix | STLport `deque<T*>` copy constructor |
| 0x00987E50 | CShaderRenderManager::ApplyPixelShader | CPixelShader handle assignment (`this` is the handle) |
| 0x00988680 | CShaderRenderManager::SetViewTransform | CShaderRenderManager::SetAmbientColour |
| 0x0082FB90 | CThing_CalculateMovementTiming | `map<unsigned __int64, CThingPosAndOrientation>::operator[]` |
| 0x0059A8F3 | CThingMapManager::GetMemoryFootprint | STLport `_Deque_iterator::operator-` (32 four-byte elements per block) |
| 0x0059AEF9 | CEngineInterpolationInfo::CEngineInterpolationInfo | STLport `_Deque_base(const allocator&, size_t)` |
| 0x0059B49B | CEnginePrimitiveRenderer::AddStaticMesh | `__uninitialized_copy` for deque iterators (`__fastcall`, hidden result in ecx) |
| 0x00A5CAB0 | C3DMeshFileBoneChunk::SetLocalizationMatrix | CMatrix4x4::Transpose() (in place) |
| 0x0054EAC0 | CMesh::~CMesh | CViewport::~CViewport (base of CScrollingViewport) |
| 0x004DF9A0, 0x004E4082 | CDisplayManager::CopyBackBufferToTexture | constructors of the CTCHaste::Multipliers and CTCCreatureGroup::Members maps |
| 0x00546AA0 | CTextContainer::GetDataFromDef | CZoomingComponent::GetDataFromDef |
| 0x00547500 | CList::GetDataFromDef | CSwappingStateComponent::GetDataFromDef |
| 0x00A351B0 | Introsort_Quicksort_Large | SGI `sort(first, last, comp)` front end, 56-byte elements |
| 0x00C20720 | EmptyStub | a state-change helper: a jump table over the 16-bit state at +0xAC |
