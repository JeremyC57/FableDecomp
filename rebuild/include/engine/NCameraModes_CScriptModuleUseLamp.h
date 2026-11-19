#ifndef FABLE_ENGINE_NCAMERAMODES_CSCRIPTMODULEUSELAMP_H
#define FABLE_ENGINE_NCAMERAMODES_CSCRIPTMODULEUSELAMP_H
// NCameraModes::CScriptModuleUseLamp (hand4a w24, new header; row 0x008539C0 OnInitLeaf): Ego_d.pdb LF_CLASS 0x3C150 (field list
// 0x3C14F, sizeof 160), the CScriptModuleOpenChest layout with PLamp in PChest's place: PCreature +0x54, PLamp +0x5C, Origin +0x64,
// Theta +0x70, Rad +0x74, Height +0x78, DeltaTheta +0x7C, DeltaRad +0x80, DeltaHeight +0x84, AxisX +0x88, AxisY +0x94.
#include "engine/NCameraModes_CScriptModuleBase.h"
namespace NCameraModes {
class CScriptModuleUseLamp : public CScriptModuleSelfTerminateBase {
public:
    virtual void Update();
    virtual bool IsShortenOnOcclusion() const;
protected:
    virtual void OnInitLeaf();
private:
    CIntelligentPointer<const CThing> PCreature;                  // +0x54
    CIntelligentPointer<const CThing> PLamp;                      // +0x5C
    C3DVector Origin;                                             // +0x64
    float     Theta;                                              // +0x70
    float     Rad;                                                // +0x74
    float     Height;                                             // +0x78
    float     DeltaTheta;                                         // +0x7C
    float     DeltaRad;                                           // +0x80
    float     DeltaHeight;                                        // +0x84
    C3DVector AxisX;                                              // +0x88
    C3DVector AxisY;                                              // +0x94
    friend struct CScriptModuleUseLampLayout;
};
struct CScriptModuleUseLampLayout {
    FABLE_STATIC_ASSERT(sizeof(CScriptModuleUseLamp) == 0xa0);
};
}
#endif
