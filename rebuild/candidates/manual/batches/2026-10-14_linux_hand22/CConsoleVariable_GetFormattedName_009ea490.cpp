// CConsoleVariable::GetFormattedName() const -- the variable's name followed by a space
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EA490, length 26 bytes.
// Callee: operator+(const CCharString&, const char*) @ 0x0099F600 (__fastcall: hidden result in
// ecx, left operand in edx, the literal " " @ 0x01258C1C on the stack) -- the same operator the
// landed CBordelloLady::GetLHTSTag uses. Mangled retail name
// ?GetFormattedName@CConsoleVariable@@EBE?AVCCharString@@XZ (private virtual const, CCharString
// by value: the hidden result pointer is the only stack argument, `ret 4`).
// Layout: the PDB shows CConsoleVariable as 16 bytes with the CGenericVarRef at +8; the dword at
// +4 (listed as padding) is the name string this function concatenates.
struct CCharString {
    void* PStringData;
    ~CCharString();                              // 0x0099EAE0 (not reached: the result is returned)
};

CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

struct CConsoleVariable {
    virtual void v00();
    CCharString Name;                            // +0x04
    unsigned char Var[8];                        // +0x08 CGenericVarRef
    virtual CCharString GetFormattedName() const;
};

CCharString CConsoleVariable::GetFormattedName() const
{
    return Name + " ";
}
