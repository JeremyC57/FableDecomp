// CTCGatherPointBuilding::vfunc_29() -- copies the member at +0x4 into the member at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FFD50, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; mov dword ptr [ecx + 0x1c], eax ; ret 
// Tiny-body shape `CopyMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTCGatherPointBuilding {
    unsigned char _pad1[0x4];
    long m_source;                         // +0x4
    unsigned char _pad2[0x14];
    long m_target;                         // +0x1c
    void vfunc_29();
};

void CTCGatherPointBuilding::vfunc_29()
{
    m_target = m_source;
}