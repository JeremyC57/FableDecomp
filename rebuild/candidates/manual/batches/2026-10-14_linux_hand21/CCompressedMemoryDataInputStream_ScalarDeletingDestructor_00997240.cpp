// CCompressedMemoryDataInputStream::`scalar deleting destructor'(unsigned int) -- the destructor @ 0x00995DF0 inlined (buffer, compressor, ~CDataStream, ~CBaseClassNonCopyable) then operator delete when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00997240, length 73 bytes.
// Callees: operator delete[] @ 0x00BFEB1C, the compressor's vtable slot 0 (deleting destructor, flag 1),
// CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300, operator delete @ 0x00BFE9BC.
// Vtables: 0x0129A758 = CCompressedMemoryDataInputStream (slot 0 of that vtable is this body),
// 0x0129A69C = CDataStream (the inlined empty intermediate destructor). The manifest calls it a
// vector deleting destructor; only bit 0 is tested, so it is the scalar `??_G`. The virtual
// destructor makes the lander grade the deleting body; MakeOne() constructs the class so its vtable
// (and this destructor) are emitted; the constructor is declared only so that helper stays small.
#include "engine/CCompressedMemoryDataInputStream.h"

struct CCompressorZlib_Calls {                                  // engine/CCompressorZlib.h holds data only: slot 0 is what `delete` calls
    virtual ~CCompressorZlib_Calls();                           // slot 0: scalar deleting destructor
};

struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                                    // 0x0099A2F0
    ~CBaseClassNonCopyable();                                   // 0x0099A300
};

struct CDataStream : CBaseClassNonCopyable {                    // vtable 0x0129A69C
    unsigned char _state[0x14];                                 // +0x04..+0x17
    virtual void v00();
    ~CDataStream() { }                                          // inline, empty: only its vtable store survives
};

template<class T> struct CScopedPointer {                       // scoped_ptr<T>
    T* ptr;
    CScopedPointer() : ptr(0) { }
    ~CScopedPointer() { if (ptr) delete ptr; }
};

struct CCompressedMemoryDataInputStream_Retail : CDataStream {
    unsigned char*                 Buffer;                      // +0x18
    long                           BufferSize;                  // +0x1c
    CScopedPointer<CCompressorZlib_Calls> Compressor;                 // +0x20
    CCompressedMemoryDataInputStream_Retail();                  // 0x00995DC0 (out of line: keeps MakeOne small)
    virtual ~CCompressedMemoryDataInputStream_Retail();
};

CCompressedMemoryDataInputStream_Retail::~CCompressedMemoryDataInputStream_Retail()
{
    if (Buffer != 0)
        delete[] Buffer;
}

FABLE_STATIC_ASSERT(sizeof(CCompressedMemoryDataInputStream_Retail) == sizeof(CCompressedMemoryDataInputStream));

void* MakeOne()                                                 // pulls the vtable and the deleting destructor into the TU
{
    return new CCompressedMemoryDataInputStream_Retail;
}
