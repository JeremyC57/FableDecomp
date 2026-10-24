// Behaviour test for CShaderRenderManager::SetVertexShaderConstantLayout (0x0098B5E0), rewritten from the landed test so
// VC7.1 /Od compiles it quickly: each scenario is its own small function and every check is one short condition (the
// landed main with ~25-term `||` chains hung cl /Od). Scenarios: index -1 selects the null layout and leaves the flags;
// the first layout (no current one) only records it; a full transition sets every dirty bit, recounts and reallocates
// the lights, flags each light, clears the light buffer to (0,0,0,1) and grows the light globals; an identical layout
// only sets bit 4; a light-globals change that would shrink keeps the current size.
// PassPattern kept from the landed test: FABLETLC_SET_VS_LAYOUT_BEHAVIOR PASS.
#include <stdio.h>
#include <string.h>

// ---- reconstruction under test (verbatim copy of src/0098b5e0.cpp) ----
#include "rebuild_abi.h"

// Retail constant-layout transition and light-buffer invalidation view.
struct CShaderConstantRangeView
{
    void* vtable00;
    fable_i32 offset04;
    fable_u32 count08;

    void Destroy();
    bool operator!=(const CShaderConstantRangeView& other) const;
};

extern void* g_CShaderConstantRangeVtable[];

struct CVertexShaderConstantLayoutView
{
    void* vtable00;
    CShaderConstantRangeView reserved04;
    CShaderConstantRangeView user10;
    CShaderConstantRangeView compression1C;
    CShaderConstantRangeView ambient28;
    CShaderConstantRangeView fogColour34;
    CShaderConstantRangeView cameraPos40;
    fable_u32 lightSize4C;
    CShaderConstantRangeView lightArray50;
    CShaderConstantRangeView lightGlobals5C;
    CShaderConstantRangeView lightAttenuations68;
    CShaderConstantRangeView combinedProjectionMatrix74;
    CShaderConstantRangeView fogTransform80;
    CShaderConstantRangeView worldMatrix8C;
    fable_u8 unknown98[0xB4];
};

FABLE_STATIC_ASSERT(sizeof(CVertexShaderConstantLayoutView) == 0x14C);

struct CVertexShaderConstantLayoutVectorView
{
    CVertexShaderConstantLayoutView** begin00;
    CVertexShaderConstantLayoutView** end04;
    CVertexShaderConstantLayoutView** capacity08;
};

struct C4DVectorView
{
    float x;
    float y;
    float z;
    float w;
};

union CSetLayoutLocal
{
    CShaderConstantRangeView range;
    C4DVectorView vector;
};

struct C4DVectorVectorView
{
    C4DVectorView* begin00;
    C4DVectorView* end04;
    C4DVectorView* capacity08;

    fable_u32 size() const
    {
        return static_cast<fable_u32>(end04 - begin00);
    }

    void resize(fable_u32 count, const C4DVectorView& value);
};

struct CLightInfoView
{
    fable_u8 unknown00[0x35];
    bool needsUpdate35;
    fable_u8 unknown36[2];
};

FABLE_STATIC_ASSERT(sizeof(CLightInfoView) == 0x38);

namespace NVectorMath
{
    void FABLE_CDECL GFClear4DVectors(
        C4DVectorView* vectors,
        fable_u32 count,
        const C4DVectorView& value);
}

struct CShaderRenderManagerSetLayoutView
{
    void* vtable00;
    CVertexShaderConstantLayoutView* currentLayout04;
    fable_i32 currentLayoutIndex08;
    CVertexShaderConstantLayoutVectorView layouts0C;
    CVertexShaderConstantLayoutView nullLayout18;
    CLightInfoView* lights164;
    C4DVectorView* lightBuffer168;
    fable_u32 lightCount16C;
    fable_u32 allocatedLightCount170;
    fable_u32 allocatedLightSize174;
    fable_u8 lightPool178[0x1C];
    C4DVectorVectorView lightGlobals194;
    fable_u8 unknown1A0[0x238];
    fable_u32 updateFlags3D8;

    void AllocateLights(fable_u32 count, fable_u32 lightSize);
    void SetVertexShaderConstantLayout(fable_i32 index);
};

FABLE_STATIC_ASSERT(
    sizeof(CShaderRenderManagerSetLayoutView) == 0x3DC);

void CShaderRenderManagerSetLayoutView::
SetVertexShaderConstantLayout(fable_i32 index)
{
    CSetLayoutLocal local;

    if (index == -1)
    {
        currentLayoutIndex08 = -1;
        currentLayout04 = &nullLayout18;
        return;
    }

    CVertexShaderConstantLayoutView* newLayout =
        layouts0C.begin00[index];

    if (currentLayout04 != 0)
    {
        updateFlags3D8 |= 4;

        if (
            currentLayout04->worldMatrix8C.offset04 !=
                newLayout->worldMatrix8C.offset04 ||
            currentLayout04->worldMatrix8C.count08 !=
                newLayout->worldMatrix8C.count08)
        {
            updateFlags3D8 |= 0x10;
        }

        if (
            currentLayout04->fogTransform80.offset04 !=
                newLayout->fogTransform80.offset04 ||
            currentLayout04->fogTransform80.count08 !=
                newLayout->fogTransform80.count08)
        {
            updateFlags3D8 |= 0x6000;
        }

        if (
            currentLayout04->ambient28.offset04 !=
                newLayout->ambient28.offset04 ||
            currentLayout04->ambient28.count08 !=
                newLayout->ambient28.count08)
        {
            updateFlags3D8 |= 4;
        }

        if (
            currentLayout04->fogColour34.offset04 !=
                newLayout->fogColour34.offset04 ||
            currentLayout04->fogColour34.count08 !=
                newLayout->fogColour34.count08)
        {
            updateFlags3D8 |= 0x20000;
        }

        if (
            currentLayout04->cameraPos40.offset04 !=
                newLayout->cameraPos40.offset04 ||
            currentLayout04->cameraPos40.count08 !=
                newLayout->cameraPos40.count08)
        {
            updateFlags3D8 |= 8;
        }

        const fable_u32 lightSize = newLayout->lightSize4C;
        if (
            currentLayout04->lightSize4C != lightSize ||
            currentLayout04->lightArray50.offset04 !=
                newLayout->lightArray50.offset04 ||
            currentLayout04->lightArray50.count08 !=
                newLayout->lightArray50.count08 ||
            currentLayout04->lightGlobals5C !=
                newLayout->lightGlobals5C)
        {
            if (static_cast<fable_i32>(lightSize) > 0)
            {
                {
                    local.range.offset04 =
                        newLayout->lightArray50.offset04;
                    local.range.count08 =
                        newLayout->lightArray50.count08;
                    local.range.vtable00 =
                        g_CShaderConstantRangeVtable;
                    lightCount16C =
                        local.range.count08 / lightSize;
                    local.range.Destroy();
                }

                if (
                    lightCount16C > allocatedLightCount170 ||
                    lightSize > allocatedLightSize174)
                {
                    AllocateLights(lightCount16C, lightSize);
                }

                for (
                    fable_u32 light = 0;
                    light < lightCount16C;
                    ++light)
                {
                    lights164[light].needsUpdate35 = true;
                }

                local.vector.x = 0.0f;
                local.vector.y = 0.0f;
                local.vector.z = 0.0f;
                local.vector.w = 1.0f;
                NVectorMath::GFClear4DVectors(
                    lightBuffer168,
                    lightCount16C * lightSize,
                    local.vector);
            }
            updateFlags3D8 |= 0x700;
        }

        const CShaderConstantRangeView& currentLightGlobals =
            currentLayout04->lightGlobals5C;
        if (
            currentLightGlobals.offset04 !=
                newLayout->lightGlobals5C.offset04 ||
            currentLightGlobals.count08 !=
                newLayout->lightGlobals5C.count08)
        {
            fable_u32 count =
                newLayout->lightGlobals5C.count08;
            if (count <= lightGlobals194.size())
            {
                count = lightGlobals194.size();
            }

            local.vector.x = 0.0f;
            local.vector.y = 0.0f;
            local.vector.z = 0.0f;
            local.vector.w = 1.0f;
            lightGlobals194.resize(count, local.vector);
            updateFlags3D8 |= 0x400;
        }

        const CShaderConstantRangeView&
            currentCombinedProjectionMatrix =
                currentLayout04->combinedProjectionMatrix74;
        if (
            currentCombinedProjectionMatrix.offset04 !=
                newLayout->combinedProjectionMatrix74.offset04 ||
            currentCombinedProjectionMatrix.count08 !=
                newLayout->combinedProjectionMatrix74.count08)
        {
            updateFlags3D8 |= 0x80;
        }
    }

    currentLayout04 = newLayout;
    currentLayoutIndex08 = index;
}

// ---- harness ----
void* g_CShaderConstantRangeVtable[1] = { 0 };

static int g_fail;
static int g_DestroyCalls, g_AllocateCalls, g_ClearCalls, g_ResizeCalls;
static fable_u32 g_AllocateCount, g_AllocateSize, g_ClearCount, g_ResizeCount;
static C4DVectorView* g_ClearDestination;
static C4DVectorView g_ResizeValue;
static C4DVectorView g_ResizeStorage[16];

void CShaderConstantRangeView::Destroy() { ++g_DestroyCalls; }
bool CShaderConstantRangeView::operator!=(const CShaderConstantRangeView& other) const
{
    if (offset04 != other.offset04) return true;
    return count08 != other.count08;
}
void C4DVectorVectorView::resize(fable_u32 count, const C4DVectorView& value)
{
    ++g_ResizeCalls; g_ResizeCount = count; g_ResizeValue = value;
    begin00 = g_ResizeStorage; end04 = g_ResizeStorage + count; capacity08 = g_ResizeStorage + 16;
}
void NVectorMath::GFClear4DVectors(C4DVectorView* vectors, fable_u32 count, const C4DVectorView& value)
{
    fable_u32 index;
    ++g_ClearCalls; g_ClearDestination = vectors; g_ClearCount = count;
    for (index = 0; index < count; ++index)
        vectors[index] = value;
}
void CShaderRenderManagerSetLayoutView::AllocateLights(fable_u32 count, fable_u32 lightSize)
{
    ++g_AllocateCalls; g_AllocateCount = count; g_AllocateSize = lightSize;
    allocatedLightCount170 = count; allocatedLightSize174 = lightSize;
}

static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static int isUnitW(const C4DVectorView& v) { return v.x == 0.0f && v.y == 0.0f && v.z == 0.0f && v.w == 1.0f; }
static void resetCalls()
{
    g_DestroyCalls = g_AllocateCalls = g_ClearCalls = g_ResizeCalls = 0;
    g_AllocateCount = g_AllocateSize = g_ClearCount = g_ResizeCount = 0;
    g_ClearDestination = 0;
}

static CShaderRenderManagerSetLayoutView manager;
static CVertexShaderConstantLayoutView oldLayout, newLayout;
static CVertexShaderConstantLayoutView* layouts[1];
static CLightInfoView lights[3];
static C4DVectorView lightBuffer[12];
static C4DVectorView existingGlobals[8];

static void freshManager()
{
    resetCalls();
    memset(&manager, 0, sizeof(manager));
    layouts[0] = &newLayout;
    manager.layouts0C.begin00 = layouts;
    manager.layouts0C.end04 = layouts + 1;
    manager.layouts0C.capacity08 = layouts + 1;
}

static void testNullLayout()
{
    freshManager();
    manager.currentLayout04 = &oldLayout;
    manager.currentLayoutIndex08 = 7;
    manager.updateFlags3D8 = 0x40000000;
    manager.SetVertexShaderConstantLayout(-1);
    check(manager.currentLayout04 == &manager.nullLayout18, "-1 selects the null layout");
    check(manager.currentLayoutIndex08 == -1, "-1 index");
    check(manager.updateFlags3D8 == 0x40000000, "-1 leaves the flags");
}

static void testFirstLayout()
{
    memset(&newLayout, 0, sizeof(newLayout));
    newLayout.worldMatrix8C.offset04 = 99;
    freshManager();
    manager.SetVertexShaderConstantLayout(0);
    check(manager.currentLayout04 == &newLayout, "first layout recorded");
    check(manager.currentLayoutIndex08 == 0, "first layout index");
    check(manager.updateFlags3D8 == 0, "no flags without a current layout");
    check(g_DestroyCalls + g_AllocateCalls + g_ClearCalls + g_ResizeCalls == 0, "no side calls without a current layout");
}

static void testFullTransition()
{
    memset(&oldLayout, 0, sizeof(oldLayout));
    memset(&newLayout, 0, sizeof(newLayout));
    memset(lights, 0, sizeof(lights));
    memset(lightBuffer, 0xCD, sizeof(lightBuffer));
    oldLayout.worldMatrix8C.offset04 = 1;   newLayout.worldMatrix8C.offset04 = 2;
    oldLayout.fogTransform80.offset04 = 3;  newLayout.fogTransform80.offset04 = 4;
    oldLayout.ambient28.offset04 = 5;       newLayout.ambient28.offset04 = 6;
    oldLayout.fogColour34.offset04 = 7;     newLayout.fogColour34.offset04 = 8;
    oldLayout.cameraPos40.offset04 = 9;     newLayout.cameraPos40.offset04 = 10;
    oldLayout.lightSize4C = 2;              newLayout.lightSize4C = 4;
    oldLayout.lightArray50.offset04 = 11;   newLayout.lightArray50.offset04 = 12;
    newLayout.lightArray50.count08 = 12;
    oldLayout.lightGlobals5C.offset04 = 13; oldLayout.lightGlobals5C.count08 = 2;
    newLayout.lightGlobals5C.offset04 = 14; newLayout.lightGlobals5C.count08 = 5;
    oldLayout.combinedProjectionMatrix74.offset04 = 15;
    newLayout.combinedProjectionMatrix74.offset04 = 16;
    freshManager();
    manager.currentLayout04 = &oldLayout;
    manager.currentLayoutIndex08 = 9;
    manager.lights164 = lights;
    manager.lightBuffer168 = lightBuffer;
    manager.lightGlobals194.begin00 = existingGlobals;
    manager.lightGlobals194.end04 = existingGlobals + 2;
    manager.lightGlobals194.capacity08 = existingGlobals + 2;
    manager.updateFlags3D8 = 0x80000000;
    manager.SetVertexShaderConstantLayout(0);
    check(manager.currentLayout04 == &newLayout, "full: layout");
    check(manager.currentLayoutIndex08 == 0, "full: index");
    check(manager.updateFlags3D8 == (0x80000000u | 4u | 0x10u | 0x6000u | 0x20000u | 8u | 0x700u | 0x400u | 0x80u), "full: every dirty bit");
    check(manager.lightCount16C == 3, "full: light count = 12 / 4");
    check(g_DestroyCalls == 1, "full: temporary range destroyed");
    check(g_AllocateCalls == 1, "full: lights reallocated");
    check(g_AllocateCount == 3, "full: allocate count");
    check(g_AllocateSize == 4, "full: allocate size");
    check(lights[0].needsUpdate35, "full: light 0 flagged");
    check(lights[1].needsUpdate35, "full: light 1 flagged");
    check(lights[2].needsUpdate35, "full: light 2 flagged");
    check(g_ClearCalls == 1, "full: light buffer cleared");
    check(g_ClearDestination == lightBuffer, "full: clear destination");
    check(g_ClearCount == 12, "full: clear count");
    check(isUnitW(lightBuffer[0]), "full: first cleared vector");
    check(isUnitW(lightBuffer[11]), "full: last cleared vector");
    check(g_ResizeCalls == 1, "full: light globals resized");
    check(g_ResizeCount == 5, "full: grows to the new count");
    check(isUnitW(g_ResizeValue), "full: resize fill value");
}

static void testSameLayout()
{
    memcpy(&oldLayout, &newLayout, sizeof(newLayout));
    freshManager();
    manager.currentLayout04 = &oldLayout;
    manager.SetVertexShaderConstantLayout(0);
    check(manager.updateFlags3D8 == 4, "same layout: only bit 4");
    check(g_DestroyCalls + g_AllocateCalls + g_ClearCalls + g_ResizeCalls == 0, "same layout: no side calls");
}

static void testGlobalsKeepSize()
{
    memcpy(&oldLayout, &newLayout, sizeof(newLayout));
    oldLayout.lightGlobals5C.count08 = 7;          // differs; new count 5 is below the current size 6
    freshManager();
    manager.currentLayout04 = &oldLayout;
    manager.lightGlobals194.begin00 = existingGlobals;
    manager.lightGlobals194.end04 = existingGlobals + 6;
    manager.lightGlobals194.capacity08 = existingGlobals + 8;
    manager.lights164 = lights;                    // the light layout compares unequal too (lightGlobals is part of it)
    manager.lightBuffer168 = lightBuffer;
    manager.allocatedLightCount170 = 3;
    manager.allocatedLightSize174 = 4;
    manager.SetVertexShaderConstantLayout(0);
    check(g_ResizeCalls == 1, "globals: resized");
    check(g_ResizeCount == 6, "globals: never shrinks below the current size");
    check(manager.updateFlags3D8 == (4u | 0x700u | 0x400u), "globals: light + globals bits");
    check(g_AllocateCalls == 0, "globals: lights fit, no reallocation");
}

int main()
{
    testNullLayout();
    testFirstLayout();
    testFullTransition();
    testSameLayout();
    testGlobalsKeepSize();
    if (g_fail)
        return 1;
    printf("FABLETLC_SET_VS_LAYOUT_BEHAVIOR PASS\n");
    return 0;
}