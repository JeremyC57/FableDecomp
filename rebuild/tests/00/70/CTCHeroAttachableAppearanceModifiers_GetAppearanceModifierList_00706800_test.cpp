// Behaviour test for 0x00706800 CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList.
#include <cstdio>
#include <cstdlib>
#include <new>

struct SLongListNode { SLongListNode* _M_next; SLongListNode* _M_prev; long _M_data; };
struct SListIterator {
    SLongListNode* _M_node;
    SListIterator(SLongListNode* x) : _M_node(x) {}
    SListIterator(const SListIterator& x) : _M_node(x._M_node) {}
};
struct CLongList {
    SLongListNode* _M_node;
    SListIterator end() { return SListIterator(_M_node); }
    SListIterator insert(SListIterator position, const long& x)
    {
        SLongListNode* tmp = (SLongListNode*)std::malloc(sizeof(SLongListNode));
        new (&tmp->_M_data) long(x);
        SLongListNode* n = position._M_node;
        SLongListNode* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const long& x) { insert(end(), x); }
};
struct CModifierEntry { long DefIndex; bool Active; };
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct CModel {
    CRetailVector< CRetailVector<CModifierEntry> > AppearanceModifiers;
    void GetAppearanceModifierList(CLongList& out, long type) const
    {
        const CRetailVector<CModifierEntry>& modifiers = AppearanceModifiers[type];
        for (const CModifierEntry* it = modifiers.begin(); it != modifiers.end(); ++it)
            out.push_back(it->DefIndex);
    }
};

int main()
{
    CModifierEntry a[3] = { { 11, true }, { 22, false }, { 33, true } };
    CModifierEntry b[1] = { { 99, true } };
    CRetailVector<CModifierEntry> types[3];
    types[0]._M_start = a; types[0]._M_finish = a + 3; types[0]._M_end_of_storage = a + 3;
    types[1]._M_start = 0; types[1]._M_finish = 0; types[1]._M_end_of_storage = 0;
    types[2]._M_start = b; types[2]._M_finish = b + 1; types[2]._M_end_of_storage = b + 1;
    CModel m;
    m.AppearanceModifiers._M_start = types; m.AppearanceModifiers._M_finish = types + 3; m.AppearanceModifiers._M_end_of_storage = types + 3;

    SLongListNode head; head._M_next = &head; head._M_prev = &head; head._M_data = 0;
    CLongList out; out._M_node = &head;
    long seed = 7; out.push_back(seed);                 // pre-existing element stays first
    m.GetAppearanceModifierList(out, 0);
    m.GetAppearanceModifierList(out, 1);                // empty type adds nothing
    m.GetAppearanceModifierList(out, 2);
    const long want[5] = { 7, 11, 22, 33, 99 };
    int n = 0; bool ok = true;
    for (SLongListNode* p = head._M_next; p != &head; p = p->_M_next, ++n)
        if (n >= 5 || p->_M_data != want[n] || p->_M_next->_M_prev != p) ok = false;
    if (n != 5 || head._M_prev->_M_data != 99) ok = false;
    if (!ok) { std::printf("00706800_TEST FAIL\n"); return 1; }
    std::printf("00706800_TEST PASS\n");
    return 0;
}