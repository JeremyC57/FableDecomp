// CThingManager::InitThingTypes() -- Register the native thing categories and size their update lists.
// Leaf: ?InitThingTypes@CThingManager@@QAEXXZ
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_UI_ORDINARY_INLINE
#define FABLE_RETAIL_INIT_THING_TYPES
#define FABLE_RETAIL_CTHINGMANAGER_BINARY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "rebuild_abi.h"
#define FABLE_RETAIL_CWORLD
#include "engine/CThingManager.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingVillage.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingObject.h"
#include "engine/CThingSoundEmitter.h"
#include "engine/CThingHolySite.h"
#include "engine/CThingSwitch.h"
#include "engine/CThingTrackNode.h"
#include "engine/CThingMarker.h"
#include "engine/CThingPhysicalSwitch.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CThingNoise.h"
#include "engine/CThing.h"
// The original non-inline resize instantiation is called at00522a3d.
namespace std { template<> void vector<CThingTypeInfo>::resize(size_type,const CThingTypeInfo&); }
void CThingManager::InitThingTypes()
{
    ThingTypeInfo.resize(16);
    AddNewThingType(CThingTypeInit(2,"CREATURE",0x2f,CThingPlayerCreature::Alloc,false,"PlayerCreature"));
    AddNewThingType(CThingTypeInit(1,"CREATURE",0x1f,CThingAICreature::Alloc,false,"AICreature"));
    AddNewThingType(CThingTypeInit(4,"VILLAGE",0x407,CThingVillage::Alloc,false,"Village"));
    AddNewThingType(CThingTypeInit(3,"BUILDING",0x47,CThingBuilding::Alloc,false,"Building"));
    AddNewThingType(CThingTypeInit(5,"OBJECT",0x807,CThingObject::Alloc,false,"Object"));
    AddNewThingType(CThingTypeInit(6,"",0x1001,CThingSoundEmitter::Alloc,false,"Sound Emitter"));
    AddNewThingType(CThingTypeInit(7,"HOLY_SITE",0x2007,CThingHolySite::Alloc,false,"Holy Site"));
    AddNewThingType(CThingTypeInit(9,"SWITCH",0x20007,CThingSwitch::Alloc,false,"Switch"));
    AddNewThingType(CThingTypeInit(10,"THING",0x10007,CThingTrackNode::Alloc,false,"TrackNode"));
    AddNewThingType(CThingTypeInit(11,"MARKER",0x8007,CThingMarker::Alloc,false,"Marker"));
    AddNewThingType(CThingTypeInit(12,"PHYSICAL_SWITCH",0x40007,CThingPhysicalSwitch::Alloc,false,"Physical Switch"));
    AddNewThingType(CThingTypeInit(13,"CREATURE",0x87,CThingDeadCreature::Alloc,false,"Dead Creature"));
    AddNewThingType(CThingTypeInit(14,"NOISE",0x101,CThingNoise::Alloc,false,"Noise"));
    AddNewThingType(CThingTypeInit(15,"THING",0x0,CThing::Alloc,false,"Thing"));
    // Ego_r thing_manager.obj local data record0x50a0 proves the name and type.
    static ThingLList dummyList={0,0};
    UpdateLists.resize(ClassTypeInfo.size(),dummyList);
}
