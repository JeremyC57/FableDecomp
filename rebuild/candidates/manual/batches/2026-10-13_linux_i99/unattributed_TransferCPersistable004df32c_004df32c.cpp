// CPersistable_004df32c::Transfer(CPersistContext&) -- 4 members at +0x25..+0x2c handed to the context's typed transfer helpers (0x00431061, 0x0043314A)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF32C, oracle length 57 bytes.
// Grade: candidate -- still DIFFER(57v57) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; lea eax,[esi+0x25] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x26] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x28] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; add esi,0x2c ; push esi ; mov ecx,edi ; call Transfer_00431061 ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __thiscall transfer helper of the
// context (2 distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of 9 bodies of this shape.
struct CPersistContext {
    void Transfer_00431061(long* value);   // 0x00431061
    void Transfer_0043314a(long* value);   // 0x0043314A
};

struct CPersistable_004df32c {
    virtual void v00();
    unsigned char _pad[0x21];
    long m_25;                          // +0x25 (via Transfer_0043314a)
    long m_26;                          // +0x26 (via Transfer_0043314a)
    long m_28;                          // +0x28 (via Transfer_00431061)
    long m_2c;                          // +0x2c (via Transfer_00431061)
    virtual void Transfer(CPersistContext& context);
};

void CPersistable_004df32c::Transfer(CPersistContext& context)
{
    context.Transfer_0043314a(&m_25);
    context.Transfer_0043314a(&m_26);
    context.Transfer_00431061(&m_28);
    context.Transfer_00431061(&m_2c);
}
