// CDisplayManager::CDisplayManager(CSystemManager&) -- initialise device resources and supported surface formats.
// Leaf: ??0CDisplayManager@@QAE@AAVCSystemManager@@@Z
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CARRAY_DISPLAY
#define FABLE_RETAIL_CWIDESTRING_CTOR
#include "stlport/display_containers.h"
#include "engine/CDisplayManager.h"
CDisplayManager::CDisplayManager(CSystemManager& systemManager)
 :SystemManager(systemManager),D3D(0),D3DDevice(0),FrameCounter(0),Gamma(1.0f)
{
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_A1R5G5B5));
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_A2B10G10R10));
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_A8R8G8B8));
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_R5G6B5));
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_X1R5G5B5));
 BackBufferFormats.push_back(CPixelFormat(D3DFMT_X8R8G8B8));
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_NONE);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_2);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_3);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_4);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_5);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_6);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_7);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_8);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_9);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_10);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_11);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_12);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_13);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_14);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_15);
 MultisampleTypes.push_back(SURFACE_MULTISAMPLE_16);
}
