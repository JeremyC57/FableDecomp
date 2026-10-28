// CMap::GetNavPreferredAt(const C2DCoordI& pos) const -- query the named height-map cell bit using top-down rows.
// Leaf: ?GetNavPreferredAt@CMap@@QBE_NABVC2DCoordI@@@Z
// Identity: proven; CWorldMap::GetNavPreferredAtBlock0063A760 calls this getter; matching PDB API order after GetSoundAt/SetSoundAt and before SetNavPreferredAt.
// Layout: unchanged quarantined CMap header reused through method-bearing alias; actual HeightMap_ptr+0x1A7C confirmed by callers. CHeightMap has vector12 rather than donor16: SizeX/SizeY at0x204/0x208. CHeightMapCell header collapses all five bitfields into NavPreferred[3], so actual PDB bitfields used locally; no CHeightMap or C2DCoordI headers.
#define CMap CMapFields
#include "engine/_quarantine/CMap.h"
#undef CMap
class C2DCoordI {public:long X,Y;};
class CHeightMapCell {public:float Height;unsigned char EngineThemePaletteIndex[3];unsigned char EngineBlend[2];unsigned char Changed:1;unsigned char Passable:1;unsigned char ShorePoint:1;unsigned char CameraPassable:1;unsigned char NavPreferred:1;};
class CHeightMap {public:unsigned char _unmodeled00[0x10];CHeightMapCell* CellsBegin;CHeightMapCell* CellsEnd;CHeightMapCell* CellsCap;unsigned char _unmodeled1c[0x204-0x1c];long SizeX,SizeY;};
class CMap:public CMapFields {public:bool GetNavPreferredAt(const C2DCoordI&)const;};
bool CMap::GetNavPreferredAt(const C2DCoordI& pos)const{CHeightMap* map=HeightMap_ptr;return map->CellsBegin[(map->SizeY-pos.Y)*(map->SizeX+1)+pos.X].NavPreferred;}