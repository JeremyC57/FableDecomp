#include <cstdio>
#include <cstring>
struct CCharStringData { char* text; long length; };
static char g_EmptyCharString_0129AAF4[1] = { 0 };
struct CCharString {
    CCharStringData* m_pStringData;
    const char* GetText() const { return m_pStringData == 0 ? g_EmptyCharString_0129AAF4 : m_pStringData->text; }
    long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->length; }
};
struct CDataOutputStream {
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { }
    virtual void Write(const void* data, long length) { std::memcpy(log + used, data, length); used += length; ++calls; }
    CCharString NewLine;
    char log[256]; long used; long calls;
    CDataOutputStream() { used = 0; calls = 0; }
    void WriteNullTerminatedString(const CCharString& text) {
        const char* characters = g_EmptyCharString_0129AAF4; long length;
        if (text.m_pStringData) { characters = text.m_pStringData->text; length = text.m_pStringData->length; } else { length = 0; }
        Write(characters, length + 1); }
};
int main()
{
    char hello[] = "hello"; CCharStringData d = { hello, 5 }; CCharString s = { &d }, empty = { 0 };
    CDataOutputStream out; out.WriteNullTerminatedString(s);
    if (out.used != 6 || std::memcmp(out.log, "hello", 6) != 0 || out.calls != 1) { std::printf("00995ae0_TEST FAIL\n"); return 1; }
    out.WriteNullTerminatedString(empty); if (out.used != 7 || out.log[6] != 0) { std::printf("00995ae0_TEST FAIL\n"); return 1; }
    std::printf("00995ae0_TEST PASS\n"); return 0;
}
