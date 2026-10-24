// Behaviour test for 0x00531E50 NUISystem::CComponent::AddActionIAmParamOf: appends a copy of the counted action pointer (shared count + 1) at the end of the ActionsIAmParamOf list
#include <cstdio>
#include <cstdlib>
class CAction { public: int id; };
struct SCountBlock { long RefCount; };
template <class T> struct CCountedPointer {
    T*           Data;
    SCountBlock* Info;
    void Construct(const CCountedPointer& other)
    {
        Data = other.Data;
        Info = other.Info;
        if (Info)
            ++Info->RefCount;
    }
};
template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CListIterator {
    SListNode<T>* _M_node;
    CListIterator(SListNode<T>* n) : _M_node(n) { }
    CListIterator(const CListIterator& other) : _M_node(other._M_node) { }
};
template <class T> struct CRetailList {
    SListNode<T>* _M_node;
    CListIterator<T> end() { return CListIterator<T>(_M_node); }
    SListNode<T>* create_node(const T& x)
    {
        SListNode<T>* tmp = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        T* data = &tmp->_M_data;
        if (data != 0)
            data->Construct(x);
        return tmp;
    }
    CListIterator<T> insert(CListIterator<T> position, const T& x)
    {
        SListNode<T>* tmp = create_node(x);
        SListNode<T>* n = position._M_node;
        SListNode<T>* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return CListIterator<T>(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
};
class CComponent {
public:
    virtual void AddActionIAmParamOf(CCountedPointer<CAction>& action);
    virtual ~CComponent() { }
    CRetailList<CCountedPointer<CAction> > ActionsIAmParamOf;
};
void CComponent::AddActionIAmParamOf(CCountedPointer<CAction>& action)
{
    ActionsIAmParamOf.push_back(action);
}
int main()
{
    typedef SListNode<CCountedPointer<CAction> > Node;
    Node head;
    head._M_next = &head; head._M_prev = &head;
    CComponent c;
    c.ActionsIAmParamOf._M_node = &head;
    CAction a1, a2;
    SCountBlock k1; k1.RefCount = 1;
    CCountedPointer<CAction> p1; p1.Data = &a1; p1.Info = &k1;
    CCountedPointer<CAction> p2; p2.Data = &a2; p2.Info = 0;
    c.AddActionIAmParamOf(p1);
    c.AddActionIAmParamOf(p2);
    Node* first = head._M_next;
    Node* second = first->_M_next;
    if (first->_M_data.Data != &a1 || first->_M_data.Info != &k1 || k1.RefCount != 2 || second->_M_data.Data != &a2 ||
        second->_M_data.Info != 0 || second->_M_next != &head || head._M_prev != second || second->_M_prev != first)
        { std::printf("00531e50_TEST FAIL\n"); return 1; }
    free(first); free(second);
    std::printf("00531e50_TEST PASS\n");
    return 0;
}