#ifndef FABLE_ENGINE_CENGINEPRIMITIVETHEMECHANNELDESC_H
#define FABLE_ENGINE_CENGINEPRIMITIVETHEMECHANNELDESC_H
// CEnginePrimitiveThemeChannelDesc (chunk 723c): Ego_r.pdb class (V-keyed: ?GetLightsInSphereFast@CEngineLightingManager@@QAEPAV
// CEngineLocalLightArray@@ABVC3DBoundingSphere@@JABVCEnginePrimitiveThemeChannelDesc@@@Z), 4 bytes, no container (retail = Ego_r).
// Constructors ??0CEnginePrimitiveThemeChannelDesc@@QAE@XZ / @EEE@Z, inline in retail: RenderSingleSprite 0x00BC04F0 zero-stores the
// three channel bytes of a (0, 0, 0) temporary and leaves Pad alone.
#include "rebuild_abi.h"
class CEnginePrimitiveThemeChannelDesc {
public:
    CEnginePrimitiveThemeChannelDesc() { }
    CEnginePrimitiveThemeChannelDesc(unsigned char index0, unsigned char index1, unsigned char blendFactor)
        : ChannelIndex0(index0), ChannelIndex1(index1), ChannelBlendFactor(blendFactor) { }
    unsigned char ChannelIndex0;                                         // +0
    unsigned char ChannelIndex1;                                         // +1
    unsigned char ChannelBlendFactor;                                    // +2
    unsigned char Pad;                                                   // +3
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveThemeChannelDesc) == 4);
#endif // FABLE_ENGINE_CENGINEPRIMITIVETHEMECHANNELDESC_H
