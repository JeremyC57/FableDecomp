# Chunk 267 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Store-then-reload clamps come from inline reference helpers.** `x -= step; if (x < lo) x = lo;` written in place gives `fst [x]` and reuses the value. Retail's `fstp [x]; fld [x]; fcomp [lo]` (and an integer `mov` copy for `x = lo`) only comes from an inline `template<class T> void ClampMin(T& v, const T& lo) { if (v < lo) v = lo; }` (0x00437AE0).
- **Global read before a call means a local.** A global read into a frame word before a call that the code then `fild`s afterwards is a local declared before the call (`long fps = g_FramesPerSecond; ... GetCurrentFrame() ...`). Put it in a `__forceinline` helper. A member that retail re-reads after the call is passed to that helper by `const&`; by value it is CSE'd into a register (0x00437AE0).
- **Implicit `const char*` -> CCharString conversion pushes the address.** An implicit conversion through `CCharString(const char*, long = -1)` makes VC re-take the temporary's address for the push (`lea edx,[esp+N]; push edx`). An explicit `CCharString(x, -1)` pushes the constructor's returned eax and can also swap frame slots with other temporaries. Seen in loop conditions (`while (!Finished && parser.SkipToItem(name))`, 0x009BBA40) and in map keys (`cs_chars["KEY"] = v`, 0x00E2A900).
- **A restore after a local's inline destructor is a scoped object.** When retail runs a restore after a local's inline destructor, the restore lives in the destructor of a scoped object declared first. CRenderStateManager's CreateCaptureBlock/RestoreCaptureBlock pair is such an object in 0x00499880. Neither a trailing call nor an explicit `{}` block reproduces it: the block changes the scheduling of other copies.
- **Member-wise POD copies need a user copy constructor.** A POD struct copied member by member straight from `[reg+off]` (load/store pairs interleaved) needs a user member-wise copy constructor. The implicit copy forms a base `lea` first. Seen in C2DBoxI (0x00499880), the same family as CPlane (0x00A86B30). Add a user default constructor too, so the containing classes stay default-constructible for tests.
- **CCountedPointer::operator= that copies Data/Info into frame words first** is `T* data = other.Data; CCPPointerInfo* info = other.Info; if (Info != info) { Release(); Data = data; Info = info; if (info) ++info->RefCount; }` (0x00E2A900).
- **A `""` sequence name pushed by address is a named local.** A `CCharString("")` argument whose address is pushed explicitly after the constructor is a block-local named string (`{ CCharString s(""); Game->StartMovieSequence(s, movie); }`), as in the landed CFireHeartHolder::Main (0x00E2A900).
- **pack(4) for a vfptr plus doubles.** A class with a vfptr and double members needs `#pragma pack(push, 4)`, or the doubles' 8-byte alignment shifts every offset by 4 (CProgressDisplay, 0x00499880 / 0x00499AA0).
- **CScriptThing's user copy constructor must call `CBaseClass(other)`.** Without it, VC calls the out-of-line `CBaseClass()` (0x00EC5140).
- **Inline-once destructor lane gap.** In VC7.1 the plain-inline decision for a destructor is function-wide. Retail expands it at some returns and calls it out of line at one; `__forceinline` expands every site. Park these rows (00F09260 / 00F09950 / 00E1FCF0 / 00E1FEF0 / 00E6B750 / 00D97170 / 00D91740), as the advisor confirmed.
- **The affine CMatrix3x4 inverse is not a returned matrix.** The inline inverse used by the CPhysicsMeshObject queries (0x00A11D10 family) is not a `GetInverse()` returning a matrix: that keeps a 0x30-byte temporary (frame 0x70 v 0x40). Retail keeps only the elements it reloads. The fld/fmul operand order also changes from one product to the next and across the four sibling functions, and swapping source operands has no effect.

Review at landing:
- Rows that needed more than 4 grades:
  - 00816150 (9, parked; the brief's 4-attempt cap was overridden by the operator's 5-per-strategy rule);
  - 00488b20 (many, parked);
  - 00f09260 (advisor rounds).
- Local models next to an existing engine header:
  - CPlayerGui and CStatBase in 00437ae0 / 004387f0 use the retail-offset view (PDB − 8 / − 0x14), not engine/CPlayerGui.h;
  - CPlayer in 0048a210 is a retail-view local (PlayerModes is 4 bytes);
  - CPersistContext and CStringParser are local models (the generated CStringParser header is 0x30 v retail 0x28).
- Five rows are catalog entries holding several functions: 00f27ea0, 00f28610, 00f33b90, 00f349f0, 00f351f0 (`Transfer<signed_char>`).
- Placeholder literals:
  - the cast key at 0x01255174 in 00e2a900 ("HERO" placeholder);
  - the error message at 0x01235404 in 009bba40.
  Both are relocation-masked; check the real text in the binary before landing the comments.
- Descriptive names for unlabelled callees:
  - in 00499880: IsDeviceLost, Flush, ToggleRendering, BeginFrame, EndFrame, Present, PrepareDebugRender;
  - the __fastcall cut-scene runner 0x00CBFB7D in 00e2a900;
  - 0x009B9C00 in 009bba40, named SkipToItem.
