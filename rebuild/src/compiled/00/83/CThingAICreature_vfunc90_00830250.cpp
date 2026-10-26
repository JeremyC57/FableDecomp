// CThingAICreature::vfunc_90() -- the member at +0xe0 plus 0x100
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00830250, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0xe0] ; add eax, 0x100 ; ret 
// Tiny-body shape `GetOffsetValue` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CThingAICreature { public:
    unsigned char _pad[0xe0];
    long m_value;                          // +0xe0
    long vfunc_90();
};

long CThingAICreature::vfunc_90()
{
    return m_value + 0x100;
}