# Hand chunk 66 (CPersistContext .. CPhysicsForceModelGotoOutsideSimple): 36 of 40 byte-exact

All 36 sources in `src/` grade MATCH or RELOCATION_MATCH (`grades.txt`). Their behaviour tests in `test/` print PASS under g++ (`tall.sh`) and under the lane's VC7.1 cl.exe (`tall.py`). Three rows are parked in `differ/` and one row is skipped.

## What matched (36)

- **CPersistContext_TransferableComponent, persist hooks (11 rows):**
  - The component is `{ vptr; T* Object; }` (PDB). Slot 0 is TransferOut(ctx, CCharString* out) and slot 1 is TransferIn(ctx, CStringParser*).
  - Scalar writers (00405a10 u8, 004cbd30 short, 004cbe00 signed char, 004aa410 double via `__ftol2`) widen the value into a `long value` local and pass `&value` to the `__fastcall` long writer 0x009E2560. The `movzx`/`movsx` width gives the element type.
  - Long-reader hooks (004aa3f0 double via `fild`, 004cbd00 short) only need a plain `long value;` local. VC homes it in the parser's dead argument slot by itself, so the landed family's parameter reuse is not required. The target pointer is read before the call into a named local.
  - 0044b7e0 is `*target = parser->ReadInt64()`.
  - 00403f90 (bool) appends "TRUE"/"FALSE" through `CCharString::operator+=(const char*)`. Each arm keeps its own `call ; ret 8`.
  - 0040f870 (float) appends `CCharString::FormatDouble(6, v)`, a static `__fastcall` that returns CCharString by value (hidden result in ecx, digit count in edx, the double on the stack). **Lesson:** the target pointer must be a NAMED local (`float* value = Object;`) for retail's `mov ecx,[ecx+4]`. Read through the member, VC picks eax.
  - 0044b800 (unsigned 64-bit) appends a `CCharString(unsigned __int64)` temporary (constructor 0x0099F930, "%I64u").
- **CPersistContext_TransferableVector (8 rows):**
  - The vector hook is `{ vptr; vector<T>* TransVector; }`. Slots: 0 count, 1 resize, 2 TransferOut(ctx, out, index), 3 TransferIn(ctx, parser, index).
  - Traits-based hooks (005663c0 float, 0074cda0 / 0074cdd0 CDefIndex, 0049b070 / 0049b0b0 CCharString) are `CPersistTraits<T> traits; traits.TransferX(ctx, arg, &(*TransVector)[index]);`. CPersistTraits is `{ T DefaultVal; }` (PDB), and its constructor `: DefaultVal() {}` gives the early `mov dword ptr [esp],0`. For CCharString, the implicit constructor and destructor call 0x0099E4B0 and 0x0099EAE0, with no EH frame.
  - 005663f0 (float) and 00614450 (u64) build the CCharString temporary in the index's dead argument slot. **Lesson:** both need the vector's first pointer as a named local (`T* first = TransVector->m_pFirst; ... first[index]`) so that retail loads the index into the dead `this` register first. The iterator-object `operator[]` makes them longer.
  - 00614430 is `first[index] = parser->ReadInt64()` with `first` as a local.
- **CPersistContext "Transfer<CAnimationSet>" (3 rows, all STLport `resize(n) { resize(n, T()); }`):**
  - 00cd3da4 is out of line, frame-pointer size TU (`optimize("y",off)` + `("s",on)`), with a 16-byte intelligent-pointer element.
  - 00f33d90 is out of line, speed TU, with a 0x34-byte def record.
  - 00f349b0 is TransferableVector::Resize = `TransVector->resize(n)` inlined. The 0x64-byte record temporary dies through inline destructors: the record and CDefClassBase vtable stores are dead, the CDefPointeeBase store 0x01230BA0 is kept, and out-of-line `~CResource` (0x009FC550) runs. **Lesson:** model CResource <- CDefPointeeBase (inline virtual dtor) <- CDefClassBase <- record with an out-of-line record constructor.
  - In all three, the temporary is a bare `T()` passed as the constructor's returned eax.
- **CPersistContextReadSection labels (3 rows, none of them that class):**
  - 0099a540 is CFileInformation's constructor (three CDateAndTime members, PDB layout; next to CDiskFileWin32::GetFileInformation).
  - 00cd45e0 is a three-CCharString record constructor.
  - 005819f2 is a CTCInventory member that evaluates `GetWorld()->GetMainPlayer()->m_pItems->GetCount()` as a dropped statement (a compiled-out range check whose out-of-line calls survive) and returns its argument.
- **CPersistTraits (2 rows):**
  - 0042d5b1 is STLport `map<CComponent*, CUIState>::operator[]` in a `y`-off + `s` TU. **Lesson:** the iterator is a 4-byte class with a user copy constructor (by-value argument built in a reserved slot), and the insert's hidden result reuses the dead key slot. The default mapped value must be a NAMED local (retail re-addresses it with `lea` after its constructor; a bare `T()` pushes eax, 80 v 83).
  - 0047cf3f is `CPersistTraits<C3DVector>::TransferBinaryIn` (`*value = stream->Read3DVector()`, POD copy = 3x `movsd`, `y`-off + `s` pragmas).
- **CPhysicalPrimitive* (5 rows):**
  - 00815010 Cylinder::TransferBinaryOut writes BaseName.TablePos, Radius and Height (PDB layout; the stream is kept in edi).
  - 00ca9820 (labelled Cylinder) is the audio-thread owner's release: ResetEvent / SetEvent / CloseHandle through dllimport, then state = 2 and handle = 0.
  - 0067f810 SetPPrimitiveFromType loops over a global three-pointer vector of counted pointers (object read once into a local, `last` re-read per iteration), calls `PPrimitive.Reset(proto->Clone())` and has two return epilogues.
  - 00c1bff0 / 00c1c070 (labelled Mesh::GetClosestPointToPos2D) are a 12-byte vector copy and a Y/Z swizzle. **Lesson:** a POD vector filled field by field in a NAMED local and returned keeps the frame copy (`mov ecx,eax` + three pairs). A memberwise user copy constructor folds the result into three x87 `fstp`, and dword members with constructors elide the frame.
- **CPhysicsFacesSegmentBranch (4 rows):**
  - 00a85ff0 `??1` is `delete PositiveChild; PositiveChild = 0;` twice. The derived vtable store is scheduled into the first test, and the inline base destructor stores the interface vtable last.
  - 00a86f60 is the scalar `??_G` of the same body (needs a `new` factory in the TU).
  - Both use a local polymorphic model: `engine/CPhysicsFacesSegmentBranch.h` flattens the vptr into `void* __vftable`, which cannot emit vtable stores. Member names and offsets are the PDB's.
  - 00ca7b30 / 00ca7b60 (labelled GetNoFaces) are `__stdcall` COM-style methods of an audio/streaming-module class (`this` on the stack, `ret 4`, return 0). They re-read two source values (vtable slots 5 and 6), zero +4, and (7b30 only) `RemoveAll` a {first, last, count} node list.
- **CPhysicsForceModelBase (1 row):** 008fa7e0 is the velocity getter used by DoVelocityDamping. **Lesson:** `add ecx,0Ch` after the first word = an inline `const C3DVector& GetVelocity()` accessor PLUS a memberwise user copy constructor into the hidden result. That copy uses integer moves here (the POD form shuffles through esi/edx, 35 v 30).

## Parked for Fable 5.1 (3)

- 00403fc0 CPersistContext_TransferableComponent::TransferIn DIFFER(33v36): retail `xor eax,eax ; mov [esp],eax ; mov byte ptr [esp],al` (dword zero-fill of the CPersistTraits<bool> slot, then the byte) vs lane `xor al,al ; mov byte ptr [esp],al` | tried: ctor `DefaultVal()` / `DefaultVal(T())` / defaulted `const T&` argument, aggregate `{false}` / `{bool()}`, temporary `CPersistTraits<bool>().TransferIn(..)`, value-initialised POD `= CPersistTraits<bool>()` (closest), padded 4-byte POD (dword only) | next: lane gap: value-initialisation of a 1-byte POD (RTM zero-fills the 4-byte slot first)
- 0040fba0 CPersistContext_TransferableVector::TransferOut DIFFER(37v37): `mov edx,[esp+0Ch]` (index) before `mov ecx,[eax]` (first) in retail, swapped in the lane | tried: `(*TransVector)[index]`, iterator-object `*(begin()+n)` (+2 bytes), named `first` pointer local, vector reference local, parameter reuse `index = ..; Write(&index)` | next: lane gap: load scheduling (the same named-local trick fixes the two sibling writers 005663f0 / 00614450, not this one)
- 00b1b4d0 CPersistContext::Init DIFFER(69v69): `mov dword ptr [eax],1Eh` right after the +0x4 zero store in retail, sunk below the argument copies in the lane | tried: init list, base class owning +0x0/+0x4, body assignments in retail order, `#pragma optimize("s",on)` (adds a frame) | next: lane gap: immediate-store sinking (same family as the 24b "1.0f store sunk past zero stores" gap)

## Skipped

- 00455fa0 CPersistContext_TransferableSubComponent::vfunc_0: the 42-byte row covers three functions with no padding between them: `mov ecx,[ecx+4] ; mov eax,[ecx] ; jmp [eax+48h]` (8 bytes, a forwarding thunk to the sub-component's slot 18), a bit getter `(Object->m_24 >> 2) & 1` (12 bytes) and the matching bit setter (`xor`/`and 4`/`xor`, `ret 4`, 22 bytes). It needs re-splitting in the catalog before it can be authored.

## Label corrections

- 00b1b4d0 `CPersistContext::Init`: a 0x24-byte request record constructor built twice by NParticleEngine::CPSCDecalRenderer slot 28 (0x00B1B710), named `SDecalRequest` here. The PDB CPersistContext layout does not fit.
- 00cd3da4, 00f33d90 `CPersistContext::Transfer<CAnimationSet>`: STLport `vector<T>::resize(size_type)`. 00f349b0 (same label) is `CPersistContext_TransferableVector<T>::Resize` (no-RTTI vtable word 0x01301C58).
- 005819f2 `CPersistContextReadSection` ctor: CTCInventory member (`ValidateIndex` here; only caller CTCInventory::UpdateContextSensitiveItems).
- 0099a540 `CPersistContextReadSection` ctor: `CFileInformation::CFileInformation()`.
- 00cd45e0 `CPersistContextReadSection` ctor: a three-CCharString record constructor (WaypointData / CThingInfo / CActivity are PDB candidates).
- 0042d5b1 `CPersistTraits::Transfer`: STLport `map<NUISystem::CComponent*, CUIState>::operator[]`.
- 0047cf3f `CPhysicsForceModelGotoOutsideSimple::DoAttractToPoint`: `CPersistTraits<C3DVector>::TransferBinaryIn` (caller 0x004859F2, a Transfer binary branch).
- 00ca9820 `CPhysicalPrimitiveCylinder::TransferBinaryOut`: CAudioThreadOwner thread release (the fallback the landed 0x00CAB870 Attach calls).
- 00c1bff0, 00c1c070 `CPhysicalPrimitiveMesh::GetClosestPointToPos2D`: 12-byte vector copy / Y-Z swizzle (named C3DVector here; render-module callers).
- 00ca7b30, 00ca7b60 `CPhysicsFacesSegmentBranch::GetNoFaces`: `__stdcall` COM-style Reset methods of an audio/streaming-module class (vtable word 0x012BF6B0 for 7b30).
- 008fa7e0 `CPhysicsForceModelBase::DoVelocityDamping`: the GetVelocity getter. DoVelocityDamping is its caller 0x008FCC10.
- Callee labels: 0x009E2560 is the `__fastcall` long writer `(ctx, CCharString* out, long*)` (manifest: CTCInventoryBase::GetItemDescription). 0x00993EB0 is a 4-byte long writer (manifest: WriteCBYTE). 0x0042AD6C is STLport `_M_lower_bound`. 0x0099A380 is the CBaseIntelligentPointer ctor. 0x0042D9E0 is the CDefClassBase ctor (manifest: CPSCBase). 0x009FC550 is `CResource::~CResource` (manifest: ~CMemoryBlock). 0x00F33DE0 / 0x00F34580 / 0x00CD3CCB are STLport `resize(n, const T&)` (manifest: CArray push_back / CVectorMap resize).
