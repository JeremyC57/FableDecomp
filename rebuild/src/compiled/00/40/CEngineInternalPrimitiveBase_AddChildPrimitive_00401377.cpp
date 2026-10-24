// CEngineInternalPrimitiveBase::AddChildPrimitive() -- trivial member that always reports 0 / false
// Leaf: AddChildPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401377, 3 bytes. Callees: none.
//
// A three-byte member (`xor eax,eax; ret`) that returns zero whatever the object. Bodies this small are
// shared by the linker between every member that compiles to "return 0", so the name attached to this
// address is the one the symbol tables happened to keep; it reads just as well as a `bool` query that
// answers "no" or an accessor for a feature the base class does not implement.
//
// Matching: `this` arrives in ecx and is never read; any zero-returning __thiscall/__fastcall body
// produces the same bytes.
struct T { int Get(); };
int T::Get() { return 0; }
