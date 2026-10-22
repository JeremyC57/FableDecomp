# Chunk 282 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 008beee0: `C3DVector dir(C2DVector(a - b)); dir.Normalise();` with a Z=0 2D->3D converting ctor and a FastInvSqrt Normalise scales X/Y in place (`fxch st(2) ; fmul st(2)`). `C3DVector(dx, dy, 0)` duplicates the reciprocal instead (`fld st(0) ; fmul st(3)`).
- 0085bd70 / 006e99b0: write x87 dot products as sequenced statements (`d = a.Y*b.Y; d += a.Z*b.Z; d += a.X*b.X;`). The terms come out in statement order. A single-expression sum gets reassociated.
- 0085bd70: a TEMPORARY CRightHandedSet argument (`ResetRHSet(CRightHandedSet(...))`) gives retail's early vptr load and push before the constant stores. A named local interleaves them.
- 008f3630: VC7.1's x87/push interleave comes from a fixed-size scheduling window counted in IR instructions from the start of the basic block. `0.5f * (r * (3.0f - lsq * (r * r)))` emits the same FastInvSqrt bytes as `(3.0f - r*r*lsq) * r * 0.5f` but carries 2 more hidden IR ops. Method: insert k dummy global stores early in the block to measure how many hidden ops retail had, then vary the expression grouping to match.
- 008f3630: inline accessors (`GetPhysics()`, `CThing::GetDef()`) and a `__forceinline` manager GetDef wrapper make VC7.1 load straight into ecx and evaluate the object call before the argument pushes. They also fixed the ebp/edi callee-saved swap. Do not stand in the eax-only CBaseIntelligentPointer ctor 0xA01B10.
- 00849380: `static_cast<const CThing*>(p)->GetInterface(&out, type)` calls the const LowerBound 0x004365B0; the non-const form calls 0x0040F020. An inline `GetShopKeeper() { return PShopKeeper.GetPItem(); }` gives `lea ecx,[eax+10h]`.
- 008f3310 / 006e99b0 / 0087e860: a memberwise user C3DVector copy ctor drops dead Z copies and flips `pos + dir` to `fld dir ; fadd pos`.
- 005331a0: a same-TU, non-template explicit specialisation (auto_inline off) of STL `_Rb_tree::insert_unique(iterator, const value_type&)` whose body only reads the value makes VC7.1 store the pair key last. A stand-in with no side effects is deleted entirely, so it must write memory.
- 00ef12f0: in-body `#pragma inline_depth(0)` ... `#pragma inline_depth()` mixes inlined and out-of-line destructor sites (~CScriptThing 0x004AA840) inside one function. The purity gate accepts pragmas.
- 0052ffd0: a free inline `Add(C2DVector& a, const C2DVector& b) { a.X += b.X; a.Y += b.Y; }` loads the added operand first. Member `operator+=` loads the destination first.
- 00855500: `v *= FastInvSqrt(v.GetLengthSquared())` with a plain-inline length spills the reciprocal (`fstp [r]; fxch st(1); fmul [r]`). A `__forceinline` Normalise keeps it on the stack (`fst [r]; fmul st(2)`). Write the squared length as `x*x + (y*y + z*z)` for the z, y, x fmul order.
- 009ead60: an implicit double->float initialisation (`float f = g_double * -2.0;`) matches the retail prologue. An explicit (float) cast moves the store before `push ebp`.
- 006eb390: an ordinary local's register priority falls as its live range grows. The out-pointer `T* tc; if (thing->GetInterface(&tc, type))` form lengthens the range compared with HasTC + GetTC and flips this->ebx / local->ebp to retail's choice.
- 008cda30 / 008cd820: an inline `bool IsDead() const`, CIntelligentPointer operator-> / operator* (evaluated before the pushes), and a ternary GetInterface out-store carry across CCreatureAction_BattleCharge* siblings. Try a matched sibling's source first.
- 006ea770: a same-TU `__declspec(noinline)` body for the combatant-map find 0x006F0250 frees a key slot (frame 5Ch vs 60h).
- 008c12d0: a local passed by pointer to an in-TU callee (even noinline, or defined after the caller) is non-escaping. With an extern callee it is address-exposed: it gets reloaded and the early return is tail-duplicated. The escape status also recolours unrelated values.
- 008f3310 / 006e99b0: before spending attempts on a single-slot scheduling miss, check whether byte-neutral edits move the slot at all. If only byte-changing edits in the same block move it, park the row as a lane gap.
- finish.py takes the name from the FIRST `// Name(` line, and `// Grade: ... (` also matches that pattern. Keep `// Module::Name(...) -- ...` as line 1, with the Grade line after it (006ea770 and 008f3630 had it reversed; fixed at finish).

Review at landing:
- Many rows took more than 4 grades over the resumed runs. 008f3630, 006e99b0 (~200), 008f3310 (~120), 0052ffd0 (256 forms) and 006eb390 all used heavy probing; re-grade them against the landing toolset.
- 00855500: retail flag offsets are +0x124/+0x125/+0x131, which do not fit the generated engine header, so the source uses a local model.
- 006ed510: the CThingCreatureBase bitfields at +0x15c need a CThing model without __int64 tail padding. The PDB-derived CThing (padded to 0x98) shifts them to +0x162.
- The landed 0x006EB600 source calls vtable slot 8 'IsRangedSequence'; the real name is IsBreakIntoMeleeSequence (0x006F0710). Only the local name is wrong; the bytes are the same.
- rescue_ext/006eb390.md is stale; the row is now matched.
- Catalog row 008f3100 carries 008c0900's name. Its real name is CCreatureAction_DragonLandFireball::PerformAction.
