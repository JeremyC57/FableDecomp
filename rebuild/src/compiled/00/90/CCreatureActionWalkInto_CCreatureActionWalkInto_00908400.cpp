// CCreatureAction_WalkInto::CCreatureAction_WalkInto(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, thing, 250, zeroed CSoundPair), then the name "ST_WALK_INTO" through the setter @ 0x008574D0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00908400, oracle length 95 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub esp,8 ; mov edx,[thing] ; xor eax,eax ; push esi ; mov [esp+4],eax ; mov [esp+8],eax ; lea eax,[esp+4] ; push eax ; mov esi,ecx ; mov ecx,[owner] ; push 0xfa ; push ecx ; push edx ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase ; push -1 ; push offset lit ; lea ecx,[esp+1Ch] ; mov dword ptr [esi],offset vftable ; call CCharString::CCharString ; lea eax,[esp+14h] ; push eax ; mov ecx,esi ; call setter ; lea ecx,[esp+14h] ; call CCharString::~CCharString ; mov eax,esi ; pop esi ; add esp,8 ; ret 8
// Two-argument combat-action constructor: the base @ 0x00857F30 takes the owner, the thing,
// the action code 250 (0xfa) and a two-dword sound pair built zeroed on the stack by its inline
// constructor; then this class's vtable (0x0128ECFC, rtti-vtables.tsv) and a scoped CCharString passed to the setter @ 0x008574D0 (its manifest label is propagated).
// One of 11 bodies of this shape.
#include "fable_string.h"

class CThingCreatureBase;
class CThing;

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase { public:
    virtual void v00();
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_WalkInto : public CCombatActionBase { public:
    virtual void v00();
    CCreatureAction_WalkInto(CThingCreatureBase* owner, CThing* thing);
};

CCreatureAction_WalkInto::CCreatureAction_WalkInto(CThingCreatureBase* owner, CThing* thing)
    : CCombatActionBase(owner, thing, 250, CSoundPair())
{
    {
        CCharString name("ST_WALK_INTO", -1);       // scoped named local (a bare temporary would reuse eax, GOTCHAS)
        SetName(name);                     // 0x008574D0
    }
}