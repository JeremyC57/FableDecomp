// CThingObject::vfunc_70() -- returns +0x84 of the object at +0xd0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703040, oracle length 13 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0xd0] ; mov eax, dword ptr [eax + 0x84] ; ret 
// Tiny-body shape `GetDwordThrough` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x84];
    long m_value;                          // +0x84
};

class CThingObject { public:
    unsigned char _pad[0xd0];
    CTarget* m_object;                     // +0xd0
    long vfunc_70();
};

long CThingObject::vfunc_70()
{
    return m_object->m_value;
}