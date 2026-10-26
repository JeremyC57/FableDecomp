// Behaviour test for 0x0051a5a0 CReplaceSerialise<COldFileFormatMapHeader6,COldFileFormatMapHeader5>::ConvertPreviousUnknownVersionUp:
// a version-5 map header converts directly; an older one goes through the version-5 link first (stubbed here); the
// version-6 header carries every version-5 field over and zeroes VersionNumber; count blocks are released.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 UInt64;
#else
typedef unsigned long long UInt64;
#endif

static int g_blocks = 0, g_deleted = 0, g_previousCalls = 0;
class CCPPointerInfo { public:
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
class COldFileFormatMapHeader5 { public:
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    UInt64 ThingUIDCount;
    unsigned int WorldSizeX;
    unsigned int WorldSizeY;
    bool UsingExtraMapCells;
    CFileLargeThemePalette LargeThemePalette;
};
class COldFileFormatMapHeader6 { public:
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    UInt64 ThingUIDCount;
    unsigned int WorldSizeX;
    unsigned int WorldSizeY;
    bool UsingExtraMapCells;
    CFileLargeThemePalette LargeThemePalette;
    unsigned int VersionNumber;
    void ConvertFromOld(const COldFileFormatMapHeader5* old)
    {
        ThingUIDCount = old->ThingUIDCount;
        WorldSizeX = old->WorldSizeX;
        WorldSizeY = old->WorldSizeY;
        UsingExtraMapCells = old->UsingExtraMapCells;
        LargeThemePalette = old->LargeThemePalette;
        VersionNumber = 0;
    }
};
#pragma pack(pop)

// stand-in for the version-5 link 0x005196E0
CCountedPointer<COldFileFormatMapHeader5> PreviousConvertPreviousUnknownVersionUp(const void* data)
{
    ++g_previousCalls;
    COldFileFormatMapHeader5* p = new COldFileFormatMapHeader5;
    p->SerialiseVersionNumber = 5;
    p->ThingUIDCount = 1;
    p->WorldSizeX = 100 + ((const unsigned char*)data)[4];
    p->WorldSizeY = 200;
    p->UsingExtraMapCells = true;
    p->LargeThemePalette.Storage[7] = 0x42;
    return CCountedPointer<COldFileFormatMapHeader5>(p);
}

CCountedPointer<COldFileFormatMapHeader6> ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<COldFileFormatMapHeader6> result(new COldFileFormatMapHeader6);
    CCountedPointer<COldFileFormatMapHeader5> held;
    COldFileFormatMapHeader5* old = (COldFileFormatMapHeader5*)data;
    if (((COldFileFormatMapHeader5*)data)->SerialiseVersionNumber != 5) {
        held = PreviousConvertPreviousUnknownVersionUp(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

int main()
{
    int ok = 1;
    if (sizeof(COldFileFormatMapHeader5) != 0x8416 || sizeof(COldFileFormatMapHeader6) != 0x841A) ok = 0;
    {
        COldFileFormatMapHeader5* v5 = new COldFileFormatMapHeader5;
        v5->SerialiseVersionNumber = 5; v5->ThingUIDCount = (((UInt64)1 << 32) | 5); v5->WorldSizeX = 7; v5->WorldSizeY = 9;
        v5->UsingExtraMapCells = false; v5->LargeThemePalette.Storage[0x83FF] = 0x11;
        CCountedPointer<COldFileFormatMapHeader6> r = ConvertPreviousUnknownVersionUp(v5);
        if (!r.object || r.object->ThingUIDCount != (((UInt64)1 << 32) | 5) || r.object->WorldSizeX != 7 || r.object->WorldSizeY != 9 ||
            r.object->UsingExtraMapCells || r.object->LargeThemePalette.Storage[0x83FF] != 0x11 || r.object->VersionNumber != 0) ok = 0;
        if (g_previousCalls != 0 || !r.control || r.control->refs != 1 || g_blocks != 1) ok = 0;
        delete v5;
    }
    if (g_blocks != 0 || g_deleted != 1) ok = 0;
    {
        unsigned char v4[5] = { 5, 0, 0, 0, 4 };   // a version-4 header
        CCountedPointer<COldFileFormatMapHeader6> r = ConvertPreviousUnknownVersionUp(v4);
        if (!r.object || r.object->ThingUIDCount != 1 || r.object->WorldSizeX != 104 || r.object->WorldSizeY != 200 ||
            !r.object->UsingExtraMapCells || r.object->LargeThemePalette.Storage[7] != 0x42 || r.object->VersionNumber != 0) ok = 0;
        if (g_previousCalls != 1 || g_blocks != 1 || g_deleted != 2) ok = 0;
    }
    if (g_blocks != 0 || g_deleted != 3) ok = 0;
    if (!ok) { std::printf("0051a5a0_TEST FAIL\n"); return 1; }
    std::printf("0051a5a0_TEST PASS\n");
    return 0;
}