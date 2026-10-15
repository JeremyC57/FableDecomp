#include <cstdio>
#include <cstring>
// Behaviour of 0x0073A880 (chunk 901 piece): slot 9 forwards to the front mode's slot 3 only when the list is non-empty
static int g_hits = 0;
struct CModeBase_Retail {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual void vfunc_3() { ++g_hits; }
};
struct SModeListNode_Retail { SModeListNode_Retail* next; SModeListNode_Retail* prev; CModeBase_Retail* data; };
struct CModeList_Retail {
    SModeListNode_Retail* node;
    bool empty() const { return node->next == node; }
    CModeBase_Retail* front() const { return node->next->data; }
};
struct CTCCreatureModeManager {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual long v03() { return 3; }
    virtual long v04() { return 4; }
    virtual long v05() { return 5; }
    virtual long v06() { return 6; }
    virtual long v07() { return 7; }
    virtual long v08() { return 8; }
    virtual void vfunc_9();
    unsigned char _pad_0x04[0x0c - 0x04];
    CModeList_Retail PModes;
};
void CTCCreatureModeManager::vfunc_9() { if (!PModes.empty()) PModes.front()->vfunc_3(); }
int main()
{
    CModeBase_Retail mode; SModeListNode_Retail sentinel, first;
    sentinel.next = &sentinel; sentinel.prev = &sentinel; sentinel.data = 0;
    CTCCreatureModeManager m; m.PModes.node = &sentinel;
    m.vfunc_9();
    first.next = &sentinel; first.prev = &sentinel; first.data = &mode; sentinel.next = &first; sentinel.prev = &first;
    m.vfunc_9(); m.vfunc_9();
    if (g_hits != 2) { std::printf("0073a880_TEST FAIL\n"); return 1; }
    std::printf("0073a880_TEST PASS\n"); return 0;
}