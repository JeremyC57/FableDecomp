# Chunk 476 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport map default construction must split `_Rb_tree_base` and the derived class. The base constructor mallocs the
  header (`_M_header(0) { _M_header = malloc(..); }`) and the derived class sets `_M_node_count(0)` and the header links;
  a flat model stores count=0 before the malloc. (006dc350, 006dcfa0)
- `CPersistTraits<long>` is not empty: it carries `long DefaultVal` (Ego_r.pdb). An empty traits local takes a 1-byte
  slot and the frame comes out 4 bytes small. (006dcfa0)
- `__uninitialized_fill_n` inside STLport `_M_insert_overflow` needs `__forceinline` to inline its placement-new loop
  like retail. (006b6990)
- An implicit copy constructor row (`??0T@@QAE@ABV0@@Z`) grades from a TU that instantiates `__uninitialized_copy` over T,
  with no user copy constructor written. (006b6010)
- A 7-byte `_Tp __x_copy = __x;` in `_M_fill_insert` makes VC7.1 add the /GS cookie: write `// Flags: /GS`. (006b8050)
- A compiler-generated member-destruction body with no vtable store = `__declspec(novtable)` class plus an empty user
  `~T() {}`. Model large retail members as sized char blocks only where they have trivial destructors. (006b7380)
- A def-pointer `push_back` loop keeps the def in a register only when GetDef and `_M_insert_overflow` have same-TU
  `__declspec(noinline)` bodies (VC then knows GetDef writes only `out` and the overflow only reads `x`). The global index
  must be read into a named local before the call, or the id takes ecx and the GetDef `this` load sinks below both
  pushes. (0069a1a0)
- A `CDefPointer<const T>` release through a const pointer needs `mutable RefCount` and const AddRef/Release (C2662
  otherwise). (0069a1a0)
- Callee-saved pushes shrink-wrapped below an early `je` (retail pushes all four at entry) come from STLport list/map
  iterators WITHOUT a user copy constructor. Adding `_List_iterator(const _List_iterator&)` /
  `_Rb_tree_iterator(const _Rb_tree_iterator&)` and STLport's `pop_front(){erase(begin());}` /
  `push_back(x){insert(end(),x);}` with by-value iterators restored retail's prologue (006d8830, 006e31e0). This
  confirms Retry 10i for map iterators too.
- `inline float __fastcall GFFloatAbs(float f) { *(unsigned long*)&f &= 0x7fffffff; return f; }` (FableWin.pdb
  `?GFFloatAbs@@YIMM@Z`) gives retail's `fst [slot] ; and [slot],ecx ; fld [slot]` for an x87 value. For a value already
  in a frame local it gives the double store `mov [s],edx ; mov [s],eax` (not a volatile signature). (006e3440)
- When a float result is both stored into a member and passed on (`m = ...; f(m)`), write ONE ternary assignment into
  the member and pass the member. A named result local or an unnamed repeated difference spills both to the frame;
  retail keeps them on x87 (`fst [slot] ; mov eax,[slot] ; fstp [member] ; push eax`). (006e3440) In a list-node
  variant (`inserted->OutputGain = GetTargetGain(..)`) the named `gain` local was the one that matched. (006e31e0)
- C3DVector `operator-` and `GetMagnitude` written inline in a class body were NOT inlined in a 600-byte function (VC7.1
  called them out of line); `__forceinline` restored retail. (006e31e0)
- Retail inlines `C3DVector::Normalise` (out of line at 0x00A14440) in GetBestAttackerForPlacementVector:
  `*this *= 1.0f / GetMagnitude()` with `operator*=(float)` matches retail's `fst [tmp] ; fmul st(2)` inv handling,
  where three `X *= inv` statements do not. A local declared first is stored first in the prologue (`best = 0` before
  `bestDot`). (006e8ce0)
- VC7.1 canonicalises single-expression sums of squares: `X*X + Y*Y + Z*Z`, `Z*Z + Y*Y + X*X` and `X*X + Z*Z + Y*Y`
  compile identically, so retail's term order for a stored vector is not reachable by reordering the source. Park it as
  "single-term x87 commutation". (006e31e0, 006e8ce0)
- `float x = (clock % 100) * (1/60.f) * (1/24.f) + ...`: VC7.1 folds or factors the two reciprocal constants where
  retail multiplies each term separately. (006bb390, parked)
- A string-building chain `out += "lit" + s + "lit";`, one statement per line, matches retail directly with the CCharString
  free operators `__fastcall`. (006b91a0)

Review at landing:
- 0069a1a0 took 4 grades plus 1 compile-error run (C2662).
- Rows that hit the 4-grade cap: 006a3a70, 006a3e00, 006b0020, 006bb390, 006d8830, 006e31e0, 006e8ce0 (all parked).
- 006e8ce0's differ file combines grade 3 with grade 4's declaration order; the combination is ungraded.
- Local models next to existing engine headers (retail layouts differ from the Ego_r.pdb debug-STL offsets):
  - CEnvironment (engine/CEnvironment.h), 006b7380;
  - CGameCameraManager (engine/CGameCameraManager.h, SpecialCameraList +0x11C retail v +0x130), 0069a1a0;
  - CTCHeroExperience (engine/CTCHeroExperience.h), differ 006d8830.
  - 006dc350/006dcfa0 derive local `COutputStreamMethods`/`CInputStreamMethods` from engine/CDataOutputStream.h /
    CMemoryDataInputStream.h to add the inline write/read helpers. Their template symbols therefore mangle with the
    wrapper types and without the allocator argument (FableWin.pdb has `AAVCDataOutputStream@@` /
    `AAVCMemoryDataInputStream@@`). The leaf is the `??$GFSerialiseMapBinaryOut@` / `??$GFSerialiseMapBinaryIn@` prefix.
- Already-landed name error: `rebuild/src/compiled/00/66/CGameScriptThing_IsSamplePlaying_0066ae10.cpp` is
  `CTCSoundPlayer::IsAttachedSoundPlaying(unsigned long) const`. Callers use it on the TC 0x10 sound player (006e3440,
  006e31e0, with SetSpecificSampleVolume on the same `this`), and PAttachedEmitter sits at +0xC.
- Already-landed: `rebuild/src/compiled/00/6e/global_GetFirstAttackerAlignedWithPlacementVector_006e8e10.cpp` defines an
  anonymous-namespace (`?A0xd8b789b9`) function at global scope. It matches by the `?Name@` prefix, but the rule says to
  put it inside `namespace { }`.
- 0x0129BA3C (1e-4f) is modelled as `g_epsilon_0129ba3c` / `g_ParticleEpsilon` in landed files and as `g_Epsilon` here;
  one name sweep should pick a single name.
