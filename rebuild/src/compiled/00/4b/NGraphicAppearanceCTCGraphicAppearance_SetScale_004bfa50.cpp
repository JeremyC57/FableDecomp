// NGraphicAppearance::CTCGraphicAppearance::SetScale(float) -- set the appearance scale, push the scaled render size into the parent's animation (interface 0x5a), re-add the parent to the render list and flag a primitive update
// Leaf: SetScale
//
// Retail 0x004BFA50, 110 bytes (`ret 4`). Ego_r.pdb `?SetScale@CTCGraphicAppearance@NGraphicAppearance@@
// QAEXM@Z`. Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined lookup of
// interface 0x5a = bit 26 of +0x28), CTCAnimationComplex::GetAnimation() @ 0x0070B460 (manifest
// "CWorld::DrawGetEnvironment", propagated), CThing::AddToRenderList() @ 0x004C7AC0.
// Layout: retail CTCGraphicAppearance (PDB -0x18): Scale +0x34, MainGraphic +0x44 (RenderSizeX +0x4c),
// UpdatePrimitiveReasons +0x58; the animation's scale at +0x88.
// Shape: the component is used straight from the lookup; the product `scale * RenderSizeX` is
// computed before the GetAnimation call and parked in the dead `scale` argument slot (the function has
// no named locals); the reason bit is a byte `or`.
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    void AddToRenderList();                                 // 0x004C7AC0
};

struct C3DAnimation {
    unsigned char _pad00[0x88];
    float         Scale;                                    // +0x88
};

class CTCAnimationComplex { public:
    C3DAnimation* GetAnimation();                           // 0x0070B460
};

#pragma pack(push, 2)
class CEngineGraphic { public:
    unsigned long BankIndex;                                // +0x00
    float         AnimStep;                                 // +0x04
    float         RenderSizeX;                              // +0x08
    bool          AdditiveAlpha;                            // +0x0c
    unsigned char Type;                                     // +0x0d
};
#pragma pack(pop)

namespace NGraphicAppearance {

class CTCGraphicAppearance { public:
    virtual void v00();
    CThing*        ParentThing;                             // +0x04 (CTCBase)
    unsigned char  _pad08[0x34 - 0x08];
    float          Scale;                                   // +0x34
    unsigned char  _pad38[0x44 - 0x38];
    CEngineGraphic MainGraphic;                             // +0x44
    unsigned short ActivePersistenceFlags;                  // +0x52
    long           FrameUpdatedWasRenderedFlag;             // +0x54
    unsigned char  UpdatePrimitiveReasons;                  // +0x58
    void SetScale(float scale);
};

void CTCGraphicAppearance::SetScale(float scale)
{
    Scale = scale;
    if (ParentThing->HasInterface(0x5a))
        ((CTCAnimationComplex*)ParentThing->GetInterface(0x5a))->GetAnimation()->Scale = scale * MainGraphic.RenderSizeX;
    ParentThing->AddToRenderList();
    UpdatePrimitiveReasons |= 4;
}

}