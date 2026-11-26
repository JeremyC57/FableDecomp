// CCreatureAction_BattleChargeTurnLeft::CCreatureAction_BattleChargeTurnLeft(CThingCreatureBase&) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), then animation "ST_BATTLE_CHARGE_TURN_LEFT"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE630, oracle length 90 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub esp,8 ; xor eax,eax ; push esi ; mov [esp+4],eax ; mov [esp+8],eax ; lea eax,[esp+4] ; push eax ; mov esi,ecx ; mov ecx,[esp+14h] ; push 0x96 ; push ecx ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase ; push -1 ; push offset lit ; lea ecx,[esp+18h] ; mov dword ptr [esi],offset vftable ; call CCharString::CCharString ; lea edx,[esp+10h] ; push edx ; mov ecx,esi ; call SetAnimationName ; lea ecx,[esp+10h] ; call CCharString::~CCharString ; mov eax,esi ; pop esi ; add esp,8 ; ret 4
// Combat-action constructor: the base @ 0x00858030 takes the owner, the action code 150 (0x96)
// and a two-dword sound pair built zeroed on the stack by its inline constructor (register-zeroed
// stores: an embedded object's inline ctor), then this class's vtable (0x0128490C,
// rtti-vtables.tsv) and the animation name set through a scoped CCharString (ctor @ 0x0099EBF0, dtor @ 0x0099EAE0; the setter's manifest label is propagated, named as in family 20).
// One of 35 bodies of this shape.
// Naming (NF457, r454b): the constructor takes `CThingCreatureBase&`, ??0CCreatureAction_BattleChargeTurnLeft@@QAE@AAVCThingCreatureBase@@@Z as both PDB name sets (Ego_r + FableWin) spell it (was ??0CCreatureAction_BattleChargeTurnLeft@@QAE@PAVCThingCreatureBase@@@Z); a reference passes the same pointer, so the object is unchanged.
#include "fable_string.h"

class CThingCreatureBase;

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase { public:
    virtual void v00();
    // NF123 (2026-11-20, q6404): creature by reference, Ego_r.pdb ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@JABVCSoundPair@@@Z (0x00858030; was PAV).
    CCombatActionBase(CThingCreatureBase& owner, long actionCode, const CSoundPair& sound);   // 0x00858030
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_BattleChargeTurnLeft : public CCombatActionBase { public:
    virtual void v00();
    CCreatureAction_BattleChargeTurnLeft(CThingCreatureBase& owner);
};

CCreatureAction_BattleChargeTurnLeft::CCreatureAction_BattleChargeTurnLeft(CThingCreatureBase& owner)
    : CCombatActionBase(owner, 150, CSoundPair())
{
    {
        CCharString name("ST_BATTLE_CHARGE_TURN_LEFT", -1);       // scoped named local (a bare temporary would reuse eax, GOTCHAS)
        SetCombatAnimation(name);            // 0x008574D0
    }
}