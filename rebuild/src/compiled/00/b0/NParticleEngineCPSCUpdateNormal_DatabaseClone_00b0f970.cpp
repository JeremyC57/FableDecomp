// NParticleEngine::CPSCUpdateNormal::DatabaseClone() -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCUpdateNormal@NParticleEngine@@
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b0/CThingTrackNode_DrawIsDrawable_00b0f970.cpp: vtable slot 2 of NParticleEngine::CPSCUpdateNormal; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
#pragma optimize("g",on)
namespace NParticleEngine { struct CPSCUpdateNormal; }
using NParticleEngine::CPSCUpdateNormal;

// Non-polymorphic class: member function pointer is a single 4-byte code pointer.
struct IDrawableObj {
    void* vtbl;
    int Slot12(CPSCUpdateNormal* node);
};

typedef int (IDrawableObj::*DrawPMF)(CPSCUpdateNormal*);

extern IDrawableObj* __fastcall GetDrawableObject_00af7e60(CPSCUpdateNormal* self);

namespace NParticleEngine {
struct CPSCUpdateNormal {
    IDrawableObj* DatabaseClone();
};
}

IDrawableObj* NParticleEngine::CPSCUpdateNormal::DatabaseClone()
{
    IDrawableObj* obj = GetDrawableObject_00af7e60(this);
    void** vt = (void**)obj->vtbl;
    void* raw = vt[0x12];
    DrawPMF pmf;
    *(void**)&pmf = raw;
    (obj->*pmf)(this);
    return obj;
}
