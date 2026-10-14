#include <cstdio>
#include <stddef.h>
struct SNode { SNode* firstChild; SNode* nextSibling; unsigned char _pad[8]; void* item; };
static SNode* g_node = 0; static long g_index = -1; static long* g_counter = 0; static long g_counterValue = -1;
struct CVtable_012b2f0c {
    virtual void v00() { }
    unsigned char _body[0x1c]; SNode* m_pFirstChild;
    void* FindNthInSubtree(SNode* node, long index, long* counter) { g_node = node; g_index = index; g_counter = counter; g_counterValue = *counter; *counter = 99; return node->item; }
    void* GetItemByIndex(long index) { long counter = 0; return FindNthInSubtree(m_pFirstChild, index, &counter); }
};
int main()
{
    int item; SNode node = { 0, 0, { 0 }, &item }; CVtable_012b2f0c tree; tree.m_pFirstChild = &node;
    void* r = tree.GetItemByIndex(5);
    if (r != &item || g_node != &node || g_index != 5 || g_counter == 0 || g_counterValue != 0 || offsetof(CVtable_012b2f0c, m_pFirstChild) != 0x20 || offsetof(SNode, item) != 0x10) { std::printf("00c190f0_TEST FAIL\n"); return 1; }
    std::printf("00c190f0_TEST PASS\n"); return 0;
}