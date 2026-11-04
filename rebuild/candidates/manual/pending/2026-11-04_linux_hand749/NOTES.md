# Chunk 749 -- NUISystem CTextSlider / CText / CMesh (ProcessEvent, Draw, deleting-destructor adjustor thunks)

Mirror: repo rebuild/include (MIRROR_HEAD.txt) plus the hdr/ overlay = `mirror/` (mkmirror.sh; needs `S` exported).

## Results
| address | row | grade | attempts |
|---|---|---|---|
| 0x00549440 | CTextSlider::ProcessEvent | RELOCATION_MATCH | 2 |
| 0x0054A390 | CTextSlider `??_E...W3AE` (adjustor{4}) | RELOCATION_MATCH | 2 |
| 0x0054A3A0 | CTextSlider `??_E...WBI@AE` (adjustor{24}) | RELOCATION_MATCH | 2 |
| 0x00550010 | CText adjustor{4} | RELOCATION_MATCH | 1 |
| 0x00550020 | CText adjustor{24} | RELOCATION_MATCH | 1 |
| 0x00550C10 | CMesh adjustor{4} | RELOCATION_MATCH | 1 |
| 0x00550C20 | CMesh adjustor{24} | RELOCATION_MATCH | 1 |
| 0x0054EF00 | CText::Draw ("vfunc_2") | DIFFER(1127v1127), parked | 20 |
| 0x005502E0 | CMesh::Draw ("vfunc_2") | DIFFER(1321v1288), parked | 13 |

vland (read-only, FABLE_HDR_OVERLAY=mirror, LANDVERIFY_WORK inside this directory): WINS 7, all RELOCATION_MATCH + PASS
(vland_result.txt; the rerun with `--recheck-landed` is vland_recheck.txt, same 7). The only warnings are RELOC_NAME:
- 0x0042B707 is labelled `CBasicString<char>::CalcStringLength`; the real name is list::size (NAME_FIXES).
- The thunk targets 0x0054A440 / 0x00550030 / 0x00550C30 are labelled "scalar_deleting_destructor". These warnings are benign:
  the bodies are `??_G` (no array branch), and VC7.1 references the vtable slot as `??_E`, a weak alias of `??_G` when no `new[]`
  is involved. The landed CScrollingViewport precedent 0x0053FC10 / 0x0053FC30 has the same shape.

## Identities (two sources each; LABELS.tsv)
- 0x00549440: CObserver-subobject vtable 0x01248A68 slot 1 (the CObserver::ProcessEvent override; `this` = CObserver at +4,
  hence `ret 4` and the -4 fixups), plus the Ego_r / FableWin name set.
- The six 8-byte "vfunc_0" rows are the VC7.1 adjustor thunks of the deleting destructor: `sub ecx,4` (CObserver at +4) or
  `sub ecx,18h` (CActionParam at +0x18; rtti_bases: CUIInterface@0, CObserver@4, CActionParam@0x18), each followed by
  `jmp ??_G`. They are slot 0 of each secondary vtable. VC7.1 emits them in the TU that defines the virtual destructor, so
  each source is the class's destructor TU: CTextSlider = landed body 0x00549A00, CText = ~CText 0x0054F6E0, CMesh = ~CMesh
  0x00550230.
- "vfunc_2" is Draw: the CText vtable 0x01249CCC and the CMesh vtable 0x01249F9C, slot 2. It overrides CComponent::Draw
  0x00530260 with the same signature and `ret 14h`. Name: FableWin.pdb `?Draw@CText@...` 0x0228B700, and the CMesh name set.
- CText slots 142..150 (bodies read): 142 GetValue 0x0054FF10, 143..145 SetValue x3, 146 GetFont 0x0054FF20, 147 SetTextWindow,
  148 GetTextWindow 0x0054FFE0, 149 ForceWidth, 150 GetAlignment 0x0054FFF0.
- CMesh slots 142..148: GetMeshIndex 0x00550B80, SetMeshIndex, SetHeight, SetWidth, SetAnimationIndex, SetMeshType,
  SetOrientation 0x00550BE0 (all PDB spellings verified).

## Header overlay (hdrcheck2.txt: every changed header default=same)
- NEW engine/NUISystem_CTextSlider.h: virtual set, slots 142 RevertValue and 143 GetValueFromProfile, retail layout to sizeof
  0x1A0. Note that `std::vector<CCountedPointer<CAction>> Actions` is at +0x16C.
- NEW engine/NUISystem_CMesh.h: retail layout to sizeof 0x1A4 (Width and Height are `unsigned long`; retail converts them with
  `fild` plus the 2^32 fix-up) and slots 142..148.
- engine/CText.h, FABLE_RETAIL_CTEXT branch:
  - Slot 142 renamed GetAlignment -> GetValue, and slots 146..150 added.
  - A new FABLE_RETAIL_CTEXT_LAYOUT holds the typed ctors / dtor / Update / Draw and the typed members (Value 0x15C,
    PFont 0x160, Handle 0x168, HandleDropShadowBottomRight 0x170, Window 0x178, ScaleText 0x188, Unavailable 0x189,
    NoRenderNextFrame 0x18A). The opaque pad is kept otherwise.
- engine/CSystemManager.h (FABLE_RETAIL_CSYSTEMMANAGER): inline GetFontManager().
- stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST): STLport list erase(iterator) and pop_front().
- engine/NUISystem_CUIScreenUnitConverter.h, new FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE: inline GetBackBufferWidth/Height
  over the static data. These are the spellings already established by landed rows.
- engine/NUISystem_CManager.h (FABLE_RETAIL_CMANAGER): GetMeshBank() 0x0041D1B4.
- NEW engine/CEnginePrimitive2DText.h: inline default ctor (Type 0x27), the out-of-line ctor 0x00543910 and operator=
  0x00543530; sizeof 0x2C.
- Used only by the parked CMesh::Draw draft:
  - NEW: C3DOrientationInterpolator.h, C3DAnimationBlendState.h, CEnginePrimitive2DStaticMesh.h,
    CEnginePrimitive2DAnimatingMesh.h.
  - New branches: C3DAnimationState.h (FABLE_RETAIL_C3DANIMATIONSTATE), CBankFile.h (FABLE_RETAIL_CBANKFILE_ENTRYQUERY:
    EntryExists 0x009CC190, GetEntryDataType 0x009CC210), CMeshDataBank.h (FABLE_RETAIL_CMESHDATABANK_BASES: `: CBankFile`,
    PeekMesh 0x00A24270), CMeshDataBankEntry.h and C3DMeshInfo.h (FABLE_RETAIL_CMESHDATABANKENTRY), C2DBoxF.h
    (FABLE_RETAIL_C2DBOXF_CORNERS: the (tl, br) ctor).

Siblings (SIBLINGS.tsv): I re-graded 16 landed includers on the overlay with g.sh (q.py), and all still match:
- Every source that defines a changed retail macro (CTEXT, CSYSTEMMANAGER, STLP_LIST, CMANAGER, C2DBOXF, CMESHDATABANK).
- The default-path includers of CText.h and CUIScreenUnitConverter.h.

0x00661E80 grades MATCH; the other 15 grade RELOCATION_MATCH. `--recheck-landed` re-grades only payload rows that are already
landed, not header siblings.

## Header debt
- The landed CMesh ctor 0x00550190 uses invented stand-in types. It should move to NUISystem_CMesh.h.
- CMeshDataBank's CBankFileAsync bytes (0x110..0x164) are opaque.
- The C3DMeshInfo body past the base is opaque.
- C3DAnimationState / C3DAnimationBlendState are modelled from the CMesh::Draw call site only: vptr 0x01249F50, ctor 0x004C2DD0,
  dtor 0x004C2DF0, SetBaseAnimation 0x00A3C260. Unproven elsewhere.
- The CEnginePrimitive2DStaticMesh / 2DAnimatingMesh argument orders come from one call site each.

## Parked rows
### 0x0054EF00 CText::Draw -- DIFFER(1127v1127), 53 diff lines, 20 grades
- Residue: only the frame slot order of five small locals differs.
  - Retail: position 0x10, scale 0x18, bool 0x1F, colour 0x20, shadowColour 0x24.
  - Ours: colour 0x10, shadowColour 0x14, position 0x18, scale 0x20, bool 0x27.
  - One ecx/edx swap follows in the 2D-text ctor argument setup.
- Tried with no effect: declaration order, a const-reference temporary for colour, the CRGBColour ctor forms, the default
  CRGBColour branch, an unpacked C2DVector, and a function-scope bool.
- What did work:
  - A single `release:` label reached by goto keeps CEnginePrimitiveHandle::Clear inline. Two release copies make it
    out of line.
  - The release block tests only parentHandle.
  - The if/else drawLayer form gives retail's ebx zero register.
- Next idea: compare where retail first stores to the colour slot 0x20 with where ours first stores to 0x10. Then try
  reading RenderColour into `colour` after the off-screen test (position and scale first), so the colour locals' lifetimes
  start later.

### 0x005502E0 CMesh::Draw -- DIFFER(1321v1288), 126 diff lines, 13 grades
- Best: differ/005502e0.cpp (try a v11).
- The early-release block matches retail byte for byte (through 0x005503B2) once the early `Handle.Clear()` is inlined. Retail
  has two inline copies of Clear that cross-jump into the final release tail. Plain C++ calls the early copy out of line:
  c2's static frequency for that site is ~0.19 (`||` 0.75 x 50/50 x 50/50).
  - Only a `do { ... } while (0)` around the early clear raises it enough (the INLINE_DEPTH_REVISIT lever 2). Retail's real
    construct is unproven.
  - goto-to-one-release (the CText fix) gives a single copy instead (x4: 1214v1288).
- `C2DVector position; position = RelativeRenderPosition;` gives retail's integer copies of the position, with retail's 0xEC
  frame. The implicit bitwise operator= does this; the user copy ctor instead puts it on x87.
- Remaining residue:
  - Retail reads zoom.X straight from the member (`fld [esi+108h]` before `fild Width`, then `fmulp`) and copies only zoom.Y
    to a frame slot (0x3C).
  - Retail spills size.X (0x28) and keeps the size + position sum as scalars at 0x40 / 0x28, which the second box
    re-initialisation copies with integer moves. Ours keeps zoom on x87 and rebuilds the second box with x87 loads.
- Next idea: zoom as `C2DVector zoom; zoom = RelativeRenderZoom;` combined with `RelativeRenderZoom.X * Width` for size.X
  (v10 copied both components). Also try one C2DBoxF reassigned (`box = C2DBoxF(position, size + position)`) for the
  second clip box, since retail reuses one slot (0x18..0x24) and drops the dead first-box TL.Y store.

## C++ attempts before helper (GFFloatToLongNear; both parked rows)
Retail rounds with a bare `fistp dword`: CText 0x0054F0B0 / 0x0054F0C0 `fistp dword ptr [esp+28h]`; CMesh 0x00550468 and
seven more box corners. This is the existing shared helper engine/GFFloatToLongNear.h (Retail-asm-helper; the impossible-list
instruction). At the CText site I compiled:

| attempt | form | result at the site |
|---|---|---|
| 1 | `position.X = (float)(long)position.X;` | `call __ftol2` x2 (try/0054ef00_ftol.dis) |
| 2 | same + `/QIfist` | `fistp qword ptr [esp+20h]` x2 (try/0054ef00_qifist.dis) |
| 3 | helper GFFloatToLongNear | `fistp dword ptr [esp+28h]` x2 = retail |

The CMesh rounding is the same expression on the box corners, and the helper is the same call. Both differ sources carry a
`// Retail-asm:` line.

## Name fixes (NAME_FIXES.tsv)
- GetValue / GetAlignment / GetFont / GetTextWindow (CText slots).
- CEnginePrimitive2DText ctor (`ABVCFontBank`, not `PAV`) and operator=.
- list::size 0x0042B707 and list::pop_front 0x0053C7D9 in full STLport spelling.
- CList::GPreviousSelected 0x013B8A98.
- CManager::GetMeshBank 0x0041D1B4.
- CMeshDataBank::PeekMesh 0x00A24270. The bound `GetMeshInfo` has no PDB symbol; the overlay first copied it from
  reloc_bindings and was corrected to the landed PeekMesh.
- ~CEnginePrimitive2DAnimatingMesh 0x00550A00.

## GOTCHAS one-liners
- VC7.1 copies a packed C2DVector local with integer moves when written `C2DVector p; p = member;` (implicit bitwise
  operator=), but enregisters it on x87 when written `C2DVector p(member);` (user copy ctor) -- CMesh::Draw 0x005502E0.
- Deleting-destructor adjustor thunks (`??_E<C>@NS@@W3AEPAXI@Z` / `WBI@AEPAXI@Z`, `sub ecx,4|18h ; jmp ??_G`) match from the
  class's destructor TU. They are benign RELOC_NAME warnings because `??_E` aliases the `??_G` body.
- `vland --recheck-landed` re-grades only payload rows that are already landed. Header siblings need their own g.sh / q.py
  grade on the overlay.
- Never copy a callee name from reloc_bindings.tsv without a PDB hit: 0x00A24270 is bound `GetMeshInfo`, which does not exist;
  the landed name is PeekMesh.
- A function whose early exit and final exit both run an inline handle Clear: one goto label gives one inline copy. Retail's
  two cross-jumped copies need the early site hotter (do-while(0) reproduces it; construct unproven).
