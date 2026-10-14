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
struct CRetailByteVector { unsigned char* m_pFirst; unsigned char* m_pLast; unsigned char* m_pEnd;
    long GetCount() const { return m_pLast - m_pFirst; }
    const void* GetData() const { return GetCount() != 0 ? (const void*)m_pFirst : (const void*)this; } };
struct CByteBlock { long m_type; long m_id; CRetailByteVector m_bytes; };
struct CStream : CDataOutputStream { void Write(const void* d, long l) { CDataOutputStream::Write(d, l); } void Write(const CByteBlock& b) { Write(b.m_bytes.GetData(), b.m_bytes.GetCount()); } };
int main()
{
    unsigned char bytes[3] = { 7, 8, 9 }; CByteBlock b = { 1, 2, { bytes, bytes + 3, bytes + 3 } };
    CStream out; out.Write(b);
    if (out.used != 3 || out.log[2] != 9 || out.calls != 1) { std::printf("00995eb0_TEST FAIL\n"); return 1; }
    CByteBlock e = { 1, 2, { 0, 0, 0 } }; out.Write(e); if (out.used != 3 || out.calls != 2) { std::printf("00995eb0_TEST FAIL\n"); return 1; }
    std::printf("00995eb0_TEST PASS\n"); return 0;
}