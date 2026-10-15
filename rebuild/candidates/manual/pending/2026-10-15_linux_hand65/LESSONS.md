# Chunk 65 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A default value built by default-constructing a temporary, copy-constructing the named default from it and destroying
  the temporary before the call = an inline helper returning a named local (`static T DefaultValue() { T v; return v; }`,
  VC7.1 has no NRVO); `T d = T();` and `T d = static_cast<const T&>(T());` are both elided.
- A `??_G` with the whole destructor chain inlined needs `__forceinline` on every virtual destructor in the chain and on
  member destructors; a derived class with an empty destructor and no members drops its own vtable store as a dead
  store without `novtable` (the three NShape `??_G` bodies store only the base vtable).
- A counted-pointer deleter with the destructor inlined = the expanded delete `if (p) { p->~T(); operator delete(p); }`
  with `__forceinline` member destructors; a plain `delete p` of a non-polymorphic class calls an out-of-line `??_G`.
- Labels: 0x004AEF50 "IsBinary" is `IsSaving` (mode 1 or 3); two-argument Transfer wrappers reveal their type through
  the default (double 0.0 = `<double>` despite a "Transfer<long>" label); 0x0099A2E0 is `CBaseClass::~CBaseClass`.
  13 of the 15 "CPatchTesselationEdgeStrip" rows and 6 of the 24 "CPersistContext" rows belong to other classes (NOTES.md).

Review at landing: all five parked rows look like lane gaps (argument copies through one register vs two, `[eax+ecx]`
operand order, swapped frame slots of two string locals) -- one attempt each at most, then the Windows RTM re-grade.
