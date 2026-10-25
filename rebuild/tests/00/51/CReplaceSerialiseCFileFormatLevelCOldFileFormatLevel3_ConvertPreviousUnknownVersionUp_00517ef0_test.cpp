// Behaviour test for 0x00517ef0 CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ConvertPreviousUnknownVersionUp:
// a version-3 record converts directly; an older record goes through the version-3 link first (stubbed here); the
// version-4 header carries the four offsets over and zeroes NavigationSectionsOffset; every count block is released.
#include <cstdio>
#include <cstdlib>

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
struct COldFileFormatLevel3 {
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    int MapDataOffset;
    int ThingDataOffset;
    int BrushDataOffset;
    int NavigationDataOffset;
};
struct CFileFormatLevel {
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    int MapDataOffset;
    int ThingDataOffset;
    int BrushDataOffset;
    int NavigationDataOffset;
    int NavigationSectionsOffset;
    void ConvertFromOld(const COldFileFormatLevel3* old)
    {
        MapDataOffset = old->MapDataOffset;
        ThingDataOffset = old->ThingDataOffset;
        BrushDataOffset = old->BrushDataOffset;
        NavigationDataOffset = old->NavigationDataOffset;
        NavigationSectionsOffset = 0;
    }
};
#pragma pack(pop)

// stand-in for the version-3 link 0x00515E90: builds a version-3 record from an older one
CCountedPointer<COldFileFormatLevel3> Level3ConvertPreviousUnknownVersionUp(const void* data)
{
    ++g_previousCalls;
    COldFileFormatLevel3* p = new COldFileFormatLevel3;
    p->SerialiseVersionNumber = 3;
    p->MapDataOffset = 100 + ((const unsigned char*)data)[4];
    p->ThingDataOffset = 200;
    p->BrushDataOffset = 300;
    p->NavigationDataOffset = 400;
    return CCountedPointer<COldFileFormatLevel3>(p);
}

CCountedPointer<CFileFormatLevel> ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<CFileFormatLevel> result(new CFileFormatLevel);
    CCountedPointer<COldFileFormatLevel3> held;
    COldFileFormatLevel3* old = (COldFileFormatLevel3*)data;
    if (((COldFileFormatLevel3*)data)->SerialiseVersionNumber != 3) {
        held = Level3ConvertPreviousUnknownVersionUp(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

int main()
{
    int ok = 1;
    if (sizeof(COldFileFormatLevel3) != 21 || sizeof(CFileFormatLevel) != 25) ok = 0;
    {
        COldFileFormatLevel3 v3; v3.ByteLength = 21; v3.SerialiseVersionNumber = 3; v3.MapDataOffset = 7; v3.ThingDataOffset = 9;
        v3.BrushDataOffset = 11; v3.NavigationDataOffset = 13;
        CCountedPointer<CFileFormatLevel> r = ConvertPreviousUnknownVersionUp(&v3);
        if (!r.object || r.object->MapDataOffset != 7 || r.object->ThingDataOffset != 9 || r.object->BrushDataOffset != 11 ||
            r.object->NavigationDataOffset != 13 || r.object->NavigationSectionsOffset != 0) ok = 0;
        if (g_previousCalls != 0 || !r.control || r.control->refs != 1 || g_blocks != 1) ok = 0;
    }
    if (g_blocks != 0 || g_deleted != 1) ok = 0;
    {
        unsigned char v2[17] = { 17, 0, 0, 0, 2 };   // a version-2 record
        CCountedPointer<CFileFormatLevel> r = ConvertPreviousUnknownVersionUp(v2);
        if (!r.object || r.object->MapDataOffset != 102 || r.object->ThingDataOffset != 200 || r.object->BrushDataOffset != 300 ||
            r.object->NavigationDataOffset != 400 || r.object->NavigationSectionsOffset != 0) ok = 0;
        if (g_previousCalls != 1 || g_blocks != 1 || g_deleted != 2) ok = 0;
    }
    if (g_blocks != 0 || g_deleted != 3) ok = 0;
    if (!ok) { std::printf("00517ef0_TEST FAIL\n"); return 1; }
    std::printf("00517ef0_TEST PASS\n");
    return 0;
}