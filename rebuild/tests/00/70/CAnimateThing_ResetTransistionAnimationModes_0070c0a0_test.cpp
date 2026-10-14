// Behaviour test for CAnimateThing::ResetTransistionAnimationModes @ 0x0070c0a0: every queued delayed animation with mode == from gets mode = to.
#include <cstdio>
template<class T> struct CRetailList { struct SNode { SNode* next; SNode* prev; T value; }; void* m_allocator; SNode* m_head; unsigned long m_size; };
enum EAnimUpdateMode { AUM_NONE, AUM_ONE, AUM_TWO, AUM_THREE };
struct CDelayedAnimation { EAnimUpdateMode UpdateMode; long m_value4; long m_value8; };
struct CAnimateThing {
    unsigned char CurrentAnim[0x50]; CRetailList<CDelayedAnimation> DelayedAnimQueue;
    void ResetTransistionAnimationModes(EAnimUpdateMode from, EAnimUpdateMode to)
    {
        for (CRetailList<CDelayedAnimation>::SNode* node = DelayedAnimQueue.m_head->next; node != DelayedAnimQueue.m_head; node = node->next)
            if (node->value.UpdateMode == from) node->value.UpdateMode = to;
    }
};
int main()
{
    typedef CRetailList<CDelayedAnimation>::SNode SNode;
    SNode head, n1, n2, n3;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &head;
    n1.value.UpdateMode = AUM_ONE; n2.value.UpdateMode = AUM_TWO; n3.value.UpdateMode = AUM_ONE;
    CAnimateThing t; t.DelayedAnimQueue.m_head = &head; t.DelayedAnimQueue.m_size = 3;
    t.ResetTransistionAnimationModes(AUM_ONE, AUM_THREE);
    bool ok = n1.value.UpdateMode == AUM_THREE && n2.value.UpdateMode == AUM_TWO && n3.value.UpdateMode == AUM_THREE;
    head.next = &head; t.ResetTransistionAnimationModes(AUM_TWO, AUM_NONE);       // empty list: nothing touched
    if (n2.value.UpdateMode != AUM_TWO) ok = false;
    if (!ok) { std::printf("0070c0a0_TEST FAIL\n"); return 1; }
    std::printf("0070c0a0_TEST PASS\n");
    return 0;
}