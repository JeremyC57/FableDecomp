// CCreatureAction_BanditKingChangeWeapons::Clone() -- new CCreatureAction_BanditKingChangeWeapons(*this) with the implicit copy constructor inlined (base copy + one member at +0xa8)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C9720, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0xac ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CBase_0062ebc0::CBase_0062ebc0(const CBase_0062ebc0&) ; mov dword ptr [esi],offset CCreatureAction_BanditKingChangeWeapons::`vftable' ; mov al,[edi+0xa8] ; mov [esi+0xa8],al ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds one byte member at +0xa8 to its base: the implicit
// copy constructor is inlined as the base copy constructor @ 0x0062EBC0, this class's vtable
// store (0x0128180C, named through rtti-vtables.tsv) and the member copy. sizeof = 0xac.
// The member's exact type is not recoverable from the body; `unsigned char` models its size. One of 35 bodies.
struct CBase_0062ebc0 {
    virtual void v00();
    unsigned char _body[0xa8 - 4];     // sizeof(CBase_0062ebc0) = 0xa8
    CBase_0062ebc0(const CBase_0062ebc0& other);     // 0x0062EBC0
};

class CCreatureAction_BanditKingChangeWeapons : public CBase_0062ebc0 { public:
    unsigned char m_extra;                          // +0xa8
    virtual void v00();                    // own vtable; copy constructor implicit
    CBase_0062ebc0* Clone();
};

CBase_0062ebc0* CCreatureAction_BanditKingChangeWeapons::Clone()
{
    return new CCreatureAction_BanditKingChangeWeapons(*this);
}