// CCreatureAction_CombatBoast::CCreatureAction_CombatBoast(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, thing, 50, zeroed CSoundPair)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00906B00, oracle length 55 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub esp,8 ; mov edx,[thing] ; xor eax,eax ; push esi ; mov [esp+4],eax ; mov [esp+8],eax ; lea eax,[esp+4] ; push eax ; mov esi,ecx ; mov ecx,[owner] ; push 0x32 ; push ecx ; push edx ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase ; mov dword ptr [esi],offset CCreatureAction_CombatBoast::`vftable' ; mov eax,esi ; pop esi ; add esp,8 ; ret 8
// Two-argument combat-action constructor: the base @ 0x00857F30 takes the owner, the thing,
// the action code 50 (0x32) and a two-dword sound pair built zeroed on the stack by its inline
// constructor; then this class's vtable (0x012633FC, rtti-vtables.tsv).
// One of 13 bodies of this shape.

struct CThingCreatureBase;
struct CThing;

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase {
    virtual void v00();
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCreatureAction_CombatBoast : CCombatActionBase {
    virtual void v00();
    CCreatureAction_CombatBoast(CThingCreatureBase* owner, CThing* thing);
};

CCreatureAction_CombatBoast::CCreatureAction_CombatBoast(CThingCreatureBase* owner, CThing* thing)
    : CCombatActionBase(owner, thing, 50, CSoundPair())
{
}
