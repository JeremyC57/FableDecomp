#include <cstdio>
#include <cstdlib>
// Behaviour of CTCAnimatingObject::ClearQueuedAnims (0x007F8BE0): every queued node is freed and the sentinel is
// linked back to itself.
static int g_freed = 0;
static void TestFree(void* p) { ++g_freed; std::free(p); }
class CAnimInit { public: long UpdateMode; unsigned char Type; bool Looping; long Anim; float Speed; float Alpha; float LastAlpha; long Frame[2]; unsigned long Flags; unsigned long TypeFlags; long Priority; };
template <class T> struct SListNode { SListNode* _M_next; SListNode* _M_prev; T _M_data; };
template <class T> struct CRetailList {
    SListNode<T>* _M_node;
    void clear()
    {
        SListNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SListNode<T>* tmp = cur;
            cur = cur->_M_next;
            TestFree(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
class CTCAnimatingObject { public:
    void* m_vtable; void* m_pThing; unsigned char m_pad08[4];
    bool Animating; long CurrentAnimationPriority;
    CRetailList<CAnimInit> QueuedAnims;
    void ClearQueuedAnims() { QueuedAnims.clear(); }
};
int main()
{
    SListNode<CAnimInit> head;
    SListNode<CAnimInit>* prev = &head;
    for (int i = 0; i < 3; ++i) {
        SListNode<CAnimInit>* n = (SListNode<CAnimInit>*)std::malloc(sizeof(SListNode<CAnimInit>));
        prev->_M_next = n; n->_M_prev = prev; prev = n;
    }
    prev->_M_next = &head; head._M_prev = prev;
    CTCAnimatingObject o; o.QueuedAnims._M_node = &head;
    o.ClearQueuedAnims();
    if (g_freed != 3 || head._M_next != &head || head._M_prev != &head) { std::printf("007f8be0_TEST FAIL\n"); return 1; }
    o.ClearQueuedAnims();
    if (g_freed != 3) { std::printf("007f8be0_TEST FAIL\n"); return 1; }
    std::printf("007f8be0_TEST PASS\n");
    return 0;
}