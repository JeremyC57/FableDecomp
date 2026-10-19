// CCompressedMemoryDataInputStream::~CCompressedMemoryDataInputStream() -- own vtable, delete[] the buffer, delete the owned compressor, then the inlined empty ~CDataStream and the ~CBaseClassNonCopyable tail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995DF0, length 52 bytes.
// Callees: operator delete[] @ 0x00BFEB1C (the Buffer, under an explicit null test), the compressor's
// scalar deleting destructor through its vtable slot 0 with flag 1 (`delete ptr`, null-checked),
// CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300 (tail call; manifest label "CBase").
// Vtables: 0x0129A758 = CCompressedMemoryDataInputStream (stored first), 0x0129A69C = CDataStream
// (the inlined empty intermediate destructor's store). engine/CCompressedMemoryDataInputStream.h:
// Buffer +0x18, Compressor +0x20 (a scoped_ptr<CCompressorZlib> whose destructor deletes the
// pointee). The destructor is kept non-virtual with a virtual function beside it so the lander
// grades the plain `??1` body (the deleting `??_G` is the next row, 0x00997240).
#include "engine/CCompressedMemoryDataInputStream.h"
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)

struct CCompressorZlib_Calls {                                  // engine/CCompressorZlib.h holds data only: slot 0 is what `delete` calls
    virtual ~CCompressorZlib_Calls();                           // slot 0: scalar deleting destructor
};

struct CBaseClassNonCopyable {
    virtual void v00();
    ~CBaseClassNonCopyable();                                   // 0x0099A300
};

struct CDataStream : CBaseClassNonCopyable {                    // vtable 0x0129A69C
    unsigned char _state[0x14];                                 // +0x04..+0x17
    virtual void v00();
    ~CDataStream() { }                                          // inline, empty: only its vtable store survives
};

template<class T> struct CScopedPointer {                       // scoped_ptr<T>
    T* ptr;
    ~CScopedPointer() { if (ptr) delete ptr; }                  // explicit null test kept by retail
};

struct CCompressedMemoryDataInputStream_Retail : CDataStream {
    unsigned char*                 Buffer;                      // +0x18
    long                           BufferSize;                  // +0x1c
    CScopedPointer<CCompressorZlib_Calls> Compressor;                 // +0x20
    virtual void v00();                                         // own vtable 0x0129A758
    ~CCompressedMemoryDataInputStream_Retail();
};

FABLE_STATIC_ASSERT(sizeof(CCompressedMemoryDataInputStream_Retail) == sizeof(CCompressedMemoryDataInputStream));

CCompressedMemoryDataInputStream_Retail::~CCompressedMemoryDataInputStream_Retail()
{
    if (Buffer != 0)
        delete[] Buffer;
}