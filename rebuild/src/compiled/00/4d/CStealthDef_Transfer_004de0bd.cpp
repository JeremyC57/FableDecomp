#pragma optimize("s",on)
// CPersistable_004de0bd::Transfer(CPersistContext&) -- 8 members at +0x28..+0x44 handed to the context's typed transfer helpers (0x00431061)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE0BD, oracle length 101 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[context] ; lea eax,[esi+0x28] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; lea eax,[esi+0x2c] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; lea eax,[esi+0x30] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; lea eax,[esi+0x34] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; lea eax,[esi+0x38] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; lea eax,[esi+0x3c] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; lea eax,[esi+0x40] ; push eax ; mov ecx,edi ; call Transfer_00431061 ; add esi,0x44 ; push esi ; mov ecx,edi ; call Transfer_00431061 ; pop edi ; pop esi ; ret 4
// Virtual persist hook: each member is passed by address to a __fastcall transfer helper of the
// context (1 distinct helpers, one per member type; their labels are propagated so they are
// named by address). The last member's address is formed in place (`add esi,N`). Member types
// are not recoverable beyond their 4-byte size. One of 9 bodies of this shape.
class CPersistContext { public:
    template <class T> void Transfer(T& value);   // 0x00431061
};

struct CPersistable_004de0bd {
    virtual void v00();
    unsigned char _pad[0x24];
    long m_28;                          // +0x28 (via Transfer_00431061)
    long m_2c;                          // +0x2c (via Transfer_00431061)
    long m_30;                          // +0x30 (via Transfer_00431061)
    long m_34;                          // +0x34 (via Transfer_00431061)
    long m_38;                          // +0x38 (via Transfer_00431061)
    long m_3c;                          // +0x3c (via Transfer_00431061)
    long m_40;                          // +0x40 (via Transfer_00431061)
    long m_44;                          // +0x44 (via Transfer_00431061)
    virtual void Transfer(CPersistContext& context);
};

void CPersistable_004de0bd::Transfer(CPersistContext& context)
{
    context.Transfer(*(float*)(&m_28));
    context.Transfer(*(float*)(&m_2c));
    context.Transfer(*(float*)(&m_30));
    context.Transfer(*(float*)(&m_34));
    context.Transfer(*(float*)(&m_38));
    context.Transfer(*(float*)(&m_3c));
    context.Transfer(*(float*)(&m_40));
    context.Transfer(*(float*)(&m_44));
}