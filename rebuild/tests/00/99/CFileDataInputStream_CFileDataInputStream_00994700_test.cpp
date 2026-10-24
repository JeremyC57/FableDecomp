// Behaviour test for CFileDataInputStream::CFileDataInputStream(CAFile&, unsigned long) (0x00994700): the stream
// words start at zero and File is kept; with a buffer the window covers the new buffer with the file's length
// and the stream is moved to the file position; with size 0 nothing is allocated or positioned.
#include <stdio.h>
#include <string.h>
inline void* operator new(unsigned int, void* p) { return p; }
static int g_bases = 0;
static int g_setPositions = 0;
static unsigned long g_lastPosition = 0;
class CAFile
{
public:
    long Position, Length;
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void v04() {} virtual void v05() {} virtual void v06() {}
    virtual long GetPosition() const { return Position; }
    virtual void v08() {}
    virtual long GetLength() const { return Length; }
};

class CBaseClassNonCopyable
{
public:
    CBaseClassNonCopyable() { ++g_bases; }
    virtual ~CBaseClassNonCopyable() {}
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
    virtual void SetPosition(unsigned long position)      // 0x00993BC0 (test: record the request)
    {
        ++g_setPositions;
        g_lastPosition = position;
        StreamPos = position;
    }
};

class CFileDataInputStream : public CDataInputStream
{
public:
    CAFile* File;                                         // +0x18
    unsigned char* Buffer;                                // +0x1c
    long BufferSize;                                      // +0x20

    CFileDataInputStream(CAFile& file, unsigned long bufferSize);
    virtual ~CFileDataInputStream() { if (BufferSize > 0) delete[] Buffer; }
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

int main()
{
    CAFile file;
    file.Position = 40; file.Length = 1000;
    {
        CFileDataInputStream s(file, 256);
        if (g_bases != 1 || s.File != &file || s.BufferSize != 256 || s.Buffer == 0) { printf("FAIL buffer\n"); return 1; }
        if (s.CurrentSourcePtr != s.Buffer || s.StreamSize != 1000 || s.SourceChunkStreamPos != 0 || s.SourceChunkBytesRemaining != 0)
        { printf("FAIL window\n"); return 1; }
        if (g_setPositions != 1 || g_lastPosition != 40 || s.StreamPos != 40) { printf("FAIL position\n"); return 1; }
        memset(s.Buffer, 0x5a, 256);
    }
    {
        unsigned char raw[sizeof(CFileDataInputStream)];
        memset(raw, 0xcd, sizeof(raw));
        CFileDataInputStream* p = new (raw) CFileDataInputStream(file, 0);
        if (p->File != &file || p->StreamPos != 0 || p->StreamSize != 0 || p->CurrentSourcePtr != 0 || p->SourceChunkStreamPos != 0
            || p->SourceChunkBytesRemaining != 0 || g_setPositions != 1) { printf("FAIL empty\n"); return 1; }
        if (p->BufferSize != (long)0xcdcdcdcd) { printf("FAIL untouched size\n"); return 1; }
    }
    printf("FILE_DATA_STREAM_00994700_TEST PASS\n");
    return 0;
}
