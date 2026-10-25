// Behaviour test for 0x009F7330 CVsyncCallbackTimer::~CVsyncCallbackTimer (links the source object): ClearAllQueries first,
// then PendingQueories (+0x08), then FreeQueoryObjects (+0x04), each clearing its nodes before freeing its head.
#include <cstdio>
#include <cstdlib>
#include <new>

struct IDirect3DDevice9;
struct CPendingQueryList {
    void* _M_node;
    void clear();
    __forceinline ~CPendingQueryList() { clear(); void* head = _M_node; if (head != 0) free(head); }
};
int main();
class CVsyncCallbackTimer {
public:
    ~CVsyncCallbackTimer();
private:
    friend int ::main();
    void ClearAllQueries();
    IDirect3DDevice9* Device; CPendingQueryList FreeQueoryObjects; CPendingQueryList PendingQueories; long PendingFrame; long FinishedFrame;
};

static int g_order[8]; static int g_n = 0;
static unsigned char g_buf[0x14];
void CVsyncCallbackTimer::ClearAllQueries() { g_order[g_n++] = (this == (CVsyncCallbackTimer*)g_buf) ? 100 : -1; }
void CPendingQueryList::clear() { g_order[g_n++] = (int)((unsigned char*)this - g_buf); }

int main()
{
    CVsyncCallbackTimer* t = (CVsyncCallbackTimer*)g_buf;
    t->FreeQueoryObjects._M_node = std::malloc(16);
    t->PendingQueories._M_node = 0;
    t->~CVsyncCallbackTimer();
    if (g_n != 3 || g_order[0] != 100 || g_order[1] != 8 || g_order[2] != 4) { std::printf("009f7330_TEST FAIL\n"); return 1; }
    std::printf("009f7330_TEST PASS\n");
    return 0;
}
