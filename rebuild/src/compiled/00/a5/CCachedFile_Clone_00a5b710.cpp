// CCachedFile::Clone() const -- heap-copies this cached file: `new CCachedFile(*this)` with the copy
// constructor inlined (CBase(), this vtable, empty CArray, empty CWideString, null CCountedPointer, then
// CCachedFile::Copy(other)); a failed allocation returns 0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5B710, length 79 bytes.
// Callees: operator new @ 0x00BFEA1A, CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CWideString::CWideString @ 0x0099AED0,
// CCachedFile::Copy @ 0x00A5B650. Vtable 0x012724D4 = CCachedFile (rtti-vtables.tsv).
// Layout (landed CCachedFile::CCachedFile @ 0x00A5B590): +4 Position, +8 BufferStartPosition (neither is
// touched here -- Copy fills them), +0xC DataBuffer ({first,last} zeroed by its inline constructor) and +0x14 its
// capacity word (initialised separately -- see the note on the include line), +0x18 Pathname
// (CWideString, out-of-line constructor), +0x1C PDiskFile (CCountedPointer<CAFile>, zeroed inline),
// +0x24 Opened / +0x25 BufferValid (left to Copy). The vtable store precedes the zero stores because the
// zeroed members are embedded objects with inline constructors; the copy constructor's body calls Copy.
// Return type is CAFile* (mangled name ?Clone@CCachedFile@@UBEPAVCAFile@@XZ).
#include <stddef.h>  // The CArray<uchar> zeroing lands as {first,last} from one inline constructor plus a separately initialised capacity word: a single 3-pointer object hoists the CWideString `lea` above all three stores.
void* __cdecl operator new(size_t size);        // 0x00BFEA1A

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                    // 0x0099A2F0
};

class CAFile : public CBaseClassNonCopyable { public:
};

struct CByteArray {                             // retail CArray<unsigned char>: {first, last} + capacity
    unsigned char* m_pFirst;
    unsigned char* m_pLast;
    CByteArray() : m_pFirst(0), m_pLast(0) { }
};

typedef unsigned short wchar_t;

class CWideString { public:
    wchar_t* m_pString;
    CWideString();                              // 0x0099AED0
};

struct CCountedPointer {
    void* m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

class CCachedFile : public CAFile { public:
    virtual CAFile* Clone() const;              // own vtable (0x012724D4)
    long            Position;                   // +0x04
    long            BufferStartPosition;        // +0x08
    CByteArray      DataBuffer;                 // +0x0C
    long            DataBufferCapacity;         // +0x14
    CWideString     Pathname;                   // +0x18
    CCountedPointer PDiskFile;                  // +0x1C
    bool            Opened;                     // +0x24
    bool            BufferValid;                // +0x25
    CCachedFile(const CCachedFile& other) : CAFile(), DataBuffer(), DataBufferCapacity(0), Pathname(), PDiskFile()
    {
        Copy(other);
    }
    void Copy(const CCachedFile& other);        // 0x00A5B650
};

CAFile* CCachedFile::Clone() const
{
    return new CCachedFile(*this);
}