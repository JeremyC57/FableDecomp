// CPersistable_004702c8::Transfer(CPersistContext&) -- 4 members at +0x28..+0x50 handed to the context's typed transfer helpers (0x00431061, 0x00431102, 0x00431143, 0x00470301)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004702C8, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; lea eax,[esi+0x28] ; push eax ; mov ecx,edi ; call Transfer_00470301 ; lea eax,[esi+0x48] ; push eax ; mov ecx,edi ; call Transfer_00431143 ; lea eax,[esi+0x4c] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; add esi,0x50 ; push esi ; mov ecx,edi ; call Transfer_00431102 ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __thiscall transfer helper of the
// context (4 distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of 9 bodies of this shape.
struct CPersistContext {
    void Transfer_00431061(long* value);   // 0x00431061
    void Transfer_00431102(long* value);   // 0x00431102
    void Transfer_00431143(long* value);   // 0x00431143
    void Transfer_00470301(long* value);   // 0x00470301
};

struct CPersistable_004702c8 {
    virtual void v00();
    unsigned char _pad[0x24];
    long m_28;                          // +0x28 (via Transfer_00470301)
    unsigned char _gap1[0x1c];
    long m_48;                          // +0x48 (via Transfer_00431143)
    long m_4c;                          // +0x4c (via Transfer_00431061)
    long m_50;                          // +0x50 (via Transfer_00431102)
    virtual void Transfer(CPersistContext& context);
};

void CPersistable_004702c8::Transfer(CPersistContext& context)
{
    context.Transfer_00470301(&m_28);
    context.Transfer_00431143(&m_48);
    context.Transfer_00431061(&m_4c);
    context.Transfer_00431102(&m_50);
}
