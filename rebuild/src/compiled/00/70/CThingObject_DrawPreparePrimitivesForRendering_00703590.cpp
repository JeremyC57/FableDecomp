// CThingObject::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- after the CThingPhysical preparation: prepare the weapon trail (interface 0x4C, when trails are shown) and the two draw-preparing components (0x54, 0xCF); take the render position and orientation interpolators from the carry pose when the object is being carried (pushing them into the physics component and updating attached particles, 0xD8) or else from the physics component; then let the graphic appearance prepare itself with them
// Leaf: DrawPreparePrimitivesForRendering
// Callees: CThingPhysical::DrawPreparePrimitivesForRendering @ 0x006A64D0 (non-virtual), CVectorMap::LowerBound const @ 0x004365B0 /
//          non-const @ 0x0040F020, CTCWeaponTrail @ 0x0076E6A0, the components' slot 11 (+0x2c), CBase ctor @ 0x0099A2D0 /
//          dtor @ 0x0099A2E0, CTCCarryable::GetCarriedPositionInfo @ 0x00705B00, physics slots 31/32 (+0x7c/+0x80),
//          CTCParticleAttachmentTracker::UpdateAttachedParticles @ 0x0071A9C0, CTCPhysicsBase::GetPositionInterpolator @ 0x006B0250 /
//          GetOrientationInterpolator @ 0x006B0460, CTCGraphicAppearance::PrepareForRenderIfNeeded @ 0x004BE490; global 0x0137548E.
// Layout: retail CThing interface bitset +0x20, CVectorMap +0x44, PhysicsTC +0x60, PTCGraphicAppearance +0x64;
//          CEnginePrimitivePrepareInfo time +0x04; interpolators per Ego_r.pdb (vtables 0x0123981C / 0x01239824).
// Shape: the const lookups go through `const CThingObject* const& self = this;` -- a reference to the this pointer (the
//        pointer itself homed, then forwarded): VC7.1 then treats the 0x47 bit load as a may-alias of the frame-local
//        interpolators and keeps the orientation vtable store ahead of it (a plain `self = this` or `this` hoists the load).
struct CEnginePrimitivePrepareInfo {
    long  _f00;
    float Time;                                         // +0x04
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CBase {
    virtual ~CBase();                                   // 0x0099A2E0
    CBase();                                            // 0x0099A2D0
};
struct C3DPositionInterpolator : CBase {                // vtable 0x0123981C
    C3DVector StartPosition;                            // +0x04
    C3DVector EndPosition;                              // +0x10
};
struct C3DOrientationInterpolator : CBase {             // vtable 0x01239824
    CRightHandedSet StartOrientation;                   // +0x04
    CRightHandedSet EndOrientation;                     // +0x1c
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#define VF(n) virtual void v##n();
struct CTCDrawPreparer {
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10)
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // slot 11 (+0x2c)
};
struct CTCWeaponTrail {
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);           // 0x0076E6A0
};
struct CTCCarryable {
    unsigned char _pad00[0x10];
    void* Carrier;                                      // +0x10
    bool GetCarriedPositionInfo(C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation, float time);   // 0x00705B00
};
struct CTCParticleAttachmentTracker {
    void UpdateAttachedParticles(float time);           // 0x0071A9C0
};
struct CTCPhysicsBase {
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15)
    VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30)
    virtual void SetRenderPosition(const C3DVector* position);              // slot 31 (+0x7c)
    virtual void SetRenderOrientation(const CRightHandedSet* orientation);  // slot 32 (+0x80)
    void GetPositionInterpolator(C3DPositionInterpolator& out) const;       // 0x006B0250
    void GetOrientationInterpolator(C3DOrientationInterpolator& out) const; // 0x006B0460
};
struct CTCGraphicAppearance {
    void PrepareForRenderIfNeeded(const CEnginePrimitivePrepareInfo& info, C3DPositionInterpolator* position,
                                  C3DOrientationInterpolator* orientation);  // 0x004BE490
};
extern bool g_ShowWeaponTrails;                         // 0x0137548E

struct CThingPhysical {
    VF(00)
    unsigned char         _pad04[0x1c];
    unsigned long         UsingComponents[9];           // +0x20
    CInterfaceMap         Components;                   // +0x44
    unsigned char         _pad4c[0x60 - 0x4c];
    CTCPhysicsBase*       PhysicsTC;                    // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006A64D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PickInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PickInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingObject : CThingPhysical {
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

bool CThingObject::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
        const CThingObject* const& self = this;
        CTCWeaponTrail* trail;
        if (g_ShowWeaponTrails && self->GetInterface(&trail, 0x4c))
            trail->DrawPreparePrimitivesForRendering(info);
        CTCDrawPreparer* preparer;
        if (PickInterface(&preparer, 0x54))
            preparer->DrawPreparePrimitivesForRendering(info);
        CTCDrawPreparer* preparer2;
        if (PickInterface(&preparer2, 0xcf))
            preparer2->DrawPreparePrimitivesForRendering(info);
        C3DPositionInterpolator position;
        C3DOrientationInterpolator orientation;
        CTCCarryable* carryable;
        if (self->PickInterface(&carryable, 0x47) && carryable->Carrier) {
            carryable->GetCarriedPositionInfo(&position, &orientation, info.Time);
            PhysicsTC->SetRenderPosition(&position.EndPosition);
            PhysicsTC->SetRenderOrientation(&orientation.EndOrientation);
            CTCParticleAttachmentTracker* tracker;
            if (GetInterface(&tracker, 0xd8))
                tracker->UpdateAttachedParticles(info.Time);
        } else {
            PhysicsTC->GetPositionInterpolator(position);
            PhysicsTC->GetOrientationInterpolator(orientation);
        }
        if (PTCGraphicAppearance)
            PTCGraphicAppearance->PrepareForRenderIfNeeded(info, &position, &orientation);
        return true;
    }
    return false;
}