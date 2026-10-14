// CBufferedFileDataOutputStream::Flush() -- write the buffered bytes at their file position, seek back to the end, and restart the buffer at the current position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994520, length 75 bytes (+ int3 padding).
// Callees: CAFile virtual slot 5 (+0x14, Seek) and slot 4 (+0x10, Write(data, size, 0)) on File.
// Layout as rebuild/include/engine/CBufferedFileDataOutputStream.h: File +0x08, Buffer +0x0c,
// BufferDataSize +0x14, DataStartFilePtr +0x18, FileSize +0x1c, FilePos +0x20.
// Shape: when there is buffered data the file is positioned at DataStartFilePtr, the buffer
// written, the file positioned back at FileSize and the count cleared; both paths then restart
// the buffer at FilePos -- VC7.1 duplicates that small tail into each path (two `pop esi ; ret`).
// The File pointer is re-read before every virtual call (the callee may change it).
struct CAFile {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long size, long flags);   // slot 4 (+0x10)
    virtual void Seek(long position);                              // slot 5 (+0x14)
};

struct CBufferedFileDataOutputStream {
    virtual void v00();
    void*          m_name;                      // +0x04 CCharString (CDataOutputStream base)
    CAFile*        File;                        // +0x08
    unsigned char* Buffer;                      // +0x0c
    long           BufferSize;                  // +0x10
    long           BufferDataSize;              // +0x14
    long           DataStartFilePtr;            // +0x18
    long           FileSize;                    // +0x1c
    long           FilePos;                     // +0x20
    void Flush();
};

void CBufferedFileDataOutputStream::Flush()
{
    if (BufferDataSize > 0) {
        File->Seek(DataStartFilePtr);
        File->Write(Buffer, BufferDataSize, 0);
        File->Seek(FileSize);
        BufferDataSize = 0;
    }
    DataStartFilePtr = FilePos;
}
