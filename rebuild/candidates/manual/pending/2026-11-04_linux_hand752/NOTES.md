# Chunk 752 notes (NUISystem CSlider / CMorphingSprite / CScrollBar)

Compiler: VC7.1 RTM 13.10.3077 (lane default). Mirror: the repo's current `rebuild/include` (read after chunks 749 / 751 / 753
landed) plus `hdr/`; every row was regraded on that mirror (LOG.tsv tag `r2`).

## Results (6 matched, 3 parked)
| Address | Row | Grade | Grades | Source |
|---|---|---|---|---|
| 0x0054C140 | CSlider `??_E...W3AEPAXI@Z` (this-4) | RELOCATION_MATCH, test PASS | 3 | src/0054c140.cpp |
| 0x0054C150 | CSlider `??_E...WBI@AEPAXI@Z` (this-0x18) | RELOCATION_MATCH, test PASS | 3 | src/0054c150.cpp |
| 0x00555120 | CMorphingSprite this-4 thunk | RELOCATION_MATCH, test PASS | 2 | src/00555120.cpp |
| 0x00555130 | CMorphingSprite this-0x18 thunk | RELOCATION_MATCH, test PASS | 2 | src/00555130.cpp |
| 0x0055A380 | CScrollBar this-4 thunk | RELOCATION_MATCH, test PASS | 2 | src/0055a380.cpp |
| 0x0055A390 | CScrollBar this-0x18 thunk | RELOCATION_MATCH, test PASS | 2 | src/0055a390.cpp |
| 0x0054AC90 | CSlider::ProcessEvent | DIFFER(1454v1454), 26 lines | 36 | differ/0054ac90.cpp |
| 0x00554740 | CMorphingSprite::Draw | DIFFER(1524v1572), 73 lines | 27 | differ/00554740.cpp |
| 0x00559A90 | CScrollBar::ProcessEvent | DIFFER(2144v2163), 78 lines | 10 | differ/00559a90.cpp |

vland (read-only, no --land): 6 / 6 RELOCATION_MATCH + PASS (`vland_result.txt`). The six WARN RELOC_NAME lines say the thunk
targets 0x0054C1B0 / 0x00555140 / 0x0055A3A0 are labelled `scalar_deleting_destructor`; VC7.1 and Ego_r.pdb both call them `??_E`
(NAME_FIXES.tsv).

## Identities (vtables in rebuild/manifest/rtti-vtables.tsv, names from Ego_r.pdb)
- The 8-byte pairs are the adjustor thunks `sub ecx,4 / 18h ; jmp`. They sit in slot 0 of each class's CObserver (+4) and
  CActionParam (+0x18) vtables: CSlider 0x01248CE0 / 0x01248CD8, jumping to 0x0054C1B0; CMorphingSprite 0x0124A474 / 0x0124A46C,
  to 0x00555140; CScrollBar 0x0124B7BC / 0x0124B7B4, to 0x0055A3A0. Chunk 750's hint held here: VC7.1 emits both thunks when the
  TU defines the virtual destructor out of line (bodies modelled on the landed ~CSlider 0x0054B360, ~CMorphingSprite 0x005541E0
  and ~CScrollBar 0x00559A10).
- 0x0054AC90 is CSlider::ProcessEvent, slot 1 of the CObserver vtable (`this` = full-4).
  - The jump table was checked by hand: SELECT(4) 0x0054B107, BACK(5) 0x0054AFF1, LEFT(20) 0x0054ACB6, RIGHT(21) 0x0054AE51,
    default 0x0054B20C.
- 0x00559A90 is CScrollBar::ProcessEvent, slot 1 of the CObserver vtable 0x0124B7BC. It tail-calls CClickable::ProcessEvent
  0x0055AD60.
- 0x00554740 is CMorphingSprite::Draw, slot 2 of the primary vtable 0x0124A49C (`ret 14h`). Its siblings are the CMovie, CText
  and CMesh Draw rows.
- New-virtual slots:
  - CMorphingSprite: 145 SetGraphicIndex 0x00555100 (stores +0x198), 149 GetGRaphicIndices 0x00555110 (PDB spelling, capital R),
    150 UpdateMorph 0x00554140 (`ret 4`), 151 ChangeGraphic 0x00554130 (`ret 8`).
  - CScrollBar: 154..159 are SetDeltaForActionUp/Down, SetAmountActionsUp/Down, SetMaxAmountActionsUp/Down
    (0x00559650..0x005596C0).
- Constants:
  - 0x0122DCB4 = 2^32 (unsigned-to-float fix-up), 0x0123003C = 25.0f, 0x012462A4 = -25.0f, 0x0122DEDC = 0.0f.
  - 0x0129BA3C = DELTA_NOTIONAL_ZERO, the operand of C2DVector::operator==.

## Header overlay (hdr/), hdrcheck2 (default path unchanged for every changed header)
`CHANGED engine/C2DVector.h default=same FABLE_RETAIL_C2DVECTOR=DIFF`, `CHANGED engine/NUISystem_CManager.h default=same
FABLE_RETAIL_CMANAGER=DIFF`, `NEW NUISystem_CSlider.h / NUISystem_CMorphingSprite.h / NUISystem_CScrollBar.h /
NUISystem_CActionParamFloat.h`.
- **engine/C2DVector.h**, retail branch only:
  - `extern const float DELTA_NOTIONAL_ZERO` (0x0129BA3C).
  - `__forceinline bool operator==(const C2DVector&) const` (fabs of each component difference <= DELTA_NOTIONAL_ZERO).
  - Evidence: Ego_r `??8C2DVector@@QBE_NABV0@@Z`, plus five caller-less out-of-line retail copies (0x0052E3D0, 0x0054A4A0,
    0x006CB570, 0x00704D30, 0x00A88C50; no call / jmp / data reference). Retail expands it at every site, but VC7.1's per-site
    heuristic calls a plain-inline copy out of line inside the CSlider switch.
- **engine/NUISystem_CManager.h** (rebased on the chunk 749 version with GetMeshBank), retail branch:
  - `C2DVector& GetMouseMovement()` (+0xB0).
  - `CCountedPointer<CGraphicDataBank> GetGraphicsBank()` (+0x10).
  - Both are Ego_r names, inline in retail (copied in place).
- **New per-class headers:**
  - NUISystem_CSlider.h: CSprite base; Dimensions +0x184 .. PAcceptButton +0x1E8; size 0x1EC; RevertValue in slot 149,
    GetValueFromProfile in slot 150.
  - NUISystem_CMorphingSprite.h: RenderCount +0x184, Handles +0x188, GraphicIndices +0x18C, SpriteGraphicIndex +0x198; size 0x19C.
  - NUISystem_CScrollBar.h: CDraggable +0x194..+0x1D8, slots 150..159.
  - NUISystem_CActionParamFloat.h: Value +8; size 0xC.
  - All are Ego_r layouts minus the 0x30 retail NUISystem shift, and they build on the landed NUISystem_CSprite.h (chunk 753).
- **Dropped from the overlay:**
  - stlport/fable_containers.h: now identical to the repo (chunk 749 landed it).
  - NUISystem_CSprite.h: the repo version (chunk 753) is used.
  - The experimental `FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE` (__forceinline Clear and default ctor). It is a modelling
    device for a parked row only, and is kept as work/CEnginePrimitiveHandle_forceinline_experiment.h.
- **Siblings rechecked** (SIBLINGS.tsv): 16 landed sources that define FABLE_RETAIL_C2DVECTOR or FABLE_RETAIL_CMANAGER. All
  16 are RELOCATION_MATCH on the overlay (parity only).

## Header debt (local models in the parked Draw draft)
- The graphics-bank chain is modelled locally, copied from the landed 0x0041C260 CGraphicInfoFrame::GetFrame model and the landed
  0x00BADB40 PeekGraphic / PeekGraphicInfoFrame model: CResource, CResourceList, CCPInfo / CTexturePointer, CGraphicFrameData,
  CGraphicFrame, CManagedTexture, CGraphicInfo (with Frames +0x28 and PeekInfoFrame), CGraphicDataBank (CArray GraphicList
  +0x1E0, CurrentFrame +0x194), CGraphicInfoFrame and its CFrameData.
- Also local: CEnginePrimitive2DSprite (0x54 bytes, ctor 0x0041BEB0).
- No shared engine headers exist for these classes yet.

## Callees (exact spellings our compile emits)
- CMorphingSprite::Draw:
  - CManager: GetInstance 0x0041E5F2 and GetLayer (slot 36).
  - Map and list: map<unsigned long,unsigned long>::operator[] 0x00553E60 and list<CEnginePrimitiveHandle*>::push_back 0x005550D0
    (out of line, NoRender path).
  - Graphics loading: CGraphicInfoFrame::BeginLoading 0x009FE620 (const), CFrameData::BeginLoading 0x009FDE80,
    InternalGetFrameForcedLoading 0x009FF6B0, CFrameData ctor 0x009FE380, CResource::ListRemove / ListInsert 0x009FBFB0 /
    0x009FBF90, CGraphicFrameData::Uninitialise 0x009FCBA0.
  - Sprite and engine: CEnginePrimitive2DSprite ctor 0x0041BEB0; engine slots 23 SetPersistentPrimitive, 28 SetChildPrimitive
    (4-argument) and 30 ClearChildPrimitive.
  - Own slots 101 IsIndependent and 104 IsLayerIndependent.
- CScrollBar::ProcessEvent:
  - CManager: GetInstance and GetUIScale 0x0041CF47 (QBE, returns C2DVector).
  - map<unsigned long,CUIState>::operator[] 0x0042D5B1, on GetStates()[0].
  - Own slots 108 LoadAndReturnDef, 82 GetStates, 31 ChangePositionDelta, 32 ChangePosition, and 154..159.
  - List: CList::GPreviousSelected.front()->ChangeTargetPositionDelta.
- CSlider::ProcessEvent: the CTextSlider callee set (landed 0x00549440).
  - Also CActionParamFloat::Value (+8).
  - GetMiscThingsDef: SoundBack for LEFT / BACK, SoundForward for RIGHT / SELECT.

## Parked rows
### 0x0054AC90 CSlider::ProcessEvent: DIFFER(1454v1454), 26 lines, 36 grades
- **Residue:** one scheduling difference in the LEFT case's action loop.
  - VC7.1 hoists `lea ebx,[edi*8]` above the first element access, giving `mov edx,[ebx+ecx]`. Retail has `mov edx,[ecx+edi*8]`
    with the lea after it.
  - The RIGHT case has the same source and matches.
  - The jump-table relocation lines are masked noise.
- **Tried, no effect:** loop index types; front() vs begin(); temporaries; fewer locals; case order swaps; a default label; extra
  empty cases; a same-TU RevertValue; a function-scope i; an `Actions[i]` local; `(*it)` iteration.
- **Next idea:** give LEFT a different loop shape from RIGHT's while keeping its bytes. Two candidates:
  - a do-while guarded by `if (!Actions.empty())`;
  - the CTextSlider (0x00549440) per-case helper split.
  - Also compare c2's frequency of the two cases. LEFT is the first case, so it is reached by the jump-table fall-through.

### 0x00554740 CMorphingSprite::Draw: DIFFER(1524v1572), 73 lines, 27 grades
- **Shape found** (each fixed real lines):
  - The visibility test is `A > 0 && (!(zoom.X <= 0) || !(zoom.Y <= 0))` (retail `test ah,41h ; jp`).
  - NoRender path: the erase arm first (`size >= RenderCount`), then push_back(new) in the else.
  - Named `C2DVector size(Size.X*RRZoom.X, Size.Y*RRZoom.Y)`: both products are computed first, height spilled, as retail.
  - Landed PeekGraphic / PeekGraphicInfoFrame forms (CArray GraphicList) with __forceinline, as in landed 0x00BADB40. These give
    retail's re-evaluated second bound test (`sbb ; and`).
  - __forceinline GetFrame and ~CGraphicFrame in the local model.
  - Y is rounded before X.
- **Residue 1:** VC7.1 calls CEnginePrimitiveHandle::Clear (`Handle.Clear()` at the RenderCount==0 / no-parent exit) and the
  handle default ctor (`push_back(new CEnginePrimitiveHandle)`) out of line. Retail expands both.
  - It is the c2 static-frequency rule (INLINE_DEPTH_REVISIT.md): `do { Handle.Clear(); } while (0)` expands Clear (54 lines).
  - __forceinline on both (header experiment) leaves 46 lines.
  - Neither is a proven retail construct, so neither is kept.
  - The same Clear / ctor bodies expand at the hotter sites: after the list walk, and in the render path.
- **Residue 2** (with residue 1 closed): register colouring only, inside the inlined GetFrame touch block and the sprite argument
  loads.
  - Retail stores the bank frame through edx and evaluates `&Head` before `Head.PrevResource` for ListInsert.
  - Ours uses eax / ecx, rotated.
  - Retail's out-of-line GetFrame 0x0041C260 (landed, same source) has retail's choice, so the inline context decides it.
  - No effect: frame declaration order, a block around the frame, the bool result, a temporary frame, a non-const GetFrame,
    CIVCountedPointer-style `operator->`, a ternary layer, a colour copy after the independence calls, a null test on the frame
    pointer.
- **Next idea:**
  - Find the natural construct that makes the cold NoRender sites hot enough. For example, retail may test `parentHandle` first
    and share one `ClearChildPrimitive` tail via a single label (the chunk 749 CText goto-release lever).
  - Then try the GetFrame call through a same-TU inline wrapper (retail may reach it through CGraphicDataBank::GetGraphicFrame).

C++ attempts before helper (the two rounding sites, 0x005549B5 `fld [esp+14h]` / 0x005549B9 `fistp dword [esp+18h]` for
RelativeRenderPosition.Y and 0x005549C7 / 0x005549CB for .X; each value is first copied to a frame slot):

| spelling | VC7.1 RTM emits |
|---|---|
| `(long)x`, `(int)x`, `(short)x` | `call __ftol2` |
| `float t = x; (long)t` | `call __ftol2` |
| `(long)(x + 0.5f)` | `fadd` + `call __ftol2` |
| `(long)floor(x + 0.5f)` with `#pragma intrinsic(floor)` /Oi | an out-of-line float `floor` (no float intrinsic) + `call __ftol2` |
| `double d = x; (long)d` | `call __ftol2` |
| same-TU `static inline long Near(float v) { return (long)v; }` | `call __ftol2` |
| `/QIfist` | `fistp qword ptr` |
| `_mm_cvtss_si32(_mm_set_ss(x))` (xmmintrin) | `cvtss2si` |
| `GFFloatToLongNear(x)` (engine/GFFloatToLongNear.h, __forceinline + `__asm fld/fistp`) | `mov [esp+14h],edx ; fld [esp+14h] ; fistp dword [esp+18h]`, exact |

- Helper calls: two (GFFloatToLongNear, RelativeRenderPosition.Y then .X).
- The bare dword fistp through the parameter's frame copy is the function-form helper. A macro form would fld the member
  directly, and retail copies it first.

### 0x00559A90 CScrollBar::ProcessEvent: DIFFER(2144v2163), 78 lines, 10 grades
- **Fixed:** the DOWN clamp sum is `GetStates()[0].Position + LoadAndReturnDef()->PositionOffset - LoadAndReturnDef()->
  PositionOffset * TargetZoom.Y`. The argument is evaluated first, which gives retail's call order and `fld [offset] ; fadd [pos]`.
- **Residue:** only frame-slot sharing (instruction stream equal).
  - Retail gives every LoadAndReturnDef CDefPointer temporary (two in the dead UP `top` line, two in the DOWN clamp condition,
    two in its body) and every GetUIScale result (two UP, two DOWN) its own slot: `sub esp,5Ch`.
  - Ours shares them across statements and between the UP / DOWN blocks: `sub esp,30h`.
  - As a result three def-pointer `mov [slot],0` stores that DSE drops on reused slots remain in retail, and one alignment pad
    moves.
- **Tried:**
  - start/end declared at the block top: no effect.
  - Named or const-reference GetUIScale locals: these grow the frame to 0x40, only within a block, and swap ebx/ebp.
  - UP / DOWN as two sequential ifs: no effect.
  - The QFE 4035 compiler: packs identically (0x30).
  - Flags /Op /Oa /Ow /Ob1 /Os /G6 /G7 /GX /GR /Z7 /Zi /Zd /GZ /RTCs: none changes it; /GX adds an EH frame retail lacks.
- **Next idea:** retail behaves as if the temporaries' lexical scope were the whole function. Try the UP / DOWN bodies as
  same-TU inline member helpers, since the inliner may not share an inlinee's locals across instances. A toy showed VC7.1
  shares even consecutive GetUIScale temporaries in one block, so the source must differ in kind, not just in order.

## GOTCHAS one-liners
- VC7.1 calls a plain-inline `C2DVector::operator==` out of line in switch cases. Retail expands it everywhere and still has
  five caller-less out-of-line copies (0x0052E3D0, 0x0054A4A0, 0x006CB570, 0x00704D30, 0x00A88C50): __forceinline in
  C2DVector.h (FABLE_RETAIL_C2DVECTOR).
- The NUISystem `??_E...W3AE` / `WBI@AE` thunks relocate to `??_E<Class>` (the slot-0 body Ego_r names ??_E). The landed
  `scalar_deleting_destructor` labels on 0x0054C1B0 / 0x00555140 / 0x0055A3A0 trigger WARN RELOC_NAME.
- In CMorphingSprite::Draw, CEnginePrimitiveHandle::Clear and its default ctor expand at hot sites but are called out of line at
  the cold NoRender exits under plain inline. This is the c2 static-frequency rule; `do { } while (0)` around the site flips it.
- A landed GetFrame model (0x0041C260) inlined into another function keeps its instructions but not its register choice.
  Retail's inline copy in Draw rotates eax/ecx/edx.
- `GetStates()[0].Position + LoadAndReturnDef()->PositionOffset`: VC7.1 evaluates the operator+ argument's call first and loads
  the argument's X first. Write the operands in the reverse of retail's load order.
- A CScrollBar-style function whose def-pointer / UI-scale temporaries each own a frame slot in retail (5Ch vs 30h) is not
  fixed by any flag, the QFE compiler or named locals. Park it as the frame-slot sharing family.
- The repo's NUISystem_CSprite.h (chunk 753) replaces chunk 729's hand copy. Rebase overlays on the current repo include before
  the final grade (`mkmirror.sh` now copies the repo's rebuild/include).
