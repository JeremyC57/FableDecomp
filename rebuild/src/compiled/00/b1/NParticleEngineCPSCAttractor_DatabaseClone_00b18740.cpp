// NParticleEngine::CPSCAttractor::DatabaseClone() -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCAttractor@NParticleEngine@@
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b1/CThingTrackNode_DrawIsDrawable_00b18740.cpp: vtable slot 2 of NParticleEngine::CPSCAttractor; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
// (was: NParticleEngine::CPSCAttractor::DatabaseClone @ 0x00b18740)
// retail: 56 57 8bf9 e8.. 8bf0 8b06 57 8bce ff5048 5f 8bc6 5e c3
// same shape as 0x00afe580.

namespace NParticleEngine { struct CPSCAttractor; }
using NParticleEngine::CPSCAttractor;

struct IDrawableObj {
    int Draw(CPSCAttractor* node);   // placeholder shape; real dispatch via vtbl
};

// helper @ 0x00af7fb0 : __fastcall on the node, returns the drawable object
extern IDrawableObj* __fastcall CreateInternalPrimitive(CPSCAttractor* self);

namespace NParticleEngine {
struct CPSCAttractor {
    IDrawableObj* DatabaseClone();
};
}

// vtable modeled as an array of member-function pointers so the call is __fastcall
// (ecx = obj, one stack arg) with no edx setup.
typedef int (IDrawableObj::*Slot)(CPSCAttractor* node);

IDrawableObj* NParticleEngine::CPSCAttractor::DatabaseClone()
{
    IDrawableObj* obj = CreateInternalPrimitive(this);
    Slot* vtbl = *(Slot**)obj;
    (obj->*vtbl[0x12])(this);
    return obj;
}
