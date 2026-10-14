// CCreatureAction_ForcePushInto::CCreatureAction_ForcePushInto(CThingCreatureBase*, bool) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), one bool member at +0x114
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090A760, length 63 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030.
// Vtable 0x012783D4 = CCreatureAction_ForcePushInto (rtti-vtables.tsv), so the manifest module
// CCombatAction_GenericStrikeResponseKnockdownRecoverBase is a propagated label and wrong.
// Shape: the zeroed pair is stored before the pushes (`xor eax,eax ; mov [esp+4],eax ; mov [esp+8],eax`),
// the bool argument is read after the base call (`mov dl,[esp+14h]`) and stored at +0x114.
struct CThingCreatureBase;

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

struct CCreatureAction_ForcePushInto : CCombatActionBase {
    bool m_pushLeft;                            // +0x114
    virtual void v00();                         // own vtable 0x012783D4
    CCreatureAction_ForcePushInto(CThingCreatureBase* owner, bool pushLeft);
};

CCreatureAction_ForcePushInto::CCreatureAction_ForcePushInto(CThingCreatureBase* owner, bool pushLeft)
    : CCombatActionBase(owner, 150, CSoundPair()), m_pushLeft(pushLeft)
{
}
