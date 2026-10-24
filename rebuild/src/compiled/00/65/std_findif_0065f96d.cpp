// std::__find_if(std::list<CCountedPointer<CVillageTaskHorn> >::iterator first, std::list<CCountedPointer<CVillageTaskHorn> >::iterator last, CIsHornTaskWithThisHorn pred, const std::input_iterator_tag&) -- finds the first horn task bound to a given horn thing (out-of-line by-value predicate)
// Leaf: __find_if
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F96D, 62 bytes, `ret 10h`. Caller: the find_if front end 0x0065FFCA. Callees: CCountedPointer
// copy ctor @ 0x0065D89B (argument built in its slot), CIsHornTaskWithThisHorn::operator()(CCountedPointer<CVillageTaskHorn>) @
// 0x0065F116 (manifest), ~CBaseIntelligentPointer @ 0x00A01C10 (the by-value predicate's CIntelligentPointer member, destroyed
// after the result store). Size TU (`#pragma optimize("s", on)`); VC picks the ebp frame itself here (many [ebp+N] parameter
// references are shorter than [esp+N]).
#pragma optimize("s", on)
class CThing;
class CVillageTaskHorn;
namespace std {
struct input_iterator_tag {};
}
class CBaseIntelligentPointer {                   // 8 bytes: vptr + item
public:
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                   // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
template <class T> class CCountedPointer {
public:
    T* m_pObject;
    void* m_pInfo;
    CCountedPointer(const CCountedPointer& other);   // 0x0065D89B (out of line)
    ~CCountedPointer();                           // 0x0065D5A0 (out of line, run by the callee)
};
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
template <class InputIter, class Predicate>
InputIter __fastcall __find_if(InputIter first, InputIter last, Predicate pred, const input_iterator_tag&)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}
}
class CIsHornTaskWithThisHorn {
public:
    CIntelligentPointer<CThing> m_horn;           // +0x00
    bool operator()(CCountedPointer<CVillageTaskHorn> task) const;   // 0x0065F116 (out of line)
};
template CListIterator<CCountedPointer<CVillageTaskHorn> > __fastcall std::__find_if(CListIterator<CCountedPointer<CVillageTaskHorn> >, CListIterator<CCountedPointer<CVillageTaskHorn> >, CIsHornTaskWithThisHorn, const std::input_iterator_tag&);