// CBankFileAsyncEntry::SetupRead(unsigned long start, CBankFileAsyncData* data, unsigned long size, unsigned long, unsigned long, EThreadedFilePriority priority) -- start reading `size` bytes of the entry from `start` into the data's buffer: synchronously when async is disabled (marks it finished), else by queueing a load descriptor on the bank's threaded file (aligned size for non-cached banks)
// Retry 10b (getsw sweep, FIXES B-15): the buffer is read through an inline getter `GetBuffer()`; the plain member read gives the parked register/scheduling difference.
// Leaf: SetupRead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5970, length 191 bytes (+1 int3), ret 0x18 (arguments 4 and 5 unused).
// Callees: CBankFileAsyncData::SetupBuffer(bool) @ 0x009D52D0, CBankFile::GetEntryDataOffset(unsigned long) @ 0x009CC270,
// CBankFile::ReadData(unsigned long offset, void* buffer, unsigned long size) @ 0x009CBF60 (catalog
// "CMapsAndRegionsDialog::OpenMapDialog", propagated), CBankFile::Align(unsigned long) @ 0x009CBB90,
// CThreadedFile::AddObject(const CThreadedFileObjectDesc&) @ 0x0098F350 (catalog "_Cons_val<...CAnimationEntry...>",
// propagated; returns the read's UID).
// Shape: the synchronous call pushes its size and buffer before evaluating the offset call (right to left); in the async
// arm the NonCached test comes first and the shared PDst store and `GetEntryDataOffset(m_index)` argument set-up
// of both arms are hoisted after it, the descriptor is a
// reference to data->LoadDesc (esi).
// Layout: CBankFileAsyncEntry (landed ctor 0x009D5260): m_pBank +4, m_index +8; retail CBankFileAsync: ThreadedFile
// +0x110, NonCached +0x148, DisableAsync +0x149; Ego_r.pdb CBankFileAsyncData: Buffer +0xc, ThreadedFileUID +0x18,
// BankFile +0x28, LoadDesc +0x2c, Finished +0x44; CThreadedFileObjectDesc: PDst, FilePos, Size, PCallback,
// QueueWhenFinished +0x10, Priority +0x14.
class CBankFileAsyncData;
enum EThreadedFilePriority { THREADED_FILE_PRIORITY_DEFAULT = 3 };
class CThreadedFileObjectDesc { public:                   // 0x18 bytes
    void* PDst;                                    // +0x00
    unsigned long FilePos;                         // +0x04
    unsigned long Size;                            // +0x08
    CBankFileAsyncData* PCallback;                 // +0x0c CThreadedFileCallback*
    bool QueueWhenFinished;                        // +0x10
    EThreadedFilePriority Priority;                // +0x14
};
class CThreadedFile { public:
public:
    unsigned long AddObject(const CThreadedFileObjectDesc&);   // 0x0098F350
};
template <class T> class CCountedPointer { public: T* Data; void* Info; };
class CBankFileAsync {
public:
    unsigned char _pad[0x110];
    CCountedPointer<CThreadedFile> ThreadedFile;   // +0x110
    unsigned char _pad118[0x30];                   // +0x118 .. +0x147
    bool NonCached;                                // +0x148
    bool DisableAsync;                             // +0x149
    unsigned long GetEntryDataOffset(unsigned long index) const;   // 0x009CC270 (CBankFile)
    void ReadData(unsigned long offset, void* buffer, unsigned long size);   // 0x009CBF60 (CBankFile)
    unsigned long Align(unsigned long size) const; // 0x009CBB90 (CBankFile)
};
class CBankFileAsyncData {
public:
    virtual void v00();
    void* _base4;                                  // +0x04 CMovableResource vptr
    void* Data;                                    // +0x08
    void* Buffer;                                  // +0x0c
    void* GetBuffer() const { return Buffer; }
    unsigned long Length;                          // +0x10
    unsigned long SkipBytes;                       // +0x14
    unsigned long ThreadedFileUID;                 // +0x18
    void* MemoryPool;                              // +0x1c
    long MemoryHandle;                             // +0x20
    unsigned long InternalAllocatedBufferSize;     // +0x24
    CBankFileAsync* BankFile;                      // +0x28
    CThreadedFileObjectDesc LoadDesc;              // +0x2c
    bool Finished;                                 // +0x44
    bool SetupBuffer(bool synchronous);            // 0x009D52D0
};

class CBankFileAsyncEntry {
public:
    virtual void v00();
    CBankFileAsync* m_pBank;                       // +0x04
    unsigned long   m_index;                       // +0x08
    unsigned long   m_alignedSize;                 // +0x0c
    void SetupRead(unsigned long start, CBankFileAsyncData* data, unsigned long size, unsigned long, unsigned long,
                   EThreadedFilePriority priority);
};

void CBankFileAsyncEntry::SetupRead(unsigned long start, CBankFileAsyncData* data, unsigned long size, unsigned long,
                                    unsigned long, EThreadedFilePriority priority)
{
    if (m_pBank->DisableAsync) {
        data->SetupBuffer(true);
        m_pBank->ReadData(m_pBank->GetEntryDataOffset(m_index) + start, data->GetBuffer(), size);
        data->Finished = true;
        return;
    }
    CThreadedFileObjectDesc& desc = data->LoadDesc;
    unsigned long readSize;
    if (m_pBank->NonCached) {
        desc.PDst = data->GetBuffer();
        desc.FilePos = m_pBank->GetEntryDataOffset(m_index) + start;
        readSize = m_pBank->Align(size);
    } else {
        desc.PDst = data->GetBuffer();
        desc.FilePos = m_pBank->GetEntryDataOffset(m_index) + start;
        readSize = size;
    }
    desc.Size = readSize;
    desc.PCallback = data;
    desc.Priority = priority;
    desc.QueueWhenFinished = false;
    data->ThreadedFileUID = data->BankFile->ThreadedFile.Data->AddObject(desc);
}