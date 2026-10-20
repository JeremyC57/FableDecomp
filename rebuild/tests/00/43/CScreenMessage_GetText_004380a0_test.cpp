#include <cstdio>
#include <cstring>
// Behaviour model of CScreenMessage::GetText @ 0x004380A0 (copy of the source logic over stand-in string classes).
static const char g_szEmpty[] = "";
struct CWideString;
struct CCharStringData { const char* Chars; unsigned long Length; char buf[64]; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length)
    {
        PStringData = new CCharStringData; std::strcpy(PStringData->buf, text);
        PStringData->Chars = PStringData->buf; PStringData->Length = (unsigned long)(length < 0 ? std::strlen(text) : length);
    }
    CCharString(const CCharString& o) { PStringData = new CCharStringData(*o.PStringData); PStringData->Chars = PStringData->buf; }
    ~CCharString() { delete PStringData; }
    CCharString GetSubString(unsigned long start, unsigned long count) const
    {
        char tmp[64]; unsigned long n = PStringData->Length > start ? PStringData->Length - start : 0;
        if (count < n) n = count;
        std::memcpy(tmp, PStringData->Chars + start, n); tmp[n] = 0;
        return CCharString(tmp, -1);
    }
    CWideString ToWideString() const;
    const char* GetCharPtr() const { return PStringData == 0 ? g_szEmpty : PStringData->Chars; }
    bool BeginsWith(const CCharString& prefix) const
    {
        const char* prefixText; unsigned long prefixLength;
        if (prefix.PStringData == 0) { prefixLength = 0; prefixText = g_szEmpty; }
        else { prefixLength = prefix.PStringData->Length; prefixText = prefix.PStringData->Chars; }
        return std::strncmp(GetCharPtr(), prefixText, prefixLength) ? false : true;
    }
};
struct CWideString {                                 // stand-in: narrow storage
    char buf[64];
    CWideString(const char* s) { std::strcpy(buf, s); }
    CWideString(const CWideString& o) { std::strcpy(buf, o.buf); }
    CWideString& operator=(const CWideString& o) { std::strcpy(buf, o.buf); return *this; }
    long GetLength() const { return (long)std::strlen(buf); }
    void SetLength(long n) { buf[n] = 0; }
    CCharString ConvertToCharString() const { return CCharString(buf, -1); }
};
CWideString CCharString::ToWideString() const { return CWideString(GetCharPtr()); }
struct CScreenMessage {
    CWideString Text; long FramesToShowFor, NumberOfCharactersShowing, LinesInText;
    CScreenMessage(const char* t, long n) : Text(t), FramesToShowFor(0), NumberOfCharactersShowing(n), LinesInText(1) {}
    CWideString GetText() const
    {
        CWideString text(Text);
        bool hasPrefix;
        {
            CCharString prefix("(HC)", -1);
            hasPrefix = text.ConvertToCharString().BeginsWith(prefix);
        }
        if (hasPrefix)
            text = Text.ConvertToCharString().GetSubString(4, Text.GetLength()).ToWideString();
        if (NumberOfCharactersShowing < text.GetLength())
            text.SetLength(NumberOfCharactersShowing);
        return text;
    }
};
int main()
{
    int ok = std::strcmp(CScreenMessage("(HC)Hello hero", 100).GetText().buf, "Hello hero") == 0
          && std::strcmp(CScreenMessage("(HC)Hello hero", 5).GetText().buf, "Hello") == 0
          && std::strcmp(CScreenMessage("Plain text", 5).GetText().buf, "Plain") == 0
          && std::strcmp(CScreenMessage("(HX)Other", 100).GetText().buf, "(HX)Other") == 0
          && std::strcmp(CScreenMessage("(HC)", 100).GetText().buf, "") == 0;
    if (!ok) { std::printf("004380a0_TEST FAIL\n"); return 1; }
    std::printf("004380a0_TEST PASS\n");
    return 0;
}