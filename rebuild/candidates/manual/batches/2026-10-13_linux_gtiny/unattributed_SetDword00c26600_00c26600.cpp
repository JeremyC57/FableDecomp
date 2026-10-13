// CClass_00c26600::SetDword_00c26600(long unused0, long value) -- stores argument 2 into the dword member at +0x48
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C26600, oracle length 10 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [esp + 8] ; mov dword ptr [ecx + 0x48], eax ; ret 8
// Tiny-body shape `SetDword` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CClass_00c26600 {
    unsigned char _pad[0x48];
    long m_value;                          // +0x48
    void SetDword_00c26600(long unused0, long value);
};

void CClass_00c26600::SetDword_00c26600(long unused0, long value)
{
    m_value = value;
}
