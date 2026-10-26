// CClass_00633bd0::ForwardThroughMember_00633bd0(long, long) -- calls the target's method @ 0x00A0D2D0 on the member pointer at +0x1f0; the 2 argument(s) are not forwarded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633BD0, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [ecx + 0x1f0] ; call 0xa0d2d0 ; ret 8
// Tiny-body shape `ForwardThroughMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CAInputProcess { public:
public:
    void RemoveFromInterface();   // 0x00A0D2D0
};

struct CClass_00633bd0 {
    unsigned char _pad[0x1f0];
    CAInputProcess* m_pTarget;                    // +0x1f0
    void ForwardThroughMember_00633bd0(long, long);
};

void CClass_00633bd0::ForwardThroughMember_00633bd0(long, long)
{
    m_pTarget->RemoveFromInterface();
}