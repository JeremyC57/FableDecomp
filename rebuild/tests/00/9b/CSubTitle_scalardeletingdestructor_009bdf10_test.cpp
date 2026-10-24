#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
static int g_order[8];
static int g_n = 0;

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                          // 0x0099B510
};

namespace NGameText {
struct CTextEntry {                                          // intrusive virtual counted pointee
    virtual void v00();
    virtual void DeleteThis();                               // slot 1 (+4)
    long RefCount;                                           // +0x04
};
}

template<class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->DeleteThis();
            Object = 0;
        }
    }
};

struct CSubTitle {                                           // 0x1c bytes
    CIVCountedPointer<NGameText::CTextEntry> TextEntry;      // +0x00 CIVCountedPointer<const NGameText::CTextEntry>
    CWideString FormattedText;                               // +0x04
    CCharString Sound;                                       // +0x08
    long  FrameStart;                                        // +0x0c
    long  FrameEnd;                                          // +0x10
    float StartTime;                                         // +0x14
    float EndTime;                                           // +0x18
};

void DeleteSubTitle(CSubTitle* subTitle)
{
    delete subTitle;
}

CCharString::~CCharString() { g_order[g_n++] = 1; }
CWideString::~CWideString() { g_order[g_n++] = 2; }
struct CTestEntry : NGameText::CTextEntry {
    virtual void v00() { }
    virtual void DeleteThis() { g_order[g_n++] = 3; }
};
void NGameText::CTextEntry::v00() { }
void NGameText::CTextEntry::DeleteThis() { }
int main()
{
    CTestEntry entry; entry.RefCount = 1;
    CSubTitle* s = new CSubTitle;
    s->TextEntry.Object = &entry;
    DeleteSubTitle(s);                               // string, wide string, then the last reference deletes the entry
    if (g_n != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || entry.RefCount != 0) {
        std::printf("009bdf10_TEST FAIL\n");
        return 1;
    }
    g_n = 0; entry.RefCount = 2;
    s = new CSubTitle;
    s->TextEntry.Object = &entry;
    DeleteSubTitle(s);                               // still referenced elsewhere: no delete
    if (g_n != 2 || entry.RefCount != 1) {
        std::printf("009bdf10_TEST FAIL\n");
        return 1;
    }
    std::printf("009bdf10_TEST PASS\n");
    return 0;
}