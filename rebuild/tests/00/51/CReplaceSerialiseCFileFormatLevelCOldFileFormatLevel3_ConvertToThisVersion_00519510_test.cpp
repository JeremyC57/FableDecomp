// Behaviour test for 0x00519510 CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ConvertToThisVersion: a record already at
// version 4 is left alone (no conversion call); an older one is replaced by the converted record (sizeof(T) bytes) and the
// converter's counted pointer is released (deleter + block free when the count drops to zero, nothing when shared).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __fastcall
#define __int64 long long
#endif
static int g_deletes, g_destroys, g_converts;
static unsigned char g_image[sizeof(unsigned char) * 0x20000];
static void __fastcall DestroyObject(void* p) { ++g_destroys; std::free(p); }
class CCPPointerInfo { public:
    unsigned int refs;                        // +0x0
    void (__fastcall* destroy)(void*);         // +0x4
    void* object;                              // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              object;                    // +0x0
    CCPPointerInfo* control;                   // +0x4
    CCountedPointer() : object(0), control(0) {}
    CCountedPointer(const CCountedPointer& o) : object(o.object), control(o.control) { if (control) ++control->refs; }
    ~CCountedPointer()
    {
        if (control) {
            --control->refs;
            if (control->refs == 0) {
                control->destroy(control->object);
                ++g_deletes; delete control;
            }
        }
    }
    T& operator*() const { return *object; }
};

#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned int ByteLength;                  // +0x00
    unsigned char SerialiseVersionNumber;      // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
    static void __fastcall ConvertToThisVersion(void* data);
};
class COldFileFormatLevel2;
class COldFileFormatLevel3 : public CReplaceSerialise<COldFileFormatLevel3, COldFileFormatLevel2> {
public:
    enum { VERSION = 3 };
    int MapDataOffset;                        // +0x05
    int ThingDataOffset;                      // +0x09
    int BrushDataOffset;                      // +0x0D
    int NavigationDataOffset;                 // +0x11
};
class CFileFormatLevel : public CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3> {
public:
    enum { VERSION = 4 };
    int MapDataOffset;                        // +0x05
    int ThingDataOffset;                      // +0x09
    int BrushDataOffset;                      // +0x0D
    int NavigationDataOffset;                 // +0x11
    int NavigationSectionsOffset;             // +0x15
};
#pragma pack(pop)

template <class T, class TOld> void __fastcall CReplaceSerialise<T, TOld>::ConvertToThisVersion(void* data)
{
    if (((CReplaceSerialise<T, TOld>*)data)->SerialiseVersionNumber != GetClassVersionNumber()) {
        *(T*)data = *ConvertPreviousUnknownVersionUp(data);
    }
}


static int g_shared;
static CCPPointerInfo* g_keep;
template <> CCountedPointer<CFileFormatLevel> __fastcall CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>::ConvertPreviousUnknownVersionUp(void* data)
{
    ++g_converts;
    CFileFormatLevel* obj = (CFileFormatLevel*)std::malloc(sizeof(CFileFormatLevel));
    std::memcpy(obj, g_image, sizeof(CFileFormatLevel));
    CCountedPointer<CFileFormatLevel> r;
    r.object = obj;
    r.control = new CCPPointerInfo;
    r.control->refs = g_shared ? 2 : 1; r.control->destroy = DestroyObject; r.control->object = obj;
    if (g_shared) g_keep = r.control;
    return r;
}
int main()
{
    const unsigned int n = sizeof(CFileFormatLevel);
    unsigned char* buf = (unsigned char*)std::malloc(n);
    for (unsigned int i = 0; i < n; ++i) { buf[i] = (unsigned char)(i * 7); g_image[i] = (unsigned char)(i * 13 + 1); }
    g_image[4] = 4;
    buf[4] = 4;
    CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>::ConvertToThisVersion(buf);
    int ok = g_converts == 0 && buf[0] == 0 && buf[5] == 35 && buf[n - 1] == (unsigned char)((n - 1) * 7);
    buf[4] = 4 - 1;
    CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>::ConvertToThisVersion(buf);
    ok &= g_converts == 1 && std::memcmp(buf, g_image, n) == 0 && g_destroys == 1 && g_deletes == 1;
    buf[4] = 1; g_shared = 1;
    CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>::ConvertToThisVersion(buf);
    ok &= g_converts == 2 && buf[4] == 4 && g_destroys == 1 && g_deletes == 1 && g_keep && g_keep->refs == 1;
    std::free(buf);
    if (!ok) { std::printf("00519510_TEST FAIL\n"); return 1; }
    std::printf("00519510_TEST PASS\n");
    return 0;
}