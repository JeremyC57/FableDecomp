# Chunk 357 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Placement / by-value copies with a nested user-copy record: an element whose memberwise USER copy ctor holds a
  3-long record with its own user copy ctor makes VC7.1 keep a second induction pair at `&record + 8`
  (`lea edi,[eax+N] ; lea esi,[ecx+N]`, then stores `[edi-k]`). The first word still goes through ecx/eax, and the
  record sits at N - 8.
  - Examples: `__uninitialized_copy` 0x00776B60 (N=0x20, record +0x18), 0x00A95A10 / 0x00BF5B50 (N=0x14, +0x0C);
    `__insertion_sort` 0x008183E0 (N=0x10, +8).
  - A 4-long record at +4 also lands the register at +0x0C, so the register marks `+8`, not the record's last word.
- 5-word record copy-construct: the implicit (bitwise) copy ctor gives `mov esi,ecx ; mov edi,eax` + five ebx moves
  (push ebx/esi/edi, 0x0081A650). An explicit memberwise copy ctor copies `[ecx+N] -> [eax+N]` through esi only
  (0x0085DB50). A 9-word POD becomes `rep movsd`.
- `__uninitialized_copy` whose loop calls an out-of-line copy ctor: retail loads result into ebp BEFORE the
  `first == last` test and returns it from the shared epilogue (`pop edi ; mov eax,ebp`). That needs
  `T* cur = result; for (; first != last; ++first, ++cur) new (cur) T(*first); return cur;`. The `++result` form
  emits a separate `mov eax,[esp+N]` early-out (0x009F6C00, 0x009E6270).
- STLport `pair` has a USER copy ctor (`pair(const pair& o) : first(o.first), second(o.second) {}`, the VC6
  work-around). A by-value pair argument shows it as "store the key word, then `add eax,4 ; mov edx,edi` block copy
  of the POD second" with an induction register at `&i->second` (0x0086C830 / 0x0081C490 / 0x004D1530). The implicit
  pair copy ctor copies the whole pair as one block.
- STLport `__insertion_sort(first, last, comp)` (`ret 4`) passes `*i` BY VALUE to the out-of-line
  `__linear_insert(first, i, val, comp)`: `push comp ; sub esp,sizeof(T) ; mov eax,esp` + the inlined copy ctor. Eight
  bodies here, all with propagated labels (0x0076AEC0 ...).
- CRGBColour copied through the landed `CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); }` loads all four
  bytes before storing. That needs bl/cl/dl plus a spill byte in a `sub esp,8` frame, and pushes `first` into a frame
  slot: the scrambled byte shuffle of 0x00721360. Member-wise byte copies give plain load/store pairs.
- Counted `__copy` over strings: when retail addresses every member as `[esi+N]`/`[edi+N]`, model the element FLAT
  (0x004AA580, 0x00566780).
  - A nested `second` struct holding {CCharString, CCharString, long} gets its own induction pair (`lea edi,[ebp+0Ch]`),
    which is right for 0x009E6600 pair<CCharString, CSymbolInfo>.
  - A nested {CCharString, CCharString} does not (0x004B84F0).
- A byte the implicit `operator=` never touches inside an element is an EMPTY struct member: 0x00A85870 copies
  +0x25 and skips +0x24.
- Polymorphic element with an implicit copy ctor (0x00A2AF20 C3DAnimationInfo : CMeshDataBankEntry): base vtable
  store, base members, derived vtable store, then derived members. A local `virtual ~Base();` + derived model
  reproduces it; the generated flattened headers cannot.

Review at landing:
- 0x00A2AF20 uses a local polymorphic C3DAnimationInfo / CMeshDataBankEntry model next to the generated
  engine/C3DAnimationInfo.h and engine/CMeshDataBankEntry.h. Those headers flatten the vtable to `void* __vftable`
  and cannot emit the two vtable stores. CIVCountedPointeeBase comes from engine/CIVCountedPointeeBase.h.
- 0x00708C20 / 0x00AF1CE0 / 0x009CA660 / 0x00A438E0 use a local polymorphic counted pointee (virtual slot 1 =
  Delete) next to engine/CIVCountedPointeeBase.h, whose flattened vtable cannot express the slot-1 call. This is the
  same local model as the landed 0x00A43720.
- 0x00B28ED0: the 0x38 stride equals sizeof(CKeyCamera) and the caller is labelled InsertKeyCamera, but Ego_r.pdb's
  CKeyCamera (CCharString at +0x30, float at +0x24) disagrees with the body (bytes at +0x24/+0x25, no string copy).
  The record stays descriptive. Worth checking whether retail CKeyCamera differs from the PDB.
- Landed callers declare some of these callees with other tails. 0x00AF15B0 and 0x00A42DA0 use
  `(..., const tag&, bool)` for 0x00AF1CE0 / 0x00A438E0, and 0x009C8660 uses `_Nonscalar_ptr_iterator_tag&, bool` for
  0x009CA660. Ours is the SGI `(..., const random_access_iterator_tag&, int*)`. The bytes and `ret 0Ch` are
  identical; only the mangled names differ (landing is by address).
- Byte-identical pairs/triples carry distinct names with the address suffix: 00776b60/007770c0, 00a95a10/00b31a30,
  0081a650/008247e0/00b3e340, 00708c20/00af1ce0, 009ca660/00a438e0, 004aa580/006237c0.
- 0x00776B60 needed exactly 4 grading attempts: POD, flat memberwise, two C3DVector, then the landed fill_n model.
  No row needed more than 4, and no catalog row holds several functions.
