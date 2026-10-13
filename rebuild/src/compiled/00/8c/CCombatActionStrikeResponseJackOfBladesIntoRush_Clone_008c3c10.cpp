// CCombatAction_StrikeResponseJackOfBladesIntoRush::Clone() -- new CCombatAction_StrikeResponseJackOfBladesIntoRush(*this) with the implicit copy constructor inlined (base copy + one member at +0x148)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C3C10, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x14c ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CBase_006f83c0::CBase_006f83c0(const CBase_006f83c0&) ; mov dword ptr [esi],offset CCombatAction_StrikeResponseJackOfBladesIntoRush::`vftable' ; mov al,[edi+0x148] ; mov [esi+0x148],al ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds one byte member at +0x148 to its base: the implicit
// copy constructor is inlined as the base copy constructor @ 0x006F83C0, this class's vtable
// store (0x012808EC, named through rtti-vtables.tsv) and the member copy. sizeof = 0x14c.
// The member's exact type is not recoverable from the body; `unsigned char` models its size. One of 35 bodies.
struct CBase_006f83c0 {
    virtual void v00();
    unsigned char _body[0x148 - 4];     // sizeof(CBase_006f83c0) = 0x148
    CBase_006f83c0(const CBase_006f83c0& other);     // 0x006F83C0
};

struct CCombatAction_StrikeResponseJackOfBladesIntoRush : CBase_006f83c0 {
    unsigned char m_extra;                          // +0x148
    virtual void v00();                    // own vtable; copy constructor implicit
    CBase_006f83c0* Clone();
};

CBase_006f83c0* CCombatAction_StrikeResponseJackOfBladesIntoRush::Clone()
{
    return new CCombatAction_StrikeResponseJackOfBladesIntoRush(*this);
}