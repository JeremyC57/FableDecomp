// CVillageTaskPickCrops::IsBetterThan(const CVillageTaskPickCrops*, const CThing&) const -- with a rival task (and this one available) this task wins when its crop is nearer to the thing; with no rival the answer is IsTaskAvailable()
// Leaf: IsBetterThan
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860D00, 282 bytes (288 with padding), `ret 8`.
//   PDB ?IsBetterThan@CVillageTaskPickCrops@@QBE_NPBV1@ABVCThing@@@Z (non-virtual).
// Callees: virtual IsTaskAvailable (slot 4, +0x10 of vtable 0x0125A928; twice), CBaseIntelligentPointer::GetPItem
//   0x00A01B50 (x2), CThing::GetPos 0x004C73D0 (x3).
// Layout (Ego_r.pdb = retail): Crop +0x08, CreatureAssignedToTask +0x10.
// Shape: the landed CVillageTaskSecuritySweep::IsBetterThan (0x00860430) with both goal points taken from the crop
//   things: by-value position copies, named deltas, `myDist < rivalDist` evaluated left first.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector r;
        r.X = X - o.X;
        r.Y = Y - o.Y;
        r.Z = Z - o.Z;
        return r;
    }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CThing : public CBaseObject { public:
    const C3DVector& GetPos() const;          // 0x004C73D0
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<T>: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x04
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
class CVillageTaskPickCrops { public:                     // vtable 0x0125A928
    virtual ~CVillageTaskPickCrops();
    virtual void v01();
    virtual long GetTaskType() const;              // slot 2 0x0065EF31
    virtual bool IsValid() const;                  // slot 3 0x00860C50
    virtual bool IsTaskAvailable() const;          // slot 4 (+0x10) 0x0065EF86
    void* m_pVillage;                              // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer Crop;                  // +0x08 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x10 CIntelligentPointer<const CThing>
    bool IsBetterThan(const CVillageTaskPickCrops* other, const CThing& thing) const;
};
bool CVillageTaskPickCrops::IsBetterThan(const CVillageTaskPickCrops* other, const CThing& thing) const
{
    if (other && IsTaskAvailable()) {
        C3DVector crop = static_cast<const CThing*>(Crop.GetPItem())->GetPos();
        C3DVector rivalCrop = static_cast<const CThing*>(other->Crop.GetPItem())->GetPos();
        C3DVector position = thing.GetPos();
        C3DVector myDelta = crop - position;
        C3DVector rivalDelta = rivalCrop - position;
        return myDelta.GetLengthSquared() < rivalDelta.GetLengthSquared();
    }
    return IsTaskAvailable();
}