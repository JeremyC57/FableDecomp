#ifndef FABLE_ENGINE_CENGINELANDSCAPEMESHBUILDER_H
#define FABLE_ENGINE_CENGINELANDSCAPEMESHBUILDER_H
// Hand-written (chunk 723): CEngineLandscapeMeshBuilder : CBaseClassNonCopyable : CBaseClass (RTTI). Retail size 0x488 (vptr +
// CIndexBuffer* SharedIndexBuffers[17][17]; the Ego_r.pdb tail past +0x488 is absent in TLC: CEngineLandscapeRenderer::LODZBandsZ
// follows at +0x498 = MeshBuilder +0x10 + 0x488). Out-of-line virtual destructor 0x00BE2B20 (Ego_r.pdb ??1CEngineLandscapeMeshBuilder@@UAE@XZ).
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
class CIndexBuffer;
class CEngineLandscapeMeshBuilder : public CBaseClassNonCopyable {
public:
    virtual ~CEngineLandscapeMeshBuilder();                              // 0x00BE2B20
    CIndexBuffer* SharedIndexBuffers[17][17];                            // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapeMeshBuilder) == 0x488);

#endif // FABLE_ENGINE_CENGINELANDSCAPEMESHBUILDER_H
