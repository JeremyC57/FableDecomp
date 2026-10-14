#pragma optimize("s",on)
// CClass_006590f4::ApplyToMember_006590f4(void*) -- calls the member method @ 0x006600BE with the argument and the address of the member at +0x5c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006590F4, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  lea eax, [ecx + 0x5c] ; push eax ; push dword ptr [esp + 8] ; call 0x6600be ; ret 4
// Tiny-body shape `ApplyToMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CMember;

struct CClass_006590f4 {
    unsigned char _pad[0x5c];
    unsigned char m_member[4];            // +0x5c (opaque)
    void Apply(void* value, void* member); // 0x006600BE
    void ApplyToMember_006590f4(void* value);
};

void CClass_006590f4::ApplyToMember_006590f4(void* value)
{
    Apply(value, &m_member);
}