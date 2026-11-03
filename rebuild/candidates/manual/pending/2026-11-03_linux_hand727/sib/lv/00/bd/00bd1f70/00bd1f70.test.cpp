#include <cstdio>
// Model of the 2D animated mesh queueing: after culling and setup, a type-0x26 primitive with a visibility def hides
// sub-meshes whose material byte is set in the visibility vector (built on demand when empty); the rest are queued with
// the same render-flag / sort-list rules as the static meshes.
struct Material { unsigned char Flags; bool AlphaTest; bool AlphaBlend; };
static int g_built = 0;
static int g_queued[8];
static int g_count = 0;
static unsigned long RenderFlagsFor(unsigned long meshFlags, const Material& m)
{
    bool alphaTest = m.AlphaTest;
    bool blend = m.AlphaBlend;
    if ((meshFlags & 0x100) && alphaTest)
        blend = true;
    unsigned long flags = 0;
    if (blend)
        flags = 2;
    else if (alphaTest)
        flags = 5;
    if (meshFlags & 0x800)
        flags |= 0xc0;
    if (meshFlags & 8)
        flags |= 0x180;
    return flags;
}
static void Queue(long type, bool hasDef, unsigned char* visibility, int* visibilitySize, const long* materialIndex, int subMeshes,
                  const Material* mats, unsigned long meshFlags)
{
    unsigned char* hidden = 0;
    if (type == 0x26) {
        if (!hasDef)
            hidden = 0;
        else {
            if (*visibilitySize == 0) {
                visibility[1] = 1;
                *visibilitySize = 3;
                ++g_built;
            }
            hidden = visibility;
        }
    }
    for (int i = 0; i < subMeshes; ++i) {
        long index = materialIndex[i];
        if (hidden && hidden[index])
            continue;
        g_queued[g_count++] = (int)RenderFlagsFor(meshFlags, mats[index]);
    }
}
int main()
{
    int bad = 0;
    Material mats[3];
    mats[0].Flags = 0; mats[0].AlphaTest = true; mats[0].AlphaBlend = false;
    mats[1].Flags = 1; mats[1].AlphaTest = false; mats[1].AlphaBlend = true;
    mats[2].Flags = 0; mats[2].AlphaTest = false; mats[2].AlphaBlend = false;
    const long index[3] = { 0, 1, 2 };
    unsigned char vis[3] = { 0, 0, 0 };
    int visSize = 0;
    Queue(0x25, true, vis, &visSize, index, 3, mats, 0);
    if (g_count != 3 || g_built != 0) bad = 1;
    g_count = 0;
    Queue(0x26, false, vis, &visSize, index, 3, mats, 0);
    if (g_count != 3 || g_built != 0) bad = 1;
    g_count = 0;
    Queue(0x26, true, vis, &visSize, index, 3, mats, 0x100);
    if (g_count != 2 || g_built != 1 || g_queued[0] != 2 || g_queued[1] != 0) bad = 1;
    g_count = 0;
    Queue(0x26, true, vis, &visSize, index, 3, mats, 8);
    if (g_count != 2 || g_built != 1 || g_queued[0] != (5 | 0x180)) bad = 1;
    if (bad) { std::printf("00bd1f70_TEST FAIL\n"); return 1; }
    std::printf("00bd1f70_TEST PASS\n");
    return 0;
}