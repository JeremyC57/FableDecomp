#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEBASE_H
struct CEngineInternalPrimitiveBase { unsigned long RenderLayerMask; CEngineInternalPrimitiveBase* NextLayerMask; };
#endif
// CEngineSceneGrid::GetCellRenderMask(unsigned long, unsigned long) -- cached or recursively combined primitive render-layer mask; retail 0x00B91DC0 length 0xb0. Calls itself for the four child cells. Uses the shared primitive-base layout (RenderLayerMask +0x30, NextLayerMask +0x40) and the PDB cell/grid layouts reconstructed in landed scene-grid relatives because no scene-grid header exists.
#include "engine/CEngineInternalPrimitiveBase.h"
struct CEngineSceneGridCell {
    CEngineSceneGridCell* Parent;
    CEngineInternalPrimitiveBase* FirstPrimitive;
    unsigned char _pad[0x2c];
    unsigned long LocalCount, TotalCount, RenderMask, OccludedLayers;
    unsigned char BoundingBoxValid:1, TotalCountValid:1, RenderMaskValid:1, WasRendered:1, HasOcclusionResults:1;
    unsigned char OccludedFrames, PreloadFrames;
};
template<class T> struct CArray { T* m_pData; long m_size, m_capacity; };
struct CEngineSceneGrid {
    unsigned long _vfptr_or_pad, Level;
    CEngineSceneGrid *LevelUp, *LevelDown;
    unsigned long Size;
    CArray<CEngineSceneGridCell*> GridCells;
    unsigned long GetCellRenderMask(unsigned long x, unsigned long y);
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

int main() {
    CEngineSceneGridCell p0={},p1={},p2={},p3={},rootCell={}; CEngineSceneGrid parent, child;
    CEngineInternalPrimitiveBase a,b;
    CEngineSceneGridCell *parentCells[1]={&rootCell}; CEngineSceneGridCell *childCells[4]={&p0,&p1,&p2,&p3};
    parent.Size=1; parent.LevelDown=&child; parent.GridCells.m_pData=parentCells;
    child.Size=2; child.LevelDown=0; child.GridCells.m_pData=childCells;
    a.RenderLayerMask=0x04; b.RenderLayerMask=0x20; a.NextLayerMask=&b; b.NextLayerMask=0;
    rootCell.FirstPrimitive=&a; rootCell.RenderMaskValid=0; rootCell.RenderMask=0;
    p0.FirstPrimitive=0; p0.RenderMaskValid=0; p0.RenderMask=0;
    p1.FirstPrimitive=0; p1.RenderMaskValid=0; p1.RenderMask=0;
    p2.FirstPrimitive=0; p2.RenderMaskValid=0; p2.RenderMask=0;
    p3.FirstPrimitive=0; p3.RenderMaskValid=0; p3.RenderMask=0;
    // Child leaves have masks from local primitive chains; root ORs all four children plus its own chain.
    CEngineInternalPrimitiveBase c0,c3; c0.RenderLayerMask=0x100; c0.NextLayerMask=0; c3.RenderLayerMask=0x800; c3.NextLayerMask=0;
    p0.FirstPrimitive=&c0; p3.FirstPrimitive=&c3;
    rootCell.BoundingBoxValid=1; rootCell.WasRendered=1; rootCell.HasOcclusionResults=1;
    unsigned long got=parent.GetCellRenderMask(0,0);
    if (!rootCell.BoundingBoxValid || !rootCell.WasRendered || !rootCell.HasOcclusionResults || got != 0x924 || rootCell.RenderMask != got || !rootCell.RenderMaskValid || p0.RenderMask!=0x100 || p3.RenderMask!=0x800) { std::printf("00b91dc0_TEST FAIL\n"); return 1; }
    // Cached queries must return the stored value even after underlying chains change.
    rootCell.FirstPrimitive=0; a.RenderLayerMask=0; if(parent.GetCellRenderMask(0,0)!=0x924) { std::printf("00b91dc0_TEST FAIL\n"); return 1; }
    // A null cell returns zero without dereferencing or caching.
    parentCells[0]=0; if(parent.GetCellRenderMask(0,0)!=0) { std::printf("00b91dc0_TEST FAIL\n"); return 1; }
    CEngineSceneGridCell offsetRoot={}, highBitLeaf={};
    CEngineSceneGridCell* offsetParent[6]={0,0,0,0,0,&offsetRoot};
    CEngineSceneGridCell* offsetChild[36]={0};
    CEngineSceneGrid upper={}, lower={};
    upper.Size=3; upper.LevelDown=&lower; upper.GridCells.m_pData=offsetParent;
    lower.Size=6; lower.GridCells.m_pData=offsetChild;
    CEngineInternalPrimitiveBase high={}; high.RenderLayerMask=0x80000000UL;
    highBitLeaf.FirstPrimitive=&high;
    offsetChild[3*6+5]=&highBitLeaf; // child (5,3) belongs to parent (2,1).
    if(upper.GetCellRenderMask(2,1)!=0x80000000UL || !offsetRoot.RenderMaskValid || !highBitLeaf.RenderMaskValid) return 2;
    high.RenderLayerMask=0;
    if(upper.GetCellRenderMask(2,1)!=0x80000000UL) return 3;
    std::printf("00b91dc0_TEST PASS\n"); return 0;
}