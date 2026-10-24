// Behaviour test for 0x007FB1C0 CTCKraken::KillTentacles: every tentacle in the list is killed (null entries skipped)
// and popped from the back until the list is empty; the attacking-tentacle cursor is left at end().
#include <stdio.h>
#include <stdlib.h>
#include <new>
#include <string.h>
class CBaseObject { };
class CThing : public CBaseObject {
public:
    void Kill(bool immediately);                                // 0x004C9B80
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                         // slot 0
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const;                              // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
typedef CIntelligentPointer<CThing> CIntelligentPointer_CThing;

struct _List_node_base {                                       // STLport: the links live in a non-template base
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};
template <class T> struct CListIterator {
    _List_node_base* _M_node;
    CListIterator() { }
    CListIterator(_List_node_base* n) : _M_node(n) { }
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    CListIterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> struct CRetailList {                         // STLport list (4 bytes: the head node)
    _List_node<T>* _M_node;
    CListIterator<T> end() { return CListIterator<T>(_M_node); }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    T& back()
    {
        CListIterator<T> tmp = end();
        return *(--tmp);
    }
    CListIterator<T> erase(CListIterator<T> position)
    {
        _List_node_base* next = position._M_node->_M_next;
        _List_node_base* prev = position._M_node->_M_prev;
        _List_node<T>* n = (_List_node<T>*)position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        _Destroy(&n->_M_data);
        free(n);
        return CListIterator<T>(next);
    }
    void pop_back()
    {
        CListIterator<T> tmp = end();
        erase(--tmp);
    }
};

class CTCKraken {
public:
    void*                                    __vfptr;           // +0x00
    CThing*                                  ParentThing;       // +0x04 (CTCBase)
    unsigned char                            _pad08[0x44 - 0x08];
    CRetailList<CIntelligentPointer_CThing>  Tentacles;         // +0x44
    CListIterator<CIntelligentPointer_CThing> AttackingTentacleIterator;   // +0x48
private:
    void KillTentacles();
public:
    void OnDie() { KillTentacles(); }
};

void CTCKraken::KillTentacles()
{
    while (Tentacles.size() > 0) {
        CThing* tentacle = ((_List_node<CIntelligentPointer_CThing>*)Tentacles._M_node->_M_prev)->_M_data.GetPItem();
        if (tentacle)
            tentacle->Kill(true);
        Tentacles.pop_back();
    }
    AttackingTentacleIterator = Tentacles.end();
}


static int g_kills = 0;
static CThing* g_order[8];
void CThing::Kill(bool) { g_order[g_kills++] = this; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }

typedef _List_node<CIntelligentPointer_CThing> TNode;
static void PushBack(_List_node_base* head, CThing* item)
{
    TNode* n = (TNode*)malloc(sizeof(TNode));
    new (&n->_M_data) CIntelligentPointer_CThing();
    n->_M_data.m_pItem = item;
    n->_M_prev = head->_M_prev;
    n->_M_next = head;
    head->_M_prev->_M_next = n;
    head->_M_prev = n;
}

int main()
{
    static CThing a, b, c;
    TNode* head = (TNode*)malloc(sizeof(TNode));
    head->_M_next = head; head->_M_prev = head;
    PushBack(head, &a);
    PushBack(head, 0);
    PushBack(head, &b);
    PushBack(head, &c);
    CTCKraken kraken;
    memset(&kraken, 0, sizeof(kraken));
    kraken.Tentacles._M_node = head;
    kraken.AttackingTentacleIterator._M_node = head->_M_next;

    kraken.OnDie();
    int ok = 1;
    if (g_kills != 3 || g_order[0] != &c || g_order[1] != &b || g_order[2] != &a) ok = 0;   // back to front
    if (head->_M_next != head || head->_M_prev != head) ok = 0;                            // emptied
    if (kraken.AttackingTentacleIterator._M_node != head) ok = 0;                          // parked at end()
    kraken.OnDie();                                     // empty: nothing more
    if (g_kills != 3) ok = 0;
    free(head);

    printf(ok ? "007fb1c0_TEST PASS\n" : "007fb1c0_TEST FAIL\n");
    return ok ? 0 : 1;
}