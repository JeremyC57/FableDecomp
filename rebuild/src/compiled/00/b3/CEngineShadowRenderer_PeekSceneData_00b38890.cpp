// CEngineShadowRenderer::PeekSceneData() -- Return a copy of the owned value.
// Leaf: ?PeekSceneData@CEngineShadowRenderer@@QAE?BVCShadowRendererSceneData@@XZ
// Identity: Own scene field+12, copy helper00b38600; FableWin exact const-by-value PeekSceneData, caller00b330ee.
class CShadowRendererSceneData {public:unsigned char Storage[576];CShadowRendererSceneData(const CShadowRendererSceneData&);};
#define CEngineShadowRenderer CEngineShadowRenderer_HeaderLayout
#include "engine/CEngineShadowRenderer.h"
#undef CEngineShadowRenderer
// Layout: active shared owner layout retained; opaque member viewed as its original composite type.
class CEngineShadowRenderer : public CEngineShadowRenderer_HeaderLayout {public:const CShadowRendererSceneData PeekSceneData();};
const CShadowRendererSceneData CEngineShadowRenderer::PeekSceneData() {return *reinterpret_cast<const CShadowRendererSceneData*>(&SceneData);}