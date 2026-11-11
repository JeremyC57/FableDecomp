// CAIStateGroup_GazeAtWork::IsRunnable() -- runnable when the group has a workplace, the creature's shop (if it keeps one) is open and a free gaze position exists
// Grade: DIFFER(114v116) -- now on shared engine/CTCShopKeeper.h (FABLE_RETAIL_CTCSHOPKEEPER_SHOP) + GETTC_EXPANDED; residue unchanged: keeper loaded straight into ecx and esi popped after the GetShopTC test, retail mov eax,[eax+4]; pop esi; mov ecx,eax (0x00918A85). ORIGINAL_FIND, GetCreature() forms tried. Next: an inline helper boundary between the lookup and GetShopTC.
// Leaf: ?IsRunnable@CAIStateGroup_GazeAtWork@@MAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00918A40, 116 bytes; Ego_r.pdb ?IsRunnable@CAIStateGroup_GazeAtWork@@MAE_NXZ (protected virtual,
//   slot 11 of vtable 0x0128FEC4).
// Callees: virtual GetOwner (slot 14, +0x38), LowerBound 0x0040F020 (CThing::GetTC<CTCShopKeeper> through Components.Find),
//   CTCShopKeeper::GetShopTC() const 0x00798F50, CTCShop::IsOpeningHours() const 0x00773950,
//   CAIStateGroup_Gaze::FindFreeGazePos() 0x00918880.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCShop.h"
#include "engine/CTCShopKeeper.h"

class CThingBuilding;
class CThingGameObject;


class CAIStateGroup_Gaze : public CAIStateGroupBase {
protected:
    virtual CThingBuilding* GetOwner() = 0;                         // slot 14 (+0x38)
    CThingGameObject* FindFreeGazePos();                            // 0x00918880
    CIntelligentPointer<CThingGameObject> GazeMarker;               // +0x14
    float ChanceOfRunning;                                          // +0x1c
    float MinJourneyForRunning;                                     // +0x20
    float MaxDistanceForCreaturesToLookAt;                          // +0x24
    long PrimaryMarkerDef;                                          // +0x28
    long SecondaryMarkerDef;                                        // +0x2c
};

class CAIStateGroup_GazeAtWork : public CAIStateGroup_Gaze {
protected:
    virtual bool IsRunnable();
};

bool CAIStateGroup_GazeAtWork::IsRunnable()
{
    if (GetOwner()) {
        CTCShopKeeper* keeper;
        if (PBrain->Creature->GetTC(&keeper)) {
            CTCShop* shop = keeper->GetShopTC();
            if (!shop || !shop->IsOpeningHours())
                return false;
        }
        return FindFreeGazePos() != 0;
    }
    return false;
}
