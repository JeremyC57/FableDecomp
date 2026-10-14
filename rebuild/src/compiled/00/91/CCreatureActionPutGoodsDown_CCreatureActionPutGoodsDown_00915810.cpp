// CCreatureAction_PutGoodsDown::CCreatureAction_PutGoodsDown(CThingCreatureBase*, long) -- base CCreatureActionBase(owner, -1.0, 0.0, the owner's table animation for the kind, zeroed CSoundPair, 0, 0), own vtable (0x0128FAD4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00915810, length 79 bytes.
// Callees: the table-animation lookup @ 0x006924B0 (register arguments: ecx owner, edx kind), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long) @ 0x00693B30.
// Shape: `ret 8`; the same argument order as 0x00842A20 with 0.0f pushed as the zero immediate.
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

struct CCreatureAction_PutGoodsDown : CCreatureActionBase {   // vtable 0x0128FAD4
    virtual void v00();
    CCreatureAction_PutGoodsDown(CThingCreatureBase* owner, long kind);
};

CCreatureAction_PutGoodsDown::CCreatureAction_PutGoodsDown(CThingCreatureBase* owner, long kind)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetTableAnimation(owner, kind), CSoundPair(), 0, 0)
{
}