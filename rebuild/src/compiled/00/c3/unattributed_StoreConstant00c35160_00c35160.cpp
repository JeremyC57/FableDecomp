// CClass_00c35160::StoreConstant_00c35160(long unused0, long unused1) -- writes 0 into the dword member at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35160, oracle length 10 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov dword ptr [ecx + 4], 0 ; ret 8
// Tiny-body shape `StoreConstant` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CClass_00c35160 {
    unsigned char _pad[0x4];
    long m_value;                      // +0x4
    void StoreConstant_00c35160(long unused0, long unused1);
};

void CClass_00c35160::StoreConstant_00c35160(long unused0, long unused1)
{
    m_value = 0;
}