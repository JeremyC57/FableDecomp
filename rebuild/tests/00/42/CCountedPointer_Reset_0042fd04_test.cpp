// behaviour test for 0x0042fd04 CCountedPointer<CIEngine >::Reset(p): release the old share (deleting the old object
// on the last reference), adopt p with a fresh count block {1, DeleteData, p}; a null p leaves the pointer empty
#include <cstdio>
static int gDeleted = 0;
static void Del(void* p) { if (p) ++gDeleted; }
struct Info { unsigned int refs; void (*del)(void*); void* data; };
struct Ptr {
    void* data; Info* info;
    void ReleaseData()
    {
        if (info) { --info->refs; if (info->refs == 0) { info->del(info->data); delete info; } }
        data = 0; info = 0;
    }
    void Reset(void* p)
    {
        ReleaseData();
        data = p;
        if (p) { info = new Info; info->refs = 1; info->del = Del; info->data = data; }
    }
};
int main()
{
    int fails = 0;
    int a = 1, b = 2;
    Ptr x; x.data = 0; x.info = 0;
    x.Reset(&a);
    if (x.data != &a || !x.info || x.info->refs != 1 || x.info->data != &a) fails |= 1;
    x.Reset(&b);
    if (gDeleted != 1 || x.data != &b || x.info->data != &b) fails |= 2;
    x.Reset(0);
    if (gDeleted != 2 || x.data || x.info) fails |= 4;
    if (fails) { std::printf("0042fd04_TEST FAIL %d\n", fails); return 1; }
    std::printf("0042fd04_TEST PASS\n");
    return 0;
}