// Test for CMouseDX::~CMouseDX (0x00AB5320): a live DirectInput device is unacquired then released (and not cleared);
// a null device is skipped; the cursor graphic counted pointer is released; the CMouse base runs last.
#include <cstdio>
static char g_log[32]; static int g_n = 0;
struct Info { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteSurface(void*) { g_log[g_n++] = 'G'; }
struct Device { virtual long Unacquire() { g_log[g_n++] = 'U'; return 0; } virtual unsigned long Release() { g_log[g_n++] = 'R'; return 0; } };
struct Mouse { virtual ~Mouse() { g_log[g_n++] = 'M'; } };
struct MouseDX : Mouse {
    Device* DMouse; void* CursorData; Info* CursorInfo;
    ~MouseDX()
    {
        if (DMouse) { DMouse->Unacquire(); DMouse->Release(); }
        if (CursorInfo) { --CursorInfo->RefCount; if (CursorInfo->RefCount == 0) { CursorInfo->DeleteFunc(CursorInfo->Data); delete CursorInfo; } }
        CursorData = 0; CursorInfo = 0;
    }
};
int main()
{
    Device dev; int surf = 0;
    MouseDX* m = new MouseDX; m->DMouse = &dev; m->CursorData = &surf;
    m->CursorInfo = new Info; m->CursorInfo->RefCount = 1; m->CursorInfo->DeleteFunc = DeleteSurface; m->CursorInfo->Data = &surf;
    delete m;
    if (g_n != 4 || g_log[0] != 'U' || g_log[1] != 'R' || g_log[2] != 'G' || g_log[3] != 'M') { std::printf("00ab5320_TEST FAIL\n"); return 1; }
    m = new MouseDX; m->DMouse = 0; m->CursorData = 0; m->CursorInfo = 0;
    delete m;
    if (g_n != 5 || g_log[4] != 'M') { std::printf("00ab5320_TEST FAIL\n"); return 1; }
    std::printf("00ab5320_TEST PASS\n");
    return 0;
}