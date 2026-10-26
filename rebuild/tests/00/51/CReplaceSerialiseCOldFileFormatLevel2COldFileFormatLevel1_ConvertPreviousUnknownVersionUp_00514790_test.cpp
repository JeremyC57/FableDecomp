// Behaviour test for 0x00514790 CReplaceSerialise<COldFileFormatLevel2,COldFileFormatLevel1>::ConvertPreviousUnknownVersionUp:
// a version-1 record converts directly; an older record goes through the version-1 link first (stubbed here); the
// version-2 header carries MapDataOffset/ThingDataOffset over and zeroes BrushDataOffset; every count block is released.
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
class COldFileFormatLevel1 { public:
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    int MapDataOffset;
    int ThingDataOffset;
};
class COldFileFormatLevel2 { public:
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    int MapDataOffset;
    int ThingDataOffset;
    int BrushDataOffset;
    void ConvertFromOld(const COldFileFormatLevel1* old)
    {
        MapDataOffset = old->MapDataOffset;
        ThingDataOffset = old->ThingDataOffset;
        BrushDataOffset = 0;
    }
};
#pragma pack(pop)

// stand-in for the version-1 link 0x00512BC0: builds a version-1 record from an older one
CCountedPointer<COldFileFormatLevel1> Level1ConvertPreviousUnknownVersionUp(const void* data)
{
    ++g_previousCalls;
    COldFileFormatLevel1* p = new COldFileFormatLevel1;
    p->SerialiseVersionNumber = 1;
    p->MapDataOffset = 100 + ((const unsigned char*)data)[4];
    p->ThingDataOffset = 200;
    return CCountedPointer<COldFileFormatLevel1>(p);
}

CCountedPointer<COldFileFormatLevel2> ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<COldFileFormatLevel2> result(new COldFileFormatLevel2);
    CCountedPointer<COldFileFormatLevel1> held;
    COldFileFormatLevel1* old = (COldFileFormatLevel1*)data;
    if (((COldFileFormatLevel1*)data)->SerialiseVersionNumber != 1) {
        held = Level1ConvertPreviousUnknownVersionUp(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

int main()
{
    int ok = 1;
    if (sizeof(COldFileFormatLevel1) != 13 || sizeof(COldFileFormatLevel2) != 17) ok = 0;
    {
        COldFileFormatLevel1 v1; v1.ByteLength = 13; v1.SerialiseVersionNumber = 1; v1.MapDataOffset = 7; v1.ThingDataOffset = 9;
        CCountedPointer<COldFileFormatLevel2> r = ConvertPreviousUnknownVersionUp(&v1);
        if (!r.object || r.object->MapDataOffset != 7 || r.object->ThingDataOffset != 9 || r.object->BrushDataOffset != 0) ok = 0;
        if (g_previousCalls != 0 || !r.control || r.control->refs != 1 || g_blocks != 1) ok = 0;
    }
    if (g_blocks != 0 || g_deleted != 1) ok = 0;
    {
        unsigned char v0[8] = { 5, 0, 0, 0, 0, 0, 0, 0 };   // a version-0 record
        CCountedPointer<COldFileFormatLevel2> r = ConvertPreviousUnknownVersionUp(v0);
        if (!r.object || r.object->MapDataOffset != 100 || r.object->ThingDataOffset != 200 || r.object->BrushDataOffset != 0) ok = 0;
        if (g_previousCalls != 1 || g_blocks != 1 || g_deleted != 2) ok = 0;   // the held version-1 record is gone
    }
    if (g_blocks != 0 || g_deleted != 3) ok = 0;
    if (!ok) { std::printf("00514790_TEST FAIL\n"); return 1; }
    std::printf("00514790_TEST PASS\n");
    return 0;
}