#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 0047cf3f: TransferBinaryIn stores the vector read from the stream through the output pointer.
struct C3DVector { float x; float y; float z; };
static int g_reads = 0;
struct CMemoryDataInputStream {
    float m_next;
    C3DVector Read3DVector() { C3DVector v; v.x = m_next; v.y = m_next + 1; v.z = m_next + 2; ++g_reads; return v; }
};
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value);
};
template <> void CPersistTraits<C3DVector>::TransferBinaryIn(CMemoryDataInputStream* stream, C3DVector* value)
{
    *value = stream->Read3DVector();
}
int main()
{
    CMemoryDataInputStream s; s.m_next = 10.0f;
    CPersistTraits<C3DVector> t;
    C3DVector out; out.x = out.y = out.z = 0;
    t.TransferBinaryIn(&s, &out);
    if (g_reads != 1 || out.x != 10.0f || out.y != 11.0f || out.z != 12.0f) { std::printf("0047cf3f_TEST FAIL\n"); return 1; }
    std::printf("0047cf3f_TEST PASS\n");
    return 0;
}