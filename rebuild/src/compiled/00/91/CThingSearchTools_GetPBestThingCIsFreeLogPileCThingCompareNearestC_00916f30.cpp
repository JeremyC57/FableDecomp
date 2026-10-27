// CThingSearchTools::GetPBestThing<CIsFreeLogPile, CThingCompare_Nearest, CThingObject>(const CIsFreeLogPile& filter, const CThingCompare_Nearest& compare, CThingObject** out) const -- the nearest scan-visible CThingObject on the class list that passes the filter
// Leaf: ??$GetPBestThing@VCIsFreeLogPile@@VCThingCompare_Nearest@@VCThingObject@@@CThingSearchTools@@QBE_NABVCIsFreeLogPile@@ABVCThingCompare_Nearest@@PAPAVCThingObject@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00916F30, 137 bytes, thiscall, `ret 0Ch`.
// Identity (probable): only caller 0x00917050, the first of CAIStateGroup_ThrowLogsOnFire's two nearest-object getters; Ego_r.pdb orders them GetPNearestFreeLogPile, GetPNearestBonfire and holds GetPBestThing<CIsFreeLogPile / CIsFreeBonfire, CThingCompare_Nearest, CThingObject>; the filter 0x00916E30 tests object-property bit 22 (the bonfire twin tests bit 14) -- which filter is which follows the PDB order, not a name in the image.
// Callees: CThingSearchTools::PeekClassList() const 0x0049C770 (Ego_r.pdb ?PeekClassList@CThingSearchTools@@IBEABV?$list@PAVCClass@@...),
//   CIsFreeLogPile::operator() 0x00916E30, CThingCompare_Nearest::operator()<CThingObject> 0x00916B10.
// Shape (family K044, representative 0x00914330): the landed GetPBestThing<CIsNearCampfireWithSpace, CThingCompare_Nearest,
//   CThingObject> 0x0093B640 walk: CThing::IsIncludedInThingScans inline (bits 0x04, 0x08, not 0x01, not 0x20 of +0x91),
//   the THING_TYPE mask test (0x807), a fresh `*it` for the null test, then the filter and the comparator; best through out.
class CClass { };
class CThing : public CClass {                           // retail layout (Ego_r.pdb offsets - 0x10 from ThingTypeFlags on)
public:
    enum { ThingTypeMask = 0 };
    unsigned char _pad_0x00[0x6c];
    unsigned long ThingTypeFlags;                        // +0x6C THING_TYPE_* inheritance bits
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                             // +0x91 Dead 0x01 / IncludeInThingScans 0x04 / Activated 0x08 / InLimbo 0x20
    bool IsIncludedInThingScans() const                  // Ego_r.pdb ?IsIncludedInThingScans@CThing@@QBE_NXZ (inline here)
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};
template <class T> inline bool IsThingOfType(const CThing* thing)   // the mask goes through a local (VC7.1 folds it late)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { public: enum { ThingTypeMask = 0x7 }; };
class CThingObject : public CThingGameObject { public: enum { ThingTypeMask = 0x807 }; };
class CThingMarker : public CThingGameObject { public: enum { ThingTypeMask = 0x8007 }; };
class CIsFreeLogPile {
public:
    bool operator()(const CThingObject& thing) const;   // 0x00916E30
};
class CThingCompare_Nearest {
public:
    float X, Y, Z;
    template <class T> T* operator()(T* best, T* candidate) const;   // <CThingObject> 0x00916B10
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport list, 4 bytes
public:
    struct const_iterator {
        _List_node_base* _M_node;
        const_iterator(_List_node_base* __x) : _M_node(__x) { }
        const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
        const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const const_iterator& __y) const { return _M_node != __y._M_node; }
    };
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
};
}
class CThingSearchTools {                                // Ego_r.pdb 28 bytes (only `this` is passed through here)
public:
    template <class TFilter, class TCompare, class TThing>
    bool GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out) const;
protected:
    const std::list<CClass*>& PeekClassList() const;     // 0x0049C770
};
template <class TFilter, class TCompare, class TThing>
bool CThingSearchTools::GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out) const
{
    TThing* best = 0;
    const std::list<CClass*>& things = PeekClassList();
    for (std::list<CClass*>::const_iterator it = things.begin(); it != things.end(); ++it) {
        if (((CThing*)*it)->IsIncludedInThingScans() && IsThingOfType<TThing>((CThing*)*it)) {
            TThing* candidate = (TThing*)*it;
            if (candidate && filter(*candidate))
                best = compare(best, candidate);
        }
    }
    *out = best;
    return best != 0;
}
template bool CThingSearchTools::GetPBestThing<CIsFreeLogPile, CThingCompare_Nearest, CThingObject >(const CIsFreeLogPile&, const CThingCompare_Nearest&, CThingObject**) const;