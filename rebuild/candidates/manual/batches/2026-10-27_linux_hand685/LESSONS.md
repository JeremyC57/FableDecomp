# Chunk 685 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Same-TU stand-in bodies (`#pragma auto_inline(off)`) for every out-of-line callee that receives a LOCAL'S ADDRESS let
  VC7.1 treat the local as non-escaping: STLport `basic_string::reserve` + `assign` for a std::string local (dead
  constructor stores vanish, start/finish live in registers, 0x007748E0); vector `_M_insert_overflow` for a counted-pointer
  local (Data/Info stay in esi/edi across CCharString calls, 0x00AB1F30, 619 -> 566 -> 556). Do NOT give bodies to callees
  that only take the local's members (CCharString ctor/op=/dtor): VC then uses their register facts (ecx preserved by the
  default constructor) and keeps `new T` in ecx instead of retail's `mov esi,eax` (566v556).
- `for (long i = 0; i < 10; i++)` with `long number = i + 1` used in the body: VC7.1 keeps `number` in a frame word beside a
  10-down counter (`inc [i] ; dec [n] ; jne`); `for (i = 1; i <= 10; i++)` emits `inc ; cmp ; jle` (0x007748E0).
- VC7.1 auto-inlines a small STLport `push_back` at only one of four call sites in a function (the second in source order);
  the others call the out-of-line body -- no source trick, let the inliner decide (0x007748E0).
- Vendor text is byte-exact only verbatim: libvorbis 1.0.1 `_01forward` passes `accumulator` (NULL, `push 0`) to the encode
  callback, not `look` (578v579, 0x00CA0BA0); C statics must be inside `extern "C" { }` or the symbol is C++-mangled.
- An `if / else if / else { error }` chain with ONE shared `return 0` (0x00C674B0): the fall-through arm joins the common
  epilogue, the other arm gets a duplicated epilogue with the pops interleaved around its last RMW (`mov eax,[dirty] ;
  pop ; pop ; or ; pop ; mov [dirty],eax`); per-arm `return 0`s move the later arm to the end of the function. Within an
  arm the source store order decides which store the scheduler sinks past the dirty-mask RMW (air, room, HF, dirty).
- Member-wise `m_reverb[slot].field = c` stores fold the array base into the displacements (`[eax+0xf8]`); a local record
  pointer materialises `lea esi,[ecx+eax+0xf4]` first (0x00C674B0). VC7.1 folds `base + slot*0x70 + 0x150` into
  `(slot + 3) * 0x70`.
- ImeUi.cpp in retail is the 2004 DirectX SDK text, older than DXUT's: no `g_bInitialized` guard, the `bUnicodeImm`
  OS-version choice between the W entry points and the WA_ wrappers, imm32 in a local handle, a once flag around
  ImeUi_SetWindow + ImmGetContext, four callbacks (DrawRect, Malloc, Free, DrawFans), SetImeApi() + ImeUi_SetSupportLevel(2)
  inlined from same-TU bodies; the IsWindowUnicode test is written ANSI arm first (0x00CB5180).
- Two globals stored in both arms of a diamond are only RELOCATION-distinguishable by store ORDER: retail's ANSI arm stores
  _ImmGetCandidateList (0x0143CD5C) before _ImmGetCompositionString (0x0143CE04) while the Unicode arm stores them the
  other way round; the grade is RELOCATION_MATCH either way, only vland's RELOC_IDENTITY check catches the swap (0x00CB5180).
- Under `#pragma optimize("s", on)` a `static inline` helper that contains a call is not auto-inlined: use `__forceinline`
  (0x00CBF29F). VC7.1 pools identical string literals before optimisation: a second helper or accessor copy with its own
  `""` occurrence is the same symbol and cannot split a register web.
- A helper taking `const CCharString&` fed with a literal makes a full-expression temporary; in an `||` chain VC7.1
  constructs them lazily, tracks them in a flags word (`or [ebp-8],1/2/4/8`) and destroys them in reverse at the end of
  the full expression (0x00CBF29F).
- `bones[parent]->Children.push_back(x); bones[parent]->n++;` re-reads the element after the call; a named
  `CBoneInfo* parent = bones[i].Get()` keeps it in ebp (0x00AB1F30). A vector local declared BEFORE `other.clear()`
  gets its zero stores interleaved with the clear's `__copy` call setup.
- The `// Leaf:` must be the exact object symbol: a private member (Ego_r `AAE`) must be declared `private:` in the model
  or the object emits `QAE` and the leaf matches nothing (0x00AB1F30).
- The attractor row (0x00B179A0): VC7.1 does not auto-inline a largish template member (CVectorMap::Find) into a big
  function -- `__forceinline`; commutative FP operand order is normalised (swapping `a*b` does nothing); member add chains
  `X*X+Y*Y+Z*Z` evaluate Z first on the FPU; `a[k] = call()` with map::operator[] evaluates operator[] first, a named local
  for the call result gives call-first; a conversion temporary (`long` bound to `const unsigned long&`) keeps its register
  copy across calls while a named address-taken local is reloaded.
- Parked gaps: dead-argument-slot homing of six successive temporaries (0x00BEC680, 545v545) and an allocator-level
  register-web split (0x00CBF29F, 930v936) -- both look like retail LTCG decisions.

Review at landing:
- Rows that needed more than 4 grades: 0x00AB1F30 (11), 0x00B179A0 (14), 0x00C674B0 (7); parked 0x00BEC680 (17) and
  0x00CBF29F (12).
- Local base models next to an engine header: CCountedPointer (0x00AB1F30, header debt above), C3DMeshInfo (0x00BEC680
  differ, `f4` -> Bank), CThing / CInterfaceMap (0x007748E0, the landed 0x008330D0 idiom), CEaxContext (0x00C674B0, the
  landed SetSlotLock structs), CGameScriptInterface via the shared header (0x00CBF29F differ).
- Errors noticed in ALREADY-LANDED sources: 0x00774B40 GetFreeSaleItemPlaces and 0x00778210 push_back call this row
  "InitGoodsPlaces" / "CTCGraphicAppearance_InitGoodsPlaces"; 0x00C689B0 is landed as CSkillTable_00c67470::
  SetSkillPropertiesEx calling "SetSkillGUID" (it is the CEaxContext slot-property dispatcher calling SetSlotLoadEffect);
  global_Destval_0099f560.cpp is CCharString::RemoveWhiteSpaceBeginEnd; CIME_Initialise_00405650.cpp spells
  "ImeUI_Initialize" and "g_ImeDrawFans"; the landed 0x00BEC8B0 / 0x00B98DE0 call 0x00BEC680 "Inner_CollectStats" /
  "SubMesh_CollectStats".
- Catalog rows holding several functions: none; no manifest splits needed.
- Descriptive names that need an owner's eye: CEaxContext::SetSlotLoadEffect, the ImeUi once flag `bImcObtained`,
  `SendCompString_Ansi` (0x00CB2230), CutsceneCameraPreloader's parameter names and `LineStartsWith`.
