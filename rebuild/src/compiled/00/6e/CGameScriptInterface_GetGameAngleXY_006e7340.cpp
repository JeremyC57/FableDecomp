// Leaf: ?GetGameAngleXY@CGameScriptInterface@@UBEMXZ
// Float literals (NF418, 2026-11-25): retail's merged __real@ pool constants were modelled as data: g_defaultGameAngleXY (table ?g_defaultGameAngleXY@@3MA -> 0x0122DEDC) -> 0.0f. Spelled at the use
//   site; VC7.1 references a plain float literal through __real@<bits> (COMDAT, exe-wide merged), e.g. `return 0.0f;` -> fld [__real@00000000].
// CGameScriptInterface::GetGameAngleXY  @ 006e7340
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

// Obj: getAngle at vtable +0x64 => slot index 25 (0..24 fillers)
struct Obj {
    virtual void f00(); virtual void f01(); virtual void f02(); virtual void f03();
    virtual void f04(); virtual void f05(); virtual void f06(); virtual void f07();
    virtual void f08(); virtual void f09(); virtual void f10(); virtual void f11();
    virtual void f12(); virtual void f13(); virtual void f14(); virtual void f15();
    virtual void f16(); virtual void f17(); virtual void f18(); virtual void f19();
    virtual void f20(); virtual void f21(); virtual void f22(); virtual void f23();
    virtual void f24();
    virtual float getAngle(); // +0x64
};

// Holder: tryGet at vtable +0x28 => slot index 10 (0..9 fillers)
struct Holder {
    virtual void g00(); virtual void g01(); virtual void g02(); virtual void g03();
    virtual void g04(); virtual void g05(); virtual void g06(); virtual void g07();
    virtual void g08(); virtual void g09();
    virtual bool tryGet(Obj** out); // +0x28
};

class CWorld { public:
    char pad34[0x34];
    Holder* holder;
};


float CGameScriptInterface::GetGameAngleXY() const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    Obj* local;
    Holder* h = self->World->holder;
    if (h->tryGet(&local)) {
        return local->getAngle();
    }
    return 0.0f;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_GetGameAngleXY -> CGameScriptInterface::GetGameAngleXY, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
