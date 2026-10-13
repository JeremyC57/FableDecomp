// CCombatAction_WillHealInto::Clone() -- new CCombatAction_WillHealInto(*this) with the implicit copy constructor inlined (base copy + one member at +0x114)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956A20, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x118 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CBase_0062f3b0::CBase_0062f3b0(const CBase_0062f3b0&) ; mov dword ptr [esi],offset CCombatAction_WillHealInto::`vftable' ; mov eax,[edi+0x114] ; mov [esi+0x114],eax ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds one dword member at +0x114 to its base: the implicit
// copy constructor is inlined as the base copy constructor @ 0x0062F3B0, this class's vtable
// store (0x012948AC, named through rtti-vtables.tsv) and the member copy. sizeof = 0x118.
// The member's exact type is not recoverable from the body; `long` models its size. One of 35 bodies.
struct CBase_0062f3b0 {
    virtual void v00();
    unsigned char _body[0x114 - 4];     // sizeof(CBase_0062f3b0) = 0x114
    CBase_0062f3b0(const CBase_0062f3b0& other);     // 0x0062F3B0
};

struct CCombatAction_WillHealInto : CBase_0062f3b0 {
    long m_extra;                          // +0x114
    virtual void v00();                    // own vtable; copy constructor implicit
    CBase_0062f3b0* Clone();
};

CBase_0062f3b0* CCombatAction_WillHealInto::Clone()
{
    return new CCombatAction_WillHealInto(*this);
}
