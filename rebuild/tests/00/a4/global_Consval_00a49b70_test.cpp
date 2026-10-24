// behaviour test for 00a49b70: placement-copy-construct a CLipSyncPhonemeSample into raw storage: symbol
// deep-copied, id copied, the shared speech frame's count bumped (a null frame copied without one).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>

class CCharString {
public:
    char* PStringData;
    explicit CCharString(const char* text) { PStringData = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(PStringData, text); }
    CCharString(const CCharString& other) { PStringData = (char*)std::malloc(std::strlen(other.PStringData) + 1); std::strcpy(PStringData, other.PStringData); }
    ~CCharString() { std::free(PStringData); }
};

struct CCPPointerInfo {
    long RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};

template<class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& p) : Data(p.Data), Info(p.Info) { if (Info) ++Info->RefCount; }
};

struct CSpeechFrame { long m_id; };

struct CLipSyncPhonemeSample {
    CCharString                   Symbol;
    unsigned long                 ID;
    CCountedPointer<CSpeechFrame> SampleFrame;
    explicit CLipSyncPhonemeSample(const char* s) : Symbol(s), ID(0) { }
};

static void Cons_val(CLipSyncPhonemeSample* ptr, const CLipSyncPhonemeSample& value)
{
    new ((void*)ptr) CLipSyncPhonemeSample(value);
}

int main()
{
    int ok = 1;
    CSpeechFrame frame;
    CCPPointerInfo info;
    CLipSyncPhonemeSample source("AA");
    CLipSyncPhonemeSample bare("M");
    void* raw = ::operator new(sizeof(CLipSyncPhonemeSample) * 2);
    CLipSyncPhonemeSample* dest = (CLipSyncPhonemeSample*)raw;
    frame.m_id = 4;
    info.RefCount = 1;
    info.DeleteFunc = 0;
    info.Data = &frame;
    source.ID = 17;
    source.SampleFrame.Data = &frame;
    source.SampleFrame.Info = &info;
    Cons_val(dest, source);
    if (std::strcmp(dest->Symbol.PStringData, "AA") != 0 || dest->Symbol.PStringData == source.Symbol.PStringData || dest->ID != 17)
        ok = 0;
    if (dest->SampleFrame.Data != &frame || dest->SampleFrame.Info != &info || info.RefCount != 2)
        ok = 0;
    Cons_val(dest + 1, bare);
    if (dest[1].SampleFrame.Info != 0 || info.RefCount != 2)
        ok = 0;
    dest[0].~CLipSyncPhonemeSample();
    dest[1].~CLipSyncPhonemeSample();
    ::operator delete(raw);
    if (!ok) {
        std::printf("00a49b70_TEST FAIL\n");
        return 1;
    }
    std::printf("00a49b70_TEST PASS\n");
    return 0;
}