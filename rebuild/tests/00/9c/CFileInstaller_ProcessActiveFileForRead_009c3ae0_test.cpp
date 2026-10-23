// 009c3ae0 behaviour test: CFileInstaller::ProcessActiveFileForRead (control flow + data of the final.cpp body)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct OVERLAPPED_Fable { unsigned long Internal, InternalHigh, Offset, OffsetHigh; void* hEvent; };
typedef void (__stdcall *LPOVERLAPPED_COMPLETION_ROUTINE_Fable)(unsigned long, unsigned long, OVERLAPPED_Fable*);
static int g_readOk = 1; static unsigned long g_readBytes = 0; static OVERLAPPED_Fable* g_readOv = 0; static void* g_readBuf = 0;
int ReadFileEx(void* file, void* buffer, unsigned long bytes, OVERLAPPED_Fable* ov, LPOVERLAPPED_COMPLETION_ROUTINE_Fable)
{ g_readBytes = bytes; g_readOv = ov; g_readBuf = buffer; return g_readOk; }
struct CBufferChunk { unsigned char* Mem; bool Used; bool IsInput; };
static CBufferChunk g_chunk; static int g_freed = 0, g_dirty = 0, g_allocs = 0; static bool g_haveChunk = true;
struct CFileInstaller {
    struct CActiveFile {
        void* SrcFileHandle; void* DstFileHandle; unsigned long CurrentSrcOffset; unsigned long NoBytesRead; unsigned long NoBytesWritten;
        unsigned long InputSize; unsigned long OutputSize; void* PFileInfo[2]; unsigned long MinWriteSize; unsigned long MinReadSize;
    };
    struct CActiveRead {
        CActiveFile* PActiveFile; long FilePos; long TransferSize; CBufferChunk* PBufferChunk; OVERLAPPED_Fable OverlappedInfo; CFileInstaller* PFileInstaller;
        CActiveRead(CFileInstaller* installer, CActiveFile& file, CBufferChunk* chunk, long pos)
            : PActiveFile(&file), FilePos(pos), PBufferChunk(chunk), PFileInstaller(installer) {}
    };
    unsigned char* PReadBuffer;
    long AvailableBuffers;
    long AllocatedInputBuffers;
    long NoActiveReads;
    bool WriteMode;
    unsigned long BufferChunkSize;
    unsigned long NoBufferChunks;
    CBufferChunk* AllocateBuffer(bool) { ++g_allocs; return g_haveChunk ? &g_chunk : 0; }
    void FreeBuffer(CBufferChunk*) { ++g_freed; }
    static void __fastcall SetDirtyDiscError(bool e) { g_dirty = e ? 1 : 2; }
    static void __stdcall OnFileReadCompletion(unsigned long, unsigned long, OVERLAPPED_Fable*) {}
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
int main()
{
    CFileInstaller fi; unsigned char buf[4]; fi.PReadBuffer = buf; fi.AvailableBuffers = 1; fi.AllocatedInputBuffers = 0;
    fi.NoActiveReads = 0; fi.WriteMode = false; fi.BufferChunkSize = 1000; fi.NoBufferChunks = 4;
    CFileInstaller::CActiveFile f; memset(&f, 0, sizeof f); f.InputSize = 2500; f.MinReadSize = 64; f.CurrentSrcOffset = 0;

    // first read: size 2500 > chunkMax (1000/64*64 = 960) -> 960, bytes 960
    fi.ProcessActiveFileForRead(f);
    CHECK(g_allocs == 1 && g_readBytes == 960 && f.CurrentSrcOffset == 960 && fi.NoActiveReads == 1);
    CFileInstaller::CActiveRead* r = (CFileInstaller::CActiveRead*)((char*)g_readOv - offsetof(CFileInstaller::CActiveRead, OverlappedInfo));
    CHECK(r->TransferSize == 960 && r->FilePos == 0 && r->OverlappedInfo.Offset == 0 && r->OverlappedInfo.hEvent == r && r->OverlappedInfo.Internal == 0);
    CHECK(g_readBuf == buf && r->PBufferChunk == &g_chunk && r->PActiveFile == &f);
    delete r;

    // tail read: 2500-1920 = 580 -> bytes rounded up to 640
    f.CurrentSrcOffset = 1920;
    fi.ProcessActiveFileForRead(f);
    r = (CFileInstaller::CActiveRead*)((char*)g_readOv - offsetof(CFileInstaller::CActiveRead, OverlappedInfo));
    CHECK(r->TransferSize == 580 && g_readBytes == 640 && f.CurrentSrcOffset == 2560 && fi.NoActiveReads == 2 && r->OverlappedInfo.Offset == 1920);
    delete r;

    // read failure: dirty disc, buffer freed, offset kept
    f.CurrentSrcOffset = 100; g_readOk = 0;
    fi.ProcessActiveFileForRead(f);
    CHECK(g_dirty == 1 && g_freed == 1 && f.CurrentSrcOffset == 100 && fi.NoActiveReads == 2);
    g_readOk = 1;

    // no chunk: nothing; no buffers available: write mode; finished file: nothing
    g_haveChunk = false; int a = g_allocs; fi.ProcessActiveFileForRead(f); CHECK(g_allocs == a + 1 && f.CurrentSrcOffset == 100);
    g_haveChunk = true; fi.AvailableBuffers = 0; fi.ProcessActiveFileForRead(f); CHECK(fi.WriteMode);
    fi.WriteMode = false; fi.AvailableBuffers = 1; fi.AllocatedInputBuffers = 3; fi.ProcessActiveFileForRead(f); CHECK(fi.WriteMode);
    fi.WriteMode = false; f.CurrentSrcOffset = 2500; a = g_allocs; fi.ProcessActiveFileForRead(f); CHECK(!fi.WriteMode && g_allocs == a);

    if (!g_fail) printf("009c3ae0_TEST PASS\n");
    return g_fail;
}