// CBufferedFileDataOutputStream::CBufferedFileDataOutputStream(CAFile* file, long bufferSize) -- named stream base, file, a fresh buffer, positions from the file's length
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009944A0, length 90 bytes (+ int3 padding).
// Callees: CBase::CBase @ 0x0099A2F0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the
// stream name at +0x04, literal @ 0x012397F8, length -1), operator new[] @ 0x00BFEB22, CAFile
// virtual slot 7 (+0x1c, GetLength) on the file.
// Vtables: 0x0129A6E0 CDataOutputStream (base, stored before its name is built), 0x0129A710
// CBufferedFileDataOutputStream. Layout as rebuild/include/engine/CBufferedFileDataOutputStream.h:
// File +0x08, Buffer +0x0c, BufferSize +0x10, BufferDataSize +0x14, DataStartFilePtr +0x18,
// FileSize +0x1c, FilePos +0x20 (the base's name string occupies +0x04, which the header leaves
// unnamed; the base is modelled here so its inline constructor reproduces).
// Shape: the init list in declaration order; File and BufferSize (scalars from the arguments) are
// stored before the allocation call, the two zeros from one register, FileSize from the virtual
// call and FilePos copied from it (one call, one register).
void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
};

struct CBase {
    virtual void v00();
    CBase();                                    // 0x0099A2F0
};

class CDataOutputStream : public CBase { public:              // vtable 0x0129A6E0
    CCharString m_name;                         // +0x04
    virtual void v00();
    CDataOutputStream() : CBase(), m_name("CBufferedFileDataOutputStream", -1) { }
};

class CAFile { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual long GetLength();                   // slot 7 (+0x1c)
};

struct CBufferedFileDataOutputStream : CDataOutputStream {   // vtable 0x0129A710
    CAFile*        File;                        // +0x08
    unsigned char* Buffer;                      // +0x0c
    long           BufferSize;                  // +0x10
    long           BufferDataSize;              // +0x14
    long           DataStartFilePtr;            // +0x18
    long           FileSize;                    // +0x1c
    long           FilePos;                     // +0x20
    virtual void v00();
    CBufferedFileDataOutputStream(CAFile* file, long bufferSize);
};

CBufferedFileDataOutputStream::CBufferedFileDataOutputStream(CAFile* file, long bufferSize)
    : CDataOutputStream()
{
    File = file;
    BufferSize = bufferSize;
    Buffer = new unsigned char[bufferSize];
    BufferDataSize = 0;
    DataStartFilePtr = 0;
    FileSize = file->GetLength();
    FilePos = FileSize;
}