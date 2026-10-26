#pragma optimize("s",on)
// CPersistable_0045e7cf::Transfer(CPersistContext&) -- 4 members at +0x3c..+0x50 handed to the context's typed transfer helpers (0x00431061, 0x0043314A, 0x0045E808, 0x0045EBFD)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045E7CF, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; lea eax,[esi+0x3c] ; push eax ; mov ecx,edi ; call Transfer_0045e808 ; lea eax,[esi+0x40] ; push eax ; mov ecx,edi ; call Transfer_0043314a ; lea eax,[esi+0x44] ; push eax ; mov ecx,edi ; call Transfer_0045ebfd ; add esi,0x50 ; push esi ; mov ecx,edi ; call Transfer_00431061 ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __fastcall transfer helper of the
// context (4 distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of 9 bodies of this shape.
class CPersistContext { public:
    template <class T> void Transfer(T& value);   // 0x00431061
    void Transfer_0045e808(long* value);   // 0x0045E808
    void Transfer_0045ebfd(long* value);   // 0x0045EBFD
};

struct CPersistable_0045e7cf {
    virtual void v00();
    unsigned char _pad[0x38];
    long m_3c;                          // +0x3c (via Transfer_0045e808)
    long m_40;                          // +0x40 (via Transfer_0043314a)
    long m_44;                          // +0x44 (via Transfer_0045ebfd)
    unsigned char _gap3[0x8];
    long m_50;                          // +0x50 (via Transfer_00431061)
    virtual void Transfer(CPersistContext& context);
};

void CPersistable_0045e7cf::Transfer(CPersistContext& context)
{
    context.Transfer_0045e808(&m_3c);
    context.Transfer(*(bool*)(&m_40));
    context.Transfer_0045ebfd(&m_44);
    context.Transfer(*(float*)(&m_50));
}