#include <cstdio>
#include <cstring>
// behaviour test: CRegion::LoadBinary -- counts first, then both id arrays, the strings (def looked up by name only
// when its index is positive), three flags, the raw mini-map scale, four int->float offsets and the exit-text map
struct Stream {
    const unsigned char* p; int left;
    void Read(void* out, int n) { if (n > left) return; std::memcpy(out, p, n); p += n; left -= n; }
    int ReadInt() { int v = 0; Read(&v, 4); return v; }
    float ReadFloat() { float v = 0; Read(&v, 4); return v; }
    bool ReadBool() { unsigned char v = 0; Read(&v, 1); return v != 0; }
    void ReadString(char* out) { int n = ReadInt(); std::memcpy(out, p, n); out[n] = 0; p += n; left -= n; }
};
struct Vec2 { float x, y; };
struct Exit { char name[16]; Vec2 offset; };
struct Region {
    int maps[4], numMaps, visMaps[4], numVis;
    char name[32], displayName[32], defName[32], graphic[32];
    int def; float scale; Vec2 miniOffset, worldOffset;
    Exit exits[4]; int numExits;
    bool appear, creatures, sounds;
};
static int DefIndexFromName(const char* n) { return std::strcmp(n, "REGION_DEF_X") == 0 ? 7 : 0; }
static void SetExit(Region& r, const char* key, const Vec2& v)
{
    for (int i = 0; i < r.numExits; ++i)
        if (std::strcmp(r.exits[i].name, key) == 0) { r.exits[i].offset = v; return; }
    std::strcpy(r.exits[r.numExits].name, key); r.exits[r.numExits].offset = v; ++r.numExits;
}
static void Load(Region& r, Stream& s)
{
    int numMaps = s.ReadInt();
    int numVis = s.ReadInt();
    r.numMaps = numMaps;
    for (int i = 0; i < numMaps; ++i) r.maps[i] = s.ReadInt();
    r.numVis = numVis;
    for (int j = 0; j < numVis; ++j) r.visMaps[j] = s.ReadInt();
    s.ReadString(r.name);
    s.ReadString(r.displayName);
    s.ReadString(r.defName);
    int index = DefIndexFromName(r.defName);
    if (index > 0) r.def = index;
    s.ReadString(r.graphic);
    r.appear = s.ReadBool(); r.creatures = s.ReadBool(); r.sounds = s.ReadBool();
    r.scale = s.ReadFloat();
    r.miniOffset.x = (float)s.ReadInt(); r.miniOffset.y = (float)s.ReadInt();
    r.worldOffset.x = (float)s.ReadInt(); r.worldOffset.y = (float)s.ReadInt();
    int n = s.ReadInt();
    for (int k = 0; k < n; ++k) {
        char key[16];
        s.ReadString(key);
        Vec2 v; s.Read(&v, 8);
        SetExit(r, key, v);
    }
}
static unsigned char g_buf[256]; static int g_len;
static void Put(const void* d, int n) { std::memcpy(g_buf + g_len, d, n); g_len += n; }
static void PutInt(int v) { Put(&v, 4); }
static void PutStr(const char* s) { int n = (int)std::strlen(s); PutInt(n); Put(s, n); }
int main()
{
    PutInt(2); PutInt(1); PutInt(10); PutInt(11); PutInt(3);
    PutStr("Oakvale"); PutStr("Oakvale Village"); PutStr("REGION_DEF_X"); PutStr("map.tga");
    unsigned char flags[3] = { 1, 0, 5 }; Put(flags, 3);
    float sc = 1.5f; Put(&sc, 4);
    PutInt(4); PutInt(-6); PutInt(100); PutInt(200);
    PutInt(1); PutStr("EXIT_A");
    float v[2] = { 2.0f, 3.0f }; Put(v, 8);
    Stream s; s.p = g_buf; s.left = g_len;
    Region r; r.def = 0; r.numExits = 0;
    Load(r, s);
    bool ok = r.numMaps == 2 && r.maps[1] == 11 && r.numVis == 1 && r.visMaps[0] == 3
        && std::strcmp(r.displayName, "Oakvale Village") == 0 && r.def == 7 && std::strcmp(r.graphic, "map.tga") == 0
        && r.appear && !r.creatures && r.sounds && r.scale == 1.5f && r.miniOffset.y == -6.0f
        && r.worldOffset.x == 100.0f && r.numExits == 1 && r.exits[0].offset.y == 3.0f && s.left == 0;
    std::printf(ok ? "006bc510_TEST PASS\n" : "006bc510_TEST FAIL\n");
    return ok ? 0 : 1;
}
