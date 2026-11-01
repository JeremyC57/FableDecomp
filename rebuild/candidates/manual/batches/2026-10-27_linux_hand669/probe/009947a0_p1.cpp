// CBufferedFileDataOutputStream::~CBufferedFileDataOutputStream() -- buffered file output stream dtor: flush the buffered bytes to the file, free the buffer, then the CDataOutputStream NewLine string and ~CDataStream
// Leaf: ??1CBufferedFileDataOutputStream
void __cdecl operator delete[](void* block);                // 0x00BFEB1C
#include "engine/CBaseClassNonCopyable.h"

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;                           // +0x00
};
class CASuspendableProcess;
class CAFile {
public:
    virtual ~CAFile();                                      // slot 0
    virtual void VirtualSlot1_unproven();
    virtual void VirtualSlot2_unproven();
    virtual void VirtualSlot3_unproven();
    virtual void Write(const void* data, long size, CASuspendableProcess* process);   // slot 4 (+0x10)
    virtual void SetPosition(long position);                // slot 5 (+0x14)
};

class CDataStream : public CBaseClassNonCopyable {          // vtable 0x0129A69C
protected:
    CDataStream();
public:
    virtual ~CDataStream() { }
    virtual void SetPosition(unsigned long position) = 0;   // slot 1
    virtual unsigned long GetPosition() const = 0;          // slot 2
    virtual void Close() = 0;                               // slot 3
};
class CDataOutputStream : public CDataStream {
public:
    CDataOutputStream();
    virtual void Write(const void* data, long size) = 0;    // slot 4
    CCharString NewLine;                                    // +0x04
};

class CBufferedFileDataOutputStream : public CDataOutputStream {   // vtable 0x0129A710
public:
    virtual ~CBufferedFileDataOutputStream();
    virtual void SetPosition(unsigned long position);       // 0x00994500
    virtual unsigned long GetPosition() const;              // 0x00994510
    virtual void Close();                                   // 0x00994810
    virtual void Write(const void* data, long size);        // 0x00994870
    void Flush();                                           // 0x00994520
    CAFile*         File;                                   // +0x08
    unsigned char*  Buffer;                                 // +0x0C
    long            BufferSize;                             // +0x10
    long            BufferDataSize;                         // +0x14
    long            DataStartFilePtr;                       // +0x18
    long            FileSize;                               // +0x1C
    long            FilePos;                                // +0x20
};

void CBufferedFileDataOutputStream::Flush()
{
    if (BufferDataSize > 0) {
        File->SetPosition(DataStartFilePtr);
        File->Write(Buffer, BufferDataSize, 0);
        File->SetPosition(FileSize);
        BufferDataSize = 0;
    }
    DataStartFilePtr = FilePos;
}

CBufferedFileDataOutputStream::~CBufferedFileDataOutputStream()
{
    Flush();
    delete[] Buffer;
}
