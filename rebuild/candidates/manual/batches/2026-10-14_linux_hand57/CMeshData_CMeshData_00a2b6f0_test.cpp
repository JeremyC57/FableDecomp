#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_deleted = 0; struct CObj { virtual void Destroy(int) { } virtual void Delete() { ++g_deleted; } unsigned long IVRefCount; };
static CObj* g_objs = 0; struct CDefPtr { CObj* m_pObject; ~CDefPtr() { CObj* obj = m_pObject; if (obj) { char b[2] = { (char)('0' + (obj - g_objs)), 0 }; std::strcat(g_seq, b); if (--obj->IVRefCount == 0) obj->Delete(); m_pObject = 0; } } };
struct CMeshData : CObj { CDefPtr m_lods[4]; CDefPtr m_pSkeleton; ~CMeshData() { } };

int main() {
    CObj o[5]; g_objs = o; for (int i = 0; i < 5; ++i) o[i].IVRefCount = (i == 2) ? 1 : 2; { CMeshData* m = new CMeshData; for (int i = 0; i < 4; ++i) m->m_lods[i].m_pObject = &o[i]; m->m_pSkeleton.m_pObject = &o[4]; m->m_lods[1].m_pObject = 0; delete m; }
    if (std::strcmp(g_seq, "4320") != 0 || g_deleted != 1 || o[0].IVRefCount != 1 || o[2].IVRefCount != 0 || o[4].IVRefCount != 1) { std::printf("00a2b6f0_TEST FAIL\n"); return 1; }
    std::printf("00a2b6f0_TEST PASS\n"); return 0;
}
