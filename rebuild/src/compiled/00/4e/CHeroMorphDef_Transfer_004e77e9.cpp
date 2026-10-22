#pragma optimize("s",on)
// CPersistable_004e77e9::Transfer(CPersistContext&) -- 4 members at +0x28..+0x58 handed to the context's typed transfer helpers (0x004E7822, 0x004E788A, 0x004E78F2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E77E9, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; lea eax,[esi+0x28] ; push eax ; mov ecx,edi ; call Transfer_004e7822 ; lea eax,[esi+0x38] ; push eax ; mov ecx,edi ; call Transfer_004e788a ; lea eax,[esi+0x48] ; push eax ; mov ecx,edi ; call Transfer_004e78f2 ; add esi,0x58 ; push esi ; mov ecx,edi ; call Transfer_004e78f2 ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __fastcall transfer helper of the
// context (3 distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of 9 bodies of this shape.
struct CPersistContext {
    void Transfer_004e7822(long* value);   // 0x004E7822
    void Transfer_004e788a(long* value);   // 0x004E788A
    void Transfer_004e78f2(long* value);   // 0x004E78F2
};

struct CPersistable_004e77e9 {
    virtual void v00();
    unsigned char _pad[0x24];
    long m_28;                          // +0x28 (via Transfer_004e7822)
    unsigned char _gap1[0xc];
    long m_38;                          // +0x38 (via Transfer_004e788a)
    unsigned char _gap2[0xc];
    long m_48;                          // +0x48 (via Transfer_004e78f2)
    unsigned char _gap3[0xc];
    long m_58;                          // +0x58 (via Transfer_004e78f2)
    virtual void Transfer(CPersistContext& context);
};

void CPersistable_004e77e9::Transfer(CPersistContext& context)
{
    context.Transfer_004e7822(&m_28);
    context.Transfer_004e788a(&m_38);
    context.Transfer_004e78f2(&m_48);
    context.Transfer_004e78f2(&m_58);
}