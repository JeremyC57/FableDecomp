// CEngineLandscapeRenderer::OnReleaseDefaultPoolResources() -- release the default-pool surfaces of every engine map's landscape map
// Leaf: ?OnReleaseDefaultPoolResources@CEngineLandscapeRenderer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B676A0, 69 bytes, plain `ret` (virtual: the CEngineComponent override).
// NF46 (round 7): ported off candidates/landscape_renderer_release_compiled.h (its DAT_01436e8c / CEngineLandscapeMapList /
//   CEngineLandscapeMapEntry stand-ins). The 0x01436E8C global is NEngineInternal::GPEngineWorldMap (Ego_r.pdb / FableWin.pdb
//   ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A); the +0x20 / +0x24 begin / end pair is its MapList (CArray<CEngineMap*>,
//   FableWin.pdb inline GetMapCount / GetMap, engine/CEngineWorldMap.h) and each entry's +0 the CEngineMap LandscapeMap (FableWin.pdb inline
//   GetLandscapeMap, engine/CEngineMap.h FABLE_RETAIL_CENGINEMAP). Identity: Ego_r.pdb ?OnReleaseDefaultPoolResources@CEngineLandscapeRenderer@@UAEXXZ
//   (virtual; the landed row spelled it non-virtual QAE, which its own note said needed the header's `virtual`) on
//   engine/CEngineLandscapeRenderer.h (overlay opt-in _ONRELEASEDEFAULTPOOLRESOURCES); callee Ego_r.pdb
//   ?ReleaseDefaultPoolSurfaces@CEngineLandscapeMap@@QAEXXZ (CEngineLandscapeMap has no engine/ header; declared here as the candidates
//   header did).
// Body shape: the world map global is re-read at the top and bottom of each iteration (the count test and the indexed load).
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_ONRELEASEDEFAULTPOOLRESOURCES
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEMAP
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"

class CEngineLandscapeMap
{
public:
    void ReleaseDefaultPoolSurfaces();             // 0x00BDC040 ?ReleaseDefaultPoolSurfaces@CEngineLandscapeMap@@QAEXXZ
};

namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C

void CEngineLandscapeRenderer::OnReleaseDefaultPoolResources()
{
    for (long i = 0; i < NEngineInternal::GPEngineWorldMap->GetMapCount(); ++i)
    {
        CEngineMap* map = NEngineInternal::GPEngineWorldMap->GetMap(i);
        if (map != 0 && map->GetLandscapeMap() != 0)
            map->GetLandscapeMap()->ReleaseDefaultPoolSurfaces();
    }
}
