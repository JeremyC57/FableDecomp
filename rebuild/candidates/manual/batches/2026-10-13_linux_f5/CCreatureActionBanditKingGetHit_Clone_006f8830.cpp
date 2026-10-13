// CCreatureAction_BanditKingGetHit::Clone() -- new CCreatureAction_BanditKingGetHit(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F8830, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x148 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase(const CCombatAction_GenericStrikeResponseBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_BanditKingGetHit::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x006F83C0 plus this class's vtable store
// (0x01263BAC, named through rtti-vtables.tsv). sizeof = 0x148. One of 196 bodies of this shape.
struct CCombatAction_GenericStrikeResponseBase {
    virtual void v00();
    unsigned char _body[0x148 - 4];    // sizeof(CCombatAction_GenericStrikeResponseBase) = sizeof(CCreatureAction_BanditKingGetHit) = 0x148
    CCombatAction_GenericStrikeResponseBase(const CCombatAction_GenericStrikeResponseBase& other);     // 0x006F83C0
};

struct CCreatureAction_BanditKingGetHit : CCombatAction_GenericStrikeResponseBase {
    virtual void v00();                    // own vtable; copy constructor implicit
    CCombatAction_GenericStrikeResponseBase* Clone();
};

CCombatAction_GenericStrikeResponseBase* CCreatureAction_BanditKingGetHit::Clone()
{
    return new CCreatureAction_BanditKingGetHit(*this);
}
