// CThingAICreature::vfunc_87() -- returns the float at +0xb4 of the object at +0xe0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00830300, oracle length 13 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0xe0] ; fld dword ptr [eax + 0xb4] ; ret 
// Tiny-body shape `GetFloatThrough` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0xb4];
    float m_value;                         // +0xb4
};

class CThingAICreature { public:
    unsigned char _pad[0xe0];
    CTarget* m_object;                     // +0xe0
    float vfunc_87();
};

float CThingAICreature::vfunc_87()
{
    return m_object->m_value;
}