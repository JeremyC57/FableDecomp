// CLHFile_00add5c0::IsNull() const -- an unreferenced 10-byte member after the two CLHFile constructors: true when its first word is zero
// Grade: DIFFER(name) -- name unproven: CLHFile / LHA have no PDB symbols, and nothing references 0x00ADD5C0 (no call, no vtable cell)
// Leaf: ?IsNull@CLHFile_00add5c0@@QBE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADD5C0, 10 bytes: `mov edx,[ecx] ; xor eax,eax ; test edx,edx ; sete al ; ret`, in the CLHFile TU
//   between the constructors 0x00ADD4A0 and the CLHFileCompletionCallback ??_G 0x00ADD5D0. The bytes reproduce as below (graded
//   RELOCATION-free MATCH under this placeholder), but the class and member names are invented: parked, not landed.
// Next idea: an inline of a header class the CLHFile TU includes (a CCountedPointer / CIVCountedPointer null test emitted out of line,
//   e.g. `operator!` or `IsNull`) -- find which header class has a pointer at +0 and a bool `== 0` accessor in Ego_r / FableWin.
class CLHFile_00add5c0 {
public:
    bool IsNull() const;
private:
    void* First;                                          // +0x00
};
bool CLHFile_00add5c0::IsNull() const
{
    return First == 0;
}
