// Behaviour test for 0x00515e90 CReplaceSerialise<COldFileFormatLevel3,COldFileFormatLevel2>::ConvertPreviousUnknownVersionUp:
// a version-2 record converts directly; an older record goes through the version-2 link first (stubbed here); the
// version-3 header carries the three offsets over and zeroes NavigationDataOffset; every count block is released.
#include <cstdio>
#include <cstdlib>

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
class COldFileFormatLevel2 { public:
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    int MapDataOffset;
    int ThingDataOffset;
    int BrushDataOffset;
};
class COldFileFormatLevel3 { public:
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    int MapDataOffset;
    int ThingDataOffset;
    int BrushDataOffset;
    int NavigationDataOffset;
    void ConvertFromOld(const COldFileFormatLevel2* old)
    {
        MapDataOffset = old->MapDataOffset;
        ThingDataOffset = old->ThingDataOffset;
        BrushDataOffset = old->BrushDataOffset;
        NavigationDataOffset = 0;
    }
};
#pragma pack(pop)

// stand-in for the version-2 link 0x00514790: builds a version-2 record from an older one
CCountedPointer<COldFileFormatLevel2> Level2ConvertPreviousUnknownVersionUp(const void* data)
{
    ++g_previousCalls;
    COldFileFormatLevel2* p = new COldFileFormatLevel2;
    p->SerialiseVersionNumber = 2;
    p->MapDataOffset = 100 + ((const unsigned char*)data)[4];
    p->ThingDataOffset = 200;
    p->BrushDataOffset = 300;
    return CCountedPointer<COldFileFormatLevel2>(p);
}

CCountedPointer<COldFileFormatLevel3> ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<COldFileFormatLevel3> result(new COldFileFormatLevel3);
    CCountedPointer<COldFileFormatLevel2> held;
    COldFileFormatLevel2* old = (COldFileFormatLevel2*)data;
    if (((COldFileFormatLevel2*)data)->SerialiseVersionNumber != 2) {
        held = Level2ConvertPreviousUnknownVersionUp(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

int main()
{
    int ok = 1;
    if (sizeof(COldFileFormatLevel2) != 17 || sizeof(COldFileFormatLevel3) != 21) ok = 0;
    {
        COldFileFormatLevel2 v2; v2.ByteLength = 17; v2.SerialiseVersionNumber = 2; v2.MapDataOffset = 7; v2.ThingDataOffset = 9; v2.BrushDataOffset = 11;
        CCountedPointer<COldFileFormatLevel3> r = ConvertPreviousUnknownVersionUp(&v2);
        if (!r.object || r.object->MapDataOffset != 7 || r.object->ThingDataOffset != 9 || r.object->BrushDataOffset != 11 ||
            r.object->NavigationDataOffset != 0) ok = 0;
        if (g_previousCalls != 0 || !r.control || r.control->refs != 1 || g_blocks != 1) ok = 0;
    }
    if (g_blocks != 0 || g_deleted != 1) ok = 0;
    {
        unsigned char v1[13] = { 13, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };   // a version-1 record
        CCountedPointer<COldFileFormatLevel3> r = ConvertPreviousUnknownVersionUp(v1);
        if (!r.object || r.object->MapDataOffset != 101 || r.object->ThingDataOffset != 200 || r.object->BrushDataOffset != 300 ||
            r.object->NavigationDataOffset != 0) ok = 0;
        if (g_previousCalls != 1 || g_blocks != 1 || g_deleted != 2) ok = 0;
    }
    if (g_blocks != 0 || g_deleted != 3) ok = 0;
    if (!ok) { std::printf("00515e90_TEST FAIL\n"); return 1; }
    std::printf("00515e90_TEST PASS\n");
    return 0;
}