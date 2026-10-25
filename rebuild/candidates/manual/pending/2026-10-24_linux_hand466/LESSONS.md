# Chunk 466 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `#pragma optimize("", off)` does not turn off inline expansion. An inline placement `operator new` is still expanded,
  so an unoptimised-TU row that calls the shared `??2@YAPAXIPAX@Z` COMDAT (0x00405C10) also needs
  `#pragma inline_depth(0)`, which stands for the TU's /Ob0. Example: the /Od STLport `_Construct` 0x009F7890 went from
  71v82 to RELOCATION_MATCH.
- NScript::CPredicate_And `??1` rows at any nesting depth match from one PDB model:
  - `CPredicateBase { virtual bool operator()() const = 0; virtual CPredicateBase* Clone() const = 0; virtual
    ~CPredicateBase() {} }`;
  - CIsThingAlive with an out-of-line dtor (0x00E241B0);
  - `CPredicate_And<P1,P2>` with `virtual ~CPredicate_And() {}`;
  - `template class` explicit instantiation and the full mangled `??1` leaf (0x00EF5B50 / 0x00EF6D50 / 0x00EF6D90 /
    0x00EF6DE0).
  - The catalog depths are shuffled: name the depth by counting derived vtable stores.
- A user dtor whose only call follows the vtable store with ecx still `this` is `~T() { NonVirtualMember(); }` over a
  base with an inline virtual dtor (0x00B564E0 `~CIndexBuffer() { Uninitialise(); }`).
- STLport /Os `vector<T*>::operator=` matches verbatim from the 0x00AADE80 model when `_M_allocate_and_copy` (member
  template) and `__fastcall __copy_trivial` are declaration-only. The ebp frame, the homed x._M_finish and the
  tail-merged copy call need nothing extra (0x005798C8).
- STLport 4.6 `list::pop_back` (`iterator __tmp = end(); erase(--__tmp);` with a declaration-only erase and
  `_List_iterator`'s user copy ctor) matches in a /Os TU. The ICF-folded body's owner is the TU it sits in (0x005791B5,
  CTCInventoryExperience's list<long>).
- Two same-TU tree-walk members (`FirstKey`, `NextKey`) used in one `for` loop: VC7.1 expands only the loop-bottom
  NextKey and keeps FirstKey a call, plain or `inline`. `__forceinline` on both expands both but threads FirstKey's null
  exits, where retail keeps a merged `xor eax,eax ; test` (0x00BF32B0, parked 192v196).

Review at landing:
- 0x00BF32B0 used all 4 grades and is parked (differ/00bf32b0.cpp plus a passing differ/00bf32b0.test.cpp). The next
  idea is in NOTES.md.
- 0x006FD9F0 and 0x00BF32B0 use local models next to existing engine headers: CGameCameraBase.h (PDB-size lists from
  +0xa8), and CVertexBufferWin32.h / CIndexBuffer.h, whose vptr is a data member while the rows need real polymorphic
  bases for the virtual delete and the base vtable store. 0x00B564E0 models CIndexBuffer with the header's member
  names and offsets.
- Already-landed sources that look wrong or crude:
  - 0x00EF6E40 (??_G of the four-term And) models the base as `template <class T> struct CPredicate { bool
    operator()(const T&) const }` with struct keys. FableWin.pdb has `class NScript::CPredicateBase` with `bool
    operator()() const` and `Clone`, so its symbol names do not match the PDB. The bytes still match.
  - 0x00BF34B0 is a placeholder-style family source (`struct CEngineDynamic3DResource { void Dtor(); }`). It is
    really `CEngineDynamic3DResource<CIndexBuffer,3>::??_G` (vtable 0x012A81B8), calling `CIndexBuffer::~CIndexBuffer`
    0x00B564E0.
  - 0x00BF3290 is labelled "C3DAnimationBlendState" in the manifest. The landed source calls it
    CEngineDynamic3DResource, which is right: it is the `CEngineDynamic3DResource<CIndexBuffer,3>` ctor (base
    CIndexBuffer ctor 0x00AC1EC0, PoolNext +0x20 zeroed).
  - 0x009F7810 / 0x009F77F0 ("operator&&", unoptimised TU) model CPredicate_And as `struct { int payload; }` with
    invented members.
- Catalog rows holding several functions: none in this chunk.
