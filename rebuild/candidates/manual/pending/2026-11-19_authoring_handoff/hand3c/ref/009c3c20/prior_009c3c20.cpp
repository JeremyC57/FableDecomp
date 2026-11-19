// CFileInstaller::ProcessActiveFileForDecompression(CActiveFile* file) -- while the zlib stream is active, decompress into the file's destination chunk (queueing it as a pending write when full or finished); otherwise feed the stream the next completed read in source order
// Leaf: ?ProcessActiveFileForDecompression@CFileInstaller
// Grade: DIFFER(877v909) -- names only: landed CTBaseSingleton<CFileInstaller>::Get, CFileInstaller::DirtyDiscError /
//   ExitThread +0x7A / CriticalSection and the landed SetDirtyDiscError body (forceinline, as retail inlines it); bytes
//   unchanged from the parked draft. Left: finished flag and read size in the dead argument slot, rotated decompress
//   loop
// Layout: retail CActiveFile (0xb0, see 0x009C4330) and CFileInstaller (critical section +4, BufferChunkSize +0x64, Event +0x84).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
typedef void* HANDLE;
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long ms);
extern "C" __declspec(dllimport) int __stdcall SetEvent(HANDLE h);
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };   // Win32 critical section, 24 bytes
class CCriticalSectionLock {                    // 8 bytes on the stack (Ego_r.pdb)
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryLock);   // 0x00A62920
    ~CCriticalSectionLock();                                              // 0x00A629A0
    _RTL_CRITICAL_SECTION* CriticalSection;     // +0x00
    bool Locked;                                // +0x04
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                 // CFileInstaller: 0x00404440 (out of line)
};
struct CBufferChunk { unsigned char* Data; };
struct CCompressorZlib {
    unsigned char _pad[0x56];
    bool Error;                                             // +0x56
    unsigned char _pad57[1];
    bool IsStreamActive() const;                            // 0x00A3A7E0
    unsigned long DecompressStream(void* out, unsigned long size, bool* finished);   // 0x00A3A840
    void FinishDecompressStream();                          // 0x00A3ABB0
    void SetStreamInput(void* in, unsigned long size);      // 0x00A3AB90
};
struct CPendingWrite { CBufferChunk* Buffer; unsigned long Offset; unsigned long Size; };
struct CPendingWriteList { void* head; void push_back(const CPendingWrite& write); };   // 0x009C6AF0
struct CActiveRead { void* Overlapped; unsigned long Offset; unsigned long Size; CBufferChunk* Buffer; };
struct CReadNode { CReadNode* next; CReadNode* prev; CActiveRead* value; };
struct CActiveFile {
    unsigned char     _pad[0x14];
    unsigned long     InputSize;                            // +0x14
    unsigned char     _pad18[0x20];
    CCompressorZlib   Decompressor;                         // +0x38
    CPendingWriteList PendingWrites;                        // +0x90
    CReadNode*        PendingDecompression;                 // +0x94
    unsigned long     DecompressSrcPos;                     // +0x98
    unsigned long     DecompressDstPos;                     // +0x9c
    CBufferChunk*     PDecompressSrcBuffer;                 // +0xa0
    CBufferChunk*     PDecompressDstBuffer;                 // +0xa4
    unsigned long     DecompressDstUsed;                    // +0xa8
    bool              DecompressFinish;                     // +0xac
};
extern const unsigned long g_CompressedFileMagic;           // 0x0129B418
struct CFileInstaller {
    void* __vftable;
    _RTL_CRITICAL_SECTION CriticalSection;                  // +0x04
    unsigned char _pad1c[0x48];
    unsigned long BufferChunkSize;                          // +0x64
    unsigned char _pad68[0x12];
    bool ExitThread;                                        // +0x7a
    unsigned char _pad7b[0x09];
    HANDLE Event;                                           // +0x84
    static bool DirtyDiscError;                             // 0x013CA814
    static __forceinline void __fastcall SetDirtyDiscError(bool error)    // 0x009C3530 (inlined here)
    {
        CCriticalSectionLock lock(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
        DirtyDiscError = error;
        if (CTBaseSingleton<CFileInstaller>::Get().ExitThread || error)
            error = true;
        CTBaseSingleton<CFileInstaller>::Get().ExitThread = error;
    }
    CBufferChunk* AllocateBuffer(bool input);               // 0x009C3A60
    void FreeBuffer(CBufferChunk* chunk);                   // 0x009C3330
    void StartWritingFile(CActiveFile* file, unsigned long outputSize);   // 0x009C3660
    void ProcessActiveFileForDecompression(CActiveFile* file);
};

void CFileInstaller::ProcessActiveFileForDecompression(CActiveFile* file)
{
    if (file->Decompressor.IsStreamActive()) {
        if (!file->PDecompressDstBuffer) {
            CCriticalSectionLock lock(CriticalSection, false);
            file->PDecompressDstBuffer = CTBaseSingleton<CFileInstaller>::Get().AllocateBuffer(false);
            file->DecompressDstUsed = 0;
        }
        if (!file->PDecompressDstBuffer)
            return;
        unsigned char* out = file->PDecompressDstBuffer->Data + file->DecompressDstUsed;
        unsigned long space = BufferChunkSize - file->DecompressDstUsed;
        bool finished = false;
        while (space > 0 && !finished && file->Decompressor.IsStreamActive()) {
            unsigned long size = space < 0x40000 ? space : 0x40000;
            unsigned long written = file->Decompressor.DecompressStream(out, size, &finished);
            if (file->Decompressor.Error) {
                SetDirtyDiscError(true);
                return;
            }
            file->DecompressDstUsed += written;
            out += written;
            space -= written;
            Sleep(1);
        }
        bool flush;
        if (finished && file->DecompressFinish) {
            flush = true;
            file->Decompressor.FinishDecompressStream();
        } else {
            flush = false;
        }
        if (!file->Decompressor.IsStreamActive()) {
            CCriticalSectionLock lock(CriticalSection, false);
            CTBaseSingleton<CFileInstaller>::Get().FreeBuffer(file->PDecompressSrcBuffer);
            file->PDecompressSrcBuffer = 0;
        }
        unsigned long used = file->DecompressDstUsed;
        if (used == BufferChunkSize || flush) {
            CCriticalSectionLock lock(CriticalSection, false);
            CPendingWrite write;
            write.Buffer = file->PDecompressDstBuffer;
            write.Offset = file->DecompressDstPos;
            write.Size = used;
            file->PendingWrites.push_back(write);
            file->PDecompressDstBuffer = 0;
            file->DecompressDstPos += used;
            SetEvent(Event);
        }
        return;
    }
    if (file->PDecompressSrcBuffer) {
        CCriticalSectionLock lock(CriticalSection, false);
        CTBaseSingleton<CFileInstaller>::Get().FreeBuffer(file->PDecompressSrcBuffer);
        file->PDecompressSrcBuffer = 0;
    }
    CCriticalSectionLock lock(CriticalSection, false);
    for (CReadNode* node = file->PendingDecompression->next; node != file->PendingDecompression; node = node->next) {
        CActiveRead* read = node->value;
        if (read->Offset != file->DecompressSrcPos)
            continue;
        unsigned long* data = (unsigned long*)read->Buffer->Data;
        unsigned long size = read->Size;
        if (read->Offset == 0) {
            if (data[0] != g_CompressedFileMagic) {
                SetDirtyDiscError(true);
                delete read;
                return;
            }
            StartWritingFile(file, data[1]);
            size -= 8;
            data += 2;
        }
        file->DecompressSrcPos += read->Size;
        node->prev->next = node->next;
        node->next->prev = node->prev;
        free(node);
        file->PDecompressSrcBuffer = read->Buffer;
        file->Decompressor.SetStreamInput(data, size);
        if (read->Size + read->Offset >= file->InputSize)
            file->DecompressFinish = true;
        delete read;
        return;
    }
}
