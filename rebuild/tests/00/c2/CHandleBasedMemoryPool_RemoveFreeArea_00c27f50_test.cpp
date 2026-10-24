// behaviour test for 00c27f50: CHandleBasedMemoryPool::RemoveFreeArea -- unlink a header from its
// free list through its back-link, repoint the successor's back-link, clear status and links.
#include <cstdio>
struct CHeader {
    void* Resource;
    long Handle;
    long Size;
    short Padding;
    unsigned char Status;
    unsigned char Locked;
    CHeader** RefAvailable;
    CHeader* NextAvailable;
    CHeader* PrevContiguous;
    CHeader* NextContiguous;
};
static void RemoveFreeArea(CHeader* area)
{
    area->Status = 0;
    *area->RefAvailable = area->NextAvailable;
    if (area->NextAvailable)
        area->NextAvailable->RefAvailable = area->RefAvailable;
    area->RefAvailable = 0;
    area->NextAvailable = 0;
}
int main()
{
    CHeader a, b, c;
    CHeader* list = &a;
    a.RefAvailable = &list; a.NextAvailable = &b; a.Status = 'N';
    b.RefAvailable = &a.NextAvailable; b.NextAvailable = &c; b.Status = 'N';
    c.RefAvailable = &b.NextAvailable; c.NextAvailable = 0; c.Status = 'N';
    int ok = 1;
    RemoveFreeArea(&b);
    if (a.NextAvailable != &c || c.RefAvailable != &a.NextAvailable || b.RefAvailable || b.NextAvailable || b.Status != 0) ok = 0;
    RemoveFreeArea(&a);
    if (list != &c || c.RefAvailable != &list || a.Status != 0) ok = 0;
    RemoveFreeArea(&c);
    if (list != 0 || c.Status != 0 || c.RefAvailable) ok = 0;
    if (!ok) { std::printf("00c27f50_TEST FAIL\n"); return 1; }
    std::printf("00c27f50_TEST PASS\n");
    return 0;
}