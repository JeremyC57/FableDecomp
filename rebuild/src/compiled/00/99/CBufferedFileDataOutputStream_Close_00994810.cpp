// CBufferedFileDataOutputStream::Close() -- flush the buffered bytes, restart the buffer at the current position, then free the buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994810, length 86 bytes (+ int3 padding).
// Callees: CAFile virtual slot 5 (+0x14, Seek) and slot 4 (+0x10, Write(data, size, 0)) on File,
// operator delete[] @ 0x00BFEB1C on Buffer. Virtual slot 3 of vtable 0x0129A710 (rtti-vtables.tsv).
// Layout as rebuild/include/engine/CBufferedFileDataOutputStream.h: File +0x08, Buffer +0x0c,
// BufferDataSize +0x14, DataStartFilePtr +0x18, FileSize +0x1c, FilePos +0x20.
// Shape: the Flush body (same instructions as @ 0x00994520, written inline here -- a call on
// `this` to the virtual Flush would not be devirtualised) followed by the buffer release; the
// Buffer value is loaded into ecx and pushed before the DataStartFilePtr store, and Buffer is
// nulled after the free.
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
    virtual void v01();
    virtual void v02();
    virtual void Close();                       // slot 3
};

void CBufferedFileDataOutputStream::Close()
{
    if (BufferDataSize > 0) {
        File->Seek(DataStartFilePtr);
        File->Write(Buffer, BufferDataSize, 0);
        File->Seek(FileSize);
        BufferDataSize = 0;
    }
    DataStartFilePtr = FilePos;
    delete[] Buffer;
    Buffer = 0;
}