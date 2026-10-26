// CMap::HeightAreaChanged(C2DBoxI*) -- notify clipped height area and retain original region up to20.
// Native0x0081e810 length247. Actual quarantined donor included; retail arrays are16bytes earlier.
// CHeightMap::AreaChanged 0x008f5fa0; conversion helper _ftol2 0x00bfea70.
#include "engine/_quarantine/CMap.h"
class C2DBoxI { public: long MinX,MinY,MaxX,MaxY;
 void Set(float x,float y,float xx,float yy) {MinX=(long)x;MinY=(long)y;MaxX=(long)xx;MaxY=(long)yy;}
};
class CHeightMap { public: void AreaChanged(const C2DBoxI&); }; // 0x008f5fa0
struct RetailMap {
 unsigned char MapState[0x1a7c]; CHeightMap* HeightMap_ptr;
 C2DBoxI ChangedHeightAreas[20],ChangedGameAreas[20];
 long NoChangedHeightAreas,NoChangedGameAreas;float SeaLevel;
 long WorldSizeX,WorldSizeY,HeightMapSizeX,HeightMapSizeY;
 void HeightAreaChanged(C2DBoxI*);
};
inline long MaxLong(long a,long b) { return a<b?b:a; }
void RetailMap::HeightAreaChanged(C2DBoxI* area) {
 C2DBoxI full;
 if(!area){full.MinX=0;full.MinY=0;full.MaxX=WorldSizeX;full.MaxY=WorldSizeY;area=&full;}
 C2DBoxI clipped;
 clipped.MinX=(long)(float)MaxLong(area->MinX,0);
 clipped.MinY=(long)(float)MaxLong(area->MinY,0);
 clipped.MaxX=(long)(float)(HeightMapSizeX<area->MaxX?HeightMapSizeX:area->MaxX);
 clipped.MaxY=(long)(float)(HeightMapSizeY<area->MaxY?HeightMapSizeY:area->MaxY);
 HeightMap_ptr->AreaChanged(clipped);
 if(NoChangedHeightAreas<20) ChangedHeightAreas[NoChangedHeightAreas++]=*area;
}