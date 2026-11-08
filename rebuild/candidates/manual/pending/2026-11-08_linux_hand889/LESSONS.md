# Chunk 889 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- To keep retail's `fmul [1/255] ; fadd st0,st0` instead of a folded `*2/255`, write the colour fields in place first (`c.Initialise(R*(1/255.f), ..., A*(1/255.f))`) and then double in a separate statement (`c.A *= 2.0f`). A constructor temp, an initializer list or a helper taking `k*2` folds or delays the doubling (0x00BCA950, `fadd st0,st0` at 0x00BCAE42).
- For retail's setle shape, spell `Max` as `b < a ? a : b` (`a < b ? b : a` gives setl). A by-value `Min(T a, T b) { return b < a ? b : a; }` called as Min(retail_first, retail_second) keeps retail's evaluation order. A const-ref Min or a pointer select stores the wrong operand (0x00BCA950 tangent loop / length).
- A camera-space side vector, `v = C3DVector(v.Y, -v.X, 0.0f)` reassigned to the same named local after an in-place TransformVector, reproduces retail's temp+copy and the unfolded 0.0 Z. A fresh local folds the zero (0x00BCA950, the camera rows at 0x00BCB2B2..).
- An unsigned tree id tested as `if (id > 0)` gives retail's `test ; jbe` in the inlined ReturnResource (0x00BCA950, jbe at 0x00BCB779).
- `for (i = 0; i < n; ++i) { long k = n - i; ... }` is the loop form that produces retail's two counters (a strength-reduced k plus a memory down-counter). A plain `for (k = n; k > 0; --k)` gives one counter (0x00BCA950: `dec ebx` mid-loop and `dec dword ptr [ebp+0x64]` at 0x00BCABAA).
- Lane tooling: never grade the same address twice at once. q.py shares one work dir per address, so the two results come back identical and both are wrong. This happened here: t38..t50 were re-graded one at a time.

Review at landing:
- Both rows used far more than 4 grades (row 1: 140, row 2: 55) under the wave's 150-grade budget. Neither matched.
- Row 2 models these locally, with no repo header: CEngineTreeFilerBase (+CNodeHeader), CEngineTreeFiler<T>, CEngineDynamic3DResource<T,N>, CEngineDynamic3DResourceManager<T>, CEngineVertexBufferPool, CEngineInternalPrimitiveSpriteTrail and CEnginePrimitiveManagerSpriteTrails. These need shared headers before the row lands. Offsets are from Ego_r and the retail reads.
- Already-landed errors: CThingFilterIsValid_func00ac20d0_00ac20d0.cpp is really CVertexBufferBase::Sizeof. CEngineParticleSimulationRenderData_IsBusy_00bb6420.cpp calls 0x00AC20C0 as `IsInUse`; it is really CVertexBufferBase::IsBusy (NAME_FIXES.tsv).
- 0x01436E80 has about ten inconsistent landed spellings (NOTES.md, Label corrections). The 0x01436E78 / 0x01436E80 split (shared-data pointer vs renderer pointer) also needs one canonical name each.
