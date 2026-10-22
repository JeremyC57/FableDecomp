// Grade: DIFFER(877v909) -- first-pass reconstruction: retail keeps the 'finished' flag and the read size in the dead argument slot (known lane gap), rotates the decompress loop with the finished test at the top, and lays the dirty-disc exits out of line
// CFileInstaller::ProcessActiveFileForDecompression(CActiveFile* file) -- while the zlib stream is active, decompress into the file's destination chunk (queueing it as a pending write when full or finished); otherwise feed the stream the next completed read in source order
// Leaf: ?ProcessActiveFileForDecompression@CFileInstaller
// Layout: retail CActiveFile (0xb0, see 0x009C4330) and CFileInstaller (critical section +4, BufferChunkSize +0x64, Event +0x84).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
typedef void* HANDLE;
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long ms);
extern "C" __declspec(dllimport) int __stdcall SetEvent(HANDLE h);
struct _RTL_CRITICAL_SECTION;
struct CCriticalSectionLock {
    void* _w[2];
    CCriticalSectionLock(_RTL_CRITICAL_SECTION* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                                // 0x00A629A0
};
struct CCodeSectionManager {
    void* __vftable;
    unsigned char CriticalSection[0x18];
    unsigned char _pad1c[0x5e];
    bool DirtyDiscError;                                    // +0x7a
};
CCodeSectionManager* __fastcall GetCodeSectionManager();    // 0x00404440 (CTBaseSingleton<>::Get, folded)
extern bool g_DirtyDiscError;                               // 0x013CA814
__forceinline void __fastcall SetDirtyDiscError(bool error) // 0x009C3530
{
    CCriticalSectionLock lock((_RTL_CRITICAL_SECTION*)GetCodeSectionManager()->CriticalSection, false);
    g_DirtyDiscError = error;
    GetCodeSectionManager()->DirtyDiscError = GetCodeSectionManager()->DirtyDiscError || error;
}
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
struct CFileInstaller;
CFileInstaller* __fastcall GetFileInstaller();              // 0x00404440 (CTBaseSingleton<CFileInstaller>::Get)
struct CFileInstaller {
    void* __vftable;
    unsigned char CriticalSection[0x18];                    // +0x04
    unsigned char _pad1c[0x48];
    unsigned long BufferChunkSize;                          // +0x64
    unsigned char _pad68[0x1c];
    HANDLE Event;                                           // +0x84
    CBufferChunk* AllocateBuffer(bool input);               // 0x009C3A60
    void FreeBuffer(CBufferChunk* chunk);                   // 0x009C3330
    void StartWritingFile(CActiveFile* file, unsigned long outputSize);   // 0x009C3660
    void ProcessActiveFileForDecompression(CActiveFile* file);
};

void CFileInstaller::ProcessActiveFileForDecompression(CActiveFile* file)
{
    if (file->Decompressor.IsStreamActive()) {
        if (!file->PDecompressDstBuffer) {
            CCriticalSectionLock lock((_RTL_CRITICAL_SECTION*)CriticalSection, false);
            file->PDecompressDstBuffer = GetFileInstaller()->AllocateBuffer(false);
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
            CCriticalSectionLock lock((_RTL_CRITICAL_SECTION*)CriticalSection, false);
            GetFileInstaller()->FreeBuffer(file->PDecompressSrcBuffer);
            file->PDecompressSrcBuffer = 0;
        }
        unsigned long used = file->DecompressDstUsed;
        if (used == BufferChunkSize || flush) {
            CCriticalSectionLock lock((_RTL_CRITICAL_SECTION*)CriticalSection, false);
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
        CCriticalSectionLock lock((_RTL_CRITICAL_SECTION*)CriticalSection, false);
        GetFileInstaller()->FreeBuffer(file->PDecompressSrcBuffer);
        file->PDecompressSrcBuffer = 0;
    }
    CCriticalSectionLock lock((_RTL_CRITICAL_SECTION*)CriticalSection, false);
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
