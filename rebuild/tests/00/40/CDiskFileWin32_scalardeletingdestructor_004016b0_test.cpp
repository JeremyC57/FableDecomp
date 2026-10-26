#include <cstdio>

class CDiskFileWin32 { public:
    void* vptr;
    char pad[0x18 - 4];
    int member18;
};

static const void* CDiskFileWin32_vftable = (const void*)0x122d06c;
static int g_599270 = 0;
static int g_m18 = 0;
static int g_base = 0;
static int g_freed = 0;

void __fastcall Member599270(void* win32) { (void)win32; g_599270++; }
void __fastcall Member18Dtor(void* p) { (void)p; g_m18++; }
void __fastcall BaseDtor_0099a300(void* win32) { (void)win32; g_base++; }
void __cdecl operator delete(void* p) { (void)p; g_freed++; }

void* __fastcall CDiskFileWin32_scalar_deleting_destructor(CDiskFileWin32* win32, int edx, unsigned int flags)
{
    (void)edx;
    win32->vptr = (void*)CDiskFileWin32_vftable;
    Member599270(win32);
    Member18Dtor((char*)win32 + 0x18);
    BaseDtor_0099a300(win32);
    if (flags & 1) {
        operator delete(win32);
    }
    return win32;
}

int main() {
    CDiskFileWin32 a;
    a.vptr = 0;
    void* r = CDiskFileWin32_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.vptr != (void*)CDiskFileWin32_vftable) { std::printf("FAIL retval/vptr\n"); return 1; }
    if (g_599270 != 1 || g_m18 != 1 || g_base != 1 || g_freed != 0) { std::printf("FAIL nofree calls\n"); return 1; }

    CDiskFileWin32 b;
    b.vptr = 0;
    CDiskFileWin32_scalar_deleting_destructor(&b, 0, 1);
    if (g_freed != 1) { std::printf("FAIL free-flag\n"); return 1; }

    std::printf("CThingTrackNode_004016b0_TEST PASS\n");
    return 0;
}