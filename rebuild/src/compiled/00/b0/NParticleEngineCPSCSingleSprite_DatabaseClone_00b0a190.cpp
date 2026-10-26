// NParticleEngine::CPSCSingleSprite::DatabaseClone() -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCSingleSprite@NParticleEngine@@
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b0/CThingTrackNode_DrawIsDrawable_00b0a190.cpp: vtable slot 2 of NParticleEngine::CPSCSingleSprite; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
namespace NParticleEngine { class CPSCSingleSprite; }
using NParticleEngine::CPSCSingleSprite;

struct IDrawableObj {
    // A vtbl of __fastcall member functions. Slot 0x12 (byte offset 0x48)
    // takes one stack arg (the node) with 'this' in ecx.
    int Draw(CPSCSingleSprite* node);   // placeholder shape; real dispatch via vtbl
};

// helper: __fastcall on the node, returns the drawable object
extern IDrawableObj* __fastcall GetDrawableObject_00af7df0(CPSCSingleSprite* self);

namespace NParticleEngine {
class CPSCSingleSprite { public:
    IDrawableObj* DatabaseClone();
};
}

// vtable modeled as an array of member-function pointers so the call is __fastcall
// (ecx = obj, one stack arg) with no edx setup.
typedef int (IDrawableObj::*Slot)(CPSCSingleSprite* node);

IDrawableObj* NParticleEngine::CPSCSingleSprite::DatabaseClone()
{
    IDrawableObj* obj = GetDrawableObject_00af7df0(this);
    Slot* vtbl = *(Slot**)obj;
    (obj->*vtbl[0x12])(this);
    return obj;
}
