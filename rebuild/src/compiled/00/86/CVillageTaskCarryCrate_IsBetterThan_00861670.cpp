// CVillageTaskCarryCrate::IsBetterThan(const CVillageTaskCarryCrate* other, const CThing& creature) const -- a task
// the creature cannot do is never better; against a rival of equal priority the nearer crate stack wins for a pickup
// (the farther for a drop), a lower priority loses; otherwise better when available or the creature fits it better
// Leaf: IsBetterThan
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00861670, 327 bytes (padded to 329+), `ret 8`.
//   PDB ?IsBetterThan@CVillageTaskCarryCrate@@QBE_NPBV1@ABVCThing@@@Z (non-virtual).
// Callees: IsCreatureValidForTask 0x008614B0, CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2), CThing::GetPos
//   0x004C73D0 (x3), IsTaskAvailable (virtual slot 4, +0x10), IsCreatureMoreAppropriateForTask 0x00861530.
// Layout: rebuild/include/engine/CVillageTaskCarryCrate.h (Ego_r.pdb = retail): PCrateStack +0x10, Pickup +0x18,
//   Priority +0x1c; modelled with the intelligent-pointer accessors as the landed IsCreatureMoreAppropriateForTask
//   0x00861530 (same TU).
// Shape: labelled exits (fail on the drop arm return, avail before IsTaskAvailable) reproduce retail block order:
//   the two tail-duplicated false epilogues and the priority test after them. Distance code as the landed
//   CVillageTaskPickCrops::IsBetterThan (0x00860D00).
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/C3DVector.h"
class CThing : public CBaseObject { public:
    const C3DVector& GetPos() const;          // 0x004C73D0
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<T>: vptr + item (8 bytes)
    void*        __vftable;                        // +0x00
    CBaseObject* m_pItem;                          // +0x04
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
class CVillageTaskCarryCrate { public:                    // vtable 0x0125A950
    virtual ~CVillageTaskCarryCrate();
    virtual void v01() const;                      // slot 1
    virtual long GetTaskType() const;              // slot 2
    virtual bool IsValid() const;                  // slot 3
    virtual bool IsTaskAvailable() const;          // slot 4 (+0x10)
    void* m_pVillage;                              // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x08 CIntelligentPointer<const CThing>
    CBaseIntelligentPointer PCrateStack;           // +0x10 CIntelligentPointer<CThing>
    bool Pickup;                                   // +0x18
    long Priority;                                 // +0x1C
    bool IsCreatureValidForTask(const CThing& creature) const;            // 0x008614B0
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const;  // 0x00861530
    bool IsBetterThan(const CVillageTaskCarryCrate* other, const CThing& creature) const;
};
bool CVillageTaskCarryCrate::IsBetterThan(const CVillageTaskCarryCrate* other, const CThing& creature) const
{
    float myDistance;
    float rivalDistance;
    if (!IsCreatureValidForTask(creature))
        goto fail;
    if (other) {
        if (Priority == other->Priority) {
            {
                C3DVector crate = static_cast<const CThing*>(PCrateStack.GetPItem())->GetPos();
                C3DVector rivalCrate = static_cast<const CThing*>(other->PCrateStack.GetPItem())->GetPos();
                C3DVector position = creature.GetPos();
                myDistance = (crate - position).GetSquaredMagnitude();
                rivalDistance = (rivalCrate - position).GetSquaredMagnitude();
            }
            if (Pickup) {
                if (myDistance < rivalDistance)
                    goto avail;
                return false;
            }
            if (myDistance > rivalDistance)
                goto avail;
fail:
            return false;
        }
        if (Priority <= other->Priority)
            goto fail;
    }
avail:
    if (IsTaskAvailable())
        return true;
    return IsCreatureMoreAppropriateForTask(creature);
}