#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_del = 0, g_strdtor = 0, g_freed = 0; static void* g_delData = 0;
class CCharString { public: void* d; ~CCharString() { ++g_strdtor; } };
class CAnimFilterBase { public: virtual ~CAnimFilterBase() { } };
struct CInfo { long count; void (*del)(void*); void* data; };
static void Deleter(void* p) { ++g_del; g_delData = p; }
struct CCounted { void* data; CInfo* info;
    ~CCounted() { if (info) { --info->count; if (info->count == 0) { info->del(info->data); ++g_freed; } } data = 0; info = 0; } };
struct CModeBase { virtual ~CModeBase() { } void* thing; CAnimFilterBase filter; CCharString name; };
struct CModeSitBase : CModeBase { unsigned char pad[0x14]; CCounted pending; virtual ~CModeSitBase() { } };
int main()
{
    CInfo info; info.count = 2; info.del = Deleter; int d; info.data = &d;
    { CModeSitBase m; m.name.d = 0; m.pending.data = &d; m.pending.info = &info; }
    if (info.count != 1 || g_del != 0 || g_freed != 0 || g_strdtor != 1) { std::printf("008d71d0_TEST FAIL\n"); return 1; }
    { CModeSitBase m; m.name.d = 0; m.pending.data = &d; m.pending.info = &info; }
    if (info.count != 0 || g_del != 1 || g_delData != &d || g_freed != 1 || g_strdtor != 2) { std::printf("008d71d0_TEST FAIL\n"); return 1; }
    { CModeSitBase m; m.name.d = 0; m.pending.data = 0; m.pending.info = 0; }
    if (g_del != 1 || g_strdtor != 3) { std::printf("008d71d0_TEST FAIL\n"); return 1; }
    std::printf("008d71d0_TEST PASS\n");
    return 0;
}