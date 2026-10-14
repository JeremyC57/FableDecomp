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
    void WriteFloat(float v) { Write(&v, 4); }
};
struct C2DVector { float X, Y; };
struct CStream : CDataOutputStream { void Write2DVector(const C2DVector& v) { WriteFloat(v.X); WriteFloat(v.Y); } };
int main()
{
    CStream out; C2DVector v = { 1.5f, -2.0f }; out.Write2DVector(v); float got[2]; std::memcpy(got, out.log, 8);
    if (out.used != 8 || out.calls != 2 || got[0] != 1.5f || got[1] != -2.0f) { std::printf("00993f90_TEST FAIL\n"); return 1; }
    std::printf("00993f90_TEST PASS\n"); return 0;
}