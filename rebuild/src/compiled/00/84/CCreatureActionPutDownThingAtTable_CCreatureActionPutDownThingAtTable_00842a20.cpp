// CCreatureAction_PutDownThingAtTable::CCreatureAction_PutDownThingAtTable(CThingCreatureBase*, long, long) -- base CCreatureActionBase(owner, -1.0, 0.5, the owner's table animation for the kind, zeroed CSoundPair, 0, 0), own vtable (0x01273C4C), the thing word at +0xa8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842A20, length 82 bytes.
// Callees: the table-animation lookup @ 0x006924B0 (register arguments: ecx owner, edx kind), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long) @ 0x00693B30.
// Shape: `ret 0Ch`; arguments evaluated right to left -- the two zeros, the sound-pair temporary (register-zeroed frame stores), the lookup, 0.5f, -1.0f, the owner.
struct CThingCreatureBase;
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CAnimationEntry;
struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};
CAnimationEntry* __fastcall GetTableAnimation(CThingCreatureBase* owner, long kind);   // 0x006924B0: the creature's animation for a table action kind (register arguments)

struct CCreatureAction_PutDownThingAtTable : CCreatureActionBase {   // vtable 0x01273C4C
    long m_thing;                           // +0xa8
    virtual void v00();
    CCreatureAction_PutDownThingAtTable(CThingCreatureBase* owner, long kind, long thing);
};

CCreatureAction_PutDownThingAtTable::CCreatureAction_PutDownThingAtTable(CThingCreatureBase* owner, long kind, long thing)
    : CCreatureActionBase(owner, -1.0f, 0.5f, GetTableAnimation(owner, kind), CSoundPair(), 0, 0)
    , m_thing(thing)
{
}