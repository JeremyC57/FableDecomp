#ifndef FABLE_ENGINE_CENGINESCENE_H
#define FABLE_ENGINE_CENGINESCENE_H
// Original class, both PDBs and native RTTI012A3E68: CBaseClassNonCopyable.
// Native constructor00B94A80 confirms 12-byte maps and fields below; donor maps are16.
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"
#include "engine/CEnginePrimitiveEnums.h"
class CEngineInternalPrimitiveBase;
class CEngineScene : public CBaseClassNonCopyable {
public:
    void RenderEntireScene(EEngineRenderLayer layer);
    void InvalidateBoundingBoxesAndDrawDistances(); // Ego_r00A46C40/FableWin02E21B80; native00B93500
private:
    unsigned char MapList[12]; // +04 nested map<long,map<long,CCountedPointer<CEngineSceneMap> > >
    float MapSize, OneOverMapSize; // +10,+14
    unsigned long MaxMapLevels, MapsRenderLayerMask; // +18,+1C
    CEngineInternalPrimitiveBase* FirstMovingPrimitive; // +20
    CEngineInternalPrimitiveBase* FirstNonPersistentPrimitive; // +24
    CCharString Name; // +28
    unsigned char RenderStats[12]; // +2C map<EEngineRenderLayer,CEngineSceneRenderStats>
    unsigned char UpdateStats[12]; // +38 CEngineSceneUpdateStats
    unsigned long CurrentOcclusionMask, OcclusionGroupDelay, OcclusionMinGroupSize; // +44,+48,+4C
    float MaxPrimitiveCellSizeRatio, MaxAveragePrimitivesPerCell; // +50,+54
    unsigned long PreloadFramesMoving; // +58
};
FABLE_STATIC_ASSERT(sizeof(CEngineScene)==0x5c);
#endif
