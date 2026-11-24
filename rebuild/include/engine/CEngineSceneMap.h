#ifndef FABLE_ENGINE_CENGINESCENEMAP_H
#define FABLE_ENGINE_CENGINESCENEMAP_H
// New header (hand2c, row 0x00B947E0): Ego_d.pdb class record CEngineSceneMap (types 0x648BB, field list 0x648BA: vfptr, private Area
// C2DBoxF +4, MaxLevels +0x14, GridLevels CArray<CCountedPointer<CEngineSceneGrid> > +0x18, Scene +0x24 in retail -- Ego_r's 16-byte
// CArray shifts Scene to +0x28 there; retail Update reads [edi+18h] / [edi+1Ch] / [edi+24h], CalculateLevel 0x00B91920 [ecx+14h] /
// [ecx+24h]) with the Ego_r.pdb / FableWin.pdb method spellings. CEngineSceneGrid: Ego_r.pdb names only (opaque layout).
#include "engine/C2DBoxF.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
class CEngineScene;
class CEngineInternalPrimitiveBase;
#ifdef FABLE_RETAIL_CENGINESCENEMAP_CTOR
// Opt-in (hand3c, row 0x00B94BF0 CEngineScene::GetMapAt, which expands the constructor): RTTI .?AVCEngineSceneMap@@ : CBaseClassNonCopyable
// (ghidra_out/rtti_bases.tsv; vtable 0x012A3E60). Ego_r.pdb / FableWin.pdb ??0CEngineSceneMap@@QAE@PAVCEngineScene@@KABVC2DBoxF@@@Z
// (out of line at 0x00B94A40, landed: CBaseClassNonCopyable() 0x0099A2F0, own vtable, empty GridLevels, Initialise) and
// ?Initialise@CEngineSceneMap@@QAEXPAVCEngineScene@@KABVC2DBoxF@@@Z (0x00B949A0, landed). Same layout (the base has no data).
#include "engine/CBaseClassNonCopyable.h"
#endif
class C2DVector;
#ifdef FABLE_RETAIL_SCENEGRID_VISIBILITY_PASS27
class CEngineCamera;
enum EEngineRenderLayer;
enum EEngineOcclusionLayer;
#endif
#ifdef FABLE_RETAIL_CENGINESCENEMAP_RESIZELEVELS
// Opt-in (p25A, row 0x00B94540 CEngineSceneMap::ResizeLevels, which expands the grid constructor): the Ego_r.pdb CEngineSceneGrid layout
// at retail offsets (36 bytes in the PDB, 0x20 in retail: the 12-byte STLport CArray) -- RTTI base CBaseClassNonCopyable (vtable
// 0x012A3E58), Level +0x04, LevelUp +0x08, LevelDown +0x0C, Size +0x10, GridCells CArray<CEngineSceneGridCell*> +0x14 -- the private
// ??0CEngineSceneGrid@@AAE@K@Z (out of line at 0x00B93EB0, landed; its body below, as that row writes it on the shared CArray) and the
// private ?MovePrimitivesDown@CEngineSceneGrid@@AAEXXZ (0x00B93860) / ?MovePrimitivesUp@CEngineSceneGrid@@AAEXXZ (0x00B91F00),
// both landed. The map is the grid's friend (as in the default declaration).
#include "engine/CBaseClassNonCopyable.h"
class CEngineSceneGridCell;
#endif
#ifdef FABLE_RETAIL_CENGINESCENEMAP_ADDPRIMITIVE
// Opt-in (hand3c, row 0x00B93B30 CEngineSceneMap::AddPrimitive): Ego_r.pdb / FableWin.pdb
// ?GetCellCreate@CEngineSceneGrid@@QAEPAVCEngineSceneGridCell@@KK@Z (public, 0x00B937A0, landed) and the private
// ?AddPrimitive@CEngineSceneGrid@@AAEXKKAAVCEngineInternalPrimitiveBase@@@Z (FableWin 0x02E1A450: GetCellCreate(x, y)->AddPrimitive(
// primitive), inline in retail; the cell's private AddPrimitive needs FABLE_RETAIL_CENGINESCENEGRIDCELL_CHECKVISIBILITYRESULTS, which
// makes the grid its friend). The map side: ?CalculateLevel@CEngineSceneMap@@ABEKM@Z (private, 0x00B91920), the private
// ?AddPrimitive@CEngineSceneMap@@AAEXAAVCEngineInternalPrimitiveBase@@ABVC2DVector@@M@Z (0x00B93B30) and the public inline
// ?GetGridLevel@CEngineSceneMap@@QAEPAVCEngineSceneGrid@@K@Z (FableWin 0x02E1CE10: GridLevels.At(level).Get()). No layout change.
#include "engine/CEngineSceneGridCell.h"
#endif
#ifdef FABLE_RETAIL_CENGINESCENEMAP_RESIZELEVELS
class CEngineSceneGrid : public CBaseClassNonCopyable {
    friend class CEngineSceneMap;
private:
    CEngineSceneGrid(unsigned long level) : Level(level), LevelUp(0), LevelDown(0), GridCells()
    {
        Size = 1 << level;
        GridCells.resize(Size * Size, 0);
    }                                                                                            // 0x00B93EB0
    void MovePrimitivesDown();                                                                   // 0x00B93860
    void MovePrimitivesUp();                                                                     // 0x00B91F00
    unsigned long Level;                                                                         // +0x04
    CEngineSceneGrid* LevelUp;                                                                   // +0x08
    CEngineSceneGrid* LevelDown;                                                                 // +0x0c
    unsigned long Size;                                                                          // +0x10
    CArray<CEngineSceneGridCell*> GridCells;                                                     // +0x14
#else
class CEngineSceneGrid {
    friend class CEngineSceneMap;
#endif
#ifdef FABLE_RETAIL_CENGINESCENEMAP_ADDPRIMITIVE
public:
    CEngineSceneGridCell* GetCellCreate(unsigned long x, unsigned long y);                     // 0x00B937A0
#endif
private:
    unsigned long GetCellTotalPrimitiveCount(unsigned long x, unsigned long y);                 // 0x00B91D20 ?GetCellTotalPrimitiveCount@CEngineSceneGrid@@AAEKKK@Z
#ifdef FABLE_RETAIL_SCENEGRID_VISIBILITY_PASS27
    // Exact donor methods and static fields used at retail 0x00B92950.
    unsigned long GetCellRenderMask(unsigned long x, unsigned long y);
    static unsigned long OcclusionGroupDelay;
    static unsigned long OcclusionMinGroupSize;
public:
    bool GetCellBoundingBoxAndDrawDistance(class C3DBoundingBox& bbox, float& max_draw_dist, unsigned long x, unsigned long y);
    CEngineSceneGridCell* GetCell(unsigned long x, unsigned long y) { return GridCells[Size * y + x]; }
    bool RenderCellClippedAndCheckVisibility(unsigned long x, unsigned long y, EEngineRenderLayer layer,
        unsigned long occlusion_mask, const CEngineCamera& frustum, bool pre_clipped,
        class CEngineSceneRenderStats& stats, EEngineOcclusionLayer occ_layer, bool occ_disable, bool& cell_visible);
private:
#endif
#ifdef FABLE_RETAIL_CENGINESCENEMAP_ADDPRIMITIVE
    void AddPrimitive(unsigned long x, unsigned long y, CEngineInternalPrimitiveBase& prim) { GetCellCreate(x, y)->AddPrimitive(prim); }
#endif
};
#ifdef FABLE_RETAIL_CENGINESCENEMAP_CTOR
class CEngineSceneMap : public CBaseClassNonCopyable {
public:
    CEngineSceneMap(CEngineScene* scene, unsigned long max_levels, const C2DBoxF& area) { Initialise(scene, max_levels, area); }
    void Initialise(CEngineScene* scene, unsigned long max_levels, const C2DBoxF& area);   // 0x00B949A0
#else
class CEngineSceneMap {
public:
#endif
    virtual ~CEngineSceneMap();
    void Update();                                                                               // 0x00B947E0 ?Update@CEngineSceneMap@@QAEXXZ
    unsigned long GetCurrentLevels() const { return GridLevels.size(); }                        // FableWin ?GetCurrentLevels@CEngineSceneMap@@QBEKXZ (inline)
#ifdef FABLE_RETAIL_CENGINESCENEMAP_ADDPRIMITIVE
    CEngineSceneGrid* GetGridLevel(unsigned long level) { return GridLevels.At(level).Get(); }  // FableWin ?GetGridLevel@CEngineSceneMap@@QAEPAVCEngineSceneGrid@@K@Z (inline)
#endif
private:
    void ResizeLevels(long new_levels);                                                          // 0x00B94540 ?ResizeLevels@CEngineSceneMap@@AAEXJ@Z
#ifdef FABLE_RETAIL_CENGINESCENEMAP_ADDPRIMITIVE
    unsigned long CalculateLevel(float size) const;                                              // 0x00B91920
    void AddPrimitive(CEngineInternalPrimitiveBase& prim, const C2DVector& world_pos, float size);   // 0x00B93B30
#endif
    C2DBoxF Area;                                                                                // +0x04
    unsigned long MaxLevels;                                                                     // +0x14
    CArray<CCountedPointer<CEngineSceneGrid> > GridLevels;                                       // +0x18
    CEngineScene* Scene;                                                                         // +0x24
};
FABLE_STATIC_ASSERT_AT(68, sizeof(CEngineSceneMap)==0x28);
#endif
