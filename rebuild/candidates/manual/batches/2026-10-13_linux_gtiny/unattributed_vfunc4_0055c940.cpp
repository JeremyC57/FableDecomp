// CClass_0055c940::vfunc_4(long unused0) -- writes 0 into the dword member at +0x8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055C940, oracle length 10 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov dword ptr [ecx + 8], 0 ; ret 4
// Tiny-body shape `StoreConstant` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CClass_0055c940 {
    unsigned char _pad[0x8];
    long m_value;                      // +0x8
    void vfunc_4(long unused0);
};

void CClass_0055c940::vfunc_4(long unused0)
{
    m_value = 0;
}
