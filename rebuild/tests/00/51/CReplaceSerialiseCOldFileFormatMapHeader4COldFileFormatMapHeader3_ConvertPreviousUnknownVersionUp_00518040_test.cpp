// Behaviour test for 0x00518040 CReplaceSerialise<Header4,Header3>::ConvertPreviousUnknownVersionUp: a version-3 record
// yields a fresh (palette-constructed) version-4 header with a single reference and no chain call; an older record
// first goes through the version-3 converter, whose temporary result is released (deleted) before returning.
#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
static int g_palettes, g_deletes, g_conv3;

class CFileLargeThemePalette {
public:
    unsigned char Storage[0x8400];
    CFileLargeThemePalette() { ++g_palettes; Storage[0] = 0x5A; }
};

class CCPPointerInfo { public:
    unsigned long refs;                                // +0x0
    void (__fastcall* destroy)(void*);                 // +0x4
    void* object;                                      // +0x8
    CCPPointerInfo(void* p, void (__fastcall* d)(void*)) : refs(1), destroy(d), object(p) {}
};
template <class T> class CCountedPointer {
public:
    T*              object;                            // +0x0
    CCPPointerInfo* control;                           // +0x4
    static void __fastcall DeleteData(void* data) { ++g_deletes; delete (T*)data; }
    CCountedPointer() : object(0), control(0) {}
    CCountedPointer(T* p) : object(p), control(p ? new CCPPointerInfo(p, DeleteData) : 0) {}
    CCountedPointer(const CCountedPointer& x) : object(x.object), control(x.control)
    {
        if (control)
            ++control->refs;
    }
    CCountedPointer& operator=(const CCountedPointer& x)
    {
        CCPPointerInfo* c = x.control;
        T* o = x.object;
        if (control != c) {
            if (control && --control->refs == 0) {
                control->destroy(control->object);
                operator delete(control);
            }
            object = o;
            control = c;
            if (control)
                ++control->refs;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (control) {
            --control->refs;
            if (control->refs == 0) {
                control->destroy(control->object);
                operator delete(control);
            }
        }
    }
};

#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned int  ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
};
class COldFileFormatMapHeader2 {};
class COldFileFormatMapHeader3 : public CReplaceSerialise<COldFileFormatMapHeader3, COldFileFormatMapHeader2> {
public:
    enum { VERSION = 3 };                              // 5 bytes: the serialise header only
};
class COldFileFormatMapHeader4 : public CReplaceSerialise<COldFileFormatMapHeader4, COldFileFormatMapHeader3> {
public:
    enum { VERSION = 4 };
    unsigned int           WorldSizeX;                 // +0x05
    unsigned int           WorldSizeY;                 // +0x09
    bool                   UsingExtraMapCells;         // +0x0D
    CFileLargeThemePalette LargeThemePalette;          // +0x0E (0x840E bytes in all)
    COldFileFormatMapHeader4() {}
    void ConvertFromOld(const COldFileFormatMapHeader3*) {}   // version 3 carries no fields to bring forward
};
#pragma pack(pop)

template <>
CCountedPointer<COldFileFormatMapHeader3> __fastcall
CReplaceSerialise<COldFileFormatMapHeader3, COldFileFormatMapHeader2>::ConvertPreviousUnknownVersionUp(void* data)
{
    ++g_conv3;
    COldFileFormatMapHeader3* h = new COldFileFormatMapHeader3;
    h->ByteLength = 5; h->SerialiseVersionNumber = 3;
    return CCountedPointer<COldFileFormatMapHeader3>(h);
}
template <class T, class TOld>
CCountedPointer<T> __fastcall CReplaceSerialise<T, TOld>::ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<T> result(new T);
    CCountedPointer<TOld> held;
    TOld* old = (TOld*)data;
    if (((TOld*)data)->SerialiseVersionNumber != TOld::GetClassVersionNumber()) {
        held = TOld::ConvertPreviousUnknownVersionUp(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

typedef CReplaceSerialise<COldFileFormatMapHeader4, COldFileFormatMapHeader3> R4;
int main()
{
    int ok = sizeof(COldFileFormatMapHeader4) == 0x840E && sizeof(COldFileFormatMapHeader3) == 5;
    unsigned char rec[16]; std::memset(rec, 0, sizeof(rec)); rec[4] = 3;
    {
        CCountedPointer<COldFileFormatMapHeader4> p = R4::ConvertPreviousUnknownVersionUp(rec);
        ok &= p.object != 0 && p.control != 0 && p.control->refs == 1 && p.control->object == p.object;
        ok &= p.object->LargeThemePalette.Storage[0] == 0x5A && g_palettes == 1 && g_conv3 == 0 && g_deletes == 0;
    }
    ok &= g_deletes == 1;
    rec[4] = 1;
    {
        CCountedPointer<COldFileFormatMapHeader4> p = R4::ConvertPreviousUnknownVersionUp(rec);
        ok &= g_conv3 == 1 && g_deletes == 2 && p.control->refs == 1 && g_palettes == 2;   // the version-3 temporary is gone
    }
    ok &= g_deletes == 3;
    if (!ok) { std::printf("00518040_TEST FAIL\n"); return 1; }
    std::printf("00518040_TEST PASS\n");
    return 0;
}