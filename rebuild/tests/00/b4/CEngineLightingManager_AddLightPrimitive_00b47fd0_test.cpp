// Behaviour test for 0x00B47FD0 CEngineLightingManager::AddLightPrimitive (NF151, q4506): rewritten on the shared headers the row now uses;
// the row object is linked (its STLport vector<CManagerLocalLight*>::push_back / _M_insert_overflow COMDATs grow LocalLightList with
// malloc). The heap, the light element constructor and the light's Delete are test definitions on the shared classes; the manager is
// zeroed storage (its constructor is another row); the heap's private state is observed from its own (test) member definitions, so every
// symbol keeps its shared-header access. PassPattern unchanged.
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWED_UPDATE_PASS23
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_ADDLIGHTPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_STLP_TAG_CTORS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVELIGHT_TYPED
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineInternalPrimitiveLight.h"

static int g_moreBlocks, g_elements;
static float g_elementRadius, g_elementSpot, g_elementR;
static unsigned long g_elementMask;
static const C3DVector* g_elementPos;
static void* g_block0;
static void* g_block1;

CDynamicObjectHeapBase::CDynamicObjectHeapBase() { }
CDynamicObjectHeapBase::~CDynamicObjectHeapBase() { }
template<> CDynamicObjectHeap<72, 16>::CDynamicObjectHeap(bool grow) { FreeList = 0; AllowHeapToGrow = grow; AllocCount = 0; }
template<> CDynamicObjectHeap<72, 16>::~CDynamicObjectHeap() { }
template<> void CDynamicObjectHeap<72, 16>::Release() { }
template<> void CDynamicObjectHeap<72, 16>::CreateMoreBlocks()
{
    ++g_moreBlocks;
    for (long i = 0; i < 15; ++i)
        StartBlock.Blocks[i].NextEntry = &StartBlock.Blocks[i + 1];
    StartBlock.Blocks[15].NextEntry = 0;
    FreeList = &StartBlock.Blocks[0];
    g_block0 = &StartBlock.Blocks[0];
    g_block1 = &StartBlock.Blocks[1];
}
CDynamicObjectHeap<72, 16> LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight(true);
CEngineLocalLightElement::CEngineLocalLightElement(const C3DVector& pos, float radius, float spot, const CRGBFloatColour& colour, unsigned long mask)
{
    ++g_elements; g_elementPos = &pos; g_elementRadius = radius; g_elementSpot = spot; g_elementR = colour.R; g_elementMask = mask;
    WorldPos = pos; Radius = radius; SpotRadius = spot; Colour = colour; ChannelMask = mask;
}
void CEngineLightingManager::CManagerLocalLight::Delete() { }
void FABLE_FASTCALL CEngineLightingManager::CManagerLocalLight::operator delete(void*) { }

static unsigned char g_managerStorage[sizeof(CEngineLightingManager)];
static unsigned char g_lightStorage[sizeof(CEngineInternalPrimitiveLight)];

int main()
{
    if (offsetof(CEngineLightingManager, LocalLightList) != 0xAC || offsetof(CEngineInternalPrimitiveLight, WorldPos) != 0x48 ||
        offsetof(CEngineInternalPrimitiveLight, Colour) != 0x54 || offsetof(CEngineInternalPrimitiveLight, ChannelMask) != 0x6C) {
        printf("00b47fd0_TEST FAIL offsets\n");
        return 1;
    }
    CEngineLightingManager* manager = (CEngineLightingManager*)g_managerStorage;
    CEngineInternalPrimitiveLight* light = (CEngineInternalPrimitiveLight*)g_lightStorage;
    int fail = 0;
    light->WorldPos.X = 1.0f; light->WorldPos.Y = 2.0f; light->WorldPos.Z = 3.0f;
    light->Colour.R = 0.5f; light->Colour.G = 0.0f; light->Colour.B = 0.0f; light->Colour.A = 1.0f;
    light->Radius = 0.05f; light->SpotRadius = 4.0f; light->ChannelMask = 0x15;

    manager->AddLightPrimitive(*light);                                  // radius below 0.1: ignored
    if (manager->LocalLightList.size() != 0 || g_elements != 0 || g_moreBlocks != 0) ++fail;

    light->Radius = 2.0f;
    light->Colour.R = 0.001f;                                            // every channel below 1/255: ignored
    manager->AddLightPrimitive(*light);
    if (manager->LocalLightList.size() != 0 || g_elements != 0) ++fail;

    light->Colour.R = 0.5f;
    manager->AddLightPrimitive(*light);                                  // queued: the heap grows once, one element built
    if (manager->LocalLightList.size() != 1 || g_elements != 1 || g_moreBlocks != 1) ++fail;
    if (g_elementPos != &light->WorldPos || g_elementRadius != 2.0f || g_elementSpot != 4.0f || g_elementR != 0.5f || g_elementMask != 0x15) ++fail;
    CEngineLightingManager::CManagerLocalLight* queued = manager->LocalLightList[0];
    if ((void*)queued != g_block0 || queued->AlreadyFoundFlag) ++fail;

    light->Colour.R = 0.0f; light->Colour.B = 0.25f;                     // one channel bright enough: queued from the free list
    manager->AddLightPrimitive(*light);
    if (manager->LocalLightList.size() != 2 || g_elements != 2 || g_moreBlocks != 1 ||
        (void*)manager->LocalLightList[1] != g_block1) ++fail;

    if (fail) { printf("00b47fd0_TEST FAIL (%d)\n", fail); return 1; }
    printf("00b47fd0_TEST PASS\n");
    return 0;
}
