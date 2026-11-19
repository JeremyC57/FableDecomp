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
class C2DVector;
class CEngineSceneGrid {
    friend class CEngineSceneMap;
private:
    unsigned long GetCellTotalPrimitiveCount(unsigned long x, unsigned long y);                 // 0x00B91D20 ?GetCellTotalPrimitiveCount@CEngineSceneGrid@@AAEKKK@Z
};
class CEngineSceneMap {
public:
    virtual ~CEngineSceneMap();
    void Update();                                                                               // 0x00B947E0 ?Update@CEngineSceneMap@@QAEXXZ
    unsigned long GetCurrentLevels() const { return GridLevels.size(); }                        // FableWin ?GetCurrentLevels@CEngineSceneMap@@QBEKXZ (inline)
private:
    void ResizeLevels(long new_levels);                                                          // 0x00B94540 ?ResizeLevels@CEngineSceneMap@@AAEXJ@Z
    C2DBoxF Area;                                                                                // +0x04
    unsigned long MaxLevels;                                                                     // +0x14
    CArray<CCountedPointer<CEngineSceneGrid> > GridLevels;                                       // +0x18
    CEngineScene* Scene;                                                                         // +0x24
};
FABLE_STATIC_ASSERT(sizeof(CEngineSceneMap)==0x28);
#endif
