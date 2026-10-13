// CPersistable_004615fb::Transfer(CPersistContext&) -- 8 members at +0x3c..+0x68 handed to the context's typed transfer helpers (0x00431102, 0x0043314A, 0x00461660, 0x00461894)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004615FB, oracle length 101 bytes.
// Grade: candidate -- still DIFFER(101v101) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; lea eax,[esi+0x3c] ; push eax ; mov ecx,edi ; call Transfer_00461660 ; lea eax,[esi+0x48] ; push eax ; mov ecx,edi ; call Transfer_00461894 ; lea eax,[esi+0x54] ; push eax ; mov ecx,edi ; call Transfer_00461894 ; lea eax,[esi+0x60] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x61] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x62] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x64] ; push eax ; mov ecx,edi ; call Transfer_00431102 ; add esi,0x68 ; push esi ; mov ecx,edi ; call Transfer_00431102 ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __thiscall transfer helper of the
// context (4 distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of 9 bodies of this shape.
struct CPersistContext {
    void Transfer_00431102(long* value);   // 0x00431102
    void Transfer_0043314a(long* value);   // 0x0043314A
    void Transfer_00461660(long* value);   // 0x00461660
    void Transfer_00461894(long* value);   // 0x00461894
};

struct CPersistable_004615fb {
    virtual void v00();
    unsigned char _pad[0x38];
    long m_3c;                          // +0x3c (via Transfer_00461660)
    unsigned char _gap1[0x8];
    long m_48;                          // +0x48 (via Transfer_00461894)
    unsigned char _gap2[0x8];
    long m_54;                          // +0x54 (via Transfer_00461894)
    unsigned char _gap3[0x8];
    long m_60;                          // +0x60 (via Transfer_0043314a)
    long m_61;                          // +0x61 (via Transfer_0043314a)
    long m_62;                          // +0x62 (via Transfer_0043314a)
    long m_64;                          // +0x64 (via Transfer_00431102)
    long m_68;                          // +0x68 (via Transfer_00431102)
    virtual void Transfer(CPersistContext& context);
};

void CPersistable_004615fb::Transfer(CPersistContext& context)
{
    context.Transfer_00461660(&m_3c);
    context.Transfer_00461894(&m_48);
    context.Transfer_00461894(&m_54);
    context.Transfer_0043314a(&m_60);
    context.Transfer_0043314a(&m_61);
    context.Transfer_0043314a(&m_62);
    context.Transfer_00431102(&m_64);
    context.Transfer_00431102(&m_68);
}
