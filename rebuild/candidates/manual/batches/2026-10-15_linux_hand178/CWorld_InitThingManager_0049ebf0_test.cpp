#include <cstdio>
#include <cstring>
static int g_deleted = 0; static void* g_current = 0;
struct TM { int id; virtual ~TM() { ++g_deleted; } };
template <class T> struct sp { T* ptr; void reset(T* p) { T* old = ptr; ptr = p; delete old; } };
static bool Init(sp<TM>& m) { TM* t = new TM; t->id = 7; m.reset(t); g_current = m.ptr; return true; }
static bool Run() { sp<TM> m; m.ptr = 0; Init(m); if (g_deleted != 0 || g_current != m.ptr) return false; Init(m); bool ok = g_deleted == 1 && g_current == m.ptr; m.reset(0); return ok && g_deleted == 2; }
int main()
{
    if (!Run()) { std::printf("0049ebf0_TEST FAIL\n"); return 1; }
    std::printf("0049ebf0_TEST PASS\n");
    return 0;
}
