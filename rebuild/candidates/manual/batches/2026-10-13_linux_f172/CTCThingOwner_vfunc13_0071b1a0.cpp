// CTCThingOwner::vfunc_13() -- sets the byte flag at +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071B1A0, oracle length 5 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx+0x10],1 ; ret
// Virtual setter: writes 1 into the one-byte member at +0x10 and returns. The class comes
// from the vtable-slot label of this address; the flag's name is descriptive. One of 8 bodies.
struct CTCThingOwner {
    virtual void v00();
    virtual void vfunc_13();
    unsigned char _pad[0xc];
    bool m_flag;                           // +0x10
};

void CTCThingOwner::vfunc_13()
{
    m_flag = true;
}
