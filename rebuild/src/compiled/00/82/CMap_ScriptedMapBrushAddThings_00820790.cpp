// CMap::ScriptedMapBrushAddThings(const CScriptedMapBrush&) -- mark listed existing things activated.
// Native00820790 length161. Original PDB map/brush/thing method identities; typed native world/worldmap PMFs, no synthetic virtual padding.
#define CMap CMapDonor
#define CThing CThingDonor
#define CWorld CWorldDonor
#define CWorldMap CWorldMapDonor
#define CScriptedMapBrush CScriptedMapBrushDonor
#include "engine/_quarantine/CMap.h"
#include "engine/_quarantine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CScriptedMapBrush.h"
#undef CScriptedMapBrush
#undef CWorldMap
#undef CWorld
#undef CThing
#undef CMap
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
typedef unsigned long long ThingUID;
#else
typedef unsigned __int64 ThingUID;
#endif
struct UIDNode {UIDNode*Next;UIDNode*Previous;ThingUID UID;};
struct UIDList {
 UIDNode*Head;
 UIDList():Head(0){UIDNode* h=static_cast<UIDNode*>(malloc(sizeof(UIDNode)));h->Next=h;h->Previous=h;Head=h;}
 void Clear(){UIDNode* p=Head->Next;while(p!=Head){UIDNode* old=p;p=p->Next;free(old);}Head->Next=Head;Head->Previous=Head;}
 ~UIDList(){Clear();free(Head);}
};
class CScriptedMapBrush { public:
 void GetThingsToAdd(UIDList&)const; //0082E9C0 PDB007355A0
 void GetThingsToDelete(UIDList&)const; //0082E910 PDB00735500
};
class CThing { public:
 unsigned char NativeThingPrefix[0x91];unsigned char Dead:1;
 void SetAsActivated(bool); //004C7770
 void Kill(bool); //004C9B80
};
class CThingManager { public:CThing* GetPThingWithUID(ThingUID);}; //0051ED80 actual byvalue64bit lookup; PDB00529000
class C2DCoordI { public:long X,Y;};
class CWorldMap { public:
 void* VTable;typedef long(CWorldMap::*MapNumberMethod)(const C2DCoordI&)const;
 __forceinline long GetMapNumberAt(const C2DCoordI& p)const{return(this->*static_cast<MapNumberMethod*>(VTable)[17])(p);}
};
class CWorld { public:
 void* VTable;
 typedef CThingManager&(CWorld::*ThingManagerMethod)();
 typedef const CWorldMap&(CWorld::*WorldMapMethod)()const;
 __forceinline CThingManager& GetThingManager(){return(this->*static_cast<ThingManagerMethod*>(VTable)[3])();}
 __forceinline const CWorldMap& PeekWorldMap()const{return(this->*static_cast<WorldMapMethod*>(VTable)[13])();}
};
class CMap { public:
 unsigned char NativeMapPrefix[0x20];CWorld* World;
 unsigned char NativeRippleAndMapState[0x1d24-0x20-sizeof(CWorld*)];C2DCoordI WorldPos;
 void ScriptedMapBrushAddThings(const CScriptedMapBrush&);
 void ScriptedMapBrushRemoveThings(const CScriptedMapBrush&);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(UIDNode)==16);
FABLE_STATIC_ASSERT(sizeof(CWorld::ThingManagerMethod)==4);
FABLE_STATIC_ASSERT(sizeof(CWorld::WorldMapMethod)==4);
FABLE_STATIC_ASSERT(sizeof(CWorldMap::MapNumberMethod)==4);
FABLE_STATIC_ASSERT(offsetof(CMap,World)==0x20);
FABLE_STATIC_ASSERT(offsetof(CMap,WorldPos)==0x1d24);
#endif
void CMap::ScriptedMapBrushAddThings(const CScriptedMapBrush& brush){
 UIDList things;brush.GetThingsToAdd(things);
 CThingManager& manager=World->GetThingManager();
 for(UIDNode* p=things.Head->Next;p!=things.Head;p=p->Next){CThing* thing=manager.GetPThingWithUID(p->UID);if(thing)thing->SetAsActivated(true);}
}