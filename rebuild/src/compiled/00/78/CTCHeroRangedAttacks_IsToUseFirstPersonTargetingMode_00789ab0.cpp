// C3DMeshInfo::HasPhysicsMesh() -- non-zero (as a byte) when the index at +0x44 is not -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789AB0, oracle length 8 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  cmp dword ptr [ecx + 0x44], -1 ; setne al ; ret 
// Tiny-body shape `HasIndex` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct C3DMeshInfo {
    unsigned char _pad[0x44];
    long m_index;                          // +0x44
    unsigned char HasPhysicsMesh();                  // a byte result: a `bool` return would zero eax first
};

unsigned char C3DMeshInfo::HasPhysicsMesh()
{
    return m_index != -1;
}