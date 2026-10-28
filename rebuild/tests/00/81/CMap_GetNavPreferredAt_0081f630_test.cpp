#include <stdio.h>
#include <string.h>
class C2DCoordI {public:long X,Y;};
class CHeightMapCell {public:float Height;unsigned char EngineThemePaletteIndex[3];unsigned char EngineBlend[2];unsigned char Changed:1;unsigned char Passable:1;unsigned char ShorePoint:1;unsigned char CameraPassable:1;unsigned char NavPreferred:1;};
class CHeightMap {public:unsigned char _unmodeled00[0x10];CHeightMapCell* CellsBegin;CHeightMapCell* CellsEnd;CHeightMapCell* CellsCap;unsigned char _unmodeled1c[0x204-0x1c];long SizeX,SizeY;};
class CMapFields {public:unsigned char _unmodeled00[0x1a7c];CHeightMap* HeightMap_ptr;};
class CMap:public CMapFields {public:bool GetNavPreferredAt(const C2DCoordI&)const;};
bool CMap::GetNavPreferredAt(const C2DCoordI& pos)const{CHeightMap* map=HeightMap_ptr;return map->CellsBegin[(map->SizeY-pos.Y)*(map->SizeX+1)+pos.X].NavPreferred;}
int main(){CHeightMapCell cells[9];memset(cells,0,sizeof(cells));CHeightMap grid;grid.CellsBegin=cells;grid.SizeX=2;grid.SizeY=2;CMap map;map.HeightMap_ptr=&grid;C2DCoordI pos;pos.X=1;pos.Y=0;cells[7].NavPreferred=true;if(!map.GetNavPreferredAt(pos))goto fail;pos.Y=2;if(map.GetNavPreferredAt(pos))goto fail;cells[1].NavPreferred=true;if(!map.GetNavPreferredAt(pos))goto fail;cells[1].NavPreferred=false;cells[1].CameraPassable=true;cells[1].ShorePoint=true;if(map.GetNavPreferredAt(pos))goto fail;puts("0081f630_TEST PASS");return 0;fail:puts("0081f630_TEST FAIL");return 1;}