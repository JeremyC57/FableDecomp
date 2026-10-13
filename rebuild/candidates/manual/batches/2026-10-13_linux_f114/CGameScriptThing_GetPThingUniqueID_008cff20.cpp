// CGameScriptThing::GetPThingUniqueID() -- returns the byte at +0x1c of the object pointed to by +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CFF20, oracle length 7 bytes.
// Grade: candidate -- still DIFFER(7v7) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  mov eax,[ecx+0x18] ; mov al,[eax+0x1c] ; ret
// Virtual getter through one pointer member: loads the object at +0x18 and returns its byte at
// +0x1c in al. Classes and member names are descriptive (vtable-slot label for the class).
// One of 11 bodies of this shape.
struct CTarget {
    unsigned char _pad[0x1c];
    unsigned char m_value;                 // +0x1c
};

struct CGameScriptThing {
    virtual void v00();
    virtual unsigned char GetPThingUniqueID();
    unsigned char _pad[0x14];
    CTarget* m_target;                     // +0x18
};

unsigned char CGameScriptThing::GetPThingUniqueID()
{
    return m_target->m_value;
}
