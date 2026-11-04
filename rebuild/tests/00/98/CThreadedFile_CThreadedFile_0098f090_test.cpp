// Behaviour test for CThreadedFile::~CThreadedFile (0x0098F090): an open file drops its manager streams, closes its handle
// and clears the open flags; a closed file does nothing.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_streamsDropped, g_handlesClosed;
struct File { void* FileHandle; bool OpenFlag, OpenedForWrite;
    void Close() { if (FileHandle != (void*)-1) { ++g_streamsDropped; ++g_handlesClosed; FileHandle = (void*)-1; OpenFlag = false; OpenedForWrite = false; } }
    ~File() { Close(); } };
int main()
{
    { File f; f.FileHandle = (void*)4; f.OpenFlag = f.OpenedForWrite = true; }
    { File g; g.FileHandle = (void*)-1; }
    bool ok = g_streamsDropped == 1 && g_handlesClosed == 1;
    printf(ok ? "0098f090_TEST PASS\n" : "0098f090_TEST FAIL\n");
    return ok ? 0 : 1;
}