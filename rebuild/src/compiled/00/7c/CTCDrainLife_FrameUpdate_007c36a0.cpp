// 007C36A0 CTCDrainLife::FrameUpdate -- drain the target and nearby enemies, then expire released shots.
// Leaf: ?FrameUpdate@CTCDrainLife@@UAEXXZ
// Retail: 0x007C36A0,606 bytes,whole function; assigned659 includes four unrelated trailing functions.
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTCTARGETED_PASS22_TARGETABLE
#define FABLE_RETAIL_CDRAINLIFEDEF_PASS22
#define FABLE_RETAIL_CTCDRAINLIFE_PASS22
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_PASS22_SHARED_NODE
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCDrainLife.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/C2DBoxI.h"
#include "engine/ThingAlive.h"
void CTCDrainLife::FrameUpdate()
{
    CThingCreatureBase& parent_creature=static_cast<CThingCreatureBase&>(GetParentThing());
    if(JustReleased) {
        JustReleased=false;
        const CTCTargetingBase* ptc_targeting;
        if(parent_creature.PeekTC(&ptc_targeting)) {
            float search_radius=PDef->SpellRange[SpellLevel];
            CThing* ptarget=ptc_targeting->GetPTargetedThing();
            if(IS_THING_ALIVE(ptarget)&&(ptarget->GetTypeFlags()&8)) {
                DoDrain(static_cast<CThingCreatureBase*>(ptarget));
                C3DVector targetpos=ptarget->GetPos();
                C2DBoxI search_area(targetpos.ToC2DCoordI(),(long)search_radius);
                std::list<CThingCreatureBase*> thingstosuck;
                PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(search_area,CThingFilter_In2DRange(targetpos,search_radius),thingstosuck);
                for(std::list<CThingCreatureBase*>::iterator it=thingstosuck.begin(),endit=thingstosuck.end();it!=endit;++it) {
                    CThingCreatureBase* something=*it;
                    if(something==&parent_creature||something==ptarget)
                        continue;
                    if(!CTCEnemy::AreThingsEnemies(parent_creature,*something))
                        continue;
                    const CTCTargeted* ptc_targeted;
                    if(something->PeekTC(&ptc_targeted)&&!ptc_targeted->IsTargetable())
                        continue;
                    if(IS_THING_ALIVE(something))
                        DoDrain(something);
                }
            }
        }
    } else {
        --SpellDelayFrames;
        bool all_dead=true;
        for(long i=0;i<(long)Shots.size();++i) {
            if(IS_THING_ALIVE(Shots[i])) {
                all_dead=false;
                break;
            }
        }
        if(all_dead||SpellDelayFrames<=0)
            parent_creature.RemoveTC(TCI_DRAIN_LIFE);
    }
}