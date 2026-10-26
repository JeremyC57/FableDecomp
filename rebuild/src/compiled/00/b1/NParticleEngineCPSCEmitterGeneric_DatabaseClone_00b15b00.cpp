// NParticleEngine::CPSCEmitterGeneric::DatabaseClone() -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCEmitterGeneric@NParticleEngine@@
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b1/CThingTrackNode_DrawIsDrawable_00b15b00.cpp: vtable slot 2 of NParticleEngine::CPSCEmitterGeneric; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
#pragma optimize("g",on)
namespace NParticleEngine { class CPSCEmitterGeneric; }
using NParticleEngine::CPSCEmitterGeneric;

// Object with a virtual method occupying vtable slot 0x12 (offset 0x48).
struct IDrawableBase {
    virtual int pad00();
    virtual int pad01();
    virtual int pad02();
    virtual int pad03();
    virtual int pad04();
    virtual int pad05();
    virtual int pad06();
    virtual int pad07();
    virtual int pad08();
    virtual int pad09();
    virtual int pad10();
    virtual int pad11();
    virtual int pad12();
    virtual int pad13();
    virtual int pad14();
    virtual int pad15();
    virtual int pad16();
    virtual int pad17();
    virtual int slot18(CPSCEmitterGeneric* node); // offset 0x48
};

extern IDrawableBase* __fastcall GetDrawableObject(CPSCEmitterGeneric* self);

namespace NParticleEngine {
class CPSCEmitterGeneric { public:
    IDrawableBase* DatabaseClone();
};
}

IDrawableBase* NParticleEngine::CPSCEmitterGeneric::DatabaseClone()
{
    IDrawableBase* obj = GetDrawableObject(this);
    obj->slot18(this);
    return obj;
}
