// Behaviour test for 0x0051C760 CNavigationAreaUpdate::CNavigationLayerUpdate scalar deleting destructor: the switchable blockages are destroyed (each line list cleared and its sentinel freed), both vector blocks freed (behind a null test), and the object freed only when bit 0 of the flags is set; returns this
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static int g_frees = 0, g_deletes = 0;
static void tfree(void* p) { if (p) ++g_frees; }
struct LNode { LNode* next; LNode* prev; };
struct LineList { LNode* node; ~LineList() { LNode* c = node->next; while (c != node) { LNode* t = c; c = c->next; tfree(t); } node->next = node; node->prev = node; if (node) tfree(node); } };
struct CSwitchableBlockage { unsigned long long UniqueID; LineList Lines; };
struct CNavigationLayerUpdate {
    int* lineStart; int* lineFinish; int* lineEos;
    CSwitchableBlockage* sbStart; CSwitchableBlockage* sbFinish; CSwitchableBlockage* sbEos;
    void* Delete(unsigned flags)
    {
        for (CSwitchableBlockage* p = sbStart; p != sbFinish; ++p) p->~CSwitchableBlockage();
        if (sbStart) tfree(sbStart);
        if (lineStart) tfree(lineStart);
        if (flags & 1) ++g_deletes;
        return this;
    }
};
inline void* operator new(tsize, void* p) { return p; }
int main()
{
    static LNode s0, s1, a, b; s0.next = &a; a.next = &b; b.next = &s0; s1.next = &s1;
    static unsigned char sbRaw[2 * sizeof(CSwitchableBlockage)];
    CSwitchableBlockage* sb = (CSwitchableBlockage*)sbRaw; sb[0].Lines.node = &s0; sb[1].Lines.node = &s1;
    static int lines[4];
    CNavigationLayerUpdate u; u.lineStart = lines; u.lineFinish = lines + 4; u.lineEos = lines + 4; u.sbStart = sb; u.sbFinish = sb + 2; u.sbEos = sb + 2;
    CHECK(u.Delete(0) == &u);
    CHECK(g_frees == 2 + 1 + 1 + 1 + 1 && g_deletes == 0);    // a, b, two sentinels, the blockage block, the line block
    CHECK(s0.next == &s0 && s0.prev == &s0);
    g_frees = 0; u.sbStart = u.sbFinish = 0; u.lineStart = 0;
    CHECK(u.Delete(1) == &u && g_frees == 0 && g_deletes == 1);
    if (fails == 0) printf("0051c760_TEST PASS\n"); else printf("0051c760_TEST FAIL\n");
    return fails;
}