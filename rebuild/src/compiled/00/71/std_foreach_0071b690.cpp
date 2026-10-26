// std::for_each(std::list<CIntelligentPointer<CThing> >::iterator first, std::list<CIntelligentPointer<CThing> >::iterator last, CSetThingInLimbo f) -- passes the limbo flag to every live (not dead) thing of a thing owner's object list
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071B690, 101 bytes, `ret 0Ch`. Caller: CTCThingOwner::OnSetInLimbo 0x0071B2BB (landed
// 0x0071B2A0/0x0071B2D0 name this callee). Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times),
// CThing::SetInLimbo(bool) @ 0x004C8CF0.
// SGI/STLport `for_each` over a VC6-era Dinkumware list (node {next, prev, value}); the iterator is a one-pointer class with a user copy ctor, so `__fastcall` passes it on the stack and only the hidden functor pointer is in ecx.
// Shape: named `target` local so GetPItem runs before the flag push (retail order); dead bit = +0x91 bit 0.
template <class T> struct SListNode {             // VC6-era Dinkumware list node
    SListNode* next;                              // +0x00
    SListNode* prev;                              // +0x04
    T value;                                      // +0x08
};
template <class T> class CListIterator {          // list<T>::iterator: one node pointer with a user copy ctor
public:
    SListNode<T>* m_node;
    CListIterator(SListNode<T>* node) : m_node(node) {}
    CListIterator(const CListIterator& o) : m_node(o.m_node) {}
    T& operator*() const { return m_node->value; }
    CListIterator& operator++() { m_node = m_node->next; return *this; }
    bool operator!=(const CListIterator& o) const { return m_node != o.m_node; }
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
class CThing {
public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool silent);                       // 0x004C9B80
    void SetInLimbo(bool inLimbo);                // 0x004C8CF0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
class CSetThingInLimbo {
public:
    bool m_inLimbo;                               // +0x00
    CSetThingInLimbo(bool inLimbo) : m_inLimbo(inLimbo) {}
    void operator()(const CIntelligentPointer<CThing>& thing) const
    {
        if (thing.GetPItem() && !thing.GetPItem()->IsDead()) {
            CThing* target = thing.GetPItem();
            target->SetInLimbo(m_inLimbo);
        }
    }
};
template CSetThingInLimbo __fastcall std::for_each(CListIterator<CIntelligentPointer<CThing> >, CListIterator<CIntelligentPointer<CThing> >, CSetThingInLimbo);