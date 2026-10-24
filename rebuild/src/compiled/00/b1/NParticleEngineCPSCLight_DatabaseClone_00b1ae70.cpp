// NParticleEngine::CPSCLight::DatabaseClone() -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCLight@NParticleEngine@@
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b1/CThingTrackNode_DrawIsDrawable_00b1ae70.cpp: vtable slot 2 of NParticleEngine::CPSCLight; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
namespace NParticleEngine { struct CPSCLight; }
using NParticleEngine::CPSCLight;

struct CPrim
{
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void Draw(CPSCLight* node);
};

CPrim* __fastcall CreatePrim(CPSCLight* self);

namespace NParticleEngine {
struct CPSCLight
{
    CPrim* __fastcall DatabaseClone();
};
}

CPrim* __fastcall NParticleEngine::CPSCLight::DatabaseClone()
{
    CPrim* p = CreatePrim(this);
    p->Draw(this);
    return p;
}
