// test for 00437040 ~CEngineManager
#include <stdio.h>
#include <string.h>
#include <new>
#include <stdlib.h>
class CIEngine;
template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() {}
};
template <class T> class list {                          // STLport list<T>: one pointer to the sentinel node
public:
    ~list();                                             // 0x00436900
    void* _M_node;
};
template <class T> class CMallocVector {                 // STLport vector<T> over the malloc allocator
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CMallocVector() { if (_M_start) free(_M_start); }
};
struct C2DBoxI { long Left, Top, Right, Bottom; };
class CEngineManager : public CTBaseSingleton<CEngineManager> {
public:
    list<CIEngine*>        Engines;                      // +0x04
    CMallocVector<C2DBoxI> ChangedAreas;                 // +0x08
};
void DestroyEngineManager(CEngineManager* p)             // emission helper, not a retail function
{
    p->CEngineManager::~CEngineManager();
}

static void* g_list; static int g_listcalls;
template <> list<CIEngine*>::~list() { g_list = this; ++g_listcalls; }
int main()
{
    static unsigned char buf[0x14];
    CEngineManager* m = new (buf) CEngineManager;
    m->ChangedAreas._M_start = (C2DBoxI*)malloc(4 * sizeof(C2DBoxI));
    m->ChangedAreas._M_finish = m->ChangedAreas._M_start;
    m->ChangedAreas._M_end_of_storage = m->ChangedAreas._M_start + 4;
    DestroyEngineManager(m);
    int ok = sizeof(CEngineManager) == 0x14 && g_listcalls == 1 && g_list == buf + 4;
    CEngineManager* e = new (buf) CEngineManager;
    e->ChangedAreas._M_start = 0;
    DestroyEngineManager(e);
    ok &= g_listcalls == 2;
    printf(ok ? "00437040_TEST PASS\n" : "00437040_TEST FAIL\n");
    return ok ? 0 : 1;
}