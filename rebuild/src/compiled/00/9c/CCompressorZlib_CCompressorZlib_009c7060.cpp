// CCompressorZlib::CCompressorZlib(const CCompressorZlib&) -- memberwise copy: the CACompressor base (vtable, workspace pointer/size, compression flag, the owned-workspace vector copied out of line), own vtable, the z_stream block copied as 14 dwords, the three state flags
// Leaf: ??0CCompressorZlib_Retail@@QAE@ABU0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7060, length 92 bytes.
// Callees: std::vector<unsigned char>::vector(const vector&) @ 0x005290E0 (OwnWorkspace; the manifest
// calls it "Vector_CopyAssignFrom"). Vtables: 0x0129B424 = CACompressor (the inlined base copy
// constructor's store), 0x01245974 = CCompressorZlib (rtti-vtables.tsv).
// Layout: in the shipped binary (and in engine/CACompressor.h / CCompressorZlib.h, retail STLport 4.6 layout) the vector is
// 12 bytes (first/last/end), so CACompressor is 0x1c, the z_stream (56 bytes = 0x38) sits at +0x1c and
// the flags ZlibInitialised/Compressing/ErrorFlag at +0x54/+0x55/+0x56 (size 0x58) -- the
// `_Retail` models carry the constructors the flat headers cannot. Both copy constructors are compiler-generated (memberwise): the base's is inlined,
// the derived one is spelled out so its `??0` is emitted; the z_stream is a POD struct, which VC7.1
// copies as a `rep movsd` block through esi/edi, and the bools are copied byte by byte.
#include "engine/CCompressorZlib.h"
#include "engine/CACompressor.h"

struct z_stream_s {                                             // zlib 1.2.1 stream (56 bytes)
    unsigned char* next_in;  unsigned int avail_in;  unsigned long total_in;
    unsigned char* next_out; unsigned int avail_out; unsigned long total_out;
    char* msg; void* state;
    void* (__cdecl* zalloc)(void*, unsigned int, unsigned int);
    void  (__cdecl* zfree)(void*, void*);
    void* opaque; int data_type; unsigned long adler; unsigned long reserved;
};

template<class T> struct CRetailVector {                        // retail std::vector: {first, last, end}
    T* first; T* last; T* end_of_storage;
    CRetailVector(const CRetailVector& other);                  // 0x005290E0
};

struct CACompressor_Retail {                                    // 0x1c bytes in retail
    virtual void v00();                                         // vtable 0x0129B424
    void*         Workspace;                                    // +0x04
    unsigned long WorkspaceSize;                                // +0x08
    bool          CompressionOn;                                // +0x0c
    CRetailVector<unsigned char> OwnWorkspace;                  // +0x10
};

struct CCompressorZlib_Retail : CACompressor_Retail {
    z_stream_s Stream;                                          // +0x1c
    bool       ZlibInitialised;                                 // +0x54
    bool       Compressing;                                     // +0x55
    bool       ErrorFlag;                                       // +0x56
    virtual void v00();                                         // own vtable 0x01245974
    CCompressorZlib_Retail(const CCompressorZlib_Retail& other);
};

FABLE_STATIC_ASSERT(sizeof(CCompressorZlib_Retail) == 0x58);
FABLE_STATIC_ASSERT(sizeof(CCompressorZlib) == sizeof(CCompressorZlib_Retail)); // engine header: retail STLport 4.6 layout

CCompressorZlib_Retail::CCompressorZlib_Retail(const CCompressorZlib_Retail& other)
    : CACompressor_Retail(other)
    , Stream(other.Stream)
    , ZlibInitialised(other.ZlibInitialised)
    , Compressing(other.Compressing)
    , ErrorFlag(other.ErrorFlag)
{
}