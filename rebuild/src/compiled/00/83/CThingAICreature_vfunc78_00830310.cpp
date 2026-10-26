// CThingAICreature::vfunc_78(long) -- returns the float at +0x9c of the object at +0xe0 (the argument is unused)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00830310, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0xe0] ; fld dword ptr [eax + 0x9c] ; ret 4
// Tiny-body shape `GetFloatThroughArg` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x9c];
    float m_value;                         // +0x9c
};

class CThingAICreature { public:
    unsigned char _pad[0xe0];
    CTarget* m_object;                     // +0xe0
    float vfunc_78(long unused);
};

float CThingAICreature::vfunc_78(long unused)
{
    return m_object->m_value;
}