// CThingSearchTools::GetPFirstLeafThing<CFilter_Bind2<CThingFilter_HasDefName,CThingFilter_HasScriptData>,CThingMarker>(const CFilter_Bind2<..>&, CThingMarker**) -- first scan-visible marker (type list 0x0b) passing both bound filters
// Leaf: GetPFirstLeafThing
// Retail: Fable.exe 0x005800D6, 91 bytes.
// Callees: CThingSearchTools::PeekTypeList(long) 0x0049C780 (`this` passed through in ecx, returns the
// per-type thing list), CThing::IsIncludedInThingScans 0x0048BC40, CThingFilter_HasDefName::operator()
// 0x006A7BE0, CThingFilter_HasScriptData::operator() 0x006A7E70.
// The generated CThingSearchTools.h models `this`; the list (VC6 Dinkumware: head node at +0, node
// next/prev/value) and the filters are local models. Leaf selector: the template's mangled name.
// Shape: a size-optimised TU (`#pragma optimize("s", on)`: unaligned loop head, the found path shares
// the epilogue with a `jmp`); the member template; the bound filter holds its two filters by reference (`[filter+0]`,
// `[filter+4]` re-read per test) and is `__forceinline` `a(thing) && b(thing)` (under /Os
// a plain inline operator() stays out of line); the loop re-reads end()
// (`cmp esi,[ebx]`); the found path stores the out pointer and shares the epilogue (`jmp`).
#pragma optimize("s", on)
#include "engine/CThingSearchTools.h"

class CThing { public:
    bool IsIncludedInThingScans();                       // 0x0048BC40
};
class CThingMarker { public: enum { ThingListType = 0x0b }; };

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

class CThingFilter_HasDefName { public:
    bool operator()(CThing* thing) const;                // 0x006A7BE0
};
class CThingFilter_HasScriptData { public:
    bool operator()(CThing* thing) const;                // 0x006A7E70
};

template <class A, class B> class CFilter_Bind2 { public:
    const A& m_first;
    const B& m_second;
    __forceinline bool operator()(CThing* thing) const { return m_first(thing) && m_second(thing); }
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

template bool CThingSearchToolsMethods::GetPFirstLeafThing<CFilter_Bind2<CThingFilter_HasDefName, CThingFilter_HasScriptData>, CThingMarker>(
    const CFilter_Bind2<CThingFilter_HasDefName, CThingFilter_HasScriptData>&, CThingMarker**);