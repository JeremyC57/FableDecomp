// Behaviour test for 009d5970: src/009d5970.cpp copied verbatim; the bank file and threaded file stubbed so the
// synchronous path (buffer set-up, direct read at entry offset + start, finished) and the queued path (desc filled with
// aligned or plain size, callback, priority, UID stored) are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CBankFileAsyncEntry::SetupRead(unsigned long start, CBankFileAsyncData* data, unsigned long size, unsigned long, unsigned long, EThreadedFilePriority priority) -- start reading `size` bytes of the entry from `start` into the data's buffer: synchronously when async is disabled (marks it finished), else by queueing a load descriptor on the bank's threaded file (aligned size for non-cached banks)
// Retry 10b (getsw sweep, FIXES B-15): the buffer is read through an inline getter `GetBuffer()`; the plain member read gives the parked register/scheduling difference.
// Leaf: SetupRead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5970, length 191 bytes (+1 int3), ret 0x18 (arguments 4 and 5 unused).
// Callees: CBankFileAsyncData::SetupBuffer(bool) @ 0x009D52D0, CBankFile::GetEntryDataOffset(unsigned long) @ 0x009CC270,
// CBankFile::ReadData(unsigned long offset, void* buffer, unsigned long size) @ 0x009CBF60 (catalog
// "CMapsAndRegionsDialog::OpenMapDialog", propagated), CBankFile::Align(unsigned long) @ 0x009CBB90,
// CThreadedFile::QueueRead(const CThreadedFileObjectDesc&) @ 0x0098F350 (catalog "_Cons_val<...CAnimationEntry...>",
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
    unsigned long QueueRead(const CThreadedFileObjectDesc& desc);   // 0x0098F350
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
    data->ThreadedFileUID = data->BankFile->ThreadedFile.Data->QueueRead(desc);
}
// ---- end source ----
void CBankFileAsyncData::v00() {}
void CBankFileAsyncEntry::v00() {}
static int g_setups = 0; static unsigned long g_readOff = 0, g_readSize = 0; static void* g_readBuf = 0;
bool CBankFileAsyncData::SetupBuffer(bool sync) { if (sync) ++g_setups; return true; }
unsigned long CBankFileAsync::GetEntryDataOffset(unsigned long index) const { return 0x1000 * index; }
void CBankFileAsync::ReadData(unsigned long off, void* buf, unsigned long size) { g_readOff = off; g_readBuf = buf; g_readSize = size; }
unsigned long CBankFileAsync::Align(unsigned long size) const { return (size + 0x7ff) & ~0x7ffUL; }
static const CThreadedFileObjectDesc* g_queued = 0;
unsigned long CThreadedFile::QueueRead(const CThreadedFileObjectDesc& d) { g_queued = &d; return 0xabc; }
int main()
{
    static unsigned char bankRaw[sizeof(CBankFileAsync)]; memset(bankRaw, 0, sizeof(bankRaw));
    CBankFileAsync* bank = (CBankFileAsync*)bankRaw; static CThreadedFile tf; bank->ThreadedFile.Data = &tf;
    static unsigned char dataRaw[sizeof(CBankFileAsyncData)]; memset(dataRaw, 0, sizeof(dataRaw));
    CBankFileAsyncData* data = (CBankFileAsyncData*)dataRaw; char buffer[16]; data->Buffer = buffer; data->BankFile = bank;
    static unsigned char entryRaw[sizeof(CBankFileAsyncEntry)]; memset(entryRaw, 0, sizeof(entryRaw));
    CBankFileAsyncEntry* e = (CBankFileAsyncEntry*)entryRaw; e->m_pBank = bank; e->m_index = 2;
    bank->DisableAsync = true;
    e->SetupRead(0x10, data, 100, 0, 0, THREADED_FILE_PRIORITY_DEFAULT);
    CHECK(g_setups == 1 && g_readOff == 0x2010 && g_readBuf == buffer && g_readSize == 100 && data->Finished && g_queued == 0);
    bank->DisableAsync = false; data->Finished = false;
    e->SetupRead(0x20, data, 100, 0, 0, (EThreadedFilePriority)1);
    CHECK(g_queued == &data->LoadDesc && data->LoadDesc.PDst == buffer && data->LoadDesc.FilePos == 0x2020 && data->LoadDesc.Size == 100);
    CHECK(data->LoadDesc.PCallback == data && data->LoadDesc.Priority == 1 && !data->LoadDesc.QueueWhenFinished && data->ThreadedFileUID == 0xabc && !data->Finished);
    bank->NonCached = true;
    e->SetupRead(0, data, 100, 0, 0, THREADED_FILE_PRIORITY_DEFAULT);
    CHECK(data->LoadDesc.Size == 0x800 && data->LoadDesc.FilePos == 0x2000 && data->LoadDesc.Priority == 3);
    if (fails == 0) printf("009d5970_TEST PASS\n"); else printf("009d5970_TEST FAIL\n");
    return fails;
}