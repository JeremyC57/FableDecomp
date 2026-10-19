# Lessons (chunk 196)
- An odd-offset intelligent pointer followed by a 4-aligned vector (CModeScorpionKingVulnerable +0x7d / +0x88) needs pack(1) on the
  pointer classes only, not on the mode class.
- A mode ctor whose base takes a string (CModeIntangible) builds the literal's temporary before the base chain: model the base
  as an inline `Base(CThing*, const CCharString&)` ctor.
- A by-value CCountedPointer argument built in its stack slot (`sub esp,8 ; mov eax,esp ; mov [eax],ebx`) needs a user copy ctor;
  C3DVector body assignments compile to plain `[esi+N]` stores only with a user `operator=`.
- The CAnimInit record's +0x18 pair is two floats zeroed by a member ctor: as pointers VC7.1 CSEs the zeros into a register.
- A const carried thing (`const CThing* item = carrying->GetThingInPrimarySlot()`) binds the const LowerBound copy 0x4365B0.
- HasTC + a GetTC whose end/hit selection is a single ternary returns the component in ecx when it is next used as `this`.
- A polymorphic thing model must carry its vptr in the root class; an empty non-polymorphic base (for a callee's real class)
  under a polymorphic derived model shifts `this` by 4.
- A loop that erases begin() each pass and keeps begin in a register comes from `T* it = v.begin(); while (it != v.end()) v.erase(it);`.
- The frame rate read before a virtual call (`mov eax,[fps] ; mov [esp+8],eax ; call ; fimul [esp+8]`) is a named local.
