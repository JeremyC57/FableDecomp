// CFileDataInputStream::CFileDataInputStream(CAFile& file, unsigned long bufferSize) -- buffered input stream over a file
// Retail 0x00994700, 113 bytes (Ego_r.pdb ??0CFileDataInputStream@@QAE@AAVCAFile@@K@Z, `ret 8`). The
// CBaseClassNonCopyable root constructor (0x0099A2F0, out of line) runs first, the inlined CDataInputStream
// constructor zeroes the five stream words (+0x04..+0x14), the CFileDataInputStream vtable (0x0129A728) and
// File (+0x18) follow. With a non-zero buffer size the buffer is allocated through operator new[] (the
// MSVCR71 thunk 0x00BFEB22), the stream window is reset over it with the file's length (CAFile vtable slot 9)
// and the stream is positioned at the file's current position (slot 7) through CDataInputStream::SetPosition
// (0x00993BC0, called directly: a virtual call on `this` in a constructor resolves statically).
// Shape: CDataInputStream is a `__declspec(novtable)` intermediate base with an inline zeroing constructor (its
// own vtable store disappears, the zero stores precede the derived vtable store); the window reset is an inline
// CDataInputStream member whose statements follow retail's store order (CurrentSourcePtr re-read from Buffer
// after the virtual call, then StreamPos, SourceChunkStreamPos, StreamSize, SourceChunkBytesRemaining).
// Model: engine/CFileDataInputStream.h is a data-only struct (File +0x18, Buffer +0x1C, BufferSize +0x20 --
// the same offsets as here); a constructor needs the class hierarchy itself, hence the local model.
// Leaf: ??0CFileDataInputStream

void* __cdecl operator new[](unsigned int size);         // 0x00BFEB22 (MSVCR71 import thunk)
void __cdecl operator delete[](void* block);              // 0x00BFEB1C

class CAFile
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual long GetPosition() const;                     // slot 7
    virtual void v08();
    virtual long GetLength() const;                       // slot 9
};

class CBaseClassNonCopyable
{
public:
    CBaseClassNonCopyable();                              // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

class __declspec(novtable) CDataInputStream : public CBaseClassNonCopyable
{
public:
    unsigned long StreamPos;                              // +0x04
    long StreamSize;                                      // +0x08
    const unsigned char* CurrentSourcePtr;                // +0x0c
    unsigned long SourceChunkStreamPos;                   // +0x10
    long SourceChunkBytesRemaining;                       // +0x14

    CDataInputStream()
        : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0)
    {
    }
    void ResetSource(const unsigned char* source, long size)
    {
        CurrentSourcePtr = source;
        StreamPos = 0;
        SourceChunkStreamPos = 0;
        StreamSize = size;
        SourceChunkBytesRemaining = 0;
    }
    virtual void SetPosition(unsigned long position);     // 0x00993BC0
};

class CFileDataInputStream : public CDataInputStream
{
public:
    CAFile* File;                                         // +0x18
    unsigned char* Buffer;                                // +0x1c
    long BufferSize;                                      // +0x20

    CFileDataInputStream(CAFile& file, unsigned long bufferSize);
    virtual ~CFileDataInputStream();
};

CFileDataInputStream::CFileDataInputStream(CAFile& file, unsigned long bufferSize)
    : File(&file)
{
    if (bufferSize > 0)
    {
        Buffer = new unsigned char[bufferSize];
        BufferSize = bufferSize;
        ResetSource(Buffer, file.GetLength());
        SetPosition(file.GetPosition());
    }
}
