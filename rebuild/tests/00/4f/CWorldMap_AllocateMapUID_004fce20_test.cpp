// Behaviour test for 004fce20: src/004fce20.cpp copied verbatim; the clock hash GFROR13 is stubbed with a scripted sequence.
#include <cstdio>
#include <cstring>
static int fails = 0;
static char g_msg[128];
#define CHECK(c) do { if (!(c)) { ++fails; _snprintf(g_msg, sizeof(g_msg), "FAIL %d: %s", __LINE__, #c); puts(g_msg); } } while (0)
// ---- source (verbatim) ----
// CWorldMap::AllocateMapUID(long) -- make a 22-bit map UID from the system clock (hashed), offset by map * allocation count, retrying until no used map has it
// Leaf: ?AllocateMapUID@CWorldMap@@IAEKJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FCE20, 318 bytes, `ret 4`.
// Callees: the CRT pow intrinsic 0x00BFEB40 and _ftol 0x00BFEA70, _allmul 0x00BFEB50, GetSystemTime (import 0x0143FE18),
//   GFROR13 0x00497890 (__fastcall).
// Shape: the search over the used maps sets a `used` flag and breaks; VC7.1 threads the flag so a hit jumps straight back
//   to the retry (reloading the range from its spill) and an exhausted search returns the UID.
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)
struct SYSTEMTIME {
    unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
};
extern "C" __declspec(dllimport) void __stdcall GetSystemTime(SYSTEMTIME* time);   // kernel32 (import 0x0143FE18)
long __fastcall GFROR13(unsigned long value);         // 0x00497890
class CMapInfo { public:                                      // 0x48 bytes
    unsigned char _pad00[0x20];
    unsigned long MapUID;                              // +0x20
    bool          Used;                                // +0x24
    unsigned char _body[0x23];
};
template <class T> class CArray { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return *(_M_start + i); }
};
class CWorldMap {
public:
    unsigned char    _pad00[0x20];
    CArray<CMapInfo> Maps;                             // +0x20
    unsigned char    _pad2c[0xac - 0x2c];
    unsigned long    CurrentMapUIDCount;               // +0xAC (PDB +0xCC)
    unsigned long AllocateMapUID(long map);
};
unsigned long CWorldMap::AllocateMapUID(long map)
{
    unsigned long count = CurrentMapUIDCount++;
    unsigned long range = (unsigned long)pow(2.0, 22.0);
    __int64 offset = (__int64)count * map;
    for (;;) {
        SYSTEMTIME time;
        GetSystemTime(&time);
        unsigned long stamp = time.wYear;
        stamp = stamp * 16 + (time.wMonth & 0xf);
        stamp = stamp * 32 + (time.wDay & 0x1f);
        stamp = stamp * 32 + (time.wHour & 0x1f);
        stamp = stamp * 64 + (time.wMinute & 0x3f);
        stamp = stamp * 64 + (time.wSecond & 0x3f);
        stamp = stamp * 8 + (time.wMilliseconds & 7);
        stamp = stamp * 0x24a1 + 0x24df;
        unsigned long hash = GFROR13(stamp);
        unsigned long remainder = range ? hash % range : 0;
        unsigned __int64 uid = (remainder + offset) & 0x3fffff;
        bool used = false;
        for (unsigned long i = 0; i < Maps.size(); ++i) {
            if (Maps[i].Used && Maps[i].MapUID == uid) {
                used = true;
                break;
            }
        }
        if (!used)
            return (unsigned long)uid;
    }
}

// ---- stubs and driver ----
static unsigned long g_hashes[4];
static int g_hashCalls = 0;
long __fastcall GFROR13(unsigned long value) { return (long)g_hashes[g_hashCalls++ & 3]; }
int main()
{
    static CMapInfo maps[3];
    memset(maps, 0, sizeof(maps));
    maps[0].Used = true;  maps[0].MapUID = 115;
    maps[1].Used = false; maps[1].MapUID = 215;
    maps[2].Used = true;  maps[2].MapUID = 999;
    static CWorldMap world;
    world.Maps._M_start = maps; world.Maps._M_finish = maps + 3; world.Maps._M_end_of_storage = maps + 3;
    world.CurrentMapUIDCount = 5;
    // 1) offset 5 * 3 = 15: hash 100 gives 115 (used, retry), hash 200 gives 215 (only an unused map has it)
    g_hashes[0] = 100; g_hashes[1] = 200; g_hashCalls = 0;
    unsigned long uid = world.AllocateMapUID(3);
    CHECK(uid == 215 && g_hashCalls == 2 && world.CurrentMapUIDCount == 6);
    // 2) the hash is reduced modulo 2^22 and the sum masked to 22 bits: offset 6 * 2 = 12
    g_hashes[0] = 0x00400000 + 0x3ffff8; g_hashCalls = 0;
    uid = world.AllocateMapUID(2);
    CHECK(uid == ((0x3ffff8 + 12) & 0x3fffff) && g_hashCalls == 1 && world.CurrentMapUIDCount == 7);
    // 3) no maps: the first UID is taken
    world.Maps._M_finish = maps;
    g_hashes[0] = 115; g_hashCalls = 0;
    uid = world.AllocateMapUID(0);
    CHECK(uid == 115 && g_hashCalls == 1);
    if (fails == 0) printf("004fce20_TEST PASS\n");
    return fails;
}