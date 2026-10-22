#pragma optimize("s",on)
// CPersistable_004696c1::Transfer(CPersistContext&) -- 4 members at +0x3c..+0x50 handed to the context's typed transfer helpers (0x00431102, 0x00466A47, 0x004696FA)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004696C1, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; lea eax,[esi+0x3c] ; push eax ; mov ecx,edi ; call Transfer_00466a47 ; lea eax,[esi+0x48] ; push eax ; mov ecx,edi ; call Transfer_00431102 ; lea eax,[esi+0x4c] ; push eax ; mov ecx,edi ; call Transfer_00431102 ; add esi,0x50 ; push esi ; mov ecx,edi ; call Transfer_004696fa ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __fastcall transfer helper of the
// context (3 distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of 9 bodies of this shape.
struct CPersistContext {
    void Transfer_00431102(long* value);   // 0x00431102
    void Transfer_00466a47(long* value);   // 0x00466A47
    void Transfer_004696fa(long* value);   // 0x004696FA
};

struct CPersistable_004696c1 {
    virtual void v00();
    unsigned char _pad[0x38];
    long m_3c;                          // +0x3c (via Transfer_00466a47)
    unsigned char _gap1[0x8];
    long m_48;                          // +0x48 (via Transfer_00431102)
    long m_4c;                          // +0x4c (via Transfer_00431102)
    long m_50;                          // +0x50 (via Transfer_004696fa)
    virtual void Transfer(CPersistContext& context);
};

void CPersistable_004696c1::Transfer(CPersistContext& context)
{
    context.Transfer_00466a47(&m_3c);
    context.Transfer_00431102(&m_48);
    context.Transfer_00431102(&m_4c);
    context.Transfer_004696fa(&m_50);
}