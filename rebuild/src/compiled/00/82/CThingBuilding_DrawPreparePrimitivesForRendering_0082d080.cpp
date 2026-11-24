// CThingBuilding::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info) -- prepare the physical thing's primitives, then let the graphic appearance prepare too
// Leaf: ?DrawPreparePrimitivesForRendering@CThingBuilding
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D080, 41 bytes (+ int3 padding).
// Callees: CThingPhysical::DrawPreparePrimitivesForRendering @ 0x006A64D0 (the base implementation, called
//          non-virtually), CTCGraphicAppearance::PrepareForDraw @ 0x004BE4E0 (manifest "CTCGraphicAppearance_PrepareForDraw").
// Identity: CThingBuilding vtable 0x0124509C slot 36 (rtti-vtables.tsv); the slot is named
//   DrawPreparePrimitivesForRendering in two other CThing vtables. The graphic appearance component pointer sits at
//   retail +0x64 of the thing.
// Shape: `if (base()) { m_pGraphic->PrepareForDraw(info); return true; } return false;` -- the argument is kept in edi.
class CEnginePrimitivePrepareInfo;

struct CTCGraphicAppearance {
    void PrepareForDraw(CEnginePrimitivePrepareInfo* info);    // 0x004BE4E0
};

class CThingPhysical { public:
    // NF374 (NF299 Q3): the PDB member is virtual (Ego_r + FableWin ?DrawPreparePrimitivesForRendering@CThingPhysical@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z);
    // the call below is qualified (direct), the TU emits no CThingPhysical vftable, and the leading pad shrinks by the vptr so +0x64 holds.
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006A64D0
    unsigned char _pad04[0x64 - 0x04];
    CTCGraphicAppearance* m_pGraphicAppearance;                 // +0x64
};

class CThingBuilding : public CThingPhysical { public:
    bool DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info);
};

bool CThingBuilding::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(*info)) {
        m_pGraphicAppearance->PrepareForDraw(info);
        return true;
    }
    return false;
}