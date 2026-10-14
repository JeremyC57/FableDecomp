// Behaviour test for CAnimateThing::GetCombinationAnimFromTypeAndIndex @ 0x0070c170: first node in either list whose type byte and pointed-to short index match.
#include <cstdio>
template<class T> struct CRetailList { struct SNode { SNode* next; SNode* prev; T value; }; void* m_allocator; SNode* m_head; unsigned long m_size; };
struct CAnimSet { long m_value0; short* m_pIndex; unsigned char _pad[0x48]; unsigned char m_type; };
struct CAnimateThing {
    unsigned char _pad[0x54]; CRetailList<CAnimSet> m_combinationAnims; CRetailList<CAnimSet> m_combinationAnims2;
    CAnimSet* GetCombinationAnimFromTypeAndIndex(long type, long index)
    {
        CRetailList<CAnimSet>::SNode* head = m_combinationAnims.m_head; CRetailList<CAnimSet>::SNode* node;
        for (node = head->next; node != head; node = node->next) if (node->value.m_type == type && *node->value.m_pIndex == index) return &node->value;
        head = m_combinationAnims2.m_head;
        for (node = head->next; node != head; node = node->next) if (node->value.m_type == type && *node->value.m_pIndex == index) return &node->value;
        return 0;
    }
};
int main()
{
    typedef CRetailList<CAnimSet>::SNode SNode;
    short idx[4] = { 1, 2, 1, -3 };
    SNode h1, a, b, h2, c, d;
    h1.next = &a; a.next = &b; b.next = &h1; h2.next = &c; c.next = &d; d.next = &h2;
    a.value.m_type = 7; a.value.m_pIndex = &idx[0]; b.value.m_type = 7; b.value.m_pIndex = &idx[1];
    c.value.m_type = 9; c.value.m_pIndex = &idx[2]; d.value.m_type = 7; d.value.m_pIndex = &idx[3];
    CAnimateThing t; t.m_combinationAnims.m_head = &h1; t.m_combinationAnims2.m_head = &h2;
    bool ok = t.GetCombinationAnimFromTypeAndIndex(7, 2) == &b.value && t.GetCombinationAnimFromTypeAndIndex(7, 1) == &a.value
        && t.GetCombinationAnimFromTypeAndIndex(9, 1) == &c.value && t.GetCombinationAnimFromTypeAndIndex(7, -3) == &d.value
        && t.GetCombinationAnimFromTypeAndIndex(8, 1) == 0 && t.GetCombinationAnimFromTypeAndIndex(9, 2) == 0;
    if (!ok) { std::printf("0070c170_TEST FAIL\n"); return 1; }
    std::printf("0070c170_TEST PASS\n");
    return 0;
}
