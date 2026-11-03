#ifndef FABLE_ENGINE_CENGINEINIT_H
#define FABLE_ENGINE_CENGINEINIT_H
// New header (chunk 731, 2026-11-03; one model, no class macro): Ego_r.pdb CEngineInit member names; retail has no DebugString
// (the Ego_r debug member at +0x0C), so it is 0x64 bytes: the landed CNewFrontendGameComponent::InitialiseEngine 0x0042E204
// builds it with Font +0x10 .. InitFlags +0x60, and CEngine::Initialise 0x00B26360 reads DefinitionManager +0x00,
// RenderManager +0x08, MemoryInit +0x34 and InitFlags +0x60. Constructor / destructor 0x0042D830 / 0x0042F7C1.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CEngineMemoryInit.h"
class CDefinitionManager;
class CEngineDef;
class CRenderManager;
class CIDrawWorld;
class CFontBank;
class CGraphicDataBank;
class CMeshDataBank;
class CEngineInit {
public:
    CEngineInit();                                                       // 0x0042D830 ??0CEngineInit@@QAE@XZ
    ~CEngineInit();                                                      // 0x0042F7C1 ??1CEngineInit@@QAE@XZ
    CDefinitionManager* DefinitionManager;                               // +0x00
    CEngineDef*         EngineDef;                                       // +0x04
    CRenderManager*     RenderManager;                                   // +0x08
    CIDrawWorld*        World;                                           // +0x0c
    CFontBank*          Font;                                            // +0x10
    CGraphicDataBank*   MainGraphicBank;                                 // +0x14
    CGraphicDataBank*   TerrainGraphicBank;                              // +0x18
    CMeshDataBank*      MeshBank;                                        // +0x1c
    CWideString         MeshResourcePath;                                // +0x20
    CWideString         GraphicsResourcePath;                            // +0x24
    CWideString         ShaderResourcePath;                              // +0x28
    CWideString         EngineCacheResourcePath;                         // +0x2c
    CWideString         WriteableEngineCacheResourcePath;                // +0x30
    CEngineMemoryInit   MemoryInit;                                      // +0x34
    long                StaticMapQuality;                                // +0x5c
    long                InitFlags;                                       // +0x60
};
FABLE_STATIC_ASSERT(sizeof(CEngineInit) == 0x64);
#endif // FABLE_ENGINE_CENGINEINIT_H
