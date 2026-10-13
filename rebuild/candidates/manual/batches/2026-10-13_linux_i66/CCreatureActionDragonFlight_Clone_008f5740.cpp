// CCreatureAction_DragonFlight::Clone() -- new CCreatureAction_DragonFlight(*this) with the implicit copy constructor inlined (base copy @ 0x0062F3B0 + 2 dword members)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F5740, oracle length 71 bytes.
// Grade: candidate -- still DIFFER(34v71) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push esi ; push edi ; push 0x11c ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CBase_0062f3b0::CBase_0062f3b0(const CBase_0062f3b0&) ; mov dword ptr [esi],offset CCreatureAction_DragonFlight::`vftable' ; mov al,[edi+0x114] ; mov [esi+0x114],al ; mov eax,[edi+0x118] ; mov [esi+0x118],eax ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds 2 members to its base: the implicit copy constructor
// is inlined as the base copy constructor @ 0x0062F3B0, this class's vtable store (0x01288544,
// rtti-vtables.tsv) and the member copies. sizeof = 0x11c. Member types are not recoverable
// beyond their size (`long` / `unsigned char`). One of 12 bodies of this shape.
struct CBase_0062f3b0 {
    virtual void v00();
    unsigned char _body[0x114 - 4];     // sizeof(CBase_0062f3b0) = 0x114
    CBase_0062f3b0(const CBase_0062f3b0& other);     // 0x0062F3B0
};

struct CCreatureAction_DragonFlight : CBase_0062f3b0 {
    unsigned char m_114;                // +0x114
    unsigned char _gap1[0x3];
    long m_118;                         // +0x118
    virtual void v00();                    // own vtable; copy constructor implicit
    CBase_0062f3b0* Clone();
};

CBase_0062f3b0* CCreatureAction_DragonFlight::Clone()
{
    return new CCreatureAction_DragonFlight(*this);
}
