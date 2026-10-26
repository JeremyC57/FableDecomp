// CCreatureAction_PickUpTankard::CCreatureAction_PickUpTankard(CThingCreatureBase*, CThing*) -- base CCreatureAction_PickUpThingAtTable(owner, thing, "DRINK_AT_TABLE_INTO", 39), own vtable (0x01292D6C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947B70, length 64 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(CThingCreatureBase*, CThing*, const CCharString&, long) @ 0x008440C0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: `ret 8`; the animation name comes from an inline helper returning it by value, so it is built in the one-word frame before any argument is pushed and its address re-derived for the call (a bare temporary argument pushes the trailing constant first and reuses the constructor's eax). Class name from rtti-vtables.tsv (the chunk label is propagated).
#include "fable_string.h"
class CThingCreatureBase; class CThing;

class CCreatureAction_PickUpThingAtTable { public:   // retail sizeof 0xb4
    virtual void v00();
    unsigned char _body[0xb4 - 4];
    CCreatureAction_PickUpThingAtTable(CThingCreatureBase* owner, CThing* thing, const CCharString& animation, long kind);   // 0x008440C0
};

static inline CCharString AnimationName() { return CCharString("DRINK_AT_TABLE_INTO", -1); }
class CCreatureAction_PickUpTankard : public CCreatureAction_PickUpThingAtTable { public:   // vtable 0x01292D6C
    virtual void v00();
    CCreatureAction_PickUpTankard(CThingCreatureBase* owner, CThing* thing);
};

CCreatureAction_PickUpTankard::CCreatureAction_PickUpTankard(CThingCreatureBase* owner, CThing* thing)
    : CCreatureAction_PickUpThingAtTable(owner, thing, AnimationName(), 39)
{
}