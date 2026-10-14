// CCompressedMemoryDataInputStream::CCompressedMemoryDataInputStream() -- CBaseClassNonCopyable(), the inlined CDataStream() zeroes the stream state, own vtable, Buffer/BufferSize null and the scoped compressor pointer null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995DC0, length 44 bytes (+ int3 padding).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (vtable 0x0129A7C4; manifest
// label "CBase"). Vtables: 0x0129A758 = CCompressedMemoryDataInputStream (rtti-vtables.tsv); the
// intermediate CDataStream (0x0129A69C, stored by the destructor @ 0x00995DF0) has an inline
// constructor whose own vtable store is dead and whose five members (+4..+0x14) are the zero run
// before this class's vtable store. engine/CCompressedMemoryDataInputStream.h: Buffer +0x18,
// BufferSize +0x1c, Compressor (a scoped_ptr<CCompressorZlib>) +0x20 -- the header holds the layout
// only, so the class is modelled locally with the same offsets. The two init-list scalars are
// stored before the vtable; the scoped pointer is a member object with an inline constructor, so
// its null store sinks below the vtable store (GOTCHAS: embedded members follow the vtable).
#include "engine/CCompressedMemoryDataInputStream.h"

struct CCompressorZlib;

struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                                    // 0x0099A2F0
};

struct CDataStream : CBaseClassNonCopyable {                    // vtable 0x0129A69C (dead store here)
    unsigned long m_position;                                   // +0x04
    unsigned long m_length;                                     // +0x08
    unsigned long m_flags;                                      // +0x0c
    unsigned long m_bufferStart;                                // +0x10
    unsigned long m_bufferEnd;                                  // +0x14
    virtual void v00();
    CDataStream() : m_position(0), m_length(0), m_flags(0), m_bufferStart(0), m_bufferEnd(0) { }
};

template<class T> struct CScopedPointer {                       // scoped_ptr<T>: one owning pointer
    T* ptr;
    CScopedPointer() : ptr(0) { }
};

struct CCompressedMemoryDataInputStream_Retail : CDataStream {
    unsigned char*                 Buffer;                      // +0x18
    long                           BufferSize;                  // +0x1c
    CScopedPointer<CCompressorZlib> Compressor;                 // +0x20
    virtual void v00();                                         // own vtable 0x0129A758
    CCompressedMemoryDataInputStream_Retail();
};

FABLE_STATIC_ASSERT(sizeof(CCompressedMemoryDataInputStream_Retail) == sizeof(CCompressedMemoryDataInputStream));

CCompressedMemoryDataInputStream_Retail::CCompressedMemoryDataInputStream_Retail()
    : CDataStream()
    , Buffer(0)
    , BufferSize(0)
    , Compressor()
{
}
