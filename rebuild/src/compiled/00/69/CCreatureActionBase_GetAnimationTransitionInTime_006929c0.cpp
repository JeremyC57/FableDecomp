// Leaf: ?CCreatureActionBase_GetAnimationTransitionInTime@@YIJPAVCCreatureActionBase@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct Foo { long Method(long arg); };
struct Sub { };

class CCreatureActionBase { public:
    char pad0[8];
    Sub  sub;
    char pad1[0x74 - 8 - sizeof(Sub)];
    Foo* f74;
};

long __fastcall CCreatureActionBase_GetAnimationTransitionInTime(CCreatureActionBase* action)
{
    Foo* f = action->f74;
    long r = ((__declspec(noinline) long)(((CBaseIntelligentPointer*)(&action->sub))->GetPItem()));
    return f->Method(r);
}