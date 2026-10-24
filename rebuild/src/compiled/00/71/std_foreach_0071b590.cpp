// std::for_each(std::list<CIntelligentPointer<CThing> >::iterator first, std::list<CIntelligentPointer<CThing> >::iterator last, CKillThing f) -- kills every live thing of a thing owner's object list
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071B590, 85 bytes, `ret 0Ch`. Caller: CTCThingOwner::OnKill 0x0071B227. Callees:
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CThing::Kill(bool) @ 0x004C9B80.
// SGI/STLport `for_each` over a VC6-era Dinkumware list (node {next, prev, value}); the iterator is a one-pointer class with a user copy ctor, so `__fastcall` passes it on the stack and only the hidden functor pointer is in ecx.
// Shape: the 1-byte functor (user ctor) comes back through the hidden pointer (`mov [ebp],bl`); a named `target` local
// (`CThing* target = thing.GetPItem(); target->Kill(flag)`) evaluates the object before pushing the flag -- the direct
// `thing.GetPItem()->Kill(flag)` pushes first and also changes the register allocation.
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
class CBaseIntelligentPointer {                   // 8 bytes: vptr + item
public:
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                     // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
class CKillThing {
public:
    bool m_silent;                                // +0x00
    CKillThing(bool silent) : m_silent(silent) {}
    void operator()(const CIntelligentPointer<CThing>& thing) const
    {
        if (thing.GetPItem()) {
            CThing* target = thing.GetPItem();
            target->Kill(m_silent);
        }
    }
};
template CKillThing __fastcall std::for_each(CListIterator<CIntelligentPointer<CThing> >, CListIterator<CIntelligentPointer<CThing> >, CKillThing);