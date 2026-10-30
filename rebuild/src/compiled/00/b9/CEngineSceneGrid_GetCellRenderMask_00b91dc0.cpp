// CEngineSceneGrid::GetCellRenderMask(unsigned long, unsigned long) -- cached or recursively combined primitive render-layer mask; retail 0x00B91DC0 length 0xb0. Calls itself for the four child cells. Uses the shared primitive-base layout (RenderLayerMask +0x30, NextLayerMask +0x40) and the PDB cell/grid layouts reconstructed in landed scene-grid relatives because no scene-grid header exists.
#include "engine/CEngineInternalPrimitiveBase.h"
class CEngineSceneGridCell { public:
    CEngineSceneGridCell* Parent;
    CEngineInternalPrimitiveBase* FirstPrimitive;
    unsigned char _pad[0x2c];
    unsigned long LocalCount, TotalCount, RenderMask, OccludedLayers;
    unsigned char BoundingBoxValid:1, TotalCountValid:1, RenderMaskValid:1, WasRendered:1, HasOcclusionResults:1;
    unsigned char OccludedFrames, PreloadFrames;
};
template<class T> class CArray { public: T* m_pData; long m_size, m_capacity; };
class CEngineSceneGrid { public:
    unsigned long _vfptr_or_pad, Level;
    CEngineSceneGrid *LevelUp, *LevelDown;
    unsigned long Size;
    CArray<CEngineSceneGridCell*> GridCells;
private:
    unsigned long GetCellRenderMask(unsigned long x, unsigned long y);   // ?GetCellRenderMask@CEngineSceneGrid@@AAEKKK@Z (Ego_r: private)
};
unsigned long CEngineSceneGrid::GetCellRenderMask(unsigned long x, unsigned long y)
{
    CEngineSceneGridCell* cell = GridCells.m_pData[y * Size + x];
    if (!cell) return 0;
    if (!cell->RenderMaskValid) {
        unsigned long mask = 0;
        if (LevelDown) {
            for (unsigned long i=0; i<2; ++i)
                for (unsigned long j=0; j<2; ++j)
                    mask |= LevelDown->GetCellRenderMask(x*2+j,y*2+i);
        }
        for (CEngineInternalPrimitiveBase* primitive=cell->FirstPrimitive; primitive; primitive=primitive->NextLayerMask)
            mask |= primitive->RenderLayerMask;
        cell->RenderMask = mask;
        cell->RenderMaskValid = 1;
    }
    return cell->RenderMask;
}