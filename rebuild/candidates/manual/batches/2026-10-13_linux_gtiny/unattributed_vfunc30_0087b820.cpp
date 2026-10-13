// CClass_0087b820::vfunc_30(long unused0) -- writes 1 into the byte member at +0xc8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087B820, oracle length 10 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx + 0xc8], 1 ; ret 4
// Tiny-body shape `StoreConstant` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CClass_0087b820 {
    unsigned char _pad[0xc8];
    unsigned char m_value;             // +0xc8
    void vfunc_30(long unused0);
};

void CClass_0087b820::vfunc_30(long unused0)
{
    m_value = 1;
}
