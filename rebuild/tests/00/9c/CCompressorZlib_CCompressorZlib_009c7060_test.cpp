#include <cstdio>
#include <stddef.h>
#include <string.h>
struct z_stream_s { unsigned char* next_in; unsigned int avail_in; unsigned long total_in; unsigned char* next_out; unsigned int avail_out; unsigned long total_out; char* msg; void* state;
    void* (__cdecl* zalloc)(void*, unsigned int, unsigned int); void (__cdecl* zfree)(void*, void*); void* opaque; int data_type; unsigned long adler; unsigned long reserved; };
static int g_vectorCopies = 0;
template<class T> struct CRetailVector { T* first; T* last; T* end_of_storage; CRetailVector() : first(0), last(0), end_of_storage(0) { } CRetailVector(const CRetailVector& o) : first(o.first), last(o.last), end_of_storage(o.end_of_storage) { ++g_vectorCopies; } };
struct CACompressor_Retail { virtual void v00() { } void* Workspace; unsigned long WorkspaceSize; bool CompressionOn; CRetailVector<unsigned char> OwnWorkspace; };
struct CCompressorZlib_Retail : CACompressor_Retail {
    z_stream_s Stream; bool ZlibInitialised; bool Compressing; bool ErrorFlag;
    virtual void v00() { }
    CCompressorZlib_Retail() { }
    CCompressorZlib_Retail(const CCompressorZlib_Retail& other) : CACompressor_Retail(other), Stream(other.Stream), ZlibInitialised(other.ZlibInitialised), Compressing(other.Compressing), ErrorFlag(other.ErrorFlag) { }
};
int main()
{
    CCompressorZlib_Retail a; unsigned char ws[4];
    a.Workspace = ws; a.WorkspaceSize = 4; a.CompressionOn = true; a.OwnWorkspace.first = ws; a.OwnWorkspace.last = ws + 4;
    memset(&a.Stream, 0, sizeof(a.Stream)); a.Stream.avail_in = 11; a.Stream.adler = 0xabcd; a.ZlibInitialised = true; a.Compressing = false; a.ErrorFlag = true;
    CCompressorZlib_Retail b(a);
    if (g_vectorCopies != 1 || b.Workspace != ws || b.WorkspaceSize != 4 || !b.CompressionOn || b.OwnWorkspace.last != ws + 4 || b.Stream.avail_in != 11 || b.Stream.adler != 0xabcd
        || !b.ZlibInitialised || b.Compressing || !b.ErrorFlag || offsetof(CCompressorZlib_Retail, Stream) != 0x1c || offsetof(CCompressorZlib_Retail, ZlibInitialised) != 0x54 || sizeof(b) != 0x58) { std::printf("009c7060_TEST FAIL\n"); return 1; }
    std::printf("009c7060_TEST PASS\n"); return 0;
}