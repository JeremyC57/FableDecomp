# Chunk 537 lessons (retry)
- A `T& q = reinterpret_cast<T&>(Member)` local is not free: VC7.1 then caches the member's fields in callee-saved
  registers across branches and can swap the roles of unrelated constants for the whole function. Re-spelling the cast in
  each expression (no local) matched 0x00492BAB at once (ndiff 31 -> 0).
- An address-taken local stays in a register in retail when the callee's *body* is visible in the TU (STLport members,
  header templates such as IS_THING_ALIVE). Give a same-TU noinline body, under the real name, for the callee AND for
  anything it passes the reference to (vector::push_back alone was not enough; push_back + _M_insert_overflow was).
  An explicit specialization defined before the caller works.
- `Filter()` on a POD filter class is value-initialized (zeroed: one extra `mov byte [esp+x],0`); retail filters have a
  user ctor.
- Real accessor names can change codegen for the better: CSystemManager::GetDisplayManager() (PDB) replaced
  `->PDisplayManager->` / CCountedPointer operator-> and fixed the `mov eax,[eax+60h]` fetch.
- Check member and vtable slot names against the vtable itself (tools/vt.py), not the parked guess: CWorld slot 13 is
  PeekWorldMap, CObservable 5/9 are RemoveObserver / AddConcurrentExclusiveObserver.
- A function whose body ignores `this` (ICF-folded getter) collects many bound names; pick by the caller's class and
  flag it for the coordinator.
- RTTI bases (ghidra_out/rtti_bases.tsv) settle base-class names fast (CBaseClassNonCopyable, CChangingStateComponent,
  CUIInterface/CObserver/CActionParam under CComponent).
