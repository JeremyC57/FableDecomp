// CCreatureAction_RemoveFoodFromTable::CCreatureAction_RemoveFoodFromTable(CThingCreatureBase*, CThing*) -- base CCreatureAction_PickUpThingAtTable(owner, thing, "TAKE_FOOD_FROM_TABLE", 0), own vtable (0x0129007C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00922EE0, length 64 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(CThingCreatureBase*, CThing*, const CCharString&, long) @ 0x008440C0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: `ret 8`; the animation name comes from an inline helper returning it by value, so it is built in the one-word frame before any argument is pushed and its address re-derived for the call (a bare temporary argument pushes the trailing constant first and reuses the constructor's eax). Class name from rtti-vtables.tsv (the chunk label is propagated).
#include "fable_string.h"
struct CThingCreatureBase; struct CThing;

struct CCreatureAction_PickUpThingAtTable {   // retail sizeof 0xb4
    virtual void v00();
    unsigned char _body[0xb4 - 4];
    CCreatureAction_PickUpThingAtTable(CThingCreatureBase* owner, CThing* thing, const CCharString& animation, long kind);   // 0x008440C0
};

static inline CCharString AnimationName() { return CCharString("TAKE_FOOD_FROM_TABLE", -1); }
struct CCreatureAction_RemoveFoodFromTable : CCreatureAction_PickUpThingAtTable {   // vtable 0x0129007C
    virtual void v00();
    CCreatureAction_RemoveFoodFromTable(CThingCreatureBase* owner, CThing* thing);
};

CCreatureAction_RemoveFoodFromTable::CCreatureAction_RemoveFoodFromTable(CThingCreatureBase* owner, CThing* thing)
    : CCreatureAction_PickUpThingAtTable(owner, thing, AnimationName(), 0)
{
}
