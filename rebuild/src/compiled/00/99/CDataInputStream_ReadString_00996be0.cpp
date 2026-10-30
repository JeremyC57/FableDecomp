// CDataInputStream::ReadString(long length) -- read `length` bytes into a temporary memory output stream, NUL-terminate it, and return its contents as a CCharString
// Leaf: ReadString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00996BE0, length 219 bytes (+ int3 padding), `ret 8` (hidden result,
// length).
// Callees: CBaseClassNonCopyable ctor @ 0x0099A2F0 and dtor @ 0x0099A300, CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 (the "\r\n" line terminator @ 0x012397F8, then the buffer), copy constructor @ 0x0099EC30,
// ~CCharString @ 0x0099EAE0, CDataInputStream::ReadWithChunking(CDataOutputStream&, unsigned long) @ 0x009964E0,
// virtual slot 4 Write(const void*, long) on the output stream, free @ 0x00BFEA14 (the CRT import).
// Vtables: CDataStream 0x0129A69C, CDataOutputStream 0x0129A6E0, CMemoryDataOutputStream 0x0129A788 (RTTI).
// Layout: engine/CDataInputStream.h for `this` (only passed on); CMemoryDataOutputStream = CDataOutputStream {vptr,
// NewLine CCharString +4} + a buffer state {first +8, last +0xc, end +0x10, position +0x14} (landed ctor 0x00996AF0).
// Shape: the stack stream is built inline (base ctor call, NewLine, the CDataOutputStream then CMemoryDataOutputStream
// vtable stores, the state zeroed by an embedded member's inline ctor) and destroyed inline (its own vtable store,
// `if (first) free(first)`, the implicit CDataOutputStream dtor destroys NewLine with no vtable store, the inline
// CDataStream dtor stores its vtable, then the out-of-line base dtor). The terminating zero byte is an inline
// by-value WriteUBYTE parameter homed in the dead `length` slot; the data pointer is the vector's `size ? first :
// (char*)this` GetData; the result string is a named local copied out (no NRVO in VC7.1).
#define CDataInputStream CDataInputStreamLayout
#include "engine/CDataInputStream.h"
#undef CDataInputStream

extern "C" void __cdecl free(void* block);   // 0x00BFEA14

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    CCharString(const CCharString& other);        // 0x0099EC30
    ~CCharString();                               // 0x0099EAE0
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable();                      // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();             // 0x0099A300
};
struct CDataStream : CBaseClassNonCopyable {      // vtable 0x0129A69C
    virtual ~CDataStream() { }
    virtual void SetPosition(unsigned long position) = 0;   // slot 1
    virtual unsigned long GetPosition() const = 0;          // slot 2
    virtual void Close() = 0;                               // slot 3
};
class CDataOutputStream : public CDataStream { public:          // vtable 0x0129A6E0
    CCharString NewLine;                          // +0x04
    CDataOutputStream() : NewLine("\r\n", -1) { }
    virtual void Write(const void* data, long length) = 0;  // slot 4 (+0x10)
    void WriteUBYTE(unsigned char value) { Write(&value, 1); }
};
struct SByteBuffer {                              // +0x08 the growable buffer of the memory stream
    unsigned char* First;                         // +0x08
    unsigned char* Last;                          // +0x0c
    unsigned char* End;                           // +0x10
    unsigned long  Position;                      // +0x14
    SByteBuffer() : First(0), Last(0), End(0), Position(0) { }
    unsigned char* GetData() { return (Last - First) ? First : (unsigned char*)this; }
};
class CMemoryDataOutputStream : public CDataOutputStream { public:   // vtable 0x0129A788
    SByteBuffer Buffer;                           // +0x08
    CMemoryDataOutputStream() { }
    ~CMemoryDataOutputStream() { if (Buffer.First) free(Buffer.First); }
    virtual void SetPosition(unsigned long position);
    virtual unsigned long GetPosition() const;
    virtual void Close();
    virtual void Write(const void* data, long length);
};

class CDataInputStream : public CDataStream { public:
    unsigned long StreamPos;                      // +0x04
    long StreamSize;                              // +0x08
    void* CurrentSourcePtr;                       // +0x0c
    unsigned long SourceChunkStreamPos;           // +0x10
    long SourceChunkBytesRemaining;               // +0x14
    void ReadWithChunking(CDataOutputStream& output, unsigned long length);   // 0x009964E0
    CCharString ReadString(long length);
};

CCharString CDataInputStream::ReadString(long length)
{
    CMemoryDataOutputStream stream;
    ReadWithChunking(stream, length);
    stream.WriteUBYTE(0);
    CCharString result((const char*)stream.Buffer.GetData(), -1);
    return result;
}