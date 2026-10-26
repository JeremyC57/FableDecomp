// NParticleEngine::CPSCDecalRenderer::DatabaseClone() -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCDecalRenderer@NParticleEngine@@
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b1/CThingTrackNode_DrawIsDrawable_00b1ff80.cpp: vtable slot 2 of NParticleEngine::CPSCDecalRenderer; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
#pragma optimize("g",on)
namespace NParticleEngine { class CPSCDecalRenderer; }
using NParticleEngine::CPSCDecalRenderer;

struct IDrawableObj {
    // slot 0x12 (offset 0x48): __fastcall method taking a CPSCDecalRenderer*
    virtual void pad00();
    virtual void pad01();
    virtual void pad02();
    virtual void pad03();
    virtual void pad04();
    virtual void pad05();
    virtual void pad06();
    virtual void pad07();
    virtual void pad08();
    virtual void pad09();
    virtual void pad10();
    virtual void pad11();
    virtual void pad12();
    virtual void pad13();
    virtual void pad14();
    virtual void pad15();
    virtual void pad16();
    virtual void pad17();
    virtual int slot12(CPSCDecalRenderer* node);
};

extern IDrawableObj* __fastcall GetDrawableObject_00af8100(CPSCDecalRenderer* self);

namespace NParticleEngine {
class CPSCDecalRenderer { public:
    IDrawableObj* DatabaseClone();
};
}

IDrawableObj* NParticleEngine::CPSCDecalRenderer::DatabaseClone()
{
    IDrawableObj* obj = GetDrawableObject_00af8100(this);
    obj->slot12(this);
    return obj;
}
