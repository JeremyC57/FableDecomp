# Chunk 458 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Left-operand-only comparator.** Source: the PDB empty functor `bool operator()(const T& a, const T&) const
  { return a.PrimaryMesh; }`. Retail shape: `__unguarded_partition`'s `while (comp(pivot, *last)) --last;` becomes a
  `jmp $` (`eb fe`) self-loop, and the pivot's bit byte stays in a dead argument slot. Write the plain STLport body;
  do not chase the self-loop (0x004C3860, 0x004C38F0).
- **Size-optimised STLport TU, where the row sits at an unaligned address.** Source: `#pragma optimize("s", on)`
  plus `__forceinline` on both `CCharString::operator<` and `CKeyPairCompareLess::operator()`. Retail shape: a
  frameless inline null-test compare with an out-of-line `CBasicString<char>::operator<` call. With the pragma alone,
  VC7.1 builds an ebp frame and calls the comparator (69v85). The implicit pair operator= stays out of line
  (0x004DD80B).
- **`__pop_heap` inside `__partial_sort`: same source, different inlining.**
  - Inline: for 8-byte pairs and the 12-byte POD CMeshInfo. pair<float, CCharString> builds `T(*i)` as a local in an
    `and esp,-8` frame (0x004D1BC0).
  - Out of line: for pair<CCharString, CPersistInfo> (0x004BB810 -> 0x4BABF0) and the 28-byte camera pair
    (0x004D1C80 -> 0x4D0B50). Declare `__pop_heap` without a body when retail calls it.
- **A float member copied by the pair copy constructor.** It goes through `fld`/`fstp` into `__partial_sort`'s
  by-value `T(*i)` argument (0x004D1B40), but moves as a plain dword in `__make_heap` (0x004D0BF0). It is the same
  source: not a lead.
- **28-byte POD pair (pair<float, CCameraTag>).** The copy constructor is seven unrolled dword moves, while the
  implicit operator= is `mov ecx,7 ; rep movsd` (0x004D0330).
- **Empty PDB comparators** (CKeyPairCompareLess, CSortPrimaryMeshesFirst/Last) model as empty classes passed by
  value in the 4-byte slot, with inline `operator()`. In the 28 sort rows outside the size-optimised TU, no same-TU noinline comparator stand-in
  was needed (0x004CD500, 0x004D2160).
- **STLport bitset with exceptions off.** Source: `if (pos >= _Nb) { puts("bitset"); abort(); }` with
  `extern "C" __declspec(noreturn) void __cdecl abort(void)`. Retail shape: the puts and abort import calls; set and
  reset are the same text (0x004CC090, 0x004CC0D0). Leaf `?set@?$bitset@$0BBC@`.
- **`vector::resize(n)`.** Source: `{ resize(n, _Tp()); }`. Retail inlines the whole temporary: for a set that is
  `_Rb_tree_base`'s `malloc(0x14)` header, `_M_empty_initialize`, then `clear()` and free. Both resize overloads
  are in the object, so use the full mangled `?resize@...QAEXI@Z` as the leaf (0x0051D240).
- **Leaf precision for `__uninitialized_copy`.** A `??$__uninitialized_copy` leaf also matches the trivial
  `@E@` COMDAT that the vector copy constructor emits. Use `??$__uninitialized_copy@PBVCEntityScriptInfo@`. The
  nested PDB class mangles `V...CEntityRuntimePersistence@NThingManager@@` (0x0052BB40, 0x0052B510).

Review at landing:
- **004dd80b used the full 4-grade budget:** 2 probe grades, 1 final grade and the closing gall.sh run. Every other
  row used 3.
- **Already-landed 0x0052BBE0 needs a new name and model.**
  - Name: it is called `__uninitialized_copy<const CEntityScriptInfo*, CEntityScriptInfo*>`, but its callers are
    0x0052C150 `_M_insert_overflow` and 0x0052C300 `_M_fill_insert`, so it is the `<T*, T*>` instantiation. The const
    one is 0x0052BB40 (this chunk).
  - Model: it uses `struct NThingManager::CEntityScriptInfo`, which mangles `U` and drops the nesting. Ego_r.pdb has
    `class NThingManager::CEntityRuntimePersistence::CEntityScriptInfo`, which is what this chunk uses.
- **Local models, no engine headers.** No row has a `this` of an engine class with a generated header except the
  CVectorMap template (004bb6b0), which is modelled locally as STLport. CCharString is declaration-only, plus the
  same-TU CBasicString<char>::operator< body where retail inlines the compare. Check whether the lander wants
  `engine/CCharString.h`, keeping in mind that TLC strings are 4-byte handles.
- **Mesh sort hazard.** CSortPrimaryMeshesFirst/Last is not a strict weak order. Retail's partition never terminates
  on a primary (First) or non-primary (Last) pivot, so any introsort over more than 16 meshes can hang. This is
  worth knowing for FableForge mods that add meshes.
- **Catalog row boundaries:** no row holds several functions. Every object size equals the trimmed row length.
- **Cheap propagated-name fixes:** the manifest labels in NOTES.md "Label corrections", for example 0x0099EAE0
  `_Cons_val`, which is `CCharString::~CCharString`, and 0x004A9CB0 `BuildTreeArray`, which is
  `_Rb_tree::_M_erase`.
