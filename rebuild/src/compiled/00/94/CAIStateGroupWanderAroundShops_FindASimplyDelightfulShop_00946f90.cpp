// CAIStateGroup_WanderAroundShops::FindASimplyDelightfulShop() -- pick, with the world seed, a random shop building whose shop component has fewer than the maximum number of shoppers (null when none)
// Leaf: ?FindASimplyDelightfulShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00946F90, 373 bytes, thiscall, plain `ret`, returns CThing*.
// Identity: Ego_r.pdb ?FindASimplyDelightfulShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@XZ (private, no argument):
//   Ego_r's member order Cleanup, InitStateGroup, OnReplace, FindASimplyDelightfulShop, FindPrettyThingInShop, IsRunnable,
//   IsStillValid matches the retail block 0x00946DD0..0x00947280 (all neighbours landed under their PDB names), and the
//   landed IsRunnable 0x00947280 calls this row as "the shop picker" (declared there as SelectRandomShop()). Catalog
//   "_global::CAIStateGroup_WanderAroundShops_SelectRandomEnemy" is wrong.
// Callees: CAIStateGroupBase::PeekThingSearchTools 0x008FD000 and GetPWorldSeed 0x008FCFD0 (Ego_r IBE spellings);
//   CThingSearchTools::GetAllLeafThings<CThingBuilding, vector<CThing*>, CThingFilter_HasTC> 0x00947780 (Ego_r instantiation;
//   its body walks PeekTypeList(3) 0x0049C780, the building list, and tests CThingFilter_HasTC::operator() 0x006A80A0, so
//   the filter holds interface 0x53); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline HasTC +
//   GetTC of interface 0x53, bit 19 of +0x28); CTCShop::GetNumShoppers 0x00772110 (Ego_r QBEJXZ: Shoppers, a vector of
//   8-byte CIntelligentPointer at +0x18, `sar 3`; landed as CShaderPreParser::GetConstantCount); vector<CThing*>::
//   _M_insert_overflow 0x00436AC0; GFROR13 0x00497890 (GFRandom inline); free 0x00BFEA14.
// Global: 0x0138CE30 (an unsigned long holding 3, read only here; compared unsigned with the long shopper count): name
//   not in either PDB dump -- descriptive.
// Shape: retail walks `shops` with a pointer and a count-down counter and never re-reads its begin/size across the
//   GetNumShoppers / push_back calls, i.e. VC7.1 knew the local vector does not escape. That needs the bodies VC7.1 had in
//   this TU: the GetAllLeafThings template (retail emits it in this TU, 0x00947780) and STLport 4.6 _M_insert_overflow
//   (the landed 0x00436AC0 text); with them declared only, `shops` escapes (392v373, p1). The component is read through
//   `CTCShop* tc; if (HasTC) tc = GetTC;` (the absent path reads the dead key slot, GOTCHAS chunk 141), and push_back gets
//   the element itself (`push ebx`, a reference into `shops`).
// Layout: CThing retail offsets as the landed GetPBestLeafThing 0x00936070 (bitset +0x20, interface map +0x44, state bits
//   +0x91); engine/CThingSearchTools.h is a flattened method-less layout, so the class is restated with its members.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type { __true_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}
template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int __n) { return __n != 0 ? (T*)malloc(__n * sizeof(T)) : 0; }
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
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    explicit vector(const allocator_type& a = allocator_type()) : _Vector_base<_Tp, _Alloc>(a) { }
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin() + n); }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false)       // 0x00436AC0 for vector<CThing*>
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};
template <class T1, class T2> struct pair { T1 first; T2 second; };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};
template <class T> struct _List_const_iterator {          // STLport _List_iterator<T, _Const_traits<T> >
    _List_node_base* _M_node;
    _List_const_iterator(_List_node_base* x) : _M_node(x) { }
    const T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_const_iterator& y) const { return _M_node != y._M_node; }
};
template <class T, class A = allocator<T> > class list {
public:
    typedef _List_const_iterator<T> const_iterator;
    _List_node_base* _M_node;                               // STLport: the header node pointer (4 bytes)
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
} // namespace std

class CTCBase;
enum ETCInterfaceType { };                                  // values not in the PDB dumps; 0x53 = the CTCShop interface
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

class CClass;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 bitset of interface types
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char StateBits;                                // +0x91 Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20
    bool IsIncludedInThingScans() const                     // out of line at 0x0048BC40, inlined here
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
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
class CThingBuilding : public CThing { public: enum { ThingListType = 3 }; };
class CTCShop {
public:
    long GetNumShoppers() const;                            // 0x00772110
};
class CThingFilter_HasTC {
public:
    ETCInterfaceType Type;
    CThingFilter_HasTC(ETCInterfaceType type) : Type(type) { }
    bool operator()(const CThing& thing) const;             // 0x006A80A0
};
class CWorldMap;
struct CThingMapManager;
class CThingManager;
class CThingSearchTools {
public:
    CWorldMap*        WorldMap;                             // +0x00
    CThingMapManager* PThingMapManager;                     // +0x04
    CThingManager*    ThingManager;                         // +0x08
    unsigned char     AreaMaps[0x0C];                       // +0x0C vector<NThingMap::CThingMap_*>
    template <class T, class TContainer, class TFilter>
    void GetAllLeafThings(const TFilter& filter, TContainer& things) const;
protected:
    const std::list<CClass*>& PeekTypeList(long type) const;   // 0x0049C780
};
template <class T, class TContainer, class TFilter>
void CThingSearchTools::GetAllLeafThings(const TFilter& filter, TContainer& things) const   // 0x00947780 here
{
    const std::list<CClass*>& list = PeekTypeList(T::ThingListType);
    for (std::list<CClass*>::const_iterator it = list.begin(); it != list.end(); ++it) {
        T* thing = (T*)*it;
        if (thing->IsIncludedInThingScans() && filter(*thing))
            things.push_back(thing);
    }
}
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
    PeekThingSearchTools().GetAllLeafThings<CThingBuilding>(CThingFilter_HasTC((ETCInterfaceType)0x53), shops);
    std::vector<CThing*> available;
    for (unsigned long i = 0; i < shops.size(); ++i) {
        CTCShop* tc;
        if (shops[i]->HasTC((ETCInterfaceType)0x53))
            tc = shops[i]->GetTC<CTCShop>((ETCInterfaceType)0x53);
        if ((unsigned long)tc->GetNumShoppers() < g_MaxShoppersPerShop_0138ce30)
            available.push_back(shops[i]);
    }
    if (available.empty())
        return 0;
    return available[GFRandom(available.size(), GetPWorldSeed())];
}