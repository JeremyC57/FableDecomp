# Chunk 387 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport fresh empty tags (`__false_type()`) homed over a live or dead argument slot.** This needs both:
  - a user ctor on the tag;
  - the out-of-line callee's body in the TU (noinline, under `#pragma auto_inline(off)`).

  With bare declarations VC7.1 opens a frame word or byte.

  Rows: 0x6D4680 reserve; 0x6CB010 push_back (tag over the live `x` slot); 0x6CB390 `_M_insert_overflow` (tags over
  the live `atend` slot, 8-byte frame without the bodies); 0x6CB170 `__uninitialized_fill_n`.
- **The POD-element `__true_type` tag of an out-of-line push_back has NO user ctor.** That gives retail's
  `mov byte ptr [esp+N],0` (0x6CE530).
- **STLport `_M_insert_overflow`'s `(max)(old_size, fill_len)` is by reference.** The constant fill length 1 and
  the old size get frame words, selected with `lea ecx,[..]` (0x6CB010). Out of line, the `fill_len` argument slot
  is used directly (0x6CB390).
- **One TU can inline the STLport vector copy ctor in one place and call it out of line in another.** In the
  record-copy loops it is inlined (`__uninitialized_copy` 0x6CB0D0, `__uninitialized_fill_n` 0x6CB170). In
  `_M_insert_overflow` (0x6CB390) and the inlined push_back (0x6CA8C0) it is called (0x5BB346). Model the vector
  copy ctor as an external declaration where retail calls it.
- **The `__uninitialized_copy` instantiation names differ.** The vector copy ctor passes `(const T*)` (0x5BAF28,
  `PBU`). `_M_insert_overflow` passes `T*` (0x6CAC20). They are distinct COMDATs at different addresses.
- **`_Rb_tree::equal_range` needs the STLport iterator:** a `_M_node` base plus a user copy ctor. With a plain
  iterator, pair.second is stored first (0x6C8350).
- **A same-TU body under `auto_inline(off)` (find, `operator--`, `__find_if`, GetSoundCriteriaStringFromMap) lets
  VC7.1 keep registers across the call.** The node stays in edi at 0x6C59E0, PDef in edi at 0x6D86D0. Retail still
  calls the function.
- **`CDefString::GetString() const { return g_DefStringTable.GetString(*this); }` as a forwarding inline makes VC
  re-`lea` the temporary for `operator=`.** Calling the table directly reuses eax (0x6D75C0).
- **An out-parameter `GetInterface(&p, type)` inside its own `if` and a CCharString in the caller's `if` are
  sibling scopes.** They share one frame word. HasTC plus an unconditional GetTC adds a word (0x6D7640).
- **Read a clamped inline accessor once into a named local.** Calling it twice keeps a second clamp; one local folds
  the clamp into the following `> 1` test (0x6D80E0).
- **`m_kind != 7 ? true : false` gives `cmp byte ; setne al`.** A bare `!= 7` goes through
  `xor ecx,ecx ; setne cl ; mov al,cl` (0x6D5AF0).
- **CHitParameters needs `#pragma pack(1)` in the retail model, with DamageMap at +0x30 (0x6D8D00).**
- **Two-member pair traits build DefaultVal from the element traits' defaults:**
  `DefaultVal(CPersistTraits<T1>().DefaultVal, CPersistTraits<T2>().DefaultVal)`. This gives the retail CCharString
  temporary copy and destroy (0x6D4B40).
- **A `find_if` with a fallback returns as `if (it != end()) return *it; return back();`** (branch order, 0x6D7430).
- **The STLport list `resize` passes end() by value with a user iterator copy ctor**, so it is built in its argument
  slot (0x6DB9D0).

Review at landing:
- **Header lengths.** Ten source headers originally under-stated the length by 1-2 bytes and have been corrected:
  0x6C8D40 156, 0x6CE230 106, 0x6CE2B0 65, 0x6D5AF0 91, 0x6D7640 118, 0x6D8D00 131, 0x6DB060 144, 0x6DB0F0 125,
  0x6DB9D0 90, 0x6DBDC0 61. The catalog lengths include int3 padding.
- **Local models next to engine headers.**
  - The CTCHeroExperience engine header carries the PDB debug layout. Retail is shifted: PDef is at +0x54.
  - CThing, CTCFollowed and CTCCreatureOpinionOfHero are also local models. Retail's CTCCreatureOpinionOfHero
    vector<bool> masks are 12-byte vectors.
  - The texture-morph rows model CCompositeBlendDesc and CEngineTextureMorph locally.
- **Landed 0x6CA950 calls its receiver `CTextureMorphs`.** The PDB CTextureMorphs is a different 20-byte class
  (CVectorMap<EHeroMorphType, CTextureMorphEntry>). The receiver there and in 0x6CA8C0 / 0x6CB390 is a
  vector<CEngineTextureMorph>, such as a CEngineTextureMorph list member (CEnginePrimitiveMorphed*::TextureMorphs).
- **Landed 0x6CAC20's leaf names a `const CCompositeBlendDesc*` first argument.** Its caller 0x6CB010 passes
  `_M_start` (non-const pointer). The const_pointer instantiation is 0x5BAF28.
- **Landed 0x6CB340 (`CRetailVector_006cb340::Destroy`) is vector<CEngineTextureMorph>::_M_clear.**
- **Speculative modelling.**
  - The element type of 0x6CE230, 0x6CE2B0 and 0x6CE530 (`const CThingCreatureBase*`) comes from the landed
    caller 0x6CE300.
  - The CPersistTraits default-value design (0x6D4B40) is inferred.
- **Grade counts and near misses.**
  - 0x6D4680 needed 3 grades.
  - 0x6CA8C0 and 0x6CE730 each used all 4 attempts. One 0x6CA8C0 attempt was a probe compile.
  - 0x6D7EC0, 0x6D8790, 0x6D8BF0, 0x6C9B70, 0x6D73D0 and 0x6CE730 are near misses (register, slot or scheduling
    only).
- **Same-TU stand-in bodies.** Several matched rows rely on `#pragma auto_inline(off)` same-TU stand-in bodies of
  out-of-line callees: 0x6C59E0, 0x6D4680, 0x6D7430, 0x6D75C0, 0x6D7640, 0x6D86D0, 0x6CB010, 0x6CB170 and 0x6CB390.
  The stand-ins are not graded; only the leaf is.
