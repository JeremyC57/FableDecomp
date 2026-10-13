// CCreatureAction_ScorpionTailPlunge::CCreatureAction_ScorpionTailPlunge(const CCreatureAction_ScorpionTailPlunge&) -- copy constructor: CCombatActionBase copy @ 0x0062F3B0, own vtable, 3 dword members copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E4F90, oracle length 63 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,[other] ; push edi ; mov esi,ecx ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; mov dword ptr [esi],offset CCreatureAction_ScorpionTailPlunge::`vftable' ; mov eax,[edi+0x114] ; mov [esi+0x114],eax ; mov eax,[edi+0x118] ; mov [esi+0x118],eax ; mov eax,[edi+0x11c] ; mov [esi+0x11c],eax ; pop edi ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a CCombatActionBase-derived class: the base copy constructor @ 0x0062F3B0 runs,
// this class's vtable (0x01286494, rtti-vtables.tsv) is installed and the 3 trailing dword
// members are copied in declaration order; `this` is returned. Member types are not
// recoverable beyond their size (`long`). One of 11 bodies of this shape.
struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x110];
    CCombatActionBase(const CCombatActionBase& other);     // 0x0062F3B0
};

struct CCreatureAction_ScorpionTailPlunge : CCombatActionBase {
    long m_114;                         // +0x114
    long m_118;                         // +0x118
    long m_11c;                         // +0x11c
    virtual void v00();
    CCreatureAction_ScorpionTailPlunge(const CCreatureAction_ScorpionTailPlunge& other);
};

CCreatureAction_ScorpionTailPlunge::CCreatureAction_ScorpionTailPlunge(const CCreatureAction_ScorpionTailPlunge& other)
    : CCombatActionBase(other)
    , m_114(other.m_114)
    , m_118(other.m_118)
    , m_11c(other.m_11c)
{
}
