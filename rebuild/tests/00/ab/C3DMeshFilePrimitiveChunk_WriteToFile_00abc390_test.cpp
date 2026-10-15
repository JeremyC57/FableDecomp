#include <cstdio>
#include <cstring>
struct CChunkedFileSaver { int dummy; };
static char g_trace[256];
static void Log(const char* s) { std::strcat(g_trace, s); }
struct CDataOutputStream { void WriteLONG(long v) { char b[32]; std::sprintf(b, "L%lx;", (unsigned long)v & 0xffffffffUL); Log(b); } };
struct CChunkedFileSuperChunk { void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const { Log("SUPER;"); } };
template <class T>
struct CVectorArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned long size() const { return (unsigned long)(end() - begin()); }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct C3DMeshFilePrimitiveChunk : CChunkedFileSuperChunk {
    CVectorArray<int> MaterialIndices;
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
    {
        if (MaterialIndices.size() == 1)
            out.WriteLONG(MaterialIndices[0]);
        else {
            int count = (int)MaterialIndices.size();
            out.WriteLONG((int)(count | 0x80000000));
            for (int i = 0; i < count; ++i)
                out.WriteLONG(MaterialIndices[i]);
        }
        CChunkedFileSuperChunk::WriteToFile(out, saver);
    }
};
int main()
{
    int one[1] = { 7 }, three[3] = { 1, 2, 3 };
    C3DMeshFilePrimitiveChunk c; CDataOutputStream out; CChunkedFileSaver saver;
    c.MaterialIndices._M_start = one; c.MaterialIndices._M_finish = one + 1; c.MaterialIndices._M_end_of_storage = one + 1;
    g_trace[0] = 0; c.WriteToFile(out, saver);
    int ok = std::strcmp(g_trace, "L7;SUPER;") == 0;
    c.MaterialIndices._M_start = three; c.MaterialIndices._M_finish = three + 3; c.MaterialIndices._M_end_of_storage = three + 3;
    g_trace[0] = 0; c.WriteToFile(out, saver);
    ok = ok && std::strcmp(g_trace, "L80000003;L1;L2;L3;SUPER;") == 0;
    if (!ok) { std::printf("00abc390_TEST FAIL\n"); return 1; }
    std::printf("00abc390_TEST PASS\n");
    return 0;
}