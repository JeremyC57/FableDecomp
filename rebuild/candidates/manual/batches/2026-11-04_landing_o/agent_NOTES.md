# Chunk 753 -- NUISystem CSprite / CEditBox / CFrontEndList slots and thunks, libpk pool forwarders, DirectShow vcall thunks

Mirror: frozen repo rebuild/include (MIRROR_HEAD.txt) plus the hdr/ overlay = `mirror/` (mkmirror.sh).

## Results (19 / 19 matched, 0 parked)
| address | row (catalog label) | proven name | grade | grades |
|---|---|---|---|---|
| 0x0041C6C0 | CSprite vfunc_147 | NUISystem::CSprite::GetWidth | RELOCATION_MATCH | 2 |
| 0x0041C6D0 | CSprite vfunc_148 | NUISystem::CSprite::GetHeight | RELOCATION_MATCH | 2 |
| 0x0041C6E0 | CSprite vfunc_0 | CSprite `??_E...W3AE` (adjustor{4}) | RELOCATION_MATCH | 2 |
| 0x0041C6F0 | CSprite vfunc_0 | CSprite `??_E...WBI@AE` (adjustor{24}) | RELOCATION_MATCH | 2 |
| 0x0052F0F0 | CSprite vfunc_83 | NUISystem::CComponent::SetName | RELOCATION_MATCH | 2 |
| 0x00540D30 | CEditBox vfunc_154 | NUISystem::CEditBox::SetAllowedCharacters | RELOCATION_MATCH | 2 |
| 0x00540D40 | CEditBox vfunc_155 | NUISystem::CEditBox::SetForbiddenCharacters | RELOCATION_MATCH | 2 |
| 0x00540D50 | CEditBox vfunc_0 | CEditBox adjustor{4} | RELOCATION_MATCH | 2 |
| 0x00540D60 | CEditBox vfunc_0 | CEditBox adjustor{24} | RELOCATION_MATCH | 2 |
| 0x0054CBF0 | CFrontEndList vfunc_48 | NUISystem::CFrontEndList::ChangeState | RELOCATION_MATCH | 3 |
| 0x0054DA50 | CFrontEndList vfunc_0 | CFrontEndList adjustor{4} | RELOCATION_MATCH | 2 |
| 0x0054DA60 | CFrontEndList vfunc_0 | CFrontEndList adjustor{24} | RELOCATION_MATCH | 2 |
| 0x00C21DF0 | CIsThingAlive::operator() | CSoundEventSet::IsBusy (libpk, landed spelling) | MATCH | 1 |
| 0x00C21E50 | CIsThingAlive::operator() | CSoundEventSet::GetFreeSlot (libpk, landed spelling) | MATCH | 1 |
| 0x00C21F20 | CIsThingAlive::operator() | CSoundGroupPool_00c21f20::Allocate (libpk, landed spelling) | MATCH | 1 |
| 0x00C465D0 | CIsThingAlive::operator() | CRegionMap::GetCount (libpk) | MATCH | 1 |
| 0x00CAEF40 | CThingFilter_IsValid::operator() | IMediaSeeking vcall{40} `??_9@$BCI@AG` | MATCH | 1 |
| 0x00CAEF50 | CThingFilter_IsValid::operator() | IMediaSeeking vcall{76} `??_9@$BEM@AG` | MATCH | 1 |
| 0x00CAEF60 | CThingFilter_IsValid::operator() | IMediaSeeking vcall{44} `??_9@$BCM@AG` | MATCH | 1 |

(The "grades" column counts g.sh grades: one draft grade plus the final file.)

vland (read-only, FABLE_HDR_OVERLAY=mirror, LANDVERIFY_WORK=vwork): WINS 19, all PASS (vland_result.txt). Warnings:
- RELOC_NAME on the six adjustor thunks: `??_E<C>@NUISystem@@UAEPAXI@Z` -> the landed `??_G` rows. These are benign, as in
  chunk 749: `??_E` is VC7.1's weak alias of `??_G` when no `new[]` exists.
- SHARED_HEADER_AVAILABLE CBaseObject on the three DirectShow thunks is a false positive. engine/CBaseObject.h is Lionhead's
  CBaseObject; the TU's `CBaseObject` is DirectShow's strmbase class of the same name (the landed CPosPassThru rows have the
  same model).

No `__asm`, helper or pragma in any row. GetWidth / GetHeight convert with `__ftol2`, which plain `(unsigned long)Size.X` emits.

## Identities (two sources each; LABELS.tsv)
- **CSprite slots 142..148** (vtable 0x0122F5D4).
  - Bodies: 142 stores +0x164, 143 +0x168, 144 +0x16C, 145 0x0041B550 stores +0x178 and refills Size from the graphic
    frame, 146 returns +0x178, 147 returns `(unsigned long)Size.X`, 148 returns `(unsigned long)Size.Y`.
  - FableWin.pdb emits the inline ones in donor order SetAngle, SetWidth, SetHeight, GetGraphicIndex, GetWidth, GetHeight
    (0x01D2CCD0..0x01D2CD90). SetGraphicIndex is out of line.
  - Ego_r layout minus 0x30 gives Angle, Width, Height and GraphicIndex at those offsets.
  - So 147 = GetWidth and 148 = GetHeight (`UAEKXZ`). Size is CComponent's C2DVector at +0xCC.
- **0x0052F0F0 is not a CSprite function.**
  - The address appears in 43 vtables. In CComponent's own table 0x0124608C (RTTI .?AVCComponent@NUISystem@@) it is slot 83.
  - The slot is SetName in engine/NUISystem_CComponent.h; Ego_r.pdb / FableWin.pdb spell it
    ?SetName@CComponent@NUISystem@@UAEXABVCCharString@@@Z.
  - Body: `add ecx,20h ; jmp CCharString::operator=`, with Name at +0x20.
  - Its neighbour, slot 84 0x0052F100, is GetName. It is landed under the wrong name CBankFile::GetBankHandle (NAME_FIXES).
- **CEditBox slots 142..155** (vtable 0x01246B8C):
  - 142 GetText, 143 SetText, 144 GetTextComponent, 145 AddCharacter, 146 KeyPressed, 147 UpdateDisplayText,
    148/149 AddActionOnEnter/Escape, 150 SetActive, 151 IsActive, 152 SetPasswordBox, 153 SetMaxLength,
    154 SetAllowedCharacters, 155 SetForbiddenCharacters.
  - FableWin.pdb's inline-emission order (GetText, AddActionOnEnter, AddActionOnEscape, IsActive, SetPasswordBox,
    SetMaxLength, SetAllowedCharacters, SetForbiddenCharacters) matches these slots.
  - So do the bodies' offsets: Ego_r minus 0x40, from the 0x30 base shrink plus two 8-byte list shrinks.
- **The six "vfunc_0" rows are VC7.1 deleting-destructor adjustor thunks** (`sub ecx,4|18h ; jmp ??_G`).
  - They sit in slot 0 of each class's CObserver (+4) and CActionParam (+0x18) vtables.
  - CSprite and CEditBox thunks match from the class's destructor TU (landed ~CSprite 0x0041AB40, ~CEditBox 0x005408F0).
  - CFrontEndList's destructor is empty and inline: the ??_G 0x0054DA70 stores the vptrs and calls ~CList. So its thunks
    match from the constructor TU (landed 0x0054C3A0).
- **0x0054CBF0 is CFrontEndList::ChangeState(unsigned long).**
  - Slot 48 of vtable 0x01249224 is CComponent's ChangeState(unsigned long) slot; CList's own override 0x0053A0A1 sits in
    the same slot.
  - The PDB name set has ?ChangeState@CFrontEndList@NUISystem@@UAEXK@Z.
  - Every case ends in CChangingStateComponent::ChangeState 0x0052CF40, bypassing CList's version.
  - Case-to-target pairing checked by hand against the retail table at 0x0054D154 (STATUS / source header).
- **libpk rows** (no PDB, descriptive names; the landed spellings of these addresses):
  - 0x00C21DF0 / 0x00C21E50 / 0x00C21F20 each pop a free list: the CSoundEventSet ctor 0x00C222D0 builds these members at
    +0x30 (ctor 0x00C239B0, vtable 0x012B3A94), +0x1F8 (0x00C23E00, 0x012B3A9C) and +0x318 (0x00C24050, 0x012B3AA4).
  - Slot 1 is the pop: `if (!count) return 0; return items[index], --count`.
  - 0x00C465D0 is CRegionMap::GetCount:
    - The CRegionSnapshot ctor calls it on its `const CRegionMap&`.
    - The landed CRegionMap ForEach / GetCellSize model fits it (region list +4, slot 15).
    - 0x00C36CF0 builds a CRegionMap with the same ctor/dtor and calls it.
- **0x00CAEF40/50/60 are VC7.1 `__stdcall` vcall thunks** (`mov eax,[esp+4] ; mov eax,[eax] ; jmp [eax+N]`), not functors.
  - They come from DirectShow ctlutil.cpp: CPosPassThru::GetDuration / GetPreroll / GetStopPosition call
    `GetSeekingLongLong(&IMediaSeeking::<m>, p)`.
  - Slots: 10 (+0x28), 19 (+0x4C), 11 (+0x2C).
  - Each thunk's sole caller is that method: 0x00CAF120 / 0x00CAF160 / 0x00CAF0E0.
  - VC7.1 spells them `??_9@$B<off>@AG` (no class name; compiled and checked), matching reloc_bindings.

## Header overlay (hdrcheck2.txt: all NEW, no existing header changed)
- NEW engine/NUISystem_CSprite.h covers CSprite : CChangingStateComponent:
  - Retail layout, sizeof 0x184.
  - Slots 142..148, with inline bodies for the six inline ones.
  - Overrides Draw / GetSize / Initialise / GetDynamicSize.
- NEW engine/NUISystem_CEditBox.h covers CEditBox : CChangingStateComponent:
  - Retail layout, sizeof 0x18C.
  - Slots 142..155, with the PDB-inline ones inline.
- NEW engine/NUISystem_CFrontEndList.h covers CFrontEndList : CList (FABLE_RETAIL_CLIST, sizeof 0x1FC, no own members):
  - Overrides plus slots 188/189 ScrollUp() / ScrollDown().
  - Inline empty destructor.
- No landed includer is affected (all three headers are new), so SIBLINGS.tsv is not needed.

## Header debt
- The landed CSprite / CEditBox / CFrontEndList rows (~CSprite 0x0041AB40, ~CEditBox 0x005408F0, ??_G rows, the CFrontEndList
  ctor, the gapscan slot rows) all use local stand-in classes. They can now move to the three new headers.
- engine/CSprite.h, engine/CFrontEndList.h and the generated global-struct headers are flattened Ego_r views; the new
  NUISystem_ headers replace them for these classes.
- NUISystem_CEditBox.h declares AddCharacter / KeyPressed with `wchar_t`. The PDB mangles `_W` (native wchar_t); a VC7.1
  build without /Zc:wchar_t mangles `G`. Unverified, and no row here calls them.

## Callee names (all bound spellings checked)
- ??4CCharString@@QAEABV0@ABV0@@Z 0x0099EFB0 and ??4CWideString@@QAEABV0@ABV0@@Z 0x0099B7D0 use the PDB spellings.
  reloc_bindings also carries stale alternatives for both (NAME_FIXES).
- ?ChangeState@CChangingStateComponent@NUISystem@@UAEXK@Z 0x0052CF40 and ?GetInstance@CManager@NUISystem@@SIPAV12@XZ 0x0041E5F2.
- __ftol2 0x00BFEA70.

## Suggestions (not fixes: libpk has no PDB, descriptive names only)
- "IsBusy" (0x00C21DF0) is a misleading descriptive name: it returns the popped free event. "OnKey" 0x00C11F00 returns that
  pointer too: retail leaves the third call's eax as the return value. OnKey has the same shape as GetPhysicalPlayer 0x00C10C90:
  - pop;
  - if none, UpdateItems and pop again;
  - if still none, stop the event below the key and pop.
  A rename wave could call these GetFreeEvent / AllocateEvent.
- 0x00C21F20's class is probably CSoundEventSet: its +0x318 member is the group free list the CSoundEventSet ctor builds.
  It is kept as the landed CSoundGroupPool_00c21f20 because PlaySoundList's `this` class is unproven.
- 0x00C467D0 / 0x00C46860 are CRegionMap's ctor and dtor. They are bound under two invented classes, CAliveHelper_00c36cf0 and
  CVarState_00c37050.
- 0x00CAEF70 (landed `??_9@$BDA@AG` under the invented interface `IComObject_Retail`) is IMediaSeeking::GetCurrentPosition's vcall
  thunk (slot 12, caller CPosPassThru::GetCurrentPosition 0x00CAF090). The symbol is unchanged; only the model is invented.

## GOTCHAS one-liners
- A no-argument virtual call on a member object compiles to `add ecx,N ; mov eax,[ecx] ; jmp [eax+k]`. With a stack argument,
  VC7.1 loads the vptr first (`mov eax,[ecx+N] ; add ecx,N ; jmp`): 0x00C465D0 vs 0x00C465B0. Both are plain member calls.
- VC7.1 names `__stdcall` vcall thunks `??_9@$B<offset>@AG` without the class (e.g. `??_9@$BCI@AG`). They are emitted in any TU that
  takes `&I::Method` of a virtual (DirectShow GetSeekingLongLong) and are not functors: 0x00CAEF40..60.
- Inline virtual getters and setters and the deleting-destructor adjustor thunks match from the destructor TU when the class
  header carries the inline bodies: defining ~C emits the vtable and every inline virtual as COMDATs. With an inline empty
  destructor (CFrontEndList), the constructor TU emits them instead.
- `(unsigned long)floatMember` in a one-line getter is `fld ; jmp __ftol2`. There is no rounding helper and no asm (CSprite::GetWidth 0x0041C6C0).
- A catalog slot label on a base-class function (0x0052F0F0 "CSprite vfunc_83") can name the first vtable that lists it. Scan
  every vtable for the address (it is in 43 here) and name the class from the base's own vtable.
