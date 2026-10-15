# Lessons from chunk 130 (candidates for GOTCHAS)
- GetInterface out pointer used unconditionally: the call tail is duplicated into the garbage-read miss path when only the call (and a base call) follows;
  when more statements follow, the two paths join (`jmp` to the shared push). A null-initialised out pointer (`T* p = 0;`) is held in esi/edi
  (`xor esi,esi` before the GetPItem call) rather than in the frame word.
- C3DVector copies: an init-list copy through a MEMBERWISE USER COPY CONSTRUCTOR (`X(o.X), Y(o.Y), Z(o.Z)`) gives fixed-offset word moves
  (`mov [esi+114h],edx`, no FPU) where an implicit/POD copy or a body assignment re-bases with `lea` (TeleportOut 008bcf20 vs 008bcea0,
  TrollThrowProjectile 008c7d80 vs 008c7390). A class with that user copy constructor needs an empty default constructor for body-assigned members.
- Several GetPItem calls evaluated before any push, then a float push and per-argument calls = an inline forwarder taking the things/components BY VALUE
  (008caf10); inline member forwarders evaluate their arguments first and their object last.
- A copy of `this` into the lookup-key frame word before an `erase(const key&)` call is `set.erase(this)` (the rvalue binds to a const& temporary).
- An implicit destructor of a polymorphic stack object stores no vtable: the counted-pointer member's inline destructor runs, then the out-of-line base
  destructor (006dd980). Same for an IP temporary (008cbe10).
- `fild [global] ; call __ftol2` = `(long)(float)global`; `fld ; fadd st(0),st(0) ; fstp` per component = an inline `operator*=(2.0f)`.
- Purity gate trap: the substring `_emit` also matches identifiers such as `m_emitter` in TESTS -- rename fields (`m_pParticles`).
- Lane gaps seen: a joined value feeding `this` that retail colours into ecx while the lane uses eax around an inlined by-value copy (00848fd0);
  esi/edi colouring of `this` vs a call-result local (00854720); zero-register hoisting above a string constructor call (0062d890); GetWorld() called
  before the argument pushes (008491e0, not yet reproduced).
