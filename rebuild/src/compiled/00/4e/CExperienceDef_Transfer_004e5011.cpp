#pragma optimize("s",on)
// CPersistable_004e5011::Transfer(CPersistContext&) -- 4 members at +0x28..+0x44 handed to the context's typed transfer helpers (0x00431102, 0x004E504A, 0x004E527E)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E5011, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; lea eax,[esi+0x28] ; push eax ; mov ecx,edi ; call Transfer_00431102 ; lea eax,[esi+0x2c] ; push eax ; mov ecx,edi ; call Transfer_004e504a ; lea eax,[esi+0x38] ; push eax ; mov ecx,edi ; call Transfer_004e527e ; add esi,0x44 ; push esi ; mov ecx,edi ; call Transfer_004e504a ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __fastcall transfer helper of the
// context (3 distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of 9 bodies of this shape.
class CPersistContext { public:
    void Transfer_00431102(long* value);   // 0x00431102
    void Transfer_004e504a(long* value);   // 0x004E504A
    void Transfer_004e527e(long* value);   // 0x004E527E
};

struct CPersistable_004e5011 {
    virtual void v00();
    unsigned char _pad[0x24];
    long m_28;                          // +0x28 (via Transfer_00431102)
    long m_2c;                          // +0x2c (via Transfer_004e504a)
    unsigned char _gap2[0x8];
    long m_38;                          // +0x38 (via Transfer_004e527e)
    unsigned char _gap3[0x8];
    long m_44;                          // +0x44 (via Transfer_004e504a)
    virtual void Transfer(CPersistContext& context);
};

void CPersistable_004e5011::Transfer(CPersistContext& context)
{
    context.Transfer_00431102(&m_28);
    context.Transfer_004e504a(&m_2c);
    context.Transfer_004e527e(&m_38);
    context.Transfer_004e504a(&m_44);
}