// CHeightMap::AlterHeightBy(long,long,float) -- clamp and update an in-range height cell.
// Retail 008f60c0, 165 bytes. SetSizeZAt 00825440. PDB CHeightMap has
// a donor 16-byte vector; retail uses 12 bytes, shifting later members by four.
// CHeightMap has only its virtual destructor (retail vtable 01288bec slot0).
#include "engine/CHeightMapCell.h"
struct CWorld; struct CMap;
struct C2DCoordI { int X,Y; C2DCoordI(int x,int y):X(x),Y(y){} };
struct C2DBoxI { C2DCoordI Min,Max; };
struct CHeightMap {
 virtual ~CHeightMap(); // 008f6de0
 CWorld* World; CMap* Map; unsigned int* WorldSeed;
 CHeightMapCell *CellsStart,*CellsFinish,*CellsEnd;
 C2DBoxI MapUpdateAreas[30]; int NoMapUpdateAreas; bool Changed;
 int SizeX,SizeY,SizeXIncludingExtraCells,SizeYIncludingExtraCells;
 void SetSizeZAt(const C2DCoordI&,float); // 00825440
 void AlterHeightBy(int x,int y,float amount);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CHeightMap,SizeX)==0x204);
FABLE_STATIC_ASSERT(offsetof(CHeightMap,SizeY)==0x208);
#endif
template<class T> inline const T& HeightMin(const T&a,const T&b){return a<b?a:b;}
template<class T> inline const T& HeightMax(const T&a,const T&b){return a<b?b:a;}
void CHeightMap::AlterHeightBy(int x,int y,float amount) {
 if(x>=0 && y>=0 && x<SizeX && y<SizeY) {
  float height=CellsStart[(SizeY-y)*(SizeX+1)+x].Height+amount;
  SetSizeZAt(C2DCoordI(x,y),HeightMax(0.0f,HeightMin(2047.0f,height)));
 }
}