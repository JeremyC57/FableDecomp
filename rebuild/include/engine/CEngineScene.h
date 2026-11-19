#ifndef FABLE_ENGINE_CENGINESCENE_H
#define FABLE_ENGINE_CENGINESCENE_H
// Original class, both PDBs and native RTTI012A3E68: CBaseClassNonCopyable.
// Native constructor00B94A80 confirms 12-byte maps and fields below; donor maps are16.
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"
#include "engine/CEnginePrimitiveEnums.h"
class CEngineInternalPrimitiveBase;
#ifdef FABLE_RETAIL_CENGINESCENE_GETMAPAT
// Opt-in (hand3c, row 0x00B94BF0 CEngineScene::GetMapAt): MapList typed as Ego_r.pdb's map<long, map<long, CCountedPointer<CEngineSceneMap> > >
// (12 bytes in retail STLport: the landed outer / inner map rows 0x00B98850 / 0x00B981F0, and the out-of-line subscripts 0x00B988E0 /
// 0x00B97C10 that GetMapAt calls) and the public ?GetMapAt@CEngineScene@@QAEAAVCEngineSceneMap@@ABVC2DVector@@@Z (Ego_r + FableWin).
// No layout change.
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
class CEngineSceneMap;
class C2DVector;
#endif
class CEngineScene : public CBaseClassNonCopyable {
public:
    void RenderEntireScene(EEngineRenderLayer layer);
    void InvalidateBoundingBoxesAndDrawDistances(); // Ego_r00A46C40/FableWin02E21B80; native00B93500
#ifdef FABLE_RETAIL_CENGINESCENE_PRELOADPRIMITIVES
    // hand3c opt-in (row 0x00B328C0 CEnginePrimitiveRenderer::LoadNewScene): Ego_r.pdb / FableWin.pdb ?PreloadPrimitives@CEngineScene@@QAE_N...K@Z
    // overloads (camera / bounding sphere / shadow clip volume + position), out of line in retail. Declarations only: no layout change.
    bool PreloadPrimitives(const class CEngineCamera& camera, unsigned long flags);                                         // 0x00B92F80
    bool PreloadPrimitives(const class C3DBoundingSphere& sphere, unsigned long flags);                                     // 0x00B92E50
    bool PreloadPrimitives(const class CShadowClipVolume& clip_volume, const class C3DVector& position, unsigned long flags); // 0x00B930A0
#endif
#ifdef FABLE_RETAIL_CENGINESCENE_GETMAXAVERAGEPRIMITIVESPERCELL
    // hand2c opt-in (row 0x00B947E0 CEngineSceneMap::Update): FableWin.pdb ?GetMaxAveragePrimitivesPerCell@CEngineScene@@QBEMXZ
    // (0x02E1C840; Ego_d.pdb CEngineScene field list: public, const, float), inline in retail (0x00B9480A `fdiv [ecx+54h]`).
    float GetMaxAveragePrimitivesPerCell() const { return MaxAveragePrimitivesPerCell; }
#endif
#ifdef FABLE_RETAIL_CENGINESCENE_GETMAPAT
    CEngineSceneMap& GetMapAt(const C2DVector& world_pos);                    // 0x00B94BF0
private:
    std::map<long, std::map<long, CCountedPointer<CEngineSceneMap> > > MapList;   // +04
#else
private:
    unsigned char MapList[12]; // +04 nested map<long,map<long,CCountedPointer<CEngineSceneMap> > >
#endif
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
FABLE_STATIC_ASSERT_AT(26, sizeof(CEngineScene)==0x5c);
#endif
