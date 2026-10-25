# Chunk 463 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `resize(n) { resize(n, _Tp()); }` forwarders (catalog bucket "vector<EWeaponClass>::_Umove<EWeaponClass*>"): the
  default element's constructor is the whole shape. POD value-init = dword zero-fill (0x0081DFB0, 0x004433F0); a user ctor
  zeroing members = per-member eax/al stores (0x009D4300, 0x00619290); an empty user ctor = no stores (0x00657069 `push ecx ;
  push ecx`); a non-POD with NO user ctor is NOT zeroed by VC7.1 (0x0081DFE0 first grade 31v68).
- "Dword zero-fill of the whole record, then a late `mov [x],-1` for one CDefString" = a constructor
  `{ memset(this, 0, sizeof(*this)); LocationName = CDefString(); }` with `#pragma intrinsic(memset)` (0x0081DFE0, 0x0081DED0).
- STLport `pair() : first(_T1()), second(_T2())`: a 20-byte POD second is copied from a zero temporary held in
  eax/edx/esi/edi and stored late, a 2-dword POD second is stored early, first's zero is an immediate after the pushes
  (0x0081DE90, 0x008701A0); write the element type as `std::pair<A, B> ` with the space before `>`.
- Store order of a default constructor: init-list members go in declaration order, a member assigned in the BODY goes
  after them, and a float immediate sinks below the argument pushes: `: BaseWeight(1.0f), ThemeTransitionActive(false)
  { BaseWeightLocked = false; }` = retail's +8..+0x10, +0x1C, +4, push, 1.0f (0x0088A4E0).
- `CPersistContext_TransferableVector<T>::Resize(long)` (vtable slot 1, private virtual, `TransVector` at +4) =
  `TransVector->resize(size)`; in a /Os TU the one-argument vector::resize must be `__forceinline` or VC7.1 calls it
  (0x005813D7); speed TUs inline it unaided (0x00618FD0, 0x00619420). Mangled `?Resize@?$CPersistContext_TransferableVector@<T>@@EAEXJ@Z`.
- Size-optimised resize(n) forwarders (`push ebp ; mov ebp,esp ; and [ebp-x],0 ; or [ebp-y],-1 ; fldz ; fstp`) match under
  `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (0x0046ACA8, 0x0046B65A, 0x004EA398, 0x00581284, 0x00657069).
- `set::insert(const value_type&)` = 8-byte frame temporary from insert_unique, then the node pointer and the bool byte copied
  into the hidden result (the const-iterator conversion); reuse the landed 0x005BAF94 model (0x004AC860, 0x00514230). A
  map::insert forwards the hidden pointer instead.
- A small class with only a default constructor is copied bitwise (`CState s = CurrentState;` = six dword moves, the two
  bools inside the last dword) (0x006BDEA0).
- A resize(n) forwarder whose default element owns a vector but whose body has NO destructor call (0x0088A4E0, also
  the same-TU 0x008883C0): every real STLport destructor chain makes VC7.1 emit the `if (_M_start) deallocate` tail --
  same-TU noinline / `auto_inline(off)` stand-ins of resize(n, x) and _M_fill_insert, a `const` named default, a
  default-argument temporary, and `/GL + /LTCG` (ltcg_grade, two-argument resize exported and noinline) all keep it.
  Model the member with its constructor only and document it.
- STLport resize(n, x) with erase and _M_fill_insert declaration-only matched first try on eight element sizes; decode the
  element size from the divide (0x2AAAAAAB sar1 = 12, sar3 = 48; 0x92492493 sar4 = 28, sar5 = 56; 0xB21642C9 sar6 = 92,
  sar7 = 184; 0x2E8BA2E9 sar3 = 44, sar4 = 88; 0x84210843 sar8 = 496; 0x10624DD3 sar5 = 500).

Review at landing:
- 0088a4e0: the Themes member is a constructor-only vector model because retail never destroys the default entry (cause
  not found); confirm the lander accepts the documented simplification.
- 00581284 / 005813d7: local `class CHeroMarriageInfo` next to engine/CHeroMarriageInfo.h -- the generated header is a POD
  `struct` (mangles U, no constructor), while FableWin.pdb has `??0CHeroMarriageInfo@@QAE@XZ` (class V, user ctor).
- 009d4300: local `CBankFile { class CRuntimeData }` stub next to engine/CBankFile.h (Ego_r layout of the outer class;
  the nested record and its constructor are not in it).
- 004ea398 / 0067f3a0: `#pragma pack(push, 2)` on CEngineGraphic = the PDB's 14-byte layout (GOTCHAS precedent), not a trick.
- 0076e5d0: the vector parameter is spelled in namespace `std` to keep the PDB-exact symbol, following the landed sibling
  0x0076E5F0; the rest of the chunk uses `std`.
- 00514230: one ICF-folded body serves every 4-byte signed-key set (CTCVillage, CTCInventoryTrade, CMiniMapDisplay
  callers); named set<long> after Ego_r CTCVillage::PlayersCrimes.
- No row needed more than 4 grades (max 3: 0051ae60, 005813d7); no catalog row holds two functions; no errors found in
  already-landed sources (coverage.tsv labels the two-argument resize bodies 0x0046ACCE / 0x0046B681 / 0x004E9F47 /
  0x004EA3C3 / 0x005811F1 / 0x00656FAB "push_back").
