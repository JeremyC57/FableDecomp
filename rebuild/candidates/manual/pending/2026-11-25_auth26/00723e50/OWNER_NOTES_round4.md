# Owner's notes for row 2, 0x00723E50 (verbatim, 2026-11-25)

These are the owner's hypotheses. Verify each against the evidence (PDBs, headers, landed matched rows, retail bytes) before you rely on it, and grade each verified candidate once.

---

Since `InitialiseFromAngles`, `SetForward`, and the `0x0066C900` union-based `Normalise()` are now verified (bringing the end down to a single swapped pair), look closely at the only remaining inputs to the top of `SetRHSetAsFlushOnNormal`:
1. Why `mov eax, [esp+1Ch]` (`&normal`) Is Hoisted Before `fsub`
In MSVC 7.1, the backend only hoists a parameter load (`mov eax, [esp+1Ch]`) above an `fsub` when one of three conditions holds in the IR right after `GetFacingAngleXY()` returns:

1. `GetFacingAngleXY()` is a `const` member function vs. non-`const` (or virtual vs. non-virtual), or returns `float` vs. `const float&`: Check `GetFacingAngleXY()` in the PDB. If it is `float GetFacingAngleXY() const` vs. non-const, or if it accesses a pointer chain (`this->...`) that clobbers `eax`/`ecx`, MSVC's alias analysis changes whether `esp+1Ch` is loaded before or after the `fsub`.
2. `operator^` (cross product) parameter evaluation order (`a ^ b` vs. `b ^ a`, or member vs. free function): Even though `g10` wrote `right_2d.InitialiseFromAngles(...) ^ normal`, C++ does not guarantee left-to-right argument evaluation for `operator^(const C3DVector& lhs, const C3DVector& rhs)`. If `operator^` is a free function `C3DVector operator^(const C3DVector&, const C3DVector&)` vs. a member function `C3DVector C3DVector::operator^(const C3DVector&) const`, MSVC evaluates the implicit `this` pointer (`ecx`) vs. the explicit reference parameter (`[esp+1Ch]`) in opposite orders—which directly dictates whether `mov eax, [esp+1Ch]` is emitted before or after `fsub`!
3. `SetUp(normal)` placement: Because `SetUp(normal)` takes `normal` directly and `operator^` also takes `normal`, MSVC's common-subexpression elimination (CSE) merges the address calculation `&normal` (`lea`/`mov eax, [esp+1Ch]`) if both calls see `normal` in the same basic block. If `SetUp` is `void SetUp(const C3DVector& up)` vs. `const C3DVector& SetUp(const C3DVector& up)` (or if `RHSet` is accessed via a getter `GetRHSet()` vs. direct member `RHSet`), the CSE live-range of `&normal` changes, moving `mov eax, [esp+1Ch]` above `fsub`.

2. Fixing the Final Swapped Pair at the End
Now that the `0x0066C900` union-based `Normalise()` reduced the tail to one swapped pair, that single swap is almost certainly caused by either:

* Instruction-level register pressure inherited from the top-of-function `mov eax, [esp+1Ch]` hoist (fixing the start will likely cascade and fix the final pair automatically), OR
* Member copy order in `CRHSet::SetForward` / `CRHSet::SetUp`: Even when written `Forward = forward;` and `Up = normal;`, MSVC 7.1 decomposes a 12-byte `C3DVector` struct copy into three 32-bit `mov`s (`x`, `y`, `z`). Whether `C3DVector` has a user-defined copy constructor / `operator=` (which disables aggregate `rep movsd`/scalar decomposition and forces explicit `this->x = rhs.x` order) vs. compiler-generated trivial copy changes the exact interleaving of `SetForward` and `SetUp` stores at the end of the function.
