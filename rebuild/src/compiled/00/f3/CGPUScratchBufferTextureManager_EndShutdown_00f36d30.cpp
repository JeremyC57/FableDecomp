// Leaf: ?EndShutdown@CGPUScratchBufferTextureManager@@QAE_NXZ
// Flags: /O2 /Oy
// Inferred: the debug-only CResourceEntry::InUse is absent in the retail 0x2c stride.
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_GPU_SCRATCH_RESOURCES_PASS29
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDSHUTDOWN
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SHADOW_BUFFERS_PASS27
#include "engine/CGPUScratchBufferTextureManager.h"
bool CGPUScratchBufferTextureManager::EndShutdown()
{
    bool succeded = true;
    if (ShutdownFlag) {
        for (long c_res = 0; c_res < Resources.size(); ++c_res) {
            CResourceEntry& resource = Resources[c_res];
            if (resource.ResourceType == RESOURCE_TYPE_TEXTURE) {
                resource.Texture = new CTexture;
                if (!resource.Texture->Initialise(resource.Dimensions, resource.NoLevels, resource.PixelFormat,
                    resource.UseageFlags, resource.MemoryPool, false, true)) {
                    succeded = false;
                    break;
                }
            } else if (resource.ResourceType == RESOURCE_TYPE_DEPTH_BUFFER_SURFACE) {
                resource.Surface = new CSurface;
                if (!resource.Surface->InitialiseAsDepthBuffer(resource.Dimensions, resource.PixelFormat,
                    resource.MultiSampleType, true)) {
                    succeded = false;
                    break;
                }
            }
        }
        if (!succeded) {
            for (long c_res = 0; c_res < Resources.size(); ++c_res) {
                CResourceEntry& resource = Resources[c_res];
                delete resource.Texture;
                delete resource.Surface;
                resource.Texture = 0;
                resource.Surface = 0;
            }
        }
        if (succeded)
            ShutdownFlag = false;
    }
    return succeded;
}