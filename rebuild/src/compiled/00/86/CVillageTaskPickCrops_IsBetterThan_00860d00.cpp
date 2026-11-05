// CVillageTaskPickCrops::IsBetterThan(const CVillageTaskPickCrops*, const CThing&) const -- with a rival task (and this one available) this task wins when its crop is nearer to the thing; with no rival the answer is IsTaskAvailable()
// Leaf: IsBetterThan
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860D00, 282 bytes (288 with padding), `ret 8`.
//   PDB ?IsBetterThan@CVillageTaskPickCrops@@QBE_NPBV1@ABVCThing@@@Z (non-virtual).
// Callees: virtual IsTaskAvailable (slot 4, +0x10 of vtable 0x0125A928; twice), CBaseIntelligentPointer::GetPItem
//   0x00A01B50 (x2), CThing::GetPos 0x004C73D0 (x3).
// Layout (Ego_r.pdb = retail): Crop +0x08, CreatureAssignedToTask +0x10.
// Shape: the landed CVillageTaskSecuritySweep::IsBetterThan (0x00860430) with both goal points taken from the crop
//   things: by-value position copies, named deltas, `myDist < rivalDist` evaluated left first.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/C3DVector.h"
class CThing : public CBaseObject { public:
    const C3DVector& GetPos() const;          // 0x004C73D0
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
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
        return myDelta.GetSquaredMagnitude() < rivalDelta.GetSquaredMagnitude();
    }
    return IsTaskAvailable();
}