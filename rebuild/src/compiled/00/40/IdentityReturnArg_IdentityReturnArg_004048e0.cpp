// C004048e0::M() -- return this (identity accessor)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004048E0, 3 bytes. Callees: none.
//
// `mov eax,ecx; ret`: a member that hands back its own object. Every accessor of the form
// "give me the X I contain" where X is the object itself (a base-interface getter, a conversion to a
// sub-object at offset 0, a trivial `operator T&`) compiles to this and the linker shares the bytes.
//
// Matching: `this` in ecx returned in eax with no other code.
struct C004048e0 { int x; C004048e0* M(); };
C004048e0* C004048e0::M(){ return this; }
