// Test for CCutsceneDef::~CCutsceneDef (0x00F29DC0): the implicit destructor destroys the eight
// std::vector<CCharString> members in reverse declaration order (+0x90 down to +0x3C; each vector destroys its strings
// front to back, then frees its buffer), then the parent's SubDefInfoMap buffer is freed and the CDefClassBase dtor runs.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_log[64]; static int g_n = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
struct VectorCCharString {
    CCharString* start; CCharString* finish; int tag;
    VectorCCharString() : start(0), finish(0), tag(0) { }
    void Fill(int t, int n) { tag = t; start = (CCharString*)std::malloc(n * sizeof(CCharString)); finish = start + n; for (int i = 0; i < n; ++i) new (start + i) CCharString(), start[i].id = t * 10 + i; }
    ~VectorCCharString() { for (CCharString* q = start; q != finish; ++q) q->~CCharString(); if (start) { std::free(start); g_log[g_n++] = -tag; } }
};
class CDefClassBase { public: virtual ~CDefClassBase() { g_log[g_n++] = 999; } };
class CParentDefClassBase : public CDefClassBase { public:
    int* SubDefInfoMap;
    CParentDefClassBase() : SubDefInfoMap(0) { }
    ~CParentDefClassBase() { if (SubDefInfoMap) { delete SubDefInfoMap; g_log[g_n++] = 500; } }
};
class CCutsceneDef : public CParentDefClassBase { public: VectorCCharString v[8]; };
int main()
{
    CCutsceneDef* d = new CCutsceneDef;
    for (int i = 0; i < 8; ++i) if (i != 3) d->v[i].Fill(i + 1, 2);     // vector 4 stays empty: no buffer, no free
    d->SubDefInfoMap = new int(1);
    CDefClassBase* b = d; delete b;
    int expect[64]; int k = 0;
    for (int i = 7; i >= 0; --i) { if (i == 3) continue; int t = i + 1; expect[k++] = t * 10; expect[k++] = t * 10 + 1; expect[k++] = -t; }
    expect[k++] = 500; expect[k++] = 999;
    if (g_n != k) { std::printf("00f29dc0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < k; ++i) if (g_log[i] != expect[i]) { std::printf("00f29dc0_TEST FAIL\n"); return 1; }
    std::printf("00f29dc0_TEST PASS\n");
    return 0;
}