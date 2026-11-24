// Leaf: ?MsgIsGameInfoClickedPast@CGameScriptInterface@@UBE_NXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct Msg { int type; int a; int b; };
struct Range { Msg* p1; int* p2; };
struct Frame { Msg m; Range r; };

struct Obj { int Dispatch(Range* r); };
extern int __fastcall GetThing1(void);
extern int __fastcall GetThing2(void* self);

bool CGameScriptInterface::MsgIsGameInfoClickedPast() const
{
    void* self = (void*)this;
    int e = GetThing1();
    int a = GetThing2(self);
    Obj* obj = *(Obj**)((*(int*)((char*)self + 4)) + 0x60);
    Frame f;
    f.r.p1 = &f.m;
    f.m.b = a;
    f.m.a = e;
    f.m.type = 0x12;
    f.r.p2 = &f.m.a;
    return obj->Dispatch(&f.r);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_MsgIsGameInfoClickedPast -> CGameScriptInterface::MsgIsGameInfoClickedPast, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
