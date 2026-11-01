// CAIStateGroup_WanderAroundShops::FindASimplyDelightfulShop() -- pick, with the world seed, a random shop building whose shop component has fewer than the maximum number of shoppers (null when none)
// Leaf: ?FindASimplyDelightfulShop@CAIStateGroup_WanderAroundShops@@
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type { __true_type() { } };
template <class T> class allocator {
public:
    allocator() { }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
template <class T, class A> class _Vector_base {
public:
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef A allocator_type;
    typedef unsigned int size_type;
    typedef T* iterator;
    explicit vector(const allocator_type& a = allocator_type()) : _Vector_base<T, A>(a) { }
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
    T& operator[](size_type n) { return *(begin() + n); }
protected:
    void _M_insert_overflow(T* position, const T& x, const __true_type&, size_type fill_len, bool atend);   // 0x00436AC0
public:
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) T(x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __true_type(), 1UL, true);
    }
};
template <class T1, class T2> struct pair { T1 first; T2 second; };
} // namespace std

class CTCBase;
enum ETCInterfaceType { TC_SHOP_0x53 = 0x53 };
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > {
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 bitset of interface types
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
};
class CThingBuilding;
class CTCShop {
public:
    long GetNumShoppers() const;                            // 0x00772110
};
class CThingFilter_HasTC {
public:
    ETCInterfaceType Type;
    CThingFilter_HasTC(ETCInterfaceType type) : Type(type) { }
};
class CThingSearchTools {
public:
    template <class T, class TContainer, class TFilter>
    void GetAllLeafThings(const TFilter& filter, TContainer& things) const;   // 0x00947780 <CThingBuilding, vector<CThing*>, CThingFilter_HasTC>
};
long __fastcall GFROR13(unsigned long value);               // 0x00497890 ?GFROR13@@YIJK@Z
inline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
extern unsigned long g_MaxShoppersPerShop_0138ce30;         // 0x0138CE30 (value 3)

class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
protected:
    unsigned long* GetPWorldSeed() const;                   // 0x008FCFD0
    const CThingSearchTools& PeekThingSearchTools() const;  // 0x008FD000
};
class CAIStateGroup_WanderAroundShops : public CAIStateGroupBase {
private:
    CThing* FindASimplyDelightfulShop();
};

CThing* CAIStateGroup_WanderAroundShops::FindASimplyDelightfulShop()
{
    std::vector<CThing*> shops;
    PeekThingSearchTools().GetAllLeafThings<CThingBuilding>(CThingFilter_HasTC(TC_SHOP_0x53), shops);
    std::vector<CThing*> available;
    for (unsigned long i = 0; i < shops.size(); ++i) {
        CThing* shop = shops[i];
        CTCShop* tc;
        if (shop->HasTC(TC_SHOP_0x53))
            tc = shop->GetTC<CTCShop>(TC_SHOP_0x53);
        if ((unsigned long)tc->GetNumShoppers() < g_MaxShoppersPerShop_0138ce30)
            available.push_back(shop);
    }
    if (available.empty())
        return 0;
    return available[GFRandom(available.size(), GetPWorldSeed())];
}
