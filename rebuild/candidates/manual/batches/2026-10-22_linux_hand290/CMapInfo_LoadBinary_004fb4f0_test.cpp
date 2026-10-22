// Behaviour test for CMapInfo::LoadBinary (0x004FB4F0): two names, three flags, the area in left/right/top/bottom order,
// the Placed flag and the low dword of a 64-bit UID.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct Stream {
    const unsigned char* data;
    int remaining;
    int nameIndex;
    void Read(void* out, int size) { if (remaining >= size) { std::memcpy(out, data, size); data += size; remaining -= size; } }
    const char* ReadPresizedString() { static const char* names[2] = { "Level", "Script" }; return names[nameIndex++]; }
    bool ReadBool() { unsigned char v = 0; Read(&v, 1); return v != 0; }
    int ReadLong() { int v = 0; Read(&v, 4); return v; }
    u64 ReadULong64() { u64 v = 0; Read(&v, 8); return v; }
};

struct MapInfo {
    int left, top, right, bottom;
    const char* levelName;
    const char* scriptName;
    unsigned int uid;
    bool used, placed, proximity, sea;
    void LoadBinary(Stream& s)
    {
        levelName = s.ReadPresizedString();
        scriptName = s.ReadPresizedString();
        used = s.ReadBool();
        proximity = s.ReadBool();
        sea = s.ReadBool();
        left = s.ReadLong();
        right = s.ReadLong();
        top = s.ReadLong();
        bottom = s.ReadLong();
        placed = s.ReadBool();
        uid = (unsigned int)s.ReadULong64();
    }
};

int main()
{
    unsigned char buf[3 + 16 + 1 + 8];
    std::memset(buf, 0, sizeof(buf));
    buf[0] = 1; buf[1] = 0; buf[2] = 5;
    int area[4] = { 10, 20, 30, 40 };
    std::memcpy(buf + 3, area, 16);
    buf[19] = 2;
    u64 uid = (u64)0x12345678 * 16 + 9;
    std::memcpy(buf + 20, &uid, 8);
    Stream s; s.data = buf; s.remaining = sizeof(buf); s.nameIndex = 0;
    MapInfo m; std::memset(&m, 0, sizeof(m));
    m.LoadBinary(s);
    if (!m.used || m.proximity || !m.sea || m.left != 10 || m.right != 20 || m.top != 30 || m.bottom != 40 || !m.placed
        || m.uid != 0x23456789u || std::strcmp(m.levelName, "Level") != 0 || std::strcmp(m.scriptName, "Script") != 0 || s.remaining != 0) {
        std::printf("004fb4f0_TEST FAIL\n"); return 1;
    }
    std::printf("004fb4f0_TEST PASS\n");
    return 0;
}
