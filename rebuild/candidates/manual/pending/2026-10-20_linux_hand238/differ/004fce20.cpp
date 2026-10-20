// Grade: DIFFER(343v318) -- the clock hash and the 64-bit UID compare match; retail keeps the count in ebx and the range in edi (reloaded from its spill only on the retry path) and returns straight out of the search loop
// CWorldMap::AllocateMapUID(long) -- make a 22-bit map UID from the system clock (hashed), offset by map * allocation count, retrying until no used map has it
// Leaf: AllocateMapUID
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)
struct SYSTEMTIME {
    unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
};
extern "C" __declspec(dllimport) void __stdcall GetSystemTime(SYSTEMTIME* time);   // kernel32 (import 0x0143FE18)
long __fastcall GFROR13(unsigned long value);         // 0x00497890
struct CMapInfo {                                      // 0x48 bytes
    unsigned char _pad00[0x20];
    unsigned long MapUID;                              // +0x20
    bool          Used;                                // +0x24
    unsigned char _body[0x23];
};
template <class T> struct CArray {
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
        unsigned long i;
        for (i = 0; i < Maps.size(); ++i) {
            if (Maps[i].Used && Maps[i].MapUID == uid)
                break;
        }
        if (i == Maps.size())
            return (unsigned long)uid;
    }
}
