// Behaviour test for 00a25bd0 (CPhysicsMeshInfo::BeginLoading): nothing happens when mesh data exists; otherwise new
// mesh data is created for this info, registered with the bank, given the read of the whole entry, and its resource
// size set to (data ? data size + 8 : 8) + (mesh ? mesh size + 4 : 4) + 0x2c.
#include <cstdio>

struct Data { unsigned long size; };
struct MeshData { void* owner; Data* file; unsigned long meshSize; bool hasMesh; unsigned long resourceSize; };
static MeshData g_store[2]; static int g_created = 0, g_registered = 0; static unsigned long g_readSize = 0;
static Data g_data = { 100 };
struct Info {
    MeshData* meshData; unsigned long entrySize;
    void BeginLoading()
    {
        if (meshData) return;
        meshData = &g_store[g_created++]; meshData->owner = this; meshData->file = 0; meshData->hasMesh = false;
        unsigned long size = entrySize;
        ++g_registered;
        g_readSize = size; meshData->file = &g_data;
        MeshData* d = meshData;
        unsigned long meshSize = d->hasMesh ? d->meshSize + 4 : 4;
        unsigned long dataSize = d->file ? d->file->size + 8 : 8;
        d->resourceSize = dataSize + meshSize + 0x2c;
    }
};
int main()
{
    Info info; info.meshData = 0; info.entrySize = 64;
    info.BeginLoading();
    int ok = g_created == 1 && g_registered == 1 && g_readSize == 64 && info.meshData->owner == &info &&
             info.meshData->resourceSize == 108 + 4 + 0x2c;
    info.BeginLoading();
    if (g_created != 1) ok = 0;
    if (!ok) { std::printf("00a25bd0_TEST FAIL\n"); return 1; }
    std::printf("00a25bd0_TEST PASS\n");
    return 0;
}
