// Behaviour test for CSubTitle::~CSubTitle (0x009BDFA0): Sound, then FormattedText, then the text entry released.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
class CCharString { public: void* PStringData; ~CCharString() { g_log[g_n++] = 1; } };
class CWideString { public: void* PStringData; ~CWideString() { g_log[g_n++] = 2; } };
struct CTextEntry {
    virtual void v00() {}
    virtual void DeleteThis() { g_log[g_n++] = 3; deleted = 1; }
    long RefCount; int deleted;
};
template<class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer() { T* object = Object; if (object) { if (--object->RefCount == 0) object->DeleteThis(); Object = 0; } }
};
struct CSubTitle {
    CIVCountedPointer<CTextEntry> TextEntry; CWideString FormattedText; CCharString Sound;
    long FrameStart, FrameEnd; float StartTime, EndTime;
};
int main()
{
    CTextEntry e; e.RefCount = 2; e.deleted = 0;
    { CSubTitle s; s.TextEntry.Object = &e; }
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 2 || e.RefCount != 1 || e.deleted) { std::printf("009bdfa0_TEST FAIL\n"); return 1; }
    g_n = 0;
    { CSubTitle s; s.TextEntry.Object = &e; }
    if (g_n != 3 || g_log[2] != 3 || e.RefCount != 0 || !e.deleted) { std::printf("009bdfa0_TEST FAIL\n"); return 1; }
    g_n = 0;
    { CSubTitle s; s.TextEntry.Object = 0; }
    if (g_n != 2) { std::printf("009bdfa0_TEST FAIL\n"); return 1; }
    std::printf("009bdfa0_TEST PASS\n");
    return 0;
}