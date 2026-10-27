// CThingSearchTools::GetPFirstThing<CThingFilter_HasScriptName, CThing>(const CThingFilter_HasScriptName& filter, CThing** out) const -- the first scan-visible thing passing the filter
// Leaf: ??$GetPFirstThing@VCThingFilter_HasScriptName@@VCThing@@@CThingSearchTools@@QBE_NABVCThingFilter_HasScriptName@@PAPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056D050, 99 bytes, thiscall, `ret 8`.
// Identity (proven): the filter callee 0x006A7E30 is bound ??RCThingFilter_HasScriptName@@QBE_NABVCThing@@@Z; the CThing type mask (0, `xor al,al`); Ego_r.pdb ??$GetPFirstThing@VCThingFilter_HasScriptName@@VCThing@@@CThingSearchTools@@QBE_N... (the only HasScriptName/CThing instantiation); callers CTCBoastUI::GlobalUpdate, CTCInventoryQuests::TakeQuest, CTCOracleMinigame::PlayGame.
// Callees: CThingSearchTools::PeekAllThings() 0x0049C770, CThingFilter_HasScriptName::operator()(const CThing&) const 0x006A7E30.
// Shape (family M136, representative 0x0056D050): the landed CThingSearchTools GetPFirstThing loop (0x00512ED0 model) -- the
//   CThing type mask read through a local (`xor al,al ; jne`), IsIncludedInThingScans inline, the filter out of line; end()
//   re-read every pass.
// Header debt: engine/CThingSearchTools.h declares no member functions; the class is modelled locally to carry the member
//   template's real name; CThing as the landed retail-layout model.
class CCharString { public: void* PStringData; };
enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
class CThing { public:
    enum { ThingTypeMask = 0 };                             // every thing matches the CThing type mask
    unsigned char _pad_0x00[0x6c];
    unsigned long ThingTypeFlags;                           // +0x6c
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                                // +0x91 Dead / IncludeInThingScans / Activated / InLimbo
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
struct CThingList {
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
class CThingFilter_HasScriptName {
public:
    bool operator()(const CThing& thing) const;             // 0x006A7E30
    CCharString Name;
};
class CWorldMap;
class CThingManager;
struct CThingMapManager;
namespace NThingMap { class CThingMap; }
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public: _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage; };
}
class CThingSearchTools {                                   // Ego_r.pdb members (retail 0x18 bytes)
public:
    CThingList* PeekAllThings() const;                      // 0x0049C770
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out) const;
    CWorldMap* WorldMap;
    CThingMapManager* PThingMapManager;
    CThingManager* ThingManager;
    std::vector<NThingMap::CThingMap*> AreaMaps;
};
template <class TFilter, class TThing>
bool CThingSearchTools::GetPFirstThing(const TFilter& filter, TThing** out) const
{
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (IsThingOfType<CThing>(thing) && thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
        {
            *out = (TThing*)thing;
            return true;
        }
    }
    return false;
}
template bool CThingSearchTools::GetPFirstThing<CThingFilter_HasScriptName, CThing>(const CThingFilter_HasScriptName&, CThing**) const;