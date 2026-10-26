// CThingDeadCreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- prepare the physical thing's primitives, then let the graphic appearance component prepare its own; false when the base refuses
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835AC0, length 41 bytes (+ int3 padding).
// Callees: CThingPhysical::DrawPreparePrimitivesForRendering @ 0x006A64D0 (the qualified base call),
// CTCGraphicAppearance::PrepareForDraw(const CEnginePrimitivePrepareInfo&) @ 0x004BE4E0 (manifest
// "CTCGraphicAppearance_PrepareForDraw").
// Identity: CThingDeadCreature vtable 0x01272B04 slot 36 = CThing::DrawPreparePrimitivesForRendering (0x004C7540 in the
// CThing vtable); Ego_r.pdb `?DrawPreparePrimitivesForRendering@CThingDeadCreature@@MAE_NABVCEnginePrimitivePrepareInfo@@@Z`.
// Layout (retail, local model): the thing's cached graphic appearance component pointer at +0x64.
// Shape: the info reference kept in edi across the base call; the success path is the `if` body with `mov al,1`.
class CEnginePrimitivePrepareInfo;

struct CTCGraphicAppearance {
    void PrepareForDraw(const CEnginePrimitivePrepareInfo& info);   // 0x004BE4E0
};

class CThingPhysical { public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* m_pGraphicAppearance; // +0x64
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006A64D0
};

class CThingDeadCreature : public CThingPhysical { public:
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

bool CThingDeadCreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
        m_pGraphicAppearance->PrepareForDraw(info);
        return true;
    }
    return false;
}