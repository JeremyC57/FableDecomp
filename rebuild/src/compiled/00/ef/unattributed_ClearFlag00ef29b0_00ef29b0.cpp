// CClass_00ef29b0::ClearFlag_00ef29b0() -- clears the byte flag at +0x48 of the object at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF29B0, oracle length 8 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[ecx+0x14] ; mov byte ptr [eax+0x48],0 ; ret
// Setter through one pointer member: loads the object at +0x14 and writes 0 into its byte at
// +0x48. Class and member names are descriptive (vtable-slot label for the class when there is
// one). One of 8 bodies of this shape.
struct CTarget {
    unsigned char _pad[0x48];
    bool m_flag;                           // +0x48
};

struct CClass_00ef29b0 {
    virtual void v00();
    virtual void ClearFlag_00ef29b0();
    unsigned char _pad[0x10];
    CTarget* m_target;                     // +0x14
};

void CClass_00ef29b0::ClearFlag_00ef29b0()
{
    m_target->m_flag = false;
}