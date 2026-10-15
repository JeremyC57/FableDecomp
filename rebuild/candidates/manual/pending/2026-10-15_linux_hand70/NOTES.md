# Hand chunk 70 (CRandomAppearanceMorph .. CRenderManager2D): 38 of 40 byte-exact

All 38 sources in `src/` grade MATCH / RELOCATION_MATCH (`grades.txt`), and every test in `test/` prints PASS under
g++ (`tall.sh`) and under the lane's VC7.1 cl.exe (`tall.py`). `payload.json` and `oracle.tsv` come from `finish.py`.
Pragma/flag use: only `004564b5` needs `#pragma optimize("s", on)` (in the source). No `/EHsc`, no `/GS`.

## What matched (38)

- **CRandomAppearanceMorph** (3 + 1 relabelled):
  - `00675b60` operator=: the implicit member-wise assignment. Each CVectorMap's implicit assignment is inlined with its own this/other registers (ebp/ebx), so you get the out-of-line vector op=, nothing for the empty comparator, then the sorted byte at +0x0d. Retail sizes: CArray is 12 bytes, CVectorMap 16.
  - `0086bcc0` AddTextureMorphToBodyPart: the morph is a named local and the map is `BodyPartTextureMorphs[bodyPart]`. The max is the value ternary `Max > id ? Max : id` with the member on the left, which gives `cmp eax,edi ; jbe` and keeps the self-store. A const-reference max template re-reads the argument through `lea`, and `id < Max` swaps the compare operands.
  - `0086bf70` GetRandomMorphSet: three member calls. The last one takes `&set.GeneralTextureMorphs` (+0x18 at retail sizes) through `add edi,18h`.
  - `00bef9c0` (label "PushMorph", really an entry-vector push_back): the VC6 `push_back`, `if (last != end) { new (last) T(v); ++last; } else InsertGrow(last, v, SScalarTag(), 1, true);`. The 28-byte record is copied through edx because the value half has a member-wise user copy constructor. Reuses the landed AddEntry model from 0x00BEFBC0.
- **CReactionFrequencyTraitsArray** `00677810` / `0067a520` (twin bodies), the destructor: `for (unsigned long i = 0; i < 79; ++i) { delete LowHigh[0][i]; delete LowHigh[1][i]; }`. The bare `operator delete` calls (no null test) mean the traits class has a trivial destructor. Leaf is `??1`.
- **CReactionFrequencyTraits_ControlledCount**:
  - `006fef30` TransferBinaryOut: header shell (`<Class>_TransferBinaryOut : <Class>`). The bool is written through `xor eax,eax ; mov al`.
  - `006feed0` LogReaction: the frame goes in a local. `Count -= 1.0f`, then the clamp is the inline ternary `Count > Max ? Max : Count`, which keeps the value on the FPU stack (`fcom ; fst ; jne ; fstp st0 ; fld`). The empty test is `<= 0.0f` against the 0.0f pool constant (`test ah,41h ; jp`).
- **CReactionFrequencyTraits_PeriodicFrequency** `006fedd0` TransferBinaryOut: header shell writing the members in declaration order.
- **CReactionMatch_MRPyramid** `006feca0` TransferBinaryOut: a non-virtual call to the base override (AsymmetricPyramid, 0x006FEC30) through a shell over the base header, then the three floats.
- **CReactionMatch_SecondaryAxisCentredMRShifted** `006775b0` constructor: the inline CReactionMatch(type) base (its vtable store is dead) plus plain initialisers. Local model, because the generated header cannot carry a vtable.
- **CReactionMatch_SecondaryAxisMRShifted** `00677540` constructor: the same shape, with `InvIntervalOnAxis(1.0f / (upper - lower))`. Retail's `fld upper ; fsub lower ; fdivr 1.0` reproduces.
- **CReactionMatch_Sphere**:
  - `0067c2f0` constructor: the COpinionSet member copy constructor (0x00477428) runs after the vtable store, then `SqRadius(radius * radius)`.
  - `007003a0` TransferBinaryOut: the retail CArray (12 bytes) puts SqRadius at +0x14, so the header cannot be used. The opinion set's inline writer sends the count and then `WriteData(GetList(), Size()*4)`, where GetList returns the array object itself when the set is empty (`ja ; mov ecx,esi`).
- **CReadableDef** `007e81b0` Transfer: the inline `CPersistContext::Transfer<bool>` template. `&value` is taken before the CRC call, the mode is a `switch` with cases 2 and 3, the save arm inlines TransferBinaryOut, and the load arm's traits temporary lives in the dead context-argument slot.
- **CRedefinerList** (NUISystem, from RTTI):
  - `00556580` OnDeactivate: the 0.2f literal is pushed and stored as an immediate, and `this` is converted once (`add esi,4`) for the two `SetState` calls through vtable slot 4.
  - `005570f0` OnActivate: the virtual `GetPosition()` result is built directly in the by-value C2DVector argument slot. That needs a user constructor **and** a user copy constructor; without the copy constructor the result goes to a frame temporary. The base OnActivate is the shared tail jump.
  - `00557360` `??_G`: three vtable stores, then `if (s_pActiveList == this) s_pActiveList = 0;`, then the out-of-line ~CScrollableList.
  - `00558b30` `??_G`, really NUISystem::CKeyRedefiner: the selected-redefiner global is cleared, then the list member destructor at +0x19c, then ~CClickable.
- **CRegion** (retail offsets sit 8 below the PDB because of the 12-byte vectors, so a local model is used):
  - `006bbf40` / `006bbf70` get the world-map and name-graphic offsets from the def at +0x24. C2DVector has a user constructor, so it is returned through the hidden pointer, and the null arm stores two immediate zeros.
  - `006bc4d0` / `006bc4f0`: `MiniMapRegionExitTextOffsets[name].X/Y = v`, with the map at +0x38 and `add ecx,38h`.
- **CRegionConnectionGraph** `00828590` GetNextRegionOnWayToRegion:
  - The inline CVectorMap find is `(it == end || key < it->first) ? end : it`, with LowerBound called on the key's own argument slot.
  - The caller is written `if (it != end) return ...; return -1;` so the found path comes first.
  - The node is reached through the counted pointer's inline `operator->`, giving `mov eax,[eax+4] ... mov ecx,eax`.
- **CRegionDef**:
  - `004564b5` (genuine CRegionDef constructor): the body is empty, and only the inline constructors of CSoundMap's two CVectorMaps and of CDefString run. The CDefString store is `or dword ptr [esi+74h],-1`, which marks the TU as `optimize("s")`. Retail CParentDefClassBase is byte-packed (InstantiationName +0x25, SubDefInfoMap +0x29), so CRegionDef's own members start at +0x3c.
  - `00a4eb00`, really C3DAnimationStats (vtable RTTI): the inline STLport `_Rb_tree` constructor (header zeroed, malloc of 0x18 bytes, count cleared, `_M_empty_initialize` re-reading the header), then body assignments in InitialiseStats' order. The movement vector is cleared by an inline `C3DVector::Clear()` (`X = Y = Z = 0.0f`), and that inlined call is what flushes malloc's deferred `add esp,4` to just before the vector stores.
- **CRegionMinimap**:
  - `0066c6a0` GetRegionMapExtents: two value objects assigned whole, with both words loaded before either store and the box re-read because of aliasing.
  - `0066ccc0` constructor: only member objects with inline constructors run (CSurface vtable plus two zeros, CTexture, the two retail 12-byte vectors). Retail MaxRegionMinimapThemeColours is at +0x54, so a local model is used.
  - `00c8d970`, really a C-module state Reset: thirteen stores in offset order.
- **CRegionScriptDef** `007d79c0`: the manifest calls it "Copy", but it is the implicit `operator=` (it returns `this`). The packed parent-def assignment is inlined, then the two limits at +0x3c and +0x40 are copied. Leaf is `??4`.
- **CRegister** (both rows relabelled):
  - `00447bb0` is CSavedInputEvent(): the CInputEvent member constructor, then the inline CCamera constructor clearing HeightLocked/ViewVecZLocked at +0x64/+0x65.
  - `0055deb0` is CActivity(): three CCharString constructors, then two byte initialisers.
- **CRegisteredTCInfo**:
  - `004d2ef0` (genuine): AllocFunc, DriverType and the Name copy constructor.
  - `00c46550`, really a 12-byte lookup key: the C2DVector is copied whole (integer moves) and `GetLength()` is called on the argument still in ecx.
- **CRenderManager2D** (two of three rows relabelled):
  - `009dbff0` Draw3DScaledText (genuine): a forwarder whose by-value CWideString is passed by address and destroyed by this function, returning `mov al,1`.
  - `00c9dce0`, really a C-module record FreeBuffers: the `__declspec(dllimport)` `free` address is hoisted into ebx, the 7-entry loop becomes a pointer walk with a down-counter, then `memset(this, 0, sizeof *this)` as `rep stosd`.
  - `00cafd90`, really an audio-object destructor: a `__declspec(novtable)` class whose destructor converts `this` to its +4 base with retail's null test (`test esi,esi ; je ; lea ecx,[esi+4] ; jmp` and `xor ecx,ecx ; jmp`). The plain polymorphic form stores vtables and drops the test.
- **(relabelled)** `00c5ce00`, really an audio ring-buffer Init (not a constructor: no `mov eax,ecx`). It stores six dwords and computes `(play + queued) % capacity` with an unsigned `div`.

## Parked for Fable 5.1 (2)

- 004016f0 CDisplayManagerInit::CDisplayManagerInit DIFFER(78v78): retail `xor eax,eax ; mov ecx,1` both hoisted, NoBackBuffers `mov [esi+19h],cl` after the `mov byte [esi+18h],10h` immediate; the lane reuses eax (`mov eax,1` after the zero group) and keeps +0x19 inside the ones group | tried: member order (ones group first, eax=1/ecx=0), member order with the Present* trio reversed (fixes the in-group order), retail store order (zeros first; only register reuse and +0x19 left) | next: lane gap: constant-register grouping/scheduling of body-assignment stores (same instructions, same length)
- 0066c7b0 CRegionMinimap::InitialiseRegionBox DIFFER(90v94): retail copies the top-left words before subtracting (`mov edx,esi ; sub eax,edx`, `mov ebx,edi ; sub edx,ebx`) and builds the wide arm's right edge in ebx (`lea ebx,[eax+esi] ; mov eax,ebx`); the lane subtracts esi/edi directly and allocates the arm sums differently | tried: flat if/else side clamp, three arms storing a C2DBoxI temporary (struct copy via `add ecx,14h`), per-arm field stores, a brx/bry join on the header fields, the join over an inlined GetRegionMapExtents (best, in differ/) | next: by-value C2DVectorI/C2DExtentsI temporaries fed to an inline C2DBoxI(topLeft, extents) setter over the flat header fields (the retail copies look like the inlined GetRegionMapExtents' re-reads CSE'd into copy registers), else lane gap: copy-register allocation

## Skipped

- None. No row contains a second function body.

## Label corrections

- `00c5ce00` "CReactionMatch_SecondaryAxisCentredMRShifted ctor" is an audio ring-buffer Init in the 0x00C5Cxxx module. Neighbours 0x00C5CE60/CF00/CF20 work the same record; the callers are 0x00C5570D, 0x00C5A3A0 and 0x00C5C2B9.
- `006bbf40` "CRegion::GetNameGraphicOffset" is **GetWorldMapOffset**: it reads retail def +0xAC, which is PDB +0xB8 WorldMapOffset. `006bbf70` is the real GetNameGraphicOffset.
- `004016f0` "CRegionDef ctor" is **CDisplayManagerInit::CDisplayManagerInit**: every store matches the PDB layout, and it is called on `esi+5Ch` by the CSystemManagerInit constructor.
- `00a4eb00` "CRegionDef ctor" is **C3DAnimationStats::C3DAnimationStats**: its vtable 0x0129D514 has the COL `.?AVC3DAnimationStats@@`.
- `00c8d970` "CRegionMinimap ctor" is the Reset of a 0x168-byte state record in the C-style module (its init is 0x00C8CE20). It is not a constructor.
- `007d79c0` "CRegionScriptDef::Copy" (void) is CRegionScriptDef::operator= (it returns `this`).
- `00447bb0` "CRegister ctor" is **CSavedInputEvent::CSavedInputEvent** (PDB layout: CInputEvent at +4, CCamera at +0x40).
- `0055deb0` "CRegister ctor" is most likely **CActivity::CActivity**: it is the only PDB record with three strings and bytes at +0x0c/+0x0d. "CRegister" is a label propagated over four bodies (also 0x006A4B30 and 0x007ABA60).
- `00c46550` "CRegisteredTCInfo ctor" is a 12-byte {C2DVector, length} lookup key, the out-of-line form of the key in the landed 0x00C465F0.
- `00558b30` "CRedefinerList `vector_deleting_destructor'" is **NUISystem::CKeyRedefiner**'s `??_G` (vtables 0x0124ADBC/AD98/AD90).
- `00bef9c0` "CRandomAppearanceMorph::PushMorph" is the push_back of CEntryTable_00befbc0's entry vector.
- `00c9dce0` "CRenderManager2D::SetupD3DUVBoxFromGraphic" is a C-module record release (free x10 + memset). Its only caller is 0x00C8F685.
- `00cafd90` "CRenderManager2D::AddLineToDrawBuffer" is the destructor of an audio object in a no-RTTI module (its `??_G` is 0x00CAFD70, vtable 0x012BF9A0).
- Callee labels: 0x00477428 "lua_stackspace" is the COpinionSet/CArray<float> copy constructor; 0x0049D870 "GFGetPreMainMemoryUsed" is GFGetCurrentFrame; 0x0099A380 "CBaseIntelligentPointer" is CBaseObject's constructor (vtable 0x0129A7D8).
