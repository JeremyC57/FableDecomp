# 2026-10-14_linux_hand48 — hand-authored chunk 48 of the class-labelled 17–96-byte pool

Chunk 48 (`mkchunk.py 48`, 40 rows, CHandleBasedMemoryPool .. CIME) was authored by the session coordinator on the Linux VC7.1
lane; the lander re-graded every row: **30 of 40** landed as MATCH/RELOCATION_MATCH with behaviour tests (compiled with the
lane's cl.exe). Parked sources stay in the lane's `differ/`. The lane notes follow.


Linux VC7.1 lane (Wine + DDK cl.exe), authored by the coordinator. Chunk 48 (`chunk48.tsv`, 40 rows): **30 of 40** graded
`RELOCATION_MATCH`/`MATCH` with behaviour tests (g++ and the lander's cl.exe). Sources from `gen.py`, tests from `gen_tests.py`,
`payload.json` / `oracle.tsv` from `finish.py`. 10 parked in `differ/` (evidence below).

## Label corrections
- 0x00CB0190 "CHeroExperienceDef::Init" is a Win32 event wrapper constructor: `CreateEventW(0, manualReset, 0, 0)` (the argument is the
  manual-reset flag: pushes 0, 0, eax, 0), the handle at +0; named `CEvent_00cb0190`.
- 0x004E26F1 (labelled ~CHeroMarriageDef) and 0x007ACAC0 (labelled ~CHeroMorphDef) are destructors of two other CDefPointeeBase
  subclasses (members at +0x74/+0x64/+0x4c/+0x40/+0x3c and strings at +0x90/+0x8c/+0x6c/+0x64); the member destructor labels
  "CActiveFile::OnReadFinished" (0x004DD3EE / 0x004DF907 / 0x00451AC6) are propagated.
- 0x00CB8110 "CHeroMorphDef::CHeroMorphDef" is a no-RTTI CBase subclass (vtable 0x012C1648) with a circular list at +4 and morph records.
- 0x004DA6C8 "CHitLocation::~CHitLocation" has no vtable store (two counted-pointer members after a word); 0x00710900 is the real one
  (a CIVCountedPointeeBase subclass releasing +0xc and +8).
- 0x00789358 "CHitParameters::~CHitParameters" is a different, non-polymorphic layout (members +0x58/+0x50/+0x4c, a freed buffer at +0x3c,
  a 12-byte member at +0x30 destroyed by the tail jump).
- 0x00868CA0 "CIME::SetEditBoxRegion" is a (key, float) tree insert at +0x30 guarded by `-1e-4 < value`; named `CTreeOwner_00868ca0::SetEntry`.
- 0x00C282E0 ("CLandscapeLayerMesh::OnPostRelocation") and 0x00C28610 ("CTCInventoryBase::GetCategoryWithIndex") are the pool's
  merge-free-area / release-handle helpers called from FreeAllocation (parked).
- 0x00404C50 calls 0x00CB28E0 ("CAIStateGroup_Freeze::IsRunnable", propagated) with `this` in ecx: the IME-enabled query.

## Lessons
- A by-value empty "iterator category" tag in a `std::find` call: retail pushes the VALUE's address twice (the empty temporary is
  overlaid on the parameter slot by the retail compiler); the lane cannot overlay a temporary on a live slot (6 rows parked, below).
- `/Os` modules: `__forceinline` on the counted-pointer release and destructor, or the lane emits them out of line (24 v 47).
- A `__declspec(novtable)` derived destructor only stores the base vtable when the base DECLARES an inline destructor
  (`~CIVCountedPointeeBase() { }`): with the implicit destructor there is no store at all (0x00710900).
- CDefPointeeBase is 4 bytes (one vptr): the derived pads start at +4 (three destructors were off by 4).
- The front-end manager's sprite loaded straight into ecx for the tail jump = an inline accessor `GetImeSprite()` (0x00404C00).
- `fcomp ; test ah,41h ; jp` on the "call" edge = `!(x <= eps)` (NaN-inclusive), not `x > eps` (`je`); `fabs` needs
  `#pragma intrinsic(fabs)` on a `double` prototype (0x0055B890).
- Vtable slot counting with a polymorphic root: the root's own `virtual v000()` IS slot 0, so the derived fillers start at v001
  (0x0055B890 / 0x0055BAE0 were off by one slot).
- Empty-class temporaries bound to `const&` in an initialiser list: give the class a user-declared default constructor, else the lane
  value-initialises each temporary with `xor eax,eax ; lea edi ; stosb` (0x005797A4). Records cleared last-word-first = body assignments
  in reverse order in the record's inline constructor.
- `add esp,4` of a `malloc` delayed to the end of a constructor: the stores before it are member-object constructors (initialiser list),
  the store after it is a body assignment (0x00CB8110).
- `area->m_size += area->m_padding` (a compound add with the short converted first, the store sunk after the bucket compare) is
  the only form that orders the two loads like retail (0x00C27EF0).
- A size-capped bucket (`shr ; cmp 0FFh ; jb`) compared signed later (`test eax,eax ; jl`) = an `unsigned long` cap then a `long` index
  (0x00C28080). The STLport vector `Size()` with `m_pFirst == 0 ? 0 : last - first` must be written with the null case first
  (`if (m_pFirst == 0) return 0;`) for retail's block order (0x00C28A80).
- A hidden-return struct forwarded straight from a virtual call (`C2DVector f() { return p->Get(...); }`) copies the hidden pointer
  through esi with no local (0x00815370); a 4-float colour built in the hidden return record = `return CRGBAColour(1,1,1,1)` with an
  inline constructor (0x00B28B60).
- The parameter slot as a local: `long id = value;` (the lane homes it in the dead slot like retail, 0x004AFD80).

## Parked (10; sources in differ/)
- 0x00407330 / 0x004AFD80 / 0x006BBFA0 / 0x00778AB0 / 0x007CA800 / 0x007FEC30 `CHitParameters::IsDivineWrathStrike` (six ranges over
  the unrolled `std::find`): retail `lea eax,[esp+8] ; push eax ; lea edx,[esp+0Ch] ; push edx` (the value's address and the empty
  tag temporary at the same slot); the lane either CSEs one `lea` and copies it (by-reference tag), pushes an uninitialised dword
  (by-value tag with a user copy constructor) or zero-fills its own frame slot (POD tag); everything else (locals for first/last,
  `setne`) matches. The `setne al` vs `xor ecx,ecx ; setne cl ; mov al,cl` split between modules is also unresolved.
- 0x0055BAE0 `CHoverable::OnHovered`: the by-value counted-pointer temporary — the temporary form keeps the vtable in edi across the
  two virtual calls (retail re-reads `[esi]`), the named-local form re-reads the vtable but reloads the local from its slot instead
  of using the returned hidden pointer (`mov eax,[esp+4]` v `mov eax,[eax]`), 98 v 96.
- 0x00C28380 `GetPointerFromHandle`, 0x00C28640 `ValidateResources`, 0x00C28690 `FreeAllocation`: retail materialises the handle-vector
  slot address (`lea eax,[eax+ecx*4]` then `mov eax,[eax]`, or an unused `lea` beside the load) — the RTM compiler keeping a reference's
  address; the lane folds it into the load for pointer locals, reference locals, `__forceinline` accessors and volatile elements.
