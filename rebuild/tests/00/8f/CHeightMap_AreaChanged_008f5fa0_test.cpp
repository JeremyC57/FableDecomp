#include <cstdio>
#include <cstring>
// CHeightMap::AreaChanged(const C2DBoxI&) -- queue a valid rectangle and mark every covered cell dirty; retail 0x008f5fa0 length 0xa5. The body copies MapUpdateAreas[30] at +0x1c, increments NoMapUpdateAreas at +0x1fc, then marks the cell Changed bit over the requested bounds. No CHeightMap header exists; the member offsets follow the landed CHeightMap::Update at 0x008f5f00, and CHeightMapCell uses the shared engine header.
#ifdef _MSC_VER
// Grade: MATCH -- q.py confirms the VC7.1 body; behavior passes under g++ and VC7.1.
#include "engine/CHeightMapCell.h"
#else
struct CHeightMapCell { float Height; unsigned char EngineThemePaletteIndex[3]; unsigned char EngineBlend[2]; unsigned char NavPreferred[3]; };
#endif
struct C2DBoxI { long MinX, MinY, MaxX, MaxY; };
struct CHeightMapCellVector {
    CHeightMapCell *first, *last, *end;
    CHeightMapCell& operator[](long i) { return *(first + i); }
};
struct CHeightMap {
    void *World;
    void *_04;
    void *Map;
    unsigned long *WorldSeed;
    CHeightMapCellVector Cells;
    C2DBoxI MapUpdateAreas[30];
    long NoMapUpdateAreas;
    bool Changed;
    long SizeX;
    long SizeY;
    void AreaChanged(C2DBoxI const&);
};
void CHeightMap::AreaChanged(C2DBoxI const& area)
{
    if (area.MinX >= area.MaxX || area.MinY >= area.MaxY || NoMapUpdateAreas >= 30)
        return;
    MapUpdateAreas[NoMapUpdateAreas] = area;
    ++NoMapUpdateAreas;
    for (long y = area.MinY; y < area.MaxY; ++y) {
        CHeightMapCell *cell = &Cells[(SizeY - y) * (SizeX + 1) + area.MinX];
        for (long x = area.MinX; x < area.MaxX; ++x, ++cell)
            reinterpret_cast<unsigned char *>(cell)[9] |= 1;
    }
}

static bool unchanged(CHeightMap& map,CHeightMapCell* cells,C2DBoxI box) {
 CHeightMap before=map;
 CHeightMapCell saved[25]; std::memcpy(saved,cells,sizeof(saved));
 map.AreaChanged(box);
 return std::memcmp(&map,&before,sizeof(map))==0 && std::memcmp(cells,saved,sizeof(saved))==0;
}
int main() {
 CHeightMap map={}; CHeightMapCell cells[25]; std::memset(cells,0xa4,sizeof(cells));
 map.Cells.first=cells;map.SizeX=4;map.SizeY=4;
 C2DBoxI area={1,1,3,3};map.AreaChanged(area);
 if(map.NoMapUpdateAreas!=1 || map.Changed || std::memcmp(&map.MapUpdateAreas[0],&area,sizeof(area))) return 1;
 for(int i=0;i<25;++i) for(int b=0;b<12;++b) {
  bool dirty=i==11 || i==12 || i==16 || i==17;
  unsigned char expected=(dirty && b==9)?0xa5:0xa4;
  if(((unsigned char*)cells)[i*12+b]!=expected) return 2;
 }
 C2DBoxI badX={3,1,1,3},badY={1,3,3,1},emptyX={1,1,1,3},emptyY={1,1,3,1};
 if(!unchanged(map,cells,badX)||!unchanged(map,cells,badY)||!unchanged(map,cells,emptyX)||!unchanged(map,cells,emptyY)) return 3;
 map.NoMapUpdateAreas=29;C2DBoxI last={0,0,1,1};map.AreaChanged(last);
 if(map.NoMapUpdateAreas!=30 || std::memcmp(&map.MapUpdateAreas[29],&last,sizeof(last)) || ((unsigned char*)cells)[20*12+9]!=0xa5) return 4;
 C2DBoxI beyond={4,4,5,5};
 if(!unchanged(map,cells,beyond)) return 5;
 map.NoMapUpdateAreas=31;if(!unchanged(map,cells,beyond)) return 6;
 std::printf("008f5fa0_TEST PASS\n");return 0;
}