#include <cstdio>
#include <stddef.h>
static int g_arrayDeletes = 0, g_compressorDeletes = 0, g_baseDtors = 0, g_deletes = 0;
struct CByte { unsigned char b; static void operator delete[](void* p) { ++g_arrayDeletes; ::operator delete(p); } };
struct CCompressorZlib { virtual ~CCompressorZlib() { ++g_compressorDeletes; } };
struct CBaseClassNonCopyable { virtual void v00() { } CBaseClassNonCopyable() { } ~CBaseClassNonCopyable() { ++g_baseDtors; } };
struct CDataStream : CBaseClassNonCopyable { unsigned char _state[0x14]; virtual void v00() { } ~CDataStream() { } };
template<class T> struct CScopedPointer { T* ptr; CScopedPointer() : ptr(0) { } ~CScopedPointer() { if (ptr) delete ptr; } };
struct CCompressedMemoryDataInputStream_Retail : CDataStream {
    CByte* Buffer; long BufferSize; CScopedPointer<CCompressorZlib> Compressor;
    CCompressedMemoryDataInputStream_Retail() : Buffer(0), BufferSize(0) { }
    virtual ~CCompressedMemoryDataInputStream_Retail() { if (Buffer != 0) delete[] Buffer; }
};
struct CCounted : CCompressedMemoryDataInputStream_Retail { void operator delete(void* p) { ++g_deletes; ::operator delete(p); } };
int main()
{
    CCounted* s = new CCounted; s->Buffer = new CByte[8]; s->Compressor.ptr = new CCompressorZlib;
    CCompressedMemoryDataInputStream_Retail* base = s;
    delete base;                                                // virtual: runs the derived deleting destructor
    if (g_arrayDeletes != 1 || g_compressorDeletes != 1 || g_baseDtors != 1 || g_deletes != 1) { std::printf("00997240_TEST FAIL\n"); return 1; }
    std::printf("00997240_TEST PASS\n"); return 0;
}
