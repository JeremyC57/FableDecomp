#include <cstdio>
#include <cstring>
class CCharStringData { public: char* text; long length; };
static char g_EmptyCharString_0129AAF4[1] = { 0 };
class CCharString { public:
    CCharStringData* m_pStringData;
    const char* GetText() const { return m_pStringData == 0 ? g_EmptyCharString_0129AAF4 : m_pStringData->text; }
    long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->length; }
};
class CDataOutputStream { public:
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { }
    virtual void Write(const void* data, long length) { std::memcpy(log + used, data, length); used += length; ++calls; }
    CCharString NewLine;
    char log[256]; long used; long calls;
    CDataOutputStream() { used = 0; calls = 0; }
    void WriteFloat(float v) { Write(&v, 4); }
};
class C3DVector { public: float X, Y, Z; };
struct CStream : CDataOutputStream { void Write3DVector(const C3DVector& v) { WriteFloat(v.X); WriteFloat(v.Y); WriteFloat(v.Z); } };
int main()
{
    CStream out; C3DVector v = { 1.5f, -2.0f, 8.25f }; out.Write3DVector(v); float got[3]; std::memcpy(got, out.log, 12);
    if (out.used != 12 || out.calls != 3 || got[0] != 1.5f || got[1] != -2.0f || got[2] != 8.25f) { std::printf("00993fd0_TEST FAIL\n"); return 1; }
    std::printf("00993fd0_TEST PASS\n"); return 0;
}