#pragma optimize("s",on)
// CClass_006590e4::ApplyToMember_006590e4(void*) -- calls the member method @ 0x00660090 with the argument and the address of the member at +0x58
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006590E4, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  lea eax, [ecx + 0x58] ; push eax ; push dword ptr [esp + 8] ; call 0x660090 ; ret 4
// Tiny-body shape `ApplyToMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CMember;

struct CClass_006590e4 {
    unsigned char _pad[0x58];
    unsigned char m_member[4];            // +0x58 (opaque)
    void Apply(void* value, void* member); // 0x00660090
    void ApplyToMember_006590e4(void* value);
};

void CClass_006590e4::ApplyToMember_006590e4(void* value)
{
    Apply(value, &m_member);
}