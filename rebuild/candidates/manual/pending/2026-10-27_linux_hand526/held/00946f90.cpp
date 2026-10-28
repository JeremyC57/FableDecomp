// CAIStateGroup_WanderAroundShops::FindASimplyDelightfulShop() -- collect the shop buildings, keep those with fewer shoppers than the per-shop limit and return one picked with the world's random seed (null when none qualifies)
// Leaf: ?FindASimplyDelightfulShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946F90, length 373 bytes, plain `ret`. Ego_r.pdb name (private).
// Callees: CAIStateGroupBase::PeekThingSearchTools 0x008FD000, CThingSearchTools::GetAllLeafThings<const CThingBuilding,
//   vector<const CThingBuilding*>, CThingFilter_HasTC> 0x00947780 (leaf type list 3 via PeekTypeList 0x0049C770+0x10,
//   CThingFilter_HasTC::operator() 0x006A80A0), CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0x53 = CTCShop,
//   bit 19 of +0x28, unchecked: a miss reads the key slot), CTCShop::GetNumShoppers 0x00772110 (ICF-folded getter),
//   vector<const CThingBuilding*>::_M_insert_overflow 0x00436AC0, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0,
//   GFROR13 0x00497890 (GFRandom inline), free 0x00BFEA14 (both vectors' storage).
// Data: g_MaxShoppersPerShop @ 0x0138CE30 (descriptive; unsigned, initialised 3, read only here).
// Shape: the same-TU GetAllLeafThings / _M_insert_overflow bodies (auto_inline off) show VC7.1 that the found vector's
//   address does not escape, so its start and size stay hoisted across the loop as in retail.
// Layout: Ego_r.pdb CTCShop (Shoppers vector of 8-byte intelligent pointers at +0x18); CThing interface bitset +0x20,
//   map +0x44, state bits +0x91. No members of `this` are touched.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type { __true_type() {} };
template <class T> class allocator { };
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T, class A = allocator<T> > class list { public:
    _List_node<T>* _M_node;
};
template <class T, class A = allocator<T> > class vector { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned long n) { return _M_start[n]; }
protected:
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);
public:
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
}

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char StateBits;                            // +0x91 Dead / IncludeInThingScans / Activated / InLimbo
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = ((CInterfaceMap*)map)->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
class CThingBuilding : public CThing { public: enum { LeafType = 3 }; };
class CClass;
class CTCShop { public:                                 // interface 0x53
    long GetNumShoppers() const;                        // 0x00772110
};
class CThingFilter_HasTC { public:
    long Type;                                          // ETCInterfaceType
    CThingFilter_HasTC(long type) : Type(type) {}
    bool operator()(const CThing& thing) const;         // 0x006A80A0
};
class CThingSearchTools { public:
    template <class T, class C, class F> void GetAllLeafThings(const F& filter, C& things) const;
protected:
    const std::list<CClass*>& PeekTypeList(long type) const;   // 0x0049C780
};

// vector<const CThingBuilding*>::_M_insert_overflow @ 0x00436AC0 and GetAllLeafThings<...> @ 0x00947780, compiled in
// this TU but kept out of line as in retail.
#pragma auto_inline(off)
template <class T, class A> __declspec(noinline) void std::vector<T, A>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend)
{
    T* storage = (T*)malloc(fill_len * sizeof(T));
    *storage = x;
    _M_start = storage;
    _M_finish = storage + (atend ? fill_len : 0);
    _M_end_of_storage = position;
}
template <class T, class C, class F> __declspec(noinline) void CThingSearchTools::GetAllLeafThings(const F& filter, C& things) const
{
    const std::list<CClass*>& all = PeekTypeList(T::LeafType);
    for (std::_List_node<CClass*>* it = all._M_node->_M_next; it != all._M_node; it = it->_M_next) {
        T* thing = (T*)it->_M_data;
        if (thing->IsIncludedInThingScans() && filter(*thing))
            things.push_back(thing);
    }
}
#pragma auto_inline(on)

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
extern unsigned long g_MaxShoppersPerShop;              // 0x0138CE30 (descriptive)

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
};
class CAIStateGroup_WanderAroundShops : public CAIStateGroupBase {
    CThing* FindASimplyDelightfulShop();
};

CThing* CAIStateGroup_WanderAroundShops::FindASimplyDelightfulShop()
{
    std::vector<const CThingBuilding*> shops;
    PeekThingSearchTools().GetAllLeafThings<const CThingBuilding>(CThingFilter_HasTC(0x53), shops);
    std::vector<const CThingBuilding*> delightful_shops;
    for (unsigned long i = 0; i < shops.size(); ++i) {
        CTCShop* shop;
        shops[i]->GetInterfaceV(&shop, 0x53);
        if ((unsigned long)shop->GetNumShoppers() < g_MaxShoppersPerShop)
            delightful_shops.push_back(shops[i]);
    }
    if (delightful_shops.empty())
        return 0;
    return (CThing*)delightful_shops[GFRandom(delightful_shops.size(), GetPWorldSeed())];
}
