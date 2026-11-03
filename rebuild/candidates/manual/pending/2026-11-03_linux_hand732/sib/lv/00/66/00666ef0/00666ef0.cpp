// CThingCreatureBase::AutoPlaceNearBuilding(const CThingBuilding*,float) -- place beside a building.
// Leaf: ?AutoPlaceNearBuilding@CThingCreatureBase@@QAE_NPBVCThingBuilding@@M@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CWORLD
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCThingOwner.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CThingFilter_PosInContainmentVolume.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CWorld.h"
extern const CCharString BUILDING_BOUNDRY;
long __fastcall GFROR13(unsigned long);
namespace {
class RandFunc {
    unsigned long* Seed;
public:
    RandFunc(unsigned long* seed) : Seed(seed) { }
    long operator()(long range) {
        *Seed=*Seed*0x24a1UL+0x24dfUL;
        *Seed=GFROR13(*Seed);
        return range ? *Seed % range : 0;
    }
};
}
// STLport _algo.c random_shuffle, specialized at its actual original argument types.
namespace std {
template<> inline void __fastcall random_shuffle<CThing**,RandFunc>(
    CThing** first,CThing** last,RandFunc& random)
{
    if(first!=last) {
        for(CThing** i=first+1;i!=last;++i) {
            CThing** j=first+random((i-first)+1);
            CThing* value=*i;
            *i=*j;
            *j=value;
        }
    }
}
}
bool CThingCreatureBase::AutoPlaceNearBuilding(const CThingBuilding* building,float distance)
{
    if (building && !building->IsDead()) {
    for (long tries=0;tries<10;++tries) {
        if (!building->GetPhysicsTC()->GetPhysicsMeshInfo())
            continue;
        bool containment=building->GetPhysicsTC()->GetPhysicsMeshObject().HasContainmentVolume(BUILDING_BOUNDRY);
        if (building->HasTC(TCI_THING_OWNER)) {
            const CTCThingOwner& owner=static_cast<const CTCThingOwner&>(building->GetTC(TCI_THING_OWNER));
            if (!owner.GetObjectList().empty()) {
                std::list<CIntelligentPointer<CThing> >::const_iterator it=owner.GetObjectList().begin();
                std::vector<CThing*> points;
                for (;it!=owner.GetObjectList().end();++it) {
                    if (it->Get() && !it->Get()->IsDead() && it->Get()->GetType()==5 &&
                        !it->Get()->HasTC(TCI_SWITCHABLE_NAVIGATION))
                    {
                        points.push_back(it->Get());
                    }
                }
                {
                    RandFunc random(GetPWorldSeed());
                    std::random_shuffle(points.begin(),points.end(),random);
                }
                {
                unsigned int count=points.size();
                for (unsigned int index=0;index<count;++index) {
                    CTCCreatureNavigation* navigation;
                    if (HasTC(TCI_NAVIGATION))
                        navigation=&static_cast<CTCCreatureNavigation&>(GetTC(TCI_NAVIGATION));
                    C3DVector position=points[index]->GetPos();
                    C3DVector original=position;
                    CThingBuilding* found=0;
                    GetWorld().GetThingSearchTools().GetPFirstLeafThing(
                        GFFilter_Bind2(CThingFilter_OverlappingSphere(position,0.0f),
                                      CThingFilter_PosInContainmentVolume(position,BUILDING_BOUNDRY)),&found);
                    if (found==building && navigation->GetNearestNavigablePosTo(original,distance,position) &&
                        !(original.X==position.X && original.Y==position.Y) &&
                        (!containment || building->IsPointInside(position))) {
                        navigation->InterruptNavigation();
                        PhysicsTC->ResetPosition(position);
                        navigation->InterruptNavigation();
                        return true;
                    }
                }
                }
            }
        }
        CTCCreatureNavigation* navigation=0;
        if (HasTC(TCI_NAVIGATION))
            navigation=&static_cast<CTCCreatureNavigation&>(GetTC(TCI_NAVIGATION));
        C3DVector position=building->GetPhysPos();
        C3DVector original=position;
        float radius=building->GetPhysicsTC()->GetRadius();
        if (containment)
            radius*=0.5f;
        else
            radius+=distance;
        if (navigation->GetNearbyWanderPosition(original,radius,position,false) &&
            (!containment || building->IsPointInside(position))) {
            navigation->InterruptNavigation();
            PhysicsTC->ResetPosition(position);
            navigation->InterruptNavigation();
            return true;
        }
    }
    }
    return false;
}