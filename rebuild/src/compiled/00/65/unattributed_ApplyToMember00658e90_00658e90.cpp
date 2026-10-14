#pragma optimize("s",on)
// CClass_00658e90::ApplyToMember_00658e90(void*) -- calls the member method @ 0x0065F856 with the argument and the address of the member at +0x54
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658E90, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  lea eax, [ecx + 0x54] ; push eax ; push dword ptr [esp + 8] ; call 0x65f856 ; ret 4
// Tiny-body shape `ApplyToMember` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CMember;

struct CClass_00658e90 {
    unsigned char _pad[0x54];
    unsigned char m_member[4];            // +0x54 (opaque)
    void Apply(void* value, void* member); // 0x0065F856
    void ApplyToMember_00658e90(void* value);
};

void CClass_00658e90::ApplyToMember_00658e90(void* value)
{
    Apply(value, &m_member);
}