// CFileInstaller::ProcessActiveFileForRead(CActiveFile& file) -- while the source still has unread bytes: when an input buffer is free, queue an overlapped ReadFileEx of the next sector-rounded block (bounded by the chunk size) and advance the read position; when the read cannot start, flag a dirty-disc error and give the buffer back; with no free buffer switch the installer to write mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3AE0, length 308 bytes (+ int3 padding), `ret 4`.
// Signature from Ego_r.pdb: ?ProcessActiveFileForRead@CFileInstaller@@IAEXAAVCActiveFile@1@@Z.
// Callees: CFileInstaller::AllocateBuffer(bool) @ 0x009C3A60 and FreeBuffer(CBufferChunk*) @ 0x009C3330 (both landed),
//   SetDirtyDiscError(bool) @ 0x009C3530 (static __fastcall, out of line here), operator new @ 0x00BFEA1A (0x28 bytes),
//   operator delete (MSVCR71 import thunk @ 0x00BFE9BC), KERNEL32 ReadFileEx (IAT 0x0143FDF4) with the completion routine
//   OnFileReadCompletion @ 0x009C37A0.
// Layout: retail CFileInstaller (PDB minus 4 past the 12-byte BufferChunks vector, minus 0x20 past the containers):
//   PReadBuffer +0x28, AvailableBuffers +0x30, AllocatedInputBuffers +0x34, NoActiveReads +0x58, WriteMode +0x60,
//   BufferChunkSize +0x64, NoBufferChunks +0x68; CActiveFile (PDB): SrcFileHandle +0, CurrentSrcOffset +8, InputSize
//   +0x14, MinReadSize +0x28; CActiveRead (PDB, 0x28): PActiveFile +0, FilePos +4, TransferSize +8, PBufferChunk +0xc,
//   OverlappedInfo +0x10, PFileInstaller +0x24.
// Shape: the CActiveRead ctor is inline (init-list stores under the new's null test); the OVERLAPPED is cleared with a
//   memset after the join, then Offset and hEvent set; the transfer size is the remainder rounded up to the sector size
//   and capped at the sector-rounded chunk size; the rounded byte count is homed in the dead `file` slot, the chunk in
//   the `push ecx` frame word.
extern "C" void* __cdecl memset(void* dst, int value, unsigned int count);
#pragma intrinsic(memset)
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* p);                  // 0x00BFE9BC (MSVCR71)

struct OVERLAPPED_Fable {
    unsigned long Internal;
    unsigned long InternalHigh;
    unsigned long Offset;
    unsigned long OffsetHigh;
    void*         hEvent;
};
typedef void (__stdcall *LPOVERLAPPED_COMPLETION_ROUTINE_Fable)(unsigned long error, unsigned long bytes, OVERLAPPED_Fable* overlapped);
extern "C" __declspec(dllimport) int __stdcall ReadFileEx(void* file, void* buffer, unsigned long bytes, OVERLAPPED_Fable* overlapped,
                                                        LPOVERLAPPED_COMPLETION_ROUTINE_Fable routine);   // IAT 0x0143FDF4

struct CBufferChunk {
    unsigned char* Mem;
    bool Used;                                          // +0x04
    bool IsInput;                                       // +0x05
};

struct CFileInstaller {
    struct CActiveFile {
        void*         SrcFileHandle;                    // +0x00
        void*         DstFileHandle;                    // +0x04
        unsigned long CurrentSrcOffset;                 // +0x08
        unsigned long NoBytesRead;                      // +0x0c
        unsigned long NoBytesWritten;                   // +0x10
        unsigned long InputSize;                        // +0x14
        unsigned long OutputSize;                       // +0x18
        void*         PFileInfo[2];                     // +0x1c
        unsigned long MinWriteSize;                     // +0x24
        unsigned long MinReadSize;                      // +0x28
    };
    struct CActiveRead {                                // 0x28 bytes
        CActiveFile*     PActiveFile;                   // +0x00
        long             FilePos;                       // +0x04
        long             TransferSize;                  // +0x08
        CBufferChunk*    PBufferChunk;                  // +0x0c
        OVERLAPPED_Fable OverlappedInfo;                // +0x10
        CFileInstaller*  PFileInstaller;                // +0x24
        CActiveRead(CFileInstaller* installer, CActiveFile& file, CBufferChunk* chunk, long pos)
            : PActiveFile(&file), FilePos(pos), PBufferChunk(chunk), PFileInstaller(installer) {}
    };

    void*          _vptr;                               // +0x00
    unsigned char  _pad04[0x28 - 0x04];
    unsigned char* PReadBuffer;                         // +0x28
    unsigned char* PWriteBuffer;                        // +0x2c
    long           AvailableBuffers;                    // +0x30
    long           AllocatedInputBuffers;               // +0x34
    unsigned char  _pad38[0x58 - 0x38];
    long           NoActiveReads;                       // +0x58
    long           NoActiveWrites;                      // +0x5c
    bool           WriteMode;                           // +0x60
    unsigned long  BufferChunkSize;                     // +0x64
    unsigned long  NoBufferChunks;                      // +0x68

    CBufferChunk* AllocateBuffer(bool pending);         // 0x009C3A60
    void FreeBuffer(CBufferChunk* chunk);               // 0x009C3330
    static void __fastcall SetDirtyDiscError(bool error);   // 0x009C3530
    static void __stdcall OnFileReadCompletion(unsigned long error, unsigned long bytes, OVERLAPPED_Fable* overlapped);   // 0x009C37A0
    void ProcessActiveFileForRead(CActiveFile& file);
};

void CFileInstaller::ProcessActiveFileForRead(CActiveFile& file)
{
    if (file.CurrentSrcOffset < file.InputSize) {
        if (AllocatedInputBuffers < NoBufferChunks - 1 && AvailableBuffers) {
            CBufferChunk* chunk = AllocateBuffer(true);
            if (chunk) {
                CActiveRead* read = new CActiveRead(this, file, chunk, file.CurrentSrcOffset);
                memset(&read->OverlappedInfo, 0, sizeof(read->OverlappedInfo));
                read->OverlappedInfo.Offset = file.CurrentSrcOffset;
                read->OverlappedInfo.hEvent = read;
                unsigned long minRead = file.MinReadSize;
                unsigned long size = file.InputSize - file.CurrentSrcOffset;
                if (file.CurrentSrcOffset == 0 && size > file.InputSize) {
                    unsigned long rem = size % BufferChunkSize;
                    if (rem)
                        size = (rem + minRead - 1) / minRead * minRead;
                }
                unsigned long chunkMax = BufferChunkSize / minRead * minRead;
                if (size > chunkMax)
                    size = chunkMax;
                read->TransferSize = size;
                unsigned long bytes = (size + minRead - 1) / minRead * minRead;
                if (ReadFileEx(file.SrcFileHandle, PReadBuffer, bytes, &read->OverlappedInfo, OnFileReadCompletion)) {
                    file.CurrentSrcOffset += bytes;
                    ++NoActiveReads;
                } else {
                    SetDirtyDiscError(true);
                    FreeBuffer(chunk);
                    delete read;
                }
            }
        } else
            WriteMode = true;
    }
}