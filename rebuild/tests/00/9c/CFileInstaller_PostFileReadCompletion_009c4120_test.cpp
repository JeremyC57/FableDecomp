// Behaviour test for 0x009C4120 CFileInstaller::PostFileReadCompletion (account, copy, queue for decompression or write).
#include <cstdio>
#include <cstring>

static int g_deleted = 0;
static int g_events = 0;

struct Info { bool decompress; };
struct Chunk { unsigned char* mem; };
struct Write { Chunk* chunk; unsigned long pos; unsigned long length; };
struct Installer;
struct File { unsigned long bytesRead; const Info* info; Write writes[4]; int writeCount; void* decomp[4]; int decompCount; };
struct Read { File* file; long pos; long size; Chunk* chunk; Installer* owner; };
struct Installer {
    unsigned char* readBuffer; long activeReads; int event, decompEvent;
    void Post(Read* read)
    {
        long transferSize = read->size;
        read->file->bytesRead += transferSize;
        --read->owner->activeReads;
        std::memcpy(read->chunk->mem, readBuffer, transferSize);
        bool deleteRead = false;
        if (read->file->info->decompress) {
            read->file->decomp[read->file->decompCount++] = read;
            ++read->owner->decompEvent;
        } else {
            Write w; w.chunk = read->chunk; w.pos = read->pos; w.length = transferSize;
            read->file->writes[read->file->writeCount++] = w;
            deleteRead = true;
        }
        ++read->owner->event;
        if (deleteRead) { ++g_deleted; delete read; }
    }
};

int main()
{
    int ok = 1;
    unsigned char src[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    unsigned char dst[8] = { 0 };
    Chunk chunk = { dst };
    Info plain = { false }, packed = { true };
    File file; std::memset(&file, 0, sizeof file); file.info = &plain;
    Installer inst; inst.readBuffer = src; inst.activeReads = 2; inst.event = 0; inst.decompEvent = 0;
    Read* r1 = new Read; r1->file = &file; r1->pos = 64; r1->size = 5; r1->chunk = &chunk; r1->owner = &inst;
    inst.Post(r1);
    if (file.bytesRead != 5 || inst.activeReads != 1 || dst[4] != 5 || dst[5] != 0) ok = 0;
    if (file.writeCount != 1 || file.writes[0].pos != 64 || file.writes[0].length != 5 || g_deleted != 1 || inst.event != 1) ok = 0;
    file.info = &packed;
    Read* r2 = new Read; r2->file = &file; r2->pos = 0; r2->size = 8; r2->chunk = &chunk; r2->owner = &inst;
    inst.Post(r2);
    if (file.bytesRead != 13 || file.decompCount != 1 || file.decomp[0] != r2 || g_deleted != 1 || inst.decompEvent != 1 ||
        inst.event != 2 || dst[7] != 8) ok = 0;
    delete r2;
    if (!ok) { std::printf("009c4120_TEST FAIL\n"); return 1; }
    std::printf("009c4120_TEST PASS\n");
    return 0;
}