// std::find_if(std::list<CMarkerInfo>::iterator first, std::list<CMarkerInfo>::iterator last, CIsMarkerBoundToThing pred) -- finds the first non-automatic minimap marker bound to a thing
// Leaf: find_if
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082C720, 71 bytes, `ret 0Ch`. Callers: CMiniMapDisplay::SetMarkerGraphic 0x0082A369, the
// markers' remove_if 0x0082C880. Callee: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the marker's PThing).
// CMarkerInfo comes from the engine header (PThing is opaque `unsigned char[8]` there, read through the intelligent-pointer view).
// SGI find_if, `__fastcall` with the hidden iterator result in ecx.
// Shape: the marker address is kept in edi (`lea edi,[esi+8]`), `last` is re-read from its slot (no free register).
#include "engine/CMarkerInfo.h"
class CThing;
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
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
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
InputIter __fastcall find_if(InputIter first, InputIter last, Predicate pred)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}
}
class CIsMarkerBoundToThing {
public:
    const CThing* m_pThing;                       // +0x00
    CIsMarkerBoundToThing(const CThing* thing) : m_pThing(thing) {}
    bool operator()(const CMarkerInfo& marker) const
    {
        return ((const CIntelligentPointer<CThing>*)marker.PThing)->GetPItem() == m_pThing && !marker.AutoMarker;
    }
};
template CListIterator<CMarkerInfo> __fastcall std::find_if(CListIterator<CMarkerInfo>, CListIterator<CMarkerInfo>, CIsMarkerBoundToThing);