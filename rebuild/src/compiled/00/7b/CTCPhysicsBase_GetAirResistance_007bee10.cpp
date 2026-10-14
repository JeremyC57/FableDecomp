// CTCPhysicsBase::GetAirResistance() -- the float at +0x54 of the object at +0x38 times the scale at +0x44
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BEE10, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x38] ; fld dword ptr [eax + 0x54] ; fmul dword ptr [ecx + 0x44] ; ret 
// Tiny-body shape `GetScaledTargetValue` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad1[0x54];
    float m_value;                         // +0x54
};

struct CTCPhysicsBase {
    unsigned char _pad2[0x38];
    CTarget* m_pTarget;                     // +0x38
    unsigned char _pad3[0x8];
    float m_scale;                          // +0x44
    float GetAirResistance();
};

float CTCPhysicsBase::GetAirResistance()
{
    return m_scale * m_pTarget->m_value;
}