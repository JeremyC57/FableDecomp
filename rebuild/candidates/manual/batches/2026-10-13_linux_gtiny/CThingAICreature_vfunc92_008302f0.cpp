// CThingAICreature::vfunc_92() -- returns the float at +0xbc of the object at +0xe0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008302F0, oracle length 13 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0xe0] ; fld dword ptr [eax + 0xbc] ; ret 
// Tiny-body shape `GetFloatThrough` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0xbc];
    float m_value;                         // +0xbc
};

struct CThingAICreature {
    unsigned char _pad[0xe0];
    CTarget* m_object;                     // +0xe0
    float vfunc_92();
};

float CThingAICreature::vfunc_92()
{
    return m_object->m_value;
}
