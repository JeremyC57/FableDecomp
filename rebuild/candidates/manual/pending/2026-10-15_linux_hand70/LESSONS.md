# Chunk 70 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A `malloc`'s `add esp,4` that retail places later in a constructor is flushed right before an inlined member call's
  stores (`MovementVector.Clear()`); plain assignments let it land straight after the call.
- Retail's `push ; sub esp,8 ; push esp ; call [eax+N]` (a call result built in an argument slot) needs the by-value
  class to have BOTH a user constructor and a user copy constructor; without the copy constructor the result goes to a
  frame temporary and is copied.
- `__declspec(novtable)` on a class whose destructor reaches a base at +4 gives retail's
  `test esi,esi ; je ; lea ecx,[esi+4] ; jmp` / `xor ecx,ecx ; jmp`; the plain polymorphic form stores vtables and drops the test.
- `m = m > a ? m : a` with the member on the left = `cmp ; jbe`; the const-reference max template re-reads the argument through `lea`.
- `x -= 1.0f; x = x > max ? max : x;` keeps the value on the FPU stack (`fcom ; fst ; jne ; fstp st0 ; fld`).
- VC7.1 groups constant stores by value register and keeps source order inside each group (the lane's register choice
  for the groups can differ from retail: parked 004016f0).
- Retail def layouts: CParentDefClassBase is byte-packed (InstantiationName +0x25, SubDefInfoMap +0x29, derived members
  from +0x3c); CVectorMap is 16 bytes (vector, comparator byte, sorted flag at +0x0d); CRegionDef sits 0x0C below
  Ego_r.pdb at WorldMapOffset/NameGraphicOffset; `or dword ptr [..],-1` for a -1 store marks an `optimize("s")` TU.

Review at landing: parked 0066c7b0 used 6 grades (over the 4-attempt rule) on the known copy-register lane gap -- give it
none. 0055deb0 is named CActivity's constructor on a layout match only (one source of evidence). 14 label corrections in NOTES.md.
