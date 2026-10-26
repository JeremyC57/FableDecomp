// CMemoryDataInputStream::CMemoryDataInputStream(const CMemoryDataOutputStream&) -- input stream reading the bytes an output stream has written so far
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995EE0, 112 bytes (padding included).
// Shape: retail evaluates GetData() and size() twice BEFORE the CBaseClassNonCopyable call (0x0099A2F0):
// the data pointer is parked in the dead argument slot, the two sizes in ebx/ebp. That is the argument
// list of ONE inline intermediate base constructor (data, size, sizeCopy) over CBaseClassNonCopyable and
// the non-polymorphic stream-state base (stores after the call, before the vtable store 0x0123675C);
// the intermediate base is `__declspec(novtable)` so only the derived vtable is stored. StreamSource is
// a body assignment that recomputes GetData() after the vtable store. The source is the output stream's
// CArray<char> Data (+8, STLport {start, finish, end_of_storage}): GetData() is
// `size() ? start : (char*)this` (24e select), size() is finish - start. (With the two bases listed
// directly in the derived init list the arguments are evaluated after the first base call.)
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};

struct CArrayChar {                                // CArray<char> (STLport vector<char>)
    char* m_start;
    char* m_finish;
    char* m_endOfStorage;
    long size() const { return m_finish - m_start; }
    const char* GetData() const { return size() ? m_start : (const char*)this; }
};

class CMemoryDataOutputStream { public:
    virtual void v00();
    unsigned char _pad_0x04[0x4];                  // +0x04
    CArrayChar Data;                               // +0x08
    unsigned long CurrentPosition;                 // +0x18
};

struct SMemoryStreamState {                        // second, non-polymorphic base
    long        m_position;                        // +0x04
    long        m_size;                            // +0x08
    const char* m_pData;                           // +0x0c
    long        m_flags;                           // +0x10
    long        m_sizeCopy;                        // +0x14
    SMemoryStreamState(const char* data, long size, long sizeCopy) : m_position(0), m_size(size), m_pData(data), m_flags(0), m_sizeCopy(sizeCopy) { }
};

struct __declspec(novtable) CMemoryDataInputStreamBase : CBaseClassNonCopyable, SMemoryStreamState {
    CMemoryDataInputStreamBase(const char* data, long size, long sizeCopy)
        : CBaseClassNonCopyable(), SMemoryStreamState(data, size, sizeCopy) { }
};

class CMemoryDataInputStream : public CMemoryDataInputStreamBase { public:   // vtable 0x0123675C
    const char* StreamSource;                      // +0x18
    CMemoryDataInputStream(const CMemoryDataOutputStream& output);
};

CMemoryDataInputStream::CMemoryDataInputStream(const CMemoryDataOutputStream& output)
    : CMemoryDataInputStreamBase(output.Data.GetData(), output.Data.size(), output.Data.size())
{
    StreamSource = output.Data.GetData();
}