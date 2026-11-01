// 004fc760 CWorldMap::DrawGetMyPlayerMapNumber
// Leaf: ?DrawGetMyPlayerMapNumber@CWorldMap@@UBEJXZ
// Flags:
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
#define CWorldMap CWorldMapFields
#include "engine/CWorldMap.h"
#undef CWorldMap
#define CPlayer CPlayerFields
#include "engine/CPlayer.h"
#undef CPlayer
#include "engine/CBaseClassNonCopyable.h"
class CPlayer{public:long GetNumber()const{return reinterpret_cast<const CPlayerFields*>(this)->Number;}};
class CWorld{public:const CPlayer*PeekMyPlayer()const;};
class C3DVector{public:float X,Y,Z;};
class C2DCoordI;class C2DBoxI;class CDrawMapInfo;class CAFile;class CASuspendableProcess;class CMap;
template<class T>class CCountedPointer;
class CIDrawWorldMap{public:
 virtual long DrawGetNoMaps()const=0;
 virtual CDrawMapInfo DrawGetMapInfo(long)const=0;
 virtual C2DBoxI DrawGetDimensions()const=0;
 virtual long DrawGetGranularity()const=0;
 virtual bool DrawIsPosInMap(const C3DVector&)const=0;
 virtual float DrawGetSizeZAt(const C3DVector&,float)const=0;
 virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector&)const=0;
 virtual long DrawGetMyPlayerMapNumber()const=0;
 virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual long DrawGetFrame()const=0;
 virtual~CIDrawWorldMap();
};
class CWorldMap:public CBaseClassNonCopyable,public CIDrawWorldMap{public:
 virtual~CWorldMap();
 virtual void DeleteAllLevels();virtual void DeleteAllRegions();
 virtual void LoadFromFile(CAFile&,long);virtual void SaveToFile(CAFile&);virtual void RemoveLevel(long);
 virtual void LoadTopologyFromFile(long,CCountedPointer<CAFile>,CASuspendableProcess&);virtual void PostLoadTopology(long);
 virtual void ForceLoad(long);virtual void ForceUnload(long);virtual bool IsMapLoaded(long)const;virtual bool IsMapUsed(long)const;
 virtual long GetGranularity()const;virtual const C2DBoxI&GetDimensions()const;
 virtual CMap&GetMapAt(const C2DCoordI&);virtual const CMap&PeekMapAt(const C2DCoordI&)const;
 virtual long GetMapNumberAt(const C2DCoordI&)const;virtual long GetMapNumberAt(const C3DVector&)const;
public:virtual long DrawGetMyPlayerMapNumber()const;
protected:
 // Layout: original World reference8, packed position vector78 from shared header.
 // Its nested original record is13 bytes: bool Set0 and C3DVector Pos1, raw TPI0x27bac.
 // Keep record incomplete and inspect its proven representation; no new packing is introduced.
 class CPlayerPositionInfo;
};
long CWorldMap::DrawGetMyPlayerMapNumber()const{
 const CPlayer*player=reinterpret_cast<const CWorldMapFields*>(this)->World->PeekMyPlayer();
 if(player){
  long player_no=player->GetNumber();
  if(*reinterpret_cast<const bool*>((*reinterpret_cast<const unsigned char*const*>(reinterpret_cast<const CWorldMapFields*>(this)->PlayerPositionInfo))+13*player_no)){
   const C3DVector&my_player_pos=*reinterpret_cast<const C3DVector*>((*reinterpret_cast<const unsigned char*const*>(reinterpret_cast<const CWorldMapFields*>(this)->PlayerPositionInfo))+13*player_no+1);
   if(my_player_pos.X>0.0f&&my_player_pos.Y>0.0f)return GetMapNumberAt(my_player_pos);
  }
 }
 return 1;
}