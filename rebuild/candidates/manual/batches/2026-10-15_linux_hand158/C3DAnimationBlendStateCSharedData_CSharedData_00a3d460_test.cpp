// Behaviour test for 0x00A3D460: member release order and counted/intrusive release semantics.
#include <cstdio>
static char g_log[16]; static int g_n;
struct Owner { long refs; void Release() { g_log[g_n++] = 'O'; } };
struct Entry { ~Entry() { g_log[g_n++] = 'E'; } };
struct Obj { void Destroy() { g_log[g_n++] = 'D'; } };
struct Block { long count; void (Obj::*pfn)(); Obj* obj; };
struct Shared {
    Owner* owner; Entry* first; Entry* last; void* data; Block* info;
    void Destruct() {
        if (info) { --info->count; if (info->count == 0) { (info->obj->*info->pfn)(); g_log[g_n++] = 'B'; } }
        data = 0; info = 0;
        for (Entry* p = first; p != last; ++p) p->~Entry();
        Owner* o = owner; if (o) { if (--o->refs == 0) o->Release(); owner = 0; }
    }
};
int main()
{
    Obj ob; Block bl = {1, &Obj::Destroy, &ob}; Owner ow = {1};
    Entry* e = (Entry*)new char[2 * sizeof(Entry)];
    Shared s = {&ow, e, e + 2, &ob, &bl}; s.Destruct();
    bool ok = g_n == 5 && g_log[0] == 'D' && g_log[1] == 'B' && g_log[2] == 'E' && g_log[3] == 'E' && g_log[4] == 'O' && !s.info && !s.owner;
    Block b2 = {2, &Obj::Destroy, &ob}; Owner o2 = {2}; Shared t = {&o2, e, e, 0, &b2}; g_n = 0; t.Destruct();
    ok = ok && g_n == 0 && b2.count == 1 && o2.refs == 1;
    delete[] (char*)e;
    if (!ok) { std::printf("00a3d460_TEST FAIL\n"); return 1; }
    std::printf("00a3d460_TEST PASS\n");
    return 0;
}
