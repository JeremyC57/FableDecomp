// Behaviour test for 0x00696240: STLport remove_copy_if over counted event pointers (keep unfinished, refcounts kept right)
#include <cstdio>

static int g_deleted = 0;
struct Info { unsigned int RefCount; void* Data; };
struct Event { bool Finished; };
static void DeleteEvent(void*) { ++g_deleted; }
struct Ptr {
    Event* Data; Info* Inf;
    Ptr& operator=(const Ptr& o)
    {
        Info* info = o.Inf; Event* data = o.Data;
        if (Inf != info) {
            if (Inf) { --Inf->RefCount; if (Inf->RefCount == 0) { DeleteEvent(Inf->Data); } }
            Data = data; Inf = info;
            if (info) ++info->RefCount;
        }
        return *this;
    }
};
struct IsFinished { bool operator()(const Ptr& p) const { return p.Data->Finished; } };

static Ptr* RemoveCopyIf(Ptr* first, Ptr* last, Ptr* result, IsFinished pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) { *result = *first; ++result; }
    return result;
}

int main()
{
    Event ev[6]; Info inf[6]; Ptr p[6];
    for (int i = 0; i < 6; ++i) { ev[i].Finished = (i == 1 || i == 4); inf[i].RefCount = 1; inf[i].Data = &ev[i]; p[i].Data = &ev[i]; p[i].Inf = &inf[i]; }
    // remove_if shape: find the first finished (index 1), then compact the rest onto it
    Ptr* e = RemoveCopyIf(p + 2, p + 6, p + 1, IsFinished());
    if (e != p + 4) { std::printf("00696240_TEST FAIL\n"); return 1; }
    if (p[0].Data != &ev[0] || p[1].Data != &ev[2] || p[2].Data != &ev[3] || p[3].Data != &ev[5]) { std::printf("00696240_TEST FAIL\n"); return 1; }
    // event 1 lost its only reference; 2,3 gained one copy then lost their old slot
    if (g_deleted != 1 || inf[1].RefCount != 0 || inf[2].RefCount != 1 || inf[3].RefCount != 1 || inf[5].RefCount != 2) { std::printf("00696240_TEST FAIL\n"); return 1; }
    std::printf("00696240_TEST PASS\n");
    return 0;
}