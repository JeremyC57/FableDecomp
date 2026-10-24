#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CThing {
public:
    unsigned char m_flags91;                      // bit 0: dead
    int m_kills;
    bool m_lastSilent;
    bool m_inLimbo;
    int m_limboCalls;
    bool m_drawable;
    int m_drawableCalls;
    CThing() : m_flags91(0), m_kills(0), m_lastSilent(false), m_inLimbo(false), m_limboCalls(0), m_drawable(false), m_drawableCalls(0) {}
    virtual ~CThing() {}
    virtual void SetAsDrawable(bool drawable) { m_drawable = drawable; ++m_drawableCalls; }
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool silent) { ++m_kills; m_lastSilent = silent; m_flags91 |= 1; }
    void SetInLimbo(bool inLimbo) { m_inLimbo = inLimbo; ++m_limboCalls; }
};
class CBaseIntelligentPointer {
public:
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(CThing* thing) { m_pItem = thing; }
    CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { m_pItem = o.m_pItem; }
};
template <class T> struct SListNode {             // VC6-era Dinkumware list node
    SListNode* next;
    SListNode* prev;
    T value;
};
template <class T> class CListIterator {
public:
    SListNode<T>* m_node;
    CListIterator(SListNode<T>* node) : m_node(node) {}
    CListIterator(const CListIterator& o) : m_node(o.m_node) {}
    T& operator*() const { return m_node->value; }
    CListIterator& operator++() { m_node = m_node->next; return *this; }
    bool operator!=(const CListIterator& o) const { return m_node != o.m_node; }
};
// test helper: a circular list with a sentinel head
template <class T> struct STestList {
    SListNode<T> head;
    SListNode<T> nodes[8];
    int n;
    STestList() : n(0) { head.next = head.prev = &head; }
    void Add(const T& v)
    {
        SListNode<T>* node = &nodes[n++];
        node->value = v;
        node->prev = head.prev; node->next = &head;
        head.prev->next = node; head.prev = node;
    }
    CListIterator<T> begin() { return CListIterator<T>(head.next); }
    CListIterator<T> end() { return CListIterator<T>(&head); }
};
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CSetThingAsDrawable {
public:
    bool m_drawable;
    CSetThingAsDrawable(bool drawable) : m_drawable(drawable) {}
    void operator()(const CIntelligentPointer<CThing>& thing) const
    {
        if (thing.GetPItem() && !thing.GetPItem()->IsDead())
            thing.GetPItem()->SetAsDrawable(m_drawable);
    }
};
int main()
{
    CThing t[3]; t[1].m_flags91 = 1;               // t[1] is already dead
    STestList<CIntelligentPointer<CThing> > l;
    l.Add(CIntelligentPointer<CThing>(&t[0])); l.Add(CIntelligentPointer<CThing>(0));
    l.Add(CIntelligentPointer<CThing>(&t[1])); l.Add(CIntelligentPointer<CThing>(&t[2]));
    std::for_each(l.begin(), l.end(), CSetThingAsDrawable(true));
    if (!t[0].m_drawable || t[1].m_drawable || !t[2].m_drawable || t[1].m_drawableCalls != 0 || t[2].m_drawableCalls != 1) { std::printf("0071b700_TEST FAIL\n"); return 1; }
    std::printf("0071b700_TEST PASS\n");
    return 0;
}