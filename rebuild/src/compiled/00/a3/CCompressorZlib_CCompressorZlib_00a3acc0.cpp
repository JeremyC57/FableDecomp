// CCompressorZlib::~CCompressorZlib() -- if a zlib session is open, end it (deflateEnd when compressing; else inflateEnd and clear the stream buffers), then the implicit CACompressor destructor frees OwnWorkspace and runs ~CBaseClass
// Leaf: ??1CCompressorZlib@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3ACC0, 91 bytes, thiscall, tail `jmp ~CBaseClass`.
// Identity (proven): stores vtable 0x01245974 first (RTTI .?AVCCompressorZlib@@ : CACompressor : CBaseClass, rtti-vtables.tsv /
//   rtti_bases.tsv); callers are the slot-0 `??_G` 0x0052AE40 and twelve owners destroying CCompressorZlib members / locals
//   (0x0051F8F9 .. 0x009C7255). Ego_r.pdb (0x00B95B10) and FableWin.pdb (0x031FA2D0) spell ??1CCompressorZlib@@UAE@XZ (the
//   binding exists). Its two arms are the landed FinishCompressStream 0x00A3A930 and FinishDecompressStream 0x00A3ABB0 of the
//   same TU, expanded inline.
// Callees: z_deflateEnd 0x00C00AE0 and z_inflateEnd 0x00C038D0 (zlib 1.2.1 built with Z_PREFIX: Ego_r / FableWin _z_deflateEnd /
//   _z_inflateEnd), free 0x00BFEA14 (OwnWorkspace's block), CBaseClass::~CBaseClass 0x0099A2E0.
// Shape: user destructor `if (ZlibInitialised) { if (Compressing) FinishCompressStream(); else FinishDecompressStream(); }`;
//   both members (and SetupStream, which clears next_in / next_out / avail_in / avail_out through the z_stream reference)
//   are defined earlier in the TU and auto-inlined, their own ZlibInitialised tests folded into the outer one and their
//   `ZlibInitialised = false` stores merged at the join. CACompressor's destructor is implicit (no 0x0129B424 store): its
//   vector<unsigned char> OwnWorkspace (STLport, +0x10) frees its block, then ~CBaseClass.
// Layout (retail 0x58, the landed ctor 0x0052ADA0): CACompressor 0x1c (Workspace +4, WorkspaceSize +8, CompressionOn +0xc,
//   OwnWorkspace +0x10), Stream z_stream_s +0x1c, ZlibInitialised +0x54, Compressing +0x55, ErrorFlag +0x56.
//   engine/CCompressorZlib.h flattens the vptr and the base into pads (header debt).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"                              // ~ 0x0099A2E0

struct internal_state;
typedef void* (__cdecl *alloc_func)(void* opaque, unsigned int items, unsigned int size);
typedef void (__cdecl *free_func)(void* opaque, void* address);

typedef struct z_stream_s {                                 // zlib 1.2.1 zlib.h, 0x38 bytes
    unsigned char* next_in;                                 // +0x00
    unsigned int avail_in;                                  // +0x04
    unsigned long total_in;                                 // +0x08
    unsigned char* next_out;                                // +0x0c
    unsigned int avail_out;                                 // +0x10
    unsigned long total_out;                                // +0x14
    char* msg;                                              // +0x18
    struct internal_state* state;                           // +0x1c
    alloc_func zalloc;                                      // +0x20
    free_func zfree;                                        // +0x24
    void* opaque;                                           // +0x28
    int data_type;                                          // +0x2c
    unsigned long adler;                                    // +0x30
    unsigned long reserved;                                 // +0x34
} z_stream;
typedef z_stream* z_streamp;

extern "C" int __cdecl z_deflateEnd(z_streamp strm);        // 0x00C00AE0
extern "C" int __cdecl z_inflateEnd(z_streamp strm);        // 0x00C038D0

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}   // namespace std

class CACompressor : public CBaseClass {                    // vtable 0x0129B424, retail 0x1c; implicit destructor
public:
    CACompressor(long workspaceSize);                       // 0x00A3AEA0
    void* Workspace;                                        // +0x04
    unsigned long WorkspaceSize;                            // +0x08
    bool CompressionOn;                                     // +0x0c
    std::vector<unsigned char> OwnWorkspace;                // +0x10
};

class CCompressorZlib : public CACompressor {               // vtable 0x01245974, retail 0x58
public:
    CCompressorZlib();
    virtual ~CCompressorZlib();
    void FinishCompressStream();                            // 0x00A3A930
    void FinishDecompressStream();                          // 0x00A3ABB0
private:
    void SetupStream(z_stream_s& stream, const void* input, unsigned long inputSize, void* output, unsigned long outputSize);
public:
    z_stream_s Stream;                                      // +0x1c
    bool ZlibInitialised;                                   // +0x54
    bool Compressing;                                       // +0x55
    bool ErrorFlag;                                         // +0x56
};

void CCompressorZlib::SetupStream(z_stream_s& stream, const void* input, unsigned long inputSize, void* output, unsigned long outputSize)
{
    stream.next_in = (unsigned char*)input;
    stream.next_out = (unsigned char*)output;
    stream.avail_in = inputSize;
    stream.avail_out = outputSize;
}

void CCompressorZlib::FinishCompressStream()
{
    if (ZlibInitialised) {
        z_deflateEnd(&Stream);
        ZlibInitialised = false;
    }
}

void CCompressorZlib::FinishDecompressStream()
{
    if (ZlibInitialised) {
        z_inflateEnd(&Stream);
        SetupStream(Stream, 0, 0, 0, 0);
        ZlibInitialised = false;
    }
}

CCompressorZlib::~CCompressorZlib()
{
    if (ZlibInitialised) {
        if (Compressing)
            FinishCompressStream();
        else
            FinishDecompressStream();
    }
}