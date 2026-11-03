// CEngineLandscapeRenderer::~CEngineLandscapeRenderer() -- delete every world map's landscape map and the landscape memory manager, then destroy the members in reverse order and the CEngineComponent base
// Leaf: ??1CEngineLandscapeRenderer@@UAE@XZ
// Flags: none
// Grade: RELOCATION_MATCH (q.py with FABLE_HDR_OVERLAY=hdr/, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B67920, length 1294 bytes (+ int3 padding), __fastcall, tail jump to
//   ~CEngineComponent 0x00B59730.
// Identity: Ego_r.pdb / FableWin.pdb ??1CEngineLandscapeRenderer@@UAE@XZ; it stores the CEngineLandscapeRenderer vtable 0x012A2B54
//   first (RTTI CEngineLandscapeRenderer : CEngineComponent).
// Shape: GetMapCount() is `return MapList.size();` (STLport: the signed pointer difference, finish first -- `mov ecx,[eax+24h] ;
//   sub ecx,[eax+20h] ; sar ecx,2`); an `end() - begin()` body loads begin first and keeps it across the loop (1296 v 1294).
// Body: for every map of the world map (0x01436E8C, `i < GetMapCount()` signed, `GetMap(i)` re-read after each call) that exists,
//   the landscape map is deleted through its virtual destructor (flag 1) and cleared (CEngineMap::GetLandscapeMap / SetLandscapeMap,
//   inline); then `delete MemoryManager` (virtual). The rest is the implicit member destruction: FastForegroundTextureLoadInfo's
//   three CArrays, the tesselator's two, the overflow buffers, the three sorter lists, CTexture[5] (Uninitialise 0x009F9F70 from the
//   last), the index buffer (Uninitialise 0x00AC1EE0 + CMovableResource vtable), pixel / vertex shaders (released inline),
//   CArray<CVertexShader> (virtual element destructors, flag 0), LandscapeThemes, ~CEngineLandscapeMeshBuilder 0x00BE2B20 (+0x10),
//   the GPU update queue (~CBaseClassNonCopyable 0x0099A300, +0x0C).
// Header: FABLE_RETAIL_CENGINELANDSCAPERENDERER / CENGINEWORLDMAP / CENGINEMAP and the new CLandscapeGPUUpdateQueue /
//   CEngineLandscapeMeshBuilder headers (hand-back hdr/), plus the shader / tesselator headers of 0x00B71880.
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEMAP
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineMap.h"

class CEngineLandscapeMap { public:                    // RTTI CEngineLandscapeMap : CMovableResource; only the virtual destructor is used
    virtual ~CEngineLandscapeMap();                    // ??1CEngineLandscapeMap@@UAE@XZ
};
class CEngineLandscapeMemoryManager { public:          // RTTI CEngineLandscapeMemoryManager : CFailedAllocationHandler, CMovableResource@4
    virtual ~CEngineLandscapeMemoryManager();          // ??1CEngineLandscapeMemoryManager@@UAE@XZ
};
extern CEngineWorldMap* g_pEngineWorldMap;            // 0x01436E8C

CEngineLandscapeRenderer::~CEngineLandscapeRenderer()
{
    for (long i = 0; i < g_pEngineWorldMap->GetMapCount(); ++i) {
        if (g_pEngineWorldMap->GetMap(i)) {
            delete g_pEngineWorldMap->GetMap(i)->GetLandscapeMap();
            g_pEngineWorldMap->GetMap(i)->SetLandscapeMap(0);
        }
    }
    delete MemoryManager;
}