// std::for_each(std::list<CIntelligentPointer<CThing> >::iterator first, std::list<CIntelligentPointer<CThing> >::iterator last, CSetThingAsDrawable f) -- passes the drawable flag (virtual CThing slot 28) to every live (not dead) thing of a thing owner's object list
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071B700, 101 bytes, `ret 0Ch`. Caller: CTCThingOwner 0x0071B2EB. Callees:
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CThing virtual slot 28 (+0x70).
// SGI/STLport `for_each` over a VC6-era Dinkumware list (node {next, prev, value}); the iterator is a one-pointer class with a user copy ctor, so `__fastcall` passes it on the stack and only the hidden functor pointer is in ecx.
// Shape: a virtual call loads the object (and its vtable) before the push, so the direct form already matches.
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
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetAsDrawable(bool drawable);    // slot 28 (+0x70)
    unsigned char _pad[0x8d];
    unsigned char m_flags91;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool silent);                       // 0x004C9B80
    void SetInLimbo(bool drawable);                // 0x004C8CF0
};
class CBaseIntelligentPointer {                   // 8 bytes: vptr + item
public:
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                     // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
class CSetThingAsDrawable {
public:
    bool m_drawable;                               // +0x00
    CSetThingAsDrawable(bool drawable) : m_drawable(drawable) {}
    void operator()(const CIntelligentPointer<CThing>& thing) const
    {
        if (thing.GetPItem() && !thing.GetPItem()->IsDead())
            thing.GetPItem()->SetAsDrawable(m_drawable);
    }
};
template CSetThingAsDrawable __fastcall std::for_each(CListIterator<CIntelligentPointer<CThing> >, CListIterator<CIntelligentPointer<CThing> >, CSetThingAsDrawable);