#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static void* g_observer = 0; static long g_param = 0;
struct CTrailListener_00c29f70;
struct CObservable_00c29f70 { virtual void AddObserver(CTrailListener_00c29f70* o) { std::strcat(g_seq, "A"); g_observer = o; } };
struct CHost_00c29f70 { unsigned char _pad[0x104]; CObservable_00c29f70 m_observable; };
struct CTrailListener_00c29f70 { long m_0; void* m_pOwner; void Update(long p) { std::strcat(g_seq, "U"); g_param = p; } void Attach(CHost_00c29f70* host, void* owner, long param) { m_pOwner = owner; host->m_observable.AddObserver(this); Update(param); } };
int main() {
    CTrailListener_00c29f70 l; CHost_00c29f70 h; int owner; l.Attach(&h, &owner, 42); if (l.m_pOwner != &owner || g_observer != &l || g_param != 42 || std::strcmp(g_seq, "AU")) { std::printf("00c29f70_TEST FAIL\n"); return 1; }
    std::printf("00c29f70_TEST PASS\n"); return 0;
}