// CMap::IsPassableAt(const C2DCoordI& pos) const -- query the named height-map cell bit using top-down rows.
// Leaf: ?IsPassableAt@CMap@@QBE_NABVC2DCoordI@@@Z
// Identity: proven; CWorldMap::IsPosPassable004FCBD0 and camera wrapper004FCC40 call this getter; actual bit1 and both PDB API.
// Layout: unchanged quarantined CMap header reused through method-bearing alias; actual HeightMap_ptr+0x1A7C confirmed by callers. CHeightMap has vector12 rather than donor16: SizeX/SizeY at0x204/0x208. CHeightMapCell header collapses all five bitfields into NavPreferred[3], so actual PDB bitfields used locally; no CHeightMap or C2DCoordI headers.
#define CMap CMapFields
#include "engine/_quarantine/CMap.h"
#undef CMap
class C2DCoordI {public:long X,Y;};
class CHeightMapCell {public:float Height;unsigned char EngineThemePaletteIndex[3];unsigned char EngineBlend[2];unsigned char Changed:1;unsigned char Passable:1;unsigned char ShorePoint:1;unsigned char CameraPassable:1;unsigned char NavPreferred:1;};
class CHeightMap {public:unsigned char _unmodeled00[0x10];CHeightMapCell* CellsBegin;CHeightMapCell* CellsEnd;CHeightMapCell* CellsCap;unsigned char _unmodeled1c[0x204-0x1c];long SizeX,SizeY;};
class CMap:public CMapFields {public:bool IsPassableAt(const C2DCoordI&)const;};
bool CMap::IsPassableAt(const C2DCoordI& pos)const{CHeightMap* map=HeightMap_ptr;return map->CellsBegin[(map->SizeY-pos.Y)*(map->SizeX+1)+pos.X].Passable;}