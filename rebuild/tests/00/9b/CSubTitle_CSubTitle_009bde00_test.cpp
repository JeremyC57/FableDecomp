// behaviour test for 009bde00: CSubTitle's copy constructor shares the text entry (its count goes up,
// a null entry is copied without one), deep-copies both strings and copies the frame/time range.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cwchar>

class CCharString {
public:
    char* PStringData;
    explicit CCharString(const char* text) { PStringData = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(PStringData, text); }
    CCharString(const CCharString& other) { PStringData = (char*)std::malloc(std::strlen(other.PStringData) + 1); std::strcpy(PStringData, other.PStringData); }
    ~CCharString() { std::free(PStringData); }
};
class CWideString {
public:
    long* PStringData;
    explicit CWideString(long v) { PStringData = (long*)std::malloc(sizeof(long)); *PStringData = v; }
    CWideString(const CWideString& other) { PStringData = (long*)std::malloc(sizeof(long)); *PStringData = *other.PStringData; }
    ~CWideString() { std::free(PStringData); }
};

struct CTextEntry {
    virtual void v00() { }
    virtual void DeleteThis() { }
    long RefCount;
};

template<class T> class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};

struct CSubTitle {
    CIVCountedPointer<CTextEntry> TextEntry;
    CWideString FormattedText;
    CCharString Sound;
    long  FrameStart;
    long  FrameEnd;
    float StartTime;
    float EndTime;
    CSubTitle() : FormattedText(5), Sound("vo_01"), FrameStart(0), FrameEnd(0), StartTime(0), EndTime(0) { }
    CSubTitle(const CSubTitle& other)
        : TextEntry(other.TextEntry), FormattedText(other.FormattedText), Sound(other.Sound),
          FrameStart(other.FrameStart), FrameEnd(other.FrameEnd), StartTime(other.StartTime), EndTime(other.EndTime)
    {
    }
};

int main()
{
    CTextEntry entry;
    CSubTitle source;
    CSubTitle blank;
    int ok = 1;
    entry.RefCount = 1;
    source.TextEntry.Object = &entry;
    source.FrameStart = 10;
    source.FrameEnd = 90;
    source.StartTime = 1.5f;
    source.EndTime = 3.25f;
    CSubTitle copy(source);
    if (copy.TextEntry.Object != &entry || entry.RefCount != 2)
        ok = 0;
    if (*copy.FormattedText.PStringData != 5 || copy.FormattedText.PStringData == source.FormattedText.PStringData)
        ok = 0;
    if (std::strcmp(copy.Sound.PStringData, "vo_01") != 0)
        ok = 0;
    if (copy.FrameStart != 10 || copy.FrameEnd != 90 || copy.StartTime != 1.5f || copy.EndTime != 3.25f)
        ok = 0;
    CSubTitle copy2(blank);
    if (copy2.TextEntry.Object != 0 || entry.RefCount != 2)
        ok = 0;
    if (!ok) {
        std::printf("009bde00_TEST FAIL\n");
        return 1;
    }
    std::printf("009bde00_TEST PASS\n");
    return 0;
}