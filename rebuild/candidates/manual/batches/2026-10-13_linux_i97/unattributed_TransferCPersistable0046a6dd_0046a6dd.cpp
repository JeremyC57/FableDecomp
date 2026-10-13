// CPersistable_0046a6dd::Transfer(CPersistContext&) -- 8 members at +0x3c..+0x68 handed to the context's typed transfer helpers (0x00431061, 0x00431102, 0x00431143, 0x0045228F, 0x00464CBF, 0x00466A47)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046A6DD, oracle length 101 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; lea eax,[esi+0x3c] ; push eax ; mov ecx,edi ; call Transfer_00464cbf ; lea eax,[esi+0x48] ; push eax ; mov ecx,edi ; call Transfer_0045228f ; lea eax,[esi+0x4c] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; lea eax,[esi+0x50] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; lea eax,[esi+0x54] ; push eax ; mov ecx,edi ; call Transfer_00431102 ; lea eax,[esi+0x58] ; push eax ; mov ecx,edi ; call Transfer_00466a47 ; lea eax,[esi+0x64] ; push eax ; mov ecx,edi ; call Transfer_00431143 ; add esi,0x68 ; push esi ; mov ecx,edi ; call Transfer_00431143 ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __thiscall transfer helper of the
// context (6 distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of 9 bodies of this shape.
struct CPersistContext {
    void Transfer_00431061(long* value);   // 0x00431061
    void Transfer_00431102(long* value);   // 0x00431102
    void Transfer_00431143(long* value);   // 0x00431143
    void Transfer_0045228f(long* value);   // 0x0045228F
    void Transfer_00464cbf(long* value);   // 0x00464CBF
    void Transfer_00466a47(long* value);   // 0x00466A47
};

struct CPersistable_0046a6dd {
    virtual void v00();
    unsigned char _pad[0x38];
    long m_3c;                          // +0x3c (via Transfer_00464cbf)
    unsigned char _gap1[0x8];
    long m_48;                          // +0x48 (via Transfer_0045228f)
    long m_4c;                          // +0x4c (via Transfer_00431061)
    long m_50;                          // +0x50 (via Transfer_00431061)
    long m_54;                          // +0x54 (via Transfer_00431102)
    long m_58;                          // +0x58 (via Transfer_00466a47)
    unsigned char _gap6[0x8];
    long m_64;                          // +0x64 (via Transfer_00431143)
    long m_68;                          // +0x68 (via Transfer_00431143)
    virtual void Transfer(CPersistContext& context);
};

void CPersistable_0046a6dd::Transfer(CPersistContext& context)
{
    context.Transfer_00464cbf(&m_3c);
    context.Transfer_0045228f(&m_48);
    context.Transfer_00431061(&m_4c);
    context.Transfer_00431061(&m_50);
    context.Transfer_00431102(&m_54);
    context.Transfer_00466a47(&m_58);
    context.Transfer_00431143(&m_64);
    context.Transfer_00431143(&m_68);
}
