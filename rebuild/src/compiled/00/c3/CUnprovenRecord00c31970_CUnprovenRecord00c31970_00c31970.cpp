// CUnprovenRecord_00c31970::CUnprovenRecord_00c31970(void* target) -- store the argument; the second word starts at its constant
// Leaf: ??0CUnprovenRecord_00c31970@@QAE@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C31970, 18 bytes, thiscall, `ret 4`, returns this.
// Identity (descriptive): an 8-byte record built on the stack of 0x00C17940 (the CEditControlCentre::DeleteSelectedTrackNode helper) from the address of another local, second word 0; the editor module carries no RTTI/PDB names, so the class is not recoverable. The manifest's "CThingFilter_SolidToThing::CThingFilter_SolidToThing" is propagated across
//   byte-identical bodies.
// Shape (family M182, representative 0x00661620): member initialisers in declaration order. Speed TU.
class CUnprovenRecord_00c31970 {
public:
    CUnprovenRecord_00c31970(void* target);
    void* Target;                                         // +0x00
    long  Value;                                          // +0x04
};
CUnprovenRecord_00c31970::CUnprovenRecord_00c31970(void* target)
    : Target(target), Value(0)
{
}