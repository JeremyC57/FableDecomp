#ifndef FABLE_ENGINE_NCAMERAMODES_CSCRIPTMODULEOPENCHEST_H
#define FABLE_ENGINE_NCAMERAMODES_CSCRIPTMODULEOPENCHEST_H
// NCameraModes::CScriptModuleOpenChest (hand4a w24, new header; row 0x00853670 OnInitLeaf): Ego_d.pdb LF_CLASS 0x3C140 (field list
// 0x3C13F, sizeof 160) and Ego_r.pdb struct_layouts_egor.tsv (0xa0, the landed ctor 0x008530B0): PCreature +0x54, PChest +0x5C,
// Origin +0x64, Theta +0x70, Rad +0x74, Height +0x78, DeltaTheta +0x7C, DeltaRad +0x80, DeltaHeight +0x84, AxisX +0x88, AxisY +0x94.
#include "engine/NCameraModes_CScriptModuleBase.h"
namespace NCameraModes {
class CScriptModuleOpenChest : public CScriptModuleSelfTerminateBase {
public:
    virtual void Update();
    virtual bool IsShortenOnOcclusion() const;
protected:
    virtual void OnInitLeaf();
private:
    CIntelligentPointer<const CThing> PCreature;                  // +0x54
    CIntelligentPointer<const CThing> PChest;                     // +0x5C
    C3DVector Origin;                                             // +0x64
    float     Theta;                                              // +0x70
    float     Rad;                                                // +0x74
    float     Height;                                             // +0x78
    float     DeltaTheta;                                         // +0x7C
    float     DeltaRad;                                           // +0x80
    float     DeltaHeight;                                        // +0x84
    C3DVector AxisX;                                              // +0x88
    C3DVector AxisY;                                              // +0x94
    friend struct CScriptModuleOpenChestLayout;
};
struct CScriptModuleOpenChestLayout {
    FABLE_STATIC_ASSERT(sizeof(CScriptModuleOpenChest) == 0xa0);
};
}
#endif
