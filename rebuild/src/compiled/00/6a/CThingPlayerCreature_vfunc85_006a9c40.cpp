// CThingPlayerCreature::vfunc_85(long) -- returns the float at +0xa8 of the object at +0xe0 (the argument is unused)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A9C40, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0xe0] ; fld dword ptr [eax + 0xa8] ; ret 4
// Tiny-body shape `GetFloatThroughArg` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0xa8];
    float m_value;                         // +0xa8
};

class CThingPlayerCreature { public:
    unsigned char _pad[0xe0];
    CTarget* m_object;                     // +0xe0
    float vfunc_85(long unused);
};

float CThingPlayerCreature::vfunc_85(long unused)
{
    return m_object->m_value;
}