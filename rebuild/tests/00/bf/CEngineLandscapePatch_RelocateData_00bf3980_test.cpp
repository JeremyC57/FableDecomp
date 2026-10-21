#include <cstdio>
class CEngineLandscapeMap;
class CWaterPatchMesh {
public:
    void OnPostRelocation();                             // 0x00BEAA30
};
class CLandscapeLayerMesh {
public:
    unsigned char        _pad00[0x3c];
    CLandscapeLayerMesh* Next;                           // +0x3c
    void OnPostRelocation();                             // 0x00BFDD70
};
template <class T> inline void RelocatePointer(T*& p, void* oldData, void* newData)
{
    if (p)
        p = (T*)((char*)p - (char*)oldData + (char*)newData);
}
class CEngineLandscapePatch {
public:
    CEngineLandscapeMap*  Map;                           // +0x04
    CLandscapeLayerMesh*  LayerMeshList;                 // +0x08
    unsigned short*       FastAccessTextureIDs;          // +0x0c
    unsigned short        FastAccessTextureIDCount;      // +0x10
    CWaterPatchMesh*      WaterPatch;                    // +0x14
private:
    virtual void RelocateData(long handle, void* oldData, void* newData, long size);
};

void CEngineLandscapePatch::RelocateData(long handle, void* oldData, void* newData, long size)
{
    if (oldData == FastAccessTextureIDs)
    {
        RelocatePointer(FastAccessTextureIDs, oldData, newData);
        return;
    }
    if (oldData == WaterPatch)
    {
        RelocatePointer(WaterPatch, oldData, newData);
        WaterPatch->OnPostRelocation();
        return;
    }
    CLandscapeLayerMesh* prev = 0;
    for (CLandscapeLayerMesh* mesh = LayerMeshList; mesh; mesh = mesh->Next)
    {
        if (oldData == mesh)
        {
            RelocatePointer(mesh, oldData, newData);
            if (prev)
                prev->Next = mesh;
            else
                LayerMeshList = mesh;
            mesh->OnPostRelocation();
            return;
        }
        prev = mesh;
    }
}

static void* g_water; static void* g_mesh; static int g_waterCalls, g_meshCalls;
void CWaterPatchMesh::OnPostRelocation() { g_water = this; ++g_waterCalls; }
void CLandscapeLayerMesh::OnPostRelocation() { g_mesh = this; ++g_meshCalls; }
typedef void (__fastcall* RelocFn)(void* self, int, long handle, void* oldData, void* newData, long size);
static CEngineLandscapePatch g_patch;
static void Reloc(void* o, void* n) { RelocFn f = (RelocFn)(*(void***)&g_patch)[0]; f(&g_patch, 0, 1, o, n, 2); }
int main()
{
    static unsigned short ids[8]; static unsigned short ids2[8];
    static CWaterPatchMesh water[2];
    static CLandscapeLayerMesh m[5];                     // m[0..2] in the list; m[3], m[4] are the new homes
    m[0].Next = &m[1]; m[1].Next = &m[2]; m[2].Next = 0;
    g_patch.LayerMeshList = &m[0]; g_patch.FastAccessTextureIDs = ids; g_patch.WaterPatch = &water[0];
    Reloc(ids, ids2);
    bool ok = g_patch.FastAccessTextureIDs == ids2 && g_waterCalls == 0 && g_meshCalls == 0;
    Reloc(&water[0], &water[1]);
    ok = ok && g_patch.WaterPatch == &water[1] && g_waterCalls == 1 && g_water == &water[1];
    m[3] = m[1]; Reloc(&m[1], &m[3]);                    // middle node: previous Next rebased
    ok = ok && m[0].Next == &m[3] && g_meshCalls == 1 && g_mesh == &m[3] && g_patch.LayerMeshList == &m[0];
    m[4] = m[0]; Reloc(&m[0], &m[4]);                    // head node: list head rebased
    ok = ok && g_patch.LayerMeshList == &m[4] && g_meshCalls == 2 && g_mesh == &m[4];
    static CLandscapeLayerMesh stray; Reloc(&stray, &m[0]); // not owned: nothing happens
    ok = ok && g_meshCalls == 2 && g_waterCalls == 1 && g_patch.LayerMeshList == &m[4];
    std::printf(ok ? "00bf3980_TEST PASS\n" : "00bf3980_TEST FAIL\n");
    return ok ? 0 : 1;
}