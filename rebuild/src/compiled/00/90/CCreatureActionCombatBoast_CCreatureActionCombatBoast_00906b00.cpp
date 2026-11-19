// CCreatureAction_CombatBoast::CCreatureAction_CombatBoast(CThingCreatureBase&, CThingCreatureBase&) -- base CCombatActionBase(owner, thing, 50, zeroed CSoundPair)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00906B00, oracle length 55 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub esp,8 ; mov edx,[thing] ; xor eax,eax ; push esi ; mov [esp+4],eax ; mov [esp+8],eax ; lea eax,[esp+4] ; push eax ; mov esi,ecx ; mov ecx,[owner] ; push 0x32 ; push ecx ; push edx ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase ; mov dword ptr [esi],offset CCreatureAction_CombatBoast::`vftable' ; mov eax,esi ; pop esi ; add esp,8 ; ret 8
// Two-argument combat-action constructor: the base @ 0x00857F30 takes the owner, the thing,
// the action code 50 (0x32) and a two-dword sound pair built zeroed on the stack by its inline
// constructor; then this class's vtable (0x012633FC, rtti-vtables.tsv).
// One of 13 bodies of this shape.
// Leaf: ??0CCreatureAction_CombatBoast@@QAE@AAVCThingCreatureBase@@0@Z
// NF95 2026-11-19 (q11340 / q11341): Ego_r.pdb 0x0092D6C0 ??0CCreatureAction_CombatBoast@@QAE@AAVCThingCreatureBase@@0@Z (both creatures by reference) and the base
//   ctor 0x00857F30 = Ego_r.pdb 0x00838C40 ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@AAVCThing@@JABVCSoundPair@@@Z; were pointer
//   forms. The second creature goes to the base as its CThing (single inheritance at offset 0: no adjustment).

class CThingCreatureBase;
class CThing;

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase { public:
    virtual void v00();
    CCombatActionBase(CThingCreatureBase& owner, CThing& thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

class CCreatureAction_CombatBoast : public CCombatActionBase { public:
    virtual void v00();
    CCreatureAction_CombatBoast(CThingCreatureBase& owner, CThingCreatureBase& thing);
};

CCreatureAction_CombatBoast::CCreatureAction_CombatBoast(CThingCreatureBase& owner, CThingCreatureBase& thing)
    : CCombatActionBase(owner, (CThing&)thing, 50, CSoundPair())
{
}