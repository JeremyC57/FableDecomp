// CTCInventoryMap::~CTCInventoryMap() -- drop the map component and list pointers, delete the owned UI components unless this is the inventory screen, then destroy the members in reverse and both bases
// Leaf: ??1CTCInventoryMap@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00606F80, 937 bytes, thiscall, tail jump into ~CTCInventoryBase 0x005C00A0 (speed TU:
//   `lea esp,[esp]` loop heads, `add esp,4` after free).
// Identity: FableWin.pdb / Ego_r.pdb ??1CTCInventoryMap@@UAE@XZ: slot 0 of vtable 0x01257A9C (RTTI CTCInventoryMap) is the
//   deleting destructor 0x00619F50, whose body calls this; the vtables stored here (0x01257A9C at +0, 0x01257A84 at +0x130)
//   are the two the landed constructor 0x00606D20 stores, and every member touched is the PDB CTCInventoryMap layout shifted
//   for the 0x130-byte retail NInventory::CTCInventoryBase. The catalog's "CTCActionUseBuyOrSteal_dtor" is propagated.
// Callees: NUISystem::CComponent deleting destructor (slot 0, `push 1`), memmove 0x00BFEAE6 (the inline vector::clear),
//   free 0x00BFEA14, _Rb_tree<long,pair<const long,CRegion>>::_M_erase 0x006196D0, operator delete 0x00BFE9BC (the
//   serialised UID counted pointer's block), CEnginePrimitiveHandle virtual destructor (slot 0, `push 0`), ~CCharString
//   0x0099EAE0 (CUIMiniMapMarker::UIDefName), ~CWideString 0x0099B510, _Rb_tree<ECategory,pair<const ECategory,
//   CComponent*>>::_M_erase 0x00613AE0, NInventory::CTCInventoryBase::~CTCInventoryBase 0x005C00A0 (tail jump; the
//   NHeroInformationScreens::CBase subobject's inline empty destructor re-stores its vtable 0x0123A3E0 first).
// Layout: engine/CTCInventoryMap.h does not exist; the landed constructor's map (Ego_r.pdb CTCInventoryMap 0x2C0 ->
//   retail 0x258: CTCInventoryBase 0x130, 12-byte STLport containers, 4-byte list): VisitedRegions +0x134, CategoriesList
//   +0x144, LogBookNameTag +0x150, LogBookAbbreviatedNameTag +0x15C, LogBookTextTag +0x168, LogBookCategory +0x174,
//   pmap_real +0x188, MiniMapMarkers +0x18C, UIRegionsInfo +0x198, MapScreenPrimitiveHandles +0x1AC,
//   MapScreenQuestPrimitiveHandles +0x1B8, MapScreenTeleportPrimitiveHandles +0x1C4, MapScreenHousePrimitiveHandles +0x1D0,
//   ActiveRegionQuestPositions +0x1DC, ActiveRegionTeleportPositions +0x1E8, ActiveRegionHousePositions +0x1F4,
//   SerialisedTeleporterUIDs +0x200, TeleportRegions +0x208, MapList +0x214, MapRegions +0x218, InventoryScreen +0x240,
//   RegionsCorrespondances +0x24C (header debt: a local model).
// Shape: the two pointer members are nulled before the InventoryScreen test; `delete Components[i]` for i below the
//   recomputed size, then the inline `Components.clear()` (erase(begin, end) = __copy_trivial / memmove); the members'
//   destructors in reverse declaration order (POD vectors just free their block; the handle vectors run the virtual
//   destructor per 8-byte element; the maps erase through the out-of-line _M_erase then reset their header; the list
//   frees its nodes re-reading the sentinel after every free).
extern "C" void __cdecl free(void* block);                              // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
void __cdecl operator delete(void* p);                                  // 0x00BFE9BC

class CThing;
class CList;
class CZoomingComponent;
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                                     // 0x0099EAE0
};
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    ~CWideString();                                                     // 0x0099B510
};
class C2DVector { public: float X, Y; };
class C2DCoordI { public: long X, Y; };
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
#include "engine/CBaseClass.h"                                          // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CEnginePersistentPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass {                      // vtable 0x0122F598, 8 bytes
public:
    CEnginePersistentPrimitiveBase* Data;                               // +0x04 CIVCountedPointer<CEnginePersistentPrimitiveBase>
    virtual ~CEnginePrimitiveHandle();
};
namespace NUISystem {
class CComponent { public:                                              // only its deleting destructor is used (slot 0)
    virtual ~CComponent();
};
}
class CHeroLogBook { public: class CHeroLogBookEntry { public: enum ECategory { }; }; };
#include "engine/CRegion.h"                                            // retail 0x58-byte map value (only its size matters here)

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { };
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> struct _IsPODTypeOf { typedef __false_type _Type; };
template <> struct _IsPODTypeOf<long> { typedef __true_type _Type; };
template <> struct _IsPODTypeOf<C2DCoordI> { typedef __true_type _Type; };
template <> struct _IsPODTypeOf<NUISystem::CComponent*> { typedef __true_type _Type; };
template <> struct _IsPODTypeOf<CHeroLogBook::CHeroLogBookEntry::ECategory> { typedef __true_type _Type; };
inline void* __copy_trivial(const void* __first, const void* __last, void* __result) {
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef typename _IsPODTypeOf<_Tp>::_Type _IsPODType;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    ~vector() { __destroy_aux(this->_M_start, this->_M_finish, _IsPODType()); }
    iterator erase(iterator __first, iterator __last) {
        pointer __i = (pointer)__copy_trivial(__last, this->_M_finish, __first);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_node<_Value>* _Link_type;
public:
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                                      // 0x00613AE0 (ECategory), 0x006196D0 (long,CRegion)
public:
    void clear() {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    ~_Rb_tree() { clear(); }
};
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    _Rb_tree<_Key, pair<const _Key, _Tp>, _Select1st<pair<const _Key, _Tp> >, _Compare, _Alloc> _M_t;
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    void clear() {
        _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
    ~_List_base() { clear(); _M_node.deallocate((_Node*)_M_node._M_data, 1); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : protected _List_base<_Tp, _Alloc> { };
} // namespace std

#include "engine/CCountedPointer.h"                                     // CCPPointerInfo / CCountedPointer<T> (inline release dtor)

class CTCBase { public:                                                 // engine/CTCBase.h offsets
    virtual ~CTCBase();                                                 // 0x00686830
    CThing* ParentThing;                                                // +0x04
    bool    ToBeRemoved;                                                // +0x08
    bool    IsInGlobalUpdateTCs;                                        // +0x09
};
namespace NInventory {
class CTCInventoryBase : public CTCBase { public:                       // vtable 0x01253DBC, retail sizeof 0x130
    std::vector<NUISystem::CComponent*> Components;                     // +0x0c
    unsigned char _rest[0x130 - 0x18];                                  // +0x18 .. (landed 0x005C00A0 model)
    virtual ~CTCInventoryBase();                                        // 0x005C00A0
};
}
namespace NHeroInformationScreens {
enum EScreenType { };
class CBase { public:                                                   // vtable 0x0123A3E0
    virtual ~CBase() { }
};
}

class CTCInventoryMap : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    class CUIRegionInfo { public:                                       // 0x28 bytes (Ego_r.pdb)
        long Region, Shops, BarberShops, TitleShops, TatooShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns;
    };
    class CUIMiniMapMarker { public:                                    // 0x1C bytes (Ego_r.pdb)
        long        Region;                                             // +0x00
        CCharString UIDefName;                                          // +0x04
        C2DVector   Position;                                           // +0x08
        long        DefNameLength;                                      // +0x10
        long        GraphicIndex;                                       // +0x14
        float       Angle;                                              // +0x18
    };
    std::vector<long>                                       VisitedRegions;                    // +0x134
    float                                                   AccumFadeTime;                     // +0x140
    std::map<CHeroLogBook::CHeroLogBookEntry::ECategory, NUISystem::CComponent*> CategoriesList;   // +0x144
    std::vector<CWideString>                                LogBookNameTag;                    // +0x150
    std::vector<CWideString>                                LogBookAbbreviatedNameTag;         // +0x15C
    std::vector<CWideString>                                LogBookTextTag;                    // +0x168
    std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory> LogBookCategory;                   // +0x174
    long                                                    GenerateDataTimer;                 // +0x180
    bool                                                    MiniMapIsPrisonCourtyard1;         // +0x184
    bool                                                    MiniMapIsPrisonCourtyard2;         // +0x185
    bool                                                    MiniMapIsHeadsmansHillNorth;       // +0x186
    bool                                                    MiniMapIsHeadsmansHillSouth;       // +0x187
    CZoomingComponent*                                      pmap_real;                         // +0x188
    std::vector<CUIMiniMapMarker>                           MiniMapMarkers;                    // +0x18C
    std::vector<CUIRegionInfo>                              UIRegionsInfo;                     // +0x198
    long                                                    QuestToBeDropped;                  // +0x1A4
    bool                                                    FirstTimeCheckingCurrentRegion;    // +0x1A8
    std::vector<CEnginePrimitiveHandle>                     MapScreenPrimitiveHandles;         // +0x1AC
    std::vector<CEnginePrimitiveHandle>                     MapScreenQuestPrimitiveHandles;    // +0x1B8
    std::vector<CEnginePrimitiveHandle>                     MapScreenTeleportPrimitiveHandles; // +0x1C4
    std::vector<CEnginePrimitiveHandle>                     MapScreenHousePrimitiveHandles;    // +0x1D0
    std::vector<C2DCoordI>                                  ActiveRegionQuestPositions;        // +0x1DC
    std::vector<C2DCoordI>                                  ActiveRegionTeleportPositions;     // +0x1E8
    std::vector<C2DCoordI>                                  ActiveRegionHousePositions;        // +0x1F4
    CCountedPointer<std::vector<unsigned __int64> >         SerialisedTeleporterUIDs;          // +0x200
    std::vector<long>                                       TeleportRegions;                   // +0x208
    CList*                                                  MapList;                           // +0x214
    std::map<long, CRegion>                                 MapRegions;                        // +0x218
    C2DBoxI                                                 MapBox;                            // +0x224
    bool                                                    WorldMapActive;                    // +0x234
    long                                                    ActiveRegionIndex;                 // +0x238
    long                                                    NumRegions;                        // +0x23C
    bool                                                    InventoryScreen;                   // +0x240
    bool                                                    OpenedViaGuildSeal;                // +0x241
    NHeroInformationScreens::EScreenType                    PreviousScreenType;                // +0x244
    bool                                                    PanMode;                           // +0x248
    std::list<std::pair<const CRegion*, const CRegion*> >   RegionsCorrespondances;            // +0x24C
    long                                                    LastEventFrame;                    // +0x250
    unsigned long                                           RegionEventLife;                   // +0x254
    virtual ~CTCInventoryMap();
};
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap) == 0x258);

CTCInventoryMap::~CTCInventoryMap()
{
    pmap_real = 0;
    MapList = 0;
    if (!InventoryScreen) {
        for (unsigned long i = 0; i < Components.size(); ++i) {
            if (Components[i])
                delete Components[i];
        }
        Components.clear();
    }
}