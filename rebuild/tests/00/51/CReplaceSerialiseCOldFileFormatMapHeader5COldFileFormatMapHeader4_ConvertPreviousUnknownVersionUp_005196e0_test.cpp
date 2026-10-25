// Behaviour test for 0x005196e0 CReplaceSerialise<COldFileFormatMapHeader5,COldFileFormatMapHeader4>::ConvertPreviousUnknownVersionUp:
// a version-4 map header converts directly; an older one goes through the version-4 link first (stubbed here); the
// version-5 header starts ThingUIDCount at 1 and carries the world size, flag and palette over; count blocks are released.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 UInt64;
#else
typedef unsigned long long UInt64;
#endif

static int g_blocks = 0, g_deleted = 0, g_previousCalls = 0;
struct CCPPointerInfo {
    unsigned int refs;
    void (*destroy)(void*);
    void* object;
};
template <class T> class CCountedPointer {
public:
    T* object;
    CCPPointerInfo* control;
    static void DeleteData(void* data) { delete (T*)data; ++g_deleted; }
    CCountedPointer() : object(0), control(0) {}
    CCountedPointer(T* p) : object(p), control(0)
    {
        if (p) { control = new CCPPointerInfo; control->refs = 1; control->destroy = DeleteData; control->object = p; ++g_blocks; }
    }
    CCountedPointer(const CCountedPointer& x) : object(x.object), control(x.control) { if (control) ++control->refs; }
    CCountedPointer& operator=(const CCountedPointer& x)
    {
        CCPPointerInfo* c = x.control;
        T* o = x.object;
        if (control != c) {
            Release();
            object = o;
            control = c;
            if (control) ++control->refs;
        }
        return *this;
    }
    void Release()
    {
        if (control && --control->refs == 0) { control->destroy(control->object); delete control; --g_blocks; }
    }
    ~CCountedPointer() { Release(); }
};

#pragma pack(push, 1)
struct CFileLargeThemePalette {
    unsigned char Storage[0x8400];
    CFileLargeThemePalette() { std::memset(Storage, 0xAB, sizeof(Storage)); }   // the real ctor 0x0050EE40 fills a default palette
};
struct COldFileFormatMapHeader4 {
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    unsigned int WorldSizeX;
    unsigned int WorldSizeY;
    bool UsingExtraMapCells;
    CFileLargeThemePalette LargeThemePalette;
};
struct COldFileFormatMapHeader5 {
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    UInt64 ThingUIDCount;
    unsigned int WorldSizeX;
    unsigned int WorldSizeY;
    bool UsingExtraMapCells;
    CFileLargeThemePalette LargeThemePalette;
    void ConvertFromOld(const COldFileFormatMapHeader4* old)
    {
        ThingUIDCount = 1;
        WorldSizeX = old->WorldSizeX;
        WorldSizeY = old->WorldSizeY;
        UsingExtraMapCells = old->UsingExtraMapCells;
        LargeThemePalette = old->LargeThemePalette;
    }
};
#pragma pack(pop)

// stand-in for the version-4 link 0x00518040
CCountedPointer<COldFileFormatMapHeader4> PreviousConvertPreviousUnknownVersionUp(const void* data)
{
    ++g_previousCalls;
    COldFileFormatMapHeader4* p = new COldFileFormatMapHeader4;
    p->SerialiseVersionNumber = 4;
    p->WorldSizeX = 100 + ((const unsigned char*)data)[4];
    p->WorldSizeY = 200;
    p->UsingExtraMapCells = true;
    p->LargeThemePalette.Storage[7] = 0x42;
    return CCountedPointer<COldFileFormatMapHeader4>(p);
}

CCountedPointer<COldFileFormatMapHeader5> ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<COldFileFormatMapHeader5> result(new COldFileFormatMapHeader5);
    CCountedPointer<COldFileFormatMapHeader4> held;
    COldFileFormatMapHeader4* old = (COldFileFormatMapHeader4*)data;
    if (((COldFileFormatMapHeader4*)data)->SerialiseVersionNumber != 4) {
        held = PreviousConvertPreviousUnknownVersionUp(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

int main()
{
    int ok = 1;
    if (sizeof(COldFileFormatMapHeader4) != 0x840E || sizeof(COldFileFormatMapHeader5) != 0x8416) ok = 0;
    {
        COldFileFormatMapHeader4* v4 = new COldFileFormatMapHeader4;
        v4->SerialiseVersionNumber = 4; v4->WorldSizeX = 7; v4->WorldSizeY = 9; v4->UsingExtraMapCells = false;
        v4->LargeThemePalette.Storage[0x83FF] = 0x11;
        CCountedPointer<COldFileFormatMapHeader5> r = ConvertPreviousUnknownVersionUp(v4);
        if (!r.object || r.object->ThingUIDCount != 1 || r.object->WorldSizeX != 7 || r.object->WorldSizeY != 9 ||
            r.object->UsingExtraMapCells || r.object->LargeThemePalette.Storage[0x83FF] != 0x11 ||
            r.object->LargeThemePalette.Storage[0] != 0xAB) ok = 0;
        if (g_previousCalls != 0 || !r.control || r.control->refs != 1 || g_blocks != 1) ok = 0;
        delete v4;
    }
    if (g_blocks != 0 || g_deleted != 1) ok = 0;
    {
        unsigned char v3[5] = { 5, 0, 0, 0, 3 };   // a version-3 header
        CCountedPointer<COldFileFormatMapHeader5> r = ConvertPreviousUnknownVersionUp(v3);
        if (!r.object || r.object->ThingUIDCount != 1 || r.object->WorldSizeX != 103 || r.object->WorldSizeY != 200 ||
            !r.object->UsingExtraMapCells || r.object->LargeThemePalette.Storage[7] != 0x42) ok = 0;
        if (g_previousCalls != 1 || g_blocks != 1 || g_deleted != 2) ok = 0;
    }
    if (g_blocks != 0 || g_deleted != 3) ok = 0;
    if (!ok) { std::printf("005196e0_TEST FAIL\n"); return 1; }
    std::printf("005196e0_TEST PASS\n");
    return 0;
}