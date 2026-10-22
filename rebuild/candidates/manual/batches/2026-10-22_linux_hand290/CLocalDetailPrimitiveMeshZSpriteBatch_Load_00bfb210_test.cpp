// Behaviour test for CLocalDetailPrimitiveMeshZSpriteBatch::Load (0x00BFB210): box, sphere, count, per-object matrix /
// scale / block, then the bulk 16-byte entries; false when an allocation fails.
#include <cstdio>
#include <cstring>

struct Stream {
    const unsigned char* p; int left;
    void Read(void* out, int n) { if (n <= 0) return; if (n <= left) { std::memcpy(out, p, n); p += n; left -= n; } }
    int ReadLong() { int v = 0; Read(&v, 4); return v; }
};
struct Object { float m[12]; float scale; int block[4]; int pad[2]; };
struct Entry { int w[4]; };
static Object g_objects[4];
static Entry g_entries[4];
static bool g_failSecond = false;

struct Batch {
    float box[6]; float sphere[4];
    Object* objects; Entry* entries; int count;
    bool Load(Stream& s)
    {
        s.Read(box, sizeof(box));
        s.Read(sphere, sizeof(sphere));
        count = s.ReadLong();
        objects = count <= 4 ? g_objects : 0;
        if (objects == 0) return false;
        for (int i = 0; i < count; ++i) {
            s.Read(objects[i].m, sizeof(objects[i].m));
            float scale; s.Read(&scale, 4); objects[i].scale = scale;
            s.Read(objects[i].block, sizeof(objects[i].block));
        }
        entries = g_failSecond ? 0 : g_entries;
        if (entries == 0) return false;
        s.Read(entries, count * (int)sizeof(Entry));
        return true;
    }
};

int main()
{
    float data[64];
    for (int i = 0; i < 64; ++i) data[i] = (float)i;
    int two = 2; std::memcpy(&data[10], &two, 4);
    Stream s; s.p = (const unsigned char*)data; s.left = (10 + 1 + 2 * 17 + 2 * 4) * 4;
    Batch b;
    if (!b.Load(s) || b.count != 2 || b.box[5] != 5.0f || b.sphere[3] != 9.0f || b.objects[0].m[0] != 11.0f
        || b.objects[0].scale != 23.0f || b.objects[1].m[0] != 28.0f || b.objects[1].scale != 40.0f || s.left != 0) {
        std::printf("00bfb210_TEST FAIL\n"); return 1;
    }
    int big = 9; std::memcpy(&data[10], &big, 4);
    Stream t; t.p = (const unsigned char*)data; t.left = 256;
    if (b.Load(t)) { std::printf("00bfb210_TEST FAIL\n"); return 1; }
    std::memcpy(&data[10], &two, 4); g_failSecond = true;
    Stream u; u.p = (const unsigned char*)data; u.left = 256;
    if (b.Load(u)) { std::printf("00bfb210_TEST FAIL\n"); return 1; }
    std::printf("00bfb210_TEST PASS\n");
    return 0;
}
