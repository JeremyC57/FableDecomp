// CThingSearchTools::GetPFirstLeafThing<CThingFilter_HasTC,CThingBuilding>(const CThingFilter_HasTC&, CThingBuilding**) -- first scan-visible building (type list 3) that has the filter's thing component
// Leaf: GetPFirstLeafThing
// Retail: Fable.exe 0x005C0D9D, 72 bytes.
// Callees: CThingSearchTools::PeekTypeList(long) 0x0049C780 (`this` passed through in ecx, returns the
// per-type thing list), CThing::IsIncludedInThingScans 0x0048BC40, CThingFilter_HasTC::operator() 0x006A80A0
// (manifest label "Bitset_Test").
// The generated CThingSearchTools.h models `this`; the list (VC6 Dinkumware: head node at +0, node
// next/prev/value) and the filters are local models. Leaf selector: the template's mangled name.
// Shape: a size-optimised TU (`#pragma optimize("s", on)`: unaligned loop head, the found path shares
// the epilogue with a `jmp`); the same member template as 0x005800D6; the filter is called out of line on the
// reference (`mov ecx,[esp+10h]`); the list rides in edi, the thing in ebx; the loop re-reads end()
// (`cmp esi,[edi]`); the found path stores the out pointer and shares the epilogue (`jmp`).
#pragma optimize("s", on)
#include "engine/CThingSearchTools.h"

struct CThing {
    bool IsIncludedInThingScans();                       // 0x0048BC40
};

struct CThingList {                                      // std::list<CThing*> (VC6)
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) {}
        CThing* operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
};

struct CThingBuilding { enum { ThingListType = 3 }; };

struct CThingFilter_HasTC {
    long m_type;
    bool operator()(CThing* thing) const;                // 0x006A80A0
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekTypeList(long type);                 // 0x0049C780
    template <class TFilter, class TThing> bool GetPFirstLeafThing(const TFilter& filter, TThing** out);
};

template <class TFilter, class TThing>
bool CThingSearchToolsMethods::GetPFirstLeafThing(const TFilter& filter, TThing** out)
{
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter(thing))
        {
            *out = (TThing*)thing;
            return true;
        }
    }
    return false;
}

template bool CThingSearchToolsMethods::GetPFirstLeafThing<CThingFilter_HasTC, CThingBuilding>(const CThingFilter_HasTC&, CThingBuilding**);