// CTCFireRangedWeaponOnLeavingStateGroup::vfunc_28() -- writes 1 into the byte member at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D65A4, oracle length 5 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov byte ptr [ecx + 0xc], 1 ; ret 
// Tiny-body shape `StoreConstant` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CTCFireRangedWeaponOnLeavingStateGroup {
    unsigned char _pad[0xc];
    unsigned char m_value;             // +0xc
    void vfunc_28();
};

void CTCFireRangedWeaponOnLeavingStateGroup::vfunc_28()
{
    m_value = 1;
}
