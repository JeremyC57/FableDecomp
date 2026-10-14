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
};
struct CRGBColour { unsigned char R, G, B, A; CRGBColour(const CRGBColour& o) : R(o.R), G(o.G), B(o.B), A(o.A) { } CRGBColour(int r, int g, int b, int a) : R(r), G(g), B(b), A(a) { } };
struct CStream : CDataOutputStream { void WriteColour(const CRGBColour& c) { CRGBColour copy(c); Write(&copy, 4); } };
int main()
{
    CStream out; CRGBColour c(1, 2, 3, 4); out.WriteColour(c);
    if (out.used != 4 || out.log[0] != 1 || out.log[1] != 2 || out.log[2] != 3 || out.log[3] != 4 || out.calls != 1) { std::printf("00993f30_TEST FAIL\n"); return 1; }
    std::printf("00993f30_TEST PASS\n"); return 0;
}