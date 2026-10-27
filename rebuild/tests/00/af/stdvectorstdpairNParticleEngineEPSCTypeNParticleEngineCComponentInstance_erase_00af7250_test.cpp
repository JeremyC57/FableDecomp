// Behaviour test for 0x00AF7250 std::vector<pair<EPSCType, CComponentInstance> >::erase(iterator)
// Model: shift the tail down one (assignment), destroy the old last entry (counted-pointer release), return position.
#include <cstdio>
static int g_released = 0;
struct Info { long RefCount; };
struct CPtr {
    int* Data; Info* PInfo;
    CPtr& operator=(const CPtr& o) { if (o.PInfo) ++o.PInfo->RefCount; release(); Data = o.Data; PInfo = o.PInfo; return *this; }
    void release() { if (PInfo) { --PInfo->RefCount; if (PInfo->RefCount == 0) ++g_released; } Data = 0; PInfo = 0; }
    ~CPtr() { release(); }
};
struct CComponentInstance { CPtr Component; bool Enabled; unsigned long InstanceName; };
struct Entry { int first; CComponentInstance second; };
struct TVector {
    Entry* _M_start; Entry* _M_finish;
    Entry* erase(Entry* position)
    {
        if (position + 1 != _M_finish)
            for (Entry* p = position; p + 1 != _M_finish; ++p) *p = *(p + 1);
        --_M_finish;
        _M_finish->~Entry();
        return position;
    }
};
int main()
{
    static unsigned char raw[3 * sizeof(Entry)];
    Entry* e = (Entry*)raw;
    Info infos[3] = { { 1 }, { 1 }, { 1 } };
    int objs[3];
    for (int i = 0; i < 3; ++i) {
        e[i].first = 10 + i; e[i].second.Component.Data = &objs[i]; e[i].second.Component.PInfo = &infos[i];
        e[i].second.Enabled = true; e[i].second.InstanceName = 100 + i;
    }
    TVector v; v._M_start = e; v._M_finish = e + 3;
    Entry* r = v.erase(e);                                 // erase the first entry
    if (r != e || v._M_finish != e + 2) { std::printf("00af7250_TEST FAIL\n"); return 1; }
    if (e[0].first != 11 || e[1].first != 12 || e[0].second.InstanceName != 101) { std::printf("00af7250_TEST FAIL\n"); return 1; }
    if (g_released != 1 || infos[0].RefCount != 0 || infos[1].RefCount != 1 || infos[2].RefCount != 1) { std::printf("00af7250_TEST FAIL\n"); return 1; }
    r = v.erase(e + 1);                                    // erase the last entry: no shift
    if (r != e + 1 || v._M_finish != e + 1 || g_released != 2) { std::printf("00af7250_TEST FAIL\n"); return 1; }
    e[0].~Entry();
    std::printf("00af7250_TEST PASS\n");
    return 0;
}