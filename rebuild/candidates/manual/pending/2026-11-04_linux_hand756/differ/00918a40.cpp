// CAIStateGroup_GazeAtWork::IsRunnable() -- runnable when the group has a workplace, the creature's shop (if it keeps one) is open and a free gaze position exists
// Grade: DIFFER(ours v retail) -- DIFFER(114v116) (try c): the keeper is loaded straight into ecx (mov ecx,[eax+4]) and esi is popped after the GetShopTC null test; retail loads eax, pops esi, then mov ecx,eax (the esi region ends at a block boundary before the call). Tried 27 spellings: GetTC/PeekTC/HasTC+GetTC(type), bool local, scopes, const, ==true.
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
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCShop.h"

class CThingBuilding;
class CThingGameObject;

class CTCShopKeeper : public CTCBase {                             // no shared header
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SHOP_KEEPER; }
    CTCShop* GetShopTC() const;                                     // 0x00798F50
};

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
