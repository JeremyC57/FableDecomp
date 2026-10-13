// CPersistable_004df562::Transfer(CPersistContext&) -- 8 members at +0x25..+0x2c handed to the context's typed transfer helpers (0x0043314A)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF562, oracle length 101 bytes.
// Grade: candidate -- still DIFFER(101v101) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; lea eax,[esi+0x25] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x26] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x27] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x28] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x29] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x2a] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x2b] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; add esi,0x2c ; push esi ; mov ecx,edi ; call Transfer_0043314a ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __thiscall transfer helper of the
// context (1 distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of 9 bodies of this shape.
struct CPersistContext {
    void Transfer_0043314a(long* value);   // 0x0043314A
};

struct CPersistable_004df562 {
    virtual void v00();
    unsigned char _pad[0x21];
    long m_25;                          // +0x25 (via Transfer_0043314a)
    long m_26;                          // +0x26 (via Transfer_0043314a)
    long m_27;                          // +0x27 (via Transfer_0043314a)
    long m_28;                          // +0x28 (via Transfer_0043314a)
    long m_29;                          // +0x29 (via Transfer_0043314a)
    long m_2a;                          // +0x2a (via Transfer_0043314a)
    long m_2b;                          // +0x2b (via Transfer_0043314a)
    long m_2c;                          // +0x2c (via Transfer_0043314a)
    virtual void Transfer(CPersistContext& context);
};

void CPersistable_004df562::Transfer(CPersistContext& context)
{
    context.Transfer_0043314a(&m_25);
    context.Transfer_0043314a(&m_26);
    context.Transfer_0043314a(&m_27);
    context.Transfer_0043314a(&m_28);
    context.Transfer_0043314a(&m_29);
    context.Transfer_0043314a(&m_2a);
    context.Transfer_0043314a(&m_2b);
    context.Transfer_0043314a(&m_2c);
}
