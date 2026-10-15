#include <cstdio>
#include <cstring>
// Behaviour of 0x0073A840 (chunk 901 piece): empty list does nothing; otherwise the front mode gets slots 6, 11, 5 in that order
static char g_log[16]; static int g_n = 0;
struct CModeBase_Retail {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual long v03() { return 3; }
    virtual long v04() { return 4; }
    virtual void vfunc_5() { g_log[g_n++] = '5'; }
    virtual void vfunc_6() { g_log[g_n++] = '6'; }
    virtual long v07() { return 7; }
    virtual long v08() { return 8; }
    virtual long v09() { return 9; }
    virtual long v10() { return 10; }
    virtual void vfunc_11() { g_log[g_n++] = 'B'; }
};
struct SModeListNode_Retail { SModeListNode_Retail* next; SModeListNode_Retail* prev; CModeBase_Retail* data; };
struct CModeList_Retail {
    SModeListNode_Retail* node;
    bool empty() const { return node->next == node; }
    CModeBase_Retail* front() const { return node->next->data; }
};
struct CTCCreatureModeManager {
    unsigned char _pad_0x00[0x0c];
    CModeList_Retail PModes;
    void func_0073a840();
};
void CTCCreatureModeManager::func_0073a840()
{
    if (!PModes.empty()) { PModes.front()->vfunc_6(); PModes.front()->vfunc_11(); PModes.front()->vfunc_5(); }
}
int main()
{
    CModeBase_Retail mode; SModeListNode_Retail sentinel, first;
    sentinel.next = &sentinel; sentinel.prev = &sentinel; sentinel.data = 0;
    CTCCreatureModeManager m; m.PModes.node = &sentinel;
    m.func_0073a840();                                   // empty: nothing
    first.next = &sentinel; first.prev = &sentinel; first.data = &mode; sentinel.next = &first; sentinel.prev = &first;
    m.func_0073a840();
    g_log[g_n] = 0;
    if (std::strcmp(g_log, "6B5") != 0) { std::printf("0073a840_TEST FAIL %s\n", g_log); return 1; }
    std::printf("0073a840_TEST PASS\n"); return 0;
}