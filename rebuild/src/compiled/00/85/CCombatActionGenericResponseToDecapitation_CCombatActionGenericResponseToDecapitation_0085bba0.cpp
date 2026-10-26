// CCombatAction_GenericResponseToDecapitation::CCombatAction_GenericResponseToDecapitation(CThingCreatureBase*) -- base CCombatActionBase(owner, 0, zeroed CSoundPair)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085BBA0, length 49 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030.
// Vtable 0x012767EC (rtti-vtables.tsv labels it CCreatureAction_HeroRunningStop -- a propagated name; the manifest
// class label is kept because the ctor address is unique to it).
// Shape: the action code is 0, so VC reuses the `xor eax,eax` of the sound pair for the pushed code, and the
// two zero stores of the pair are sunk below the pushes (`push ecx ; push eax ; push edx ; mov [esp+10h],eax ;
// mov [esp+14h],eax ; call`). The engine header CCombatAction_GenericResponseToDecapitation.h (Ego_r.pdb)
// is 0x18 bytes larger than retail (base sizeof 0x12c v 0x114), so the layout is modelled locally.
class CThingCreatureBase;

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

class CCombatAction_GenericResponseToDecapitation : public CCombatActionBase { public:
    virtual void v00();                         // own vtable 0x012767EC
    CCombatAction_GenericResponseToDecapitation(CThingCreatureBase* owner);
};

CCombatAction_GenericResponseToDecapitation::CCombatAction_GenericResponseToDecapitation(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 0, CSoundPair())
{
}