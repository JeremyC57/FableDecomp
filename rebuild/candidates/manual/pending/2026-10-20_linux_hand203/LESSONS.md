# Chunk 203 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A fixed-trip float accumulation (`d = a[i] - b[i]; s += d*d` over five axes) is a ROLLED `for (i = 0; i < 5; ++i)` from `s = 0.0f`: VC7.1 unrolls it and folds `0 + d*d`, giving the `fld st(0) ; fmulp` first axis and `faddp st(2) ; fstp st(0)` later ones; the hand-unrolled form finishes the last add in place (0x00700400).
- Two inlined stream reads into ONE reused float local both home in the dead stream-argument slot; a second named local gets its own frame word (0x006FFA40).
- C3DVector operators returning a constructed temporary (`return C3DVector(X * s, Y * s, Z * s);`) are scalar-replaced: retail keeps scaled x/y on the x87 stack (z spilled) for the following `+`; operators that fill a named `result` local copy it through memory (0x00852B70/0x00852C40). Worth one try on the parked camera-module operand-order rows 0x852220/0x8531E0.
- With the landed member `operator+` (`result.X = X + other.X`) VC loads the RIGHT operand first: retail `fld member ; fadd [ret]` needs the call result on the left (`GetPosition() + DollyVector`, 0x00852350).
- An implicit `const char*` -> CCharString conversion inside a by-value record ctor (`CDefClassInit("CScriptDef", alloc)`) re-addresses the temporary with `lea` for the copy ctor; an explicit `CCharString("..", -1)` reuses eax (0x00F2A0F0).
- A CVectorMap `Add` (push_back of a {key, counted pointer} temporary + Dirty) must be `__forceinline`, else VC7.1 emits it out of line even in a speed TU (0x00CB8230).
- Retail CScriptBase: Threads list one word at +4, CVectorMaps 16 bytes, so EntityScriptBindings +0x18, LastWorldUpdateFrame +0x28, PActiveThread +0x2c, RuntimeScriptedEntityPersist +0x30 (PDB -0x10).
- In the 0x00E2xxxx quest module ~CScriptThing is inline (CScriptThing vtable store, PImp release with the re-read block, both words cleared, then out-of-line CBaseClass::~CBaseClass) -- model `virtual ~CScriptThing() {}` over a CBaseClass with an out-of-line virtual dtor (0x00E29F50).
- A camera-module base call common to both arms of an if/else is ONE statement after it: VC tail-duplicates it and pops the arm's callee-saved register first (0x00852FD0).
- A compiler-generated `operator=` of a struct whose C3DVector members have a user copy ctor and no operator= gives the re-based pointer-register copies; it is emitted for a one-line `a = b;` helper, leaf `??4Class@@QAEAAV0@ABV0@@Z` (0x0075BA30).

Review at landing:
- 0x00C40750 is sourced as `CEffectOwner_00c40750::Reset` (line 1) although the catalog row says CScreenEffectPriorityStack::Reset; its block members are named by kind (no SDK struct matches).
- 0x00852C40 is sourced as CScriptModuleThingLookAtUpperBody::Update (catalog: ThingLookAtFace); the Face/UpperBody pairing of the two bodies is by elimination.
- Landed name conflicts: 0x0143BC28 (`g_renderFlags_0143bc28` vs `g_flags_0143bc28`), 0x01375550 (`long` vs `const long` g_FramesPerSecond), 0x00CBD310 (three element-type spellings), 0x00F35B10/0x00F35B30 (RegisterFilter/IsDone vs SetPredicate/IsActiveThreadTerminating).
- Several engine headers carry PDB offsets that retail does not use here: CReactionMatch_Sphere (SqRadius +0x14 retail, +0x18 header), CScriptBase (no header; retail -0x10), CSBCCorridor (local model next to the header, offset asserted).
- No row needed a pragma or a flag; no row exceeded 4 grades.
