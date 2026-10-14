// CPhysicsFacesSegmentLeaf::GetNoFaces() -- the element count of the 2-byte-element range at +0x4/+0x8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A860F0, oracle length 9 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 8] ; sub eax, dword ptr [ecx + 4] ; sar eax, 1 ; ret 
// Tiny-body shape `GetCount` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct SElement { unsigned char _body[0x2]; };

struct CPhysicsFacesSegmentLeaf {
    unsigned char _pad[0x4];
    SElement* m_first;                     // +0x4
    SElement* m_last;                      // +0x8
    long GetNoFaces();
};

long CPhysicsFacesSegmentLeaf::GetNoFaces()
{
    return m_last - m_first;
}