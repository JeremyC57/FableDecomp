// Behaviour test for CEnginePrimitiveManagerRepeatedStaticMeshes::PreloadPrimitive @ 0x00B9BCC0 -- LOD pick + texture load requests
#include <cstdio>
#include <cmath>
struct FrameData {
    bool HasFrame;
    signed char SmallestResidentMip;
    signed char LowestRequestedMip;
    int BeginLoads;
};
struct GraphicFrame {
    FrameData* Data;
    FrameData Storage;
    bool RequestLoad(unsigned long mip)
    {
        if (Data) {
            if (Data->HasFrame) {
                if ((unsigned long)Data->LowestRequestedMip > mip) Data->LowestRequestedMip = (signed char)mip;
                if ((unsigned long)Data->SmallestResidentMip > mip) ++Data->BeginLoads;
                return false;
            }
            ++Data->BeginLoads;
            return true;
        }
        Data = &Storage;
        Data->HasFrame = false; Data->SmallestResidentMip = 0; Data->LowestRequestedMip = 0; Data->BeginLoads = 1;
        return true;
    }
};
static bool IsBelowLODCount(unsigned long lod) { if (lod >= 3) return false; return true; }
struct MeshInfo { float LODDistance[4]; bool HasLOD[4]; };
static unsigned long PickLOD(const MeshInfo& info, float ratio)
{
    unsigned long lod;
    for (lod = 0; IsBelowLODCount(lod); ++lod)
        if (!info.HasLOD[lod] || info.LODDistance[lod] > ratio)
            break;
    return lod;
}
static float Ratio(float cx, float px, float radius, float fov, float scale)
{
    float dx = cx - px;
    float distance = (float)std::sqrt(dx * dx) - radius;
    distance = distance > 0.0001f ? distance : 0.0001f;
    return distance / (radius * fov) * scale;
}
int main()
{
    bool ok = true;
    MeshInfo info = { { 1.0f, 2.0f, 4.0f, 8.0f }, { true, true, true, true } };
    ok = ok && PickLOD(info, 0.5f) == 0;
    ok = ok && PickLOD(info, 1.5f) == 1;
    ok = ok && PickLOD(info, 3.0f) == 2;
    ok = ok && PickLOD(info, 100.0f) == 3;
    info.HasLOD[1] = false;
    ok = ok && PickLOD(info, 3.0f) == 1;
    ok = ok && Ratio(10.0f, 0.0f, 2.0f, 1.0f, 1.0f) == 4.0f;
    ok = ok && Ratio(1.0f, 0.0f, 2.0f, 1.0f, 1.0f) > 0.0f;   // clamped inside the radius
    GraphicFrame fresh = { 0 };
    ok = ok && fresh.RequestLoad(0) && fresh.Data == &fresh.Storage && fresh.Storage.BeginLoads == 1;
    ok = ok && fresh.RequestLoad(0) && fresh.Storage.BeginLoads == 2;           // still no frame: load again, started
    fresh.Storage.HasFrame = true; fresh.Storage.SmallestResidentMip = 2; fresh.Storage.LowestRequestedMip = 3;
    ok = ok && !fresh.RequestLoad(0) && fresh.Storage.BeginLoads == 3 && fresh.Storage.LowestRequestedMip == 0;
    fresh.Storage.SmallestResidentMip = 0;
    ok = ok && !fresh.RequestLoad(0) && fresh.Storage.BeginLoads == 3;          // resident: nothing to start
    if (ok) std::printf("00b9bcc0_TEST PASS\n"); else std::printf("00b9bcc0_TEST FAIL\n");
    return ok ? 0 : 1;
}