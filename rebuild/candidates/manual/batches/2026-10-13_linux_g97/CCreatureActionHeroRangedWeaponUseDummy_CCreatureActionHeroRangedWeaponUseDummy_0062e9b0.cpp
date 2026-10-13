// CCreatureAction_HeroRangedWeaponUseDummy::CCreatureAction_HeroRangedWeaponUseDummy(CThingCreatureBase*) -- base CCombatActionBase(owner, 150, zeroed CSoundPair)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062E9B0, oracle length 53 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub esp,8 ; xor eax,eax ; push esi ; mov [esp+4],eax ; mov [esp+8],eax ; lea eax,[esp+4] ; push eax ; mov esi,ecx ; mov ecx,[esp+14h] ; push 0x96 ; push ecx ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase ; mov dword ptr [esi],offset vftable ; mov eax,esi ; pop esi ; add esp,8 ; ret 4
// Combat-action constructor: the base @ 0x00858030 takes the owner, the action code 150 (0x96)
// and a two-dword sound pair built zeroed on the stack by its inline constructor (register-zeroed
// stores: an embedded object's inline ctor), then this class's vtable (0x01259BD4,
// rtti-vtables.tsv).
// One of 13 bodies of this shape.

struct CThingCreatureBase;

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase {
    virtual void v00();
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

struct CCreatureAction_HeroRangedWeaponUseDummy : CCombatActionBase {
    virtual void v00();
    CCreatureAction_HeroRangedWeaponUseDummy(CThingCreatureBase* owner);
};

CCreatureAction_HeroRangedWeaponUseDummy::CCreatureAction_HeroRangedWeaponUseDummy(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 150, CSoundPair())
{
}
