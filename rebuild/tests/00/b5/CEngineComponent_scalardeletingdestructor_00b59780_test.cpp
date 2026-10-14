#include <cstdio>
#include <cstdlib>
static int g_deleted = 0;
struct CEngineComponent { CEngineComponent** m_ppRegisteredSlot; long m_id; virtual ~CEngineComponent() { if (m_ppRegisteredSlot != 0) *m_ppRegisteredSlot = 0; } void operator delete(void* p) { ++g_deleted; std::free(p); } };
int main() { CEngineComponent* c = new CEngineComponent; CEngineComponent* slot = c; c->m_ppRegisteredSlot = &slot; delete c; if (slot != 0 || g_deleted != 1) { std::printf("00b59780_TEST FAIL\n"); return 1; }
    CEngineComponent* d = new CEngineComponent; d->m_ppRegisteredSlot = 0; delete d; if (g_deleted != 2) { std::printf("00b59780_TEST FAIL\n"); return 1; } std::printf("00b59780_TEST PASS\n"); return 0; }