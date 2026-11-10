#ifndef FABLE_ENGINE_CHEIGHTMAP_H
#define FABLE_ENGINE_CHEIGHTMAP_H
#ifdef FABLE_RETAIL_CHEIGHTMAP
// New header (chunk 1017, row 0x0081EA50 CMap::GetGroundSizeZAt): the retail CHeightMap. Ego_r.pdb (struct_layouts_egor.tsv) member
// order and names: the vptr (virtual ??1CHeightMap@@UAE@XZ), World +0x04, Map +0x08, WorldSeed +0x0C, Cells vector<CHeightMapCell>
// +0x10 (STLport, 12 bytes: retail offsets are the PDB's minus 4 from here on), MapUpdateAreas C2DBoxI[30] +0x1C, NoMapUpdateAreas
// +0x1FC, Changed +0x200, SizeX +0x204, SizeY +0x208, SizeXIncludingExtraCells +0x20C, SizeYIncludingExtraCells +0x210 (retail
// GetGroundSizeZAt 0x0081EA50 reads +0x10 / +0x204 / +0x208 / +0x20C / +0x210; the landed CHeightMap rows 0x008F5F00..0x008F63C0
// use the same offsets). sizeof 0x214. Every member but the destructor is private in the PDB (?GetSizeXIncludingExtraCells@CHeightMap
// @@ABEJXZ etc.), so CMap is a friend. Only what rows use is declared.
#include "stlport/fable_containers.h"
#include "engine/C2DBoxI.h"
#include "engine/CHeightMapCell.h"
class CWorld;class CMap;
class CHeightMap {
    friend class CMap;
public:
    virtual ~CHeightMap();
private:
    long GetSizeX() const { return SizeX; }                                          // ?GetSizeX@CHeightMap@@ABEJXZ (inline)
    long GetSizeY() const { return SizeY; }                                          // ?GetSizeY@CHeightMap@@ABEJXZ (inline)
    long GetSizeXIncludingExtraCells() const { return SizeXIncludingExtraCells; }    // ?GetSizeXIncludingExtraCells@CHeightMap@@ABEJXZ
    long GetSizeYIncludingExtraCells() const { return SizeYIncludingExtraCells; }    // ?GetSizeYIncludingExtraCells@CHeightMap@@ABEJXZ
    CWorld* World;                                                                    // +0x04
    CMap* Map;                                                                        // +0x08
    unsigned long* WorldSeed;                                                         // +0x0C
    std::vector<CHeightMapCell> Cells;                                                // +0x10
    C2DBoxI MapUpdateAreas[30];                                                       // +0x1C
    long NoMapUpdateAreas;                                                            // +0x1FC
    bool Changed;                                                                     // +0x200
    long SizeX;                                                                       // +0x204
    long SizeY;                                                                       // +0x208
    long SizeXIncludingExtraCells;                                                    // +0x20C
    long SizeYIncludingExtraCells;                                                    // +0x210
};
FABLE_STATIC_ASSERT(sizeof(CHeightMap)==0x214);
#endif
#endif // FABLE_ENGINE_CHEIGHTMAP_H
