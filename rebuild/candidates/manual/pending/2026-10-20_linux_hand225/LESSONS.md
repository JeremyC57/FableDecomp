# Chunk 225 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Same-TU callee stand-ins teach VC7.1 what a callee clobbers: compiling a NON-template stand-in body above the row under `#pragma auto_inline(off)` fixed hoisted loop-invariant temporaries and `this`-in-register shapes (0x00A76A80 find/resize, 0x00A79930 `_M_insert_overflow` 0x00A82380 + sort 0x00A84160, half of 0x008D8330 GetSubDef). A TEMPLATE member stand-in is still inlined despite `auto_inline(off)` -- make it a plain function.
- A function with one shared `return true` exit keeps all callee-saved pushes at entry; an early `return` inside the walk lets VC sink pushes below the first test (0x00A76CC0 matched on the nested form; 0x00A786C0 still sinks `push ebp ; push edi` with every form tried).
- Inline budget: when the same inline predicate is called four times and retail inlines only the first two, write the first two through an inline helper and the last two through the out-of-line function (0x00A76640 GoToHigherDetail: inline float-pair Contains, then ContainsPoint 0x00535CD0).
- STLport `__uninitialized_copy(first, last, result, const __false_type&)` compiles `__fastcall` with `ret 8` and the empty tag homed in the dead `result` slot; with the element's copy ctor implicit, VC keeps the vector copy ctors out of line -- `__forceinline` on the record's and the vector's copy ctors is needed to get the whole body inline (0x00A85420, still a register-allocation gap).
- A struct member initialised from a `const C2DVector&` copies with integer moves; retail's `fld X ; fld Y ; fxch ; fstp ; fstp` copy is a ctor that takes the C2DVector BY VALUE (0x00A1DE30 CNavWayPoint).
- A C2DVector product used twice (`dir * distance` returned by an inline `operator*`) evaluates both multiplications before the adds; writing `pos.X + dir.X * d` inline interleaves them (0x00A198C0).
- `return position;` from a C3DVector-returning function wants the C3DVector user copy ctor (else VC routes the hidden pointer through edx); the same ctor builds a by-value result in the argument area (0x00DAC650 GetHomePos).
- `ghidra_out/cgsi_slots_aligned.tsv` is the correct CGameScriptInterface slot table; `cgsi_vtable_slots.tsv` is 4 rows off (0x00EB3D10).
- Perlin's reference `noise2` (float macros, `s_curve(t) = t*t*(3 - 2t)`, `lerp = a + t*(b - a)`) reproduces CNoise::PerlinNoise to within scheduling (0x00F35C60): the engine copied the reference source.
- g++ x64 tests: `long` is 8 bytes; a test that reads 4-byte file fields into `long` walks off its buffer (0x00A772D0 hung until switched to `int`).

Review at landing:
- 0x00A76A80, 0x00A79930 (and parked 0x008D8330) carry same-TU stand-in BODIES for out-of-line callees (find/resize, `_M_insert_overflow`, sort, GetSubDef); `finish.py`/landing must keep only the row function as coverage and treat the stand-ins as scaffolding.
- 0x00A76640 declares ContainsPoint 0x00535CD0 with a `bool` result; other landed sources bind 0x00535CD0 with a `long` result -- check the shared declaration before landing.
- Local base models were used where the engine headers are flat/PDB-offset (CModeStandBase 0x0073D860, CMovie 0x00547E40, CNavigatorAStar/CNavigatorFlyer layouts: retail Route at +0x8c is 8 bytes below the Ego_r.pdb offset, STLport 12-byte vectors vs the PDB's 16-byte Dinkumware `_Vector_val`).
- Wrong catalog labels (9, listed in NOTES.md): 0x00CBC130, 0x00446BE0, 0x00441CB0, 0x00441EE0, 0x00AFBBD0, 0x004608C6, 0x0074AA00, 0x00AFF0A0, 0x00A85420.
- 0x007009D0's catalog row includes its jump table; 0x00A76E00 and 0x00B18820 need the lane's decision on the inline-asm `fistp` helper (asm_bake family).
- No row needed more than 4 grades; 0x00F35C60's third grade re-ran an unchanged source after a failed scripted edit (counted).
