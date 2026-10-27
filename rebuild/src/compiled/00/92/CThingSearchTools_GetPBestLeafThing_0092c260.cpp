// CThingSearchTools::GetPBestLeafThing(const CThingFilter_In2DRange& filter, const CThingCompare_Nearest& compare, CThingBuilding** out) const -- the nearest scan-visible thing of type list 3 that passes the filter
// Leaf: ??$GetPBestLeafThing@VCThingFilter_In2DRange@@VCThingCompare_Nearest@@VCThingBuilding@@@CThingSearchTools@@QBE_NABVCThingFilter_In2DRange@@ABVCThingCompare_Nearest@@PAPAVCThingBuilding@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092c260, 116 bytes, thiscall, `ret 0Ch` (`this` only passed on to PeekTypeList).
// Identity (proven): Ego_r.pdb ??$GetPBestLeafThing@VCThingFilter_In2DRange@@VCThingCompare_Nearest@@VCThingBuilding@@@CThingSearchTools@@QBE_NABVCThingFilter_In2DRange@@ABVCThingCompare_Nearest@@PAPAVCThingBuilding@@@Z; its filter call 0x006A76B0 is CThingFilter_In2DRange::operator() (Ego_r.pdb takes a const CThing&), its compare 0x0092BE70 CThingCompare_Nearest::operator()<CThingBuilding>, type list 3; caller CAIStateGroup_MinionWander::FaceAwayFromBuildings 0x0092BF50.
// Callees: CThingSearchTools::PeekTypeList(long) const 0x0049C780, CThingFilter_In2DRange::operator() 0x006A76B0,
//   CThingCompare_Nearest::operator()<CThingBuilding>(CThingBuilding*, CThingBuilding*) const 0x0092BE70.
// Body: the landed GetPBestLeafThing 0x00936070 loop -- CThing::IsIncludedInThingScans inline (state byte +0x91: IncludeInThingScans
//   0x04, Activated 0x08, not Dead 0x01, not InLimbo 0x20), the filter, `best = compare(best, thing)`; `*out = best; return best != 0;`.
// Header debt: engine/CThingSearchTools.h is a flat layout without methods, CThing has no header; modelled locally.
class CClass;                                              // Ego_r.pdb: PeekTypeList's list element type
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct _List_node {
    _List_node<_Tp>* _M_next;
    _List_node<_Tp>* _M_prev;
    _Tp _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport 4.6: the header node pointer, 4 bytes
public:
    _List_node<_Tp>* _M_node;
};
}
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char StateBits;                               // +0x91 Dead 0x01 / IncludeInThingScans 0x04 / Activated 0x08 / InLimbo 0x20
    bool IsIncludedInThingScans() const                    // out of line at 0x0048BC40, inline here
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};
class CThingBuilding : public CThing { public: enum { ThingListType = 3 }; };
class CThingFilter_In2DRange {
public:
    bool operator()(const CThing& thing) const;            // 0x006A76B0
};
class CThingCompare_Nearest {
public:
    template <class T> T* operator()(T* best, T* candidate) const;   // 0x0092BE70 for CThingBuilding
};
class CThingSearchTools {
protected:
    const std::list<CClass*>& PeekTypeList(long type) const;   // 0x0049C780
public:
    template <class TFilter, class TCompare, class TThing> bool GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out) const;
};
template <class TFilter, class TCompare, class TThing>
bool CThingSearchTools::GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out) const
{
    TThing* best = 0;
    const std::list<CClass*>& things = PeekTypeList(TThing::ThingListType);
    for (std::_List_node<CClass*>* it = things._M_node->_M_next; it != things._M_node; it = it->_M_next)
    {
        CThing* thing = (CThing*)it->_M_data;
        if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
            best = compare(best, (TThing*)thing);
    }
    *out = best;
    return best != 0;
}
template bool CThingSearchTools::GetPBestLeafThing<CThingFilter_In2DRange, CThingCompare_Nearest, CThingBuilding>(const CThingFilter_In2DRange&, const CThingCompare_Nearest&, CThingBuilding**) const;