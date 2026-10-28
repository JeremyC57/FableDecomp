// CWorldMap::PostRegionLoad(long region) -- for every village thing in the world: populate it with villagers the first time (else create any missing dummy villagers), fill its crate stacks and add its crate tasks, bracketed by loading-progress ticks; then re-enable sound themes
// Leaf: ?PostRegionLoad@CWorldMap@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005064C0, 542 bytes, `ret 4` (virtual; the region argument is unused).
//   Ego_r.pdb / FableWin.pdb ?PostRegionLoad@CWorldMap@@UAEXJ@Z.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list header and vector storage),
//   CThingSearchTools::GetAllThings<CThing, list<CThing*>, CThingFilter_HasTC> 0x004ACD70 (same-TU body; World->PThingSearchTools,
//   +0x20) -> CThingSearchTools::PeekClassList 0x0049C770, CThingFilter_HasTC::operator() 0x006A80A0,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, __fastcall NProgressDisplay::DisplayProgress 0x009E9F40,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline CThing::GetTC<CTCVillage>, TC 0x22),
//   CWorldMap::PopulateRegionWithVillagers(CThing&) 0x00502E90, CTCVillage::CreateDummyVillagersIfNecessary 0x00658603,
//   static __fastcall CTCCrateStack::FillStacks(const CTCVillage&, long) 0x007B6B10,
//   vector<long>::_M_insert_overflow 0x00440990 (same-TU body), CTCVillage::AddCrateTasksFromAndTo 0x00659C7E.
// Layout: retail CThing interface bitset +0x20 (TC 0x22 = word +0x24 bit 2), interface map +0x44, ThingTypeFlags +0x6C,
//   scan bits +0x91; CTCVillage PopulatedOnce byte +0x7A; CWorldMap World +0x08, SoundThemesDisabled +0x4C.
// Shape: the same-TU noinline bodies of GetAllThings and _M_insert_overflow (real retail callees, STLport 4.6 / the CThing
//   search template) let VC7.1 see the list and the vectors do not escape, so it keeps their pointers in registers and
//   hoists the push_back constants as retail does; the STLport _List_base ctor builds the header in a local first.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memmove)
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b); }   // 0x009E9F40

namespace std {
struct __true_type { __true_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc&) : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend = false);
};

template <class _List_Tp> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    _List_Tp    _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _List_base(const _Alloc&) : _M_node(0)
    {
        _Node* __n = (_Node*)malloc(sizeof(_Node));
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node = __n;
    }
    ~_List_base()
    {
        clear();
        free(_M_node);
    }
    void clear()
    {
        _Node* __cur = _M_node->_M_next;
        while (__cur != _M_node) {
            _Node* __tmp = __cur;
            __cur = __cur->_M_next;
            free(__tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    _Node* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) { }
    void push_back(const _Tp& __x)
    {
        _Node* __tmp = (_Node*)malloc(sizeof(_Node));
        new (&__tmp->_M_data) _Tp(__x);
        _Node* __pos = this->_M_node;
        __tmp->_M_next = __pos;
        __tmp->_M_prev = __pos->_M_prev;
        __pos->_M_prev->_M_next = __tmp;
        __pos->_M_prev = __tmp;
    }
};

// Same-TU bodies of the STLport 4.6 helpers retail calls out of line (0x00440990), so VC7.1 sees the value argument
// is only read.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&,
                                                                  unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = (_Tp*)((char*)memmove(__new_start, this->_M_start, (char*)__position - (char*)this->_M_start)
                               + ((char*)__position - (char*)this->_M_start));
    for (unsigned int __n = __fill_len; __n > 0; --__n, ++__new_finish)
        *__new_finish = __x;
    if (!__atend)
        __new_finish = (_Tp*)((char*)memmove(__new_finish, __position, (char*)this->_M_finish - (char*)__position)
                              + ((char*)this->_M_finish - (char*)__position));
    if (this->_M_start)
        free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)
}

enum ETCInterfaceType { TC_VILLAGE = 0x22 };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    value_type* _M_start;
    value_type* _M_finish;
    value_type* _M_end_of_storage;
    iterator LowerBound(const K& key);                 // 0x0040F020
    iterator end() { return _M_finish; }
};

class CClass {
public:
    virtual ~CClass();
};
class CThing : public CClass {
public:
    unsigned char _pad_0x04[0x20 - 0x04];
    unsigned long UsingComponents[9];                  // +0x20 bitset<274>
    CVectorMap<ETCInterfaceType, CTCBase*> Components; // +0x44
    unsigned char _pad_0x54[0x6c - 0x54];
    unsigned long ThingTypeFlags;                      // +0x6C (Ego_r.pdb +0x7C)
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char Dead : 1;                            // +0x91 (Ego_r.pdb +0xA1)
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    bool IsIncludedInThingScans() const               // 0x0048BC40 out of line
    {
        return IncludeInThingScans && Activated && !Dead && !InLimbo;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!(UsingComponents[T::InterfaceType / 32] & (1UL << (T::InterfaceType % 32))))
            return false;
        CVectorMap<ETCInterfaceType, CTCBase*>& components = Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::iterator p = components.LowerBound((ETCInterfaceType)T::InterfaceType);
        if (p == components.end() || (ETCInterfaceType)T::InterfaceType < p->first)
            p = components.end();
        *out = (T*)p->second;
        return true;
    }
};
class CTCVillage {
public:
    enum { InterfaceType = TC_VILLAGE };
    unsigned char _pad00[0x7a];
    bool          PopulatedOnce;                       // +0x7A
    void CreateDummyVillagersIfNecessary();            // 0x00658603
    void AddCrateTasksFromAndTo(const std::vector<long>& from, const std::vector<long>& to);   // 0x00659C7E
};
class CTCCrateStack {
public:
    static void __fastcall FillStacks(const CTCVillage& village, long count);   // 0x007B6B10
};

class CThingFilter_HasTC {
public:
    ETCInterfaceType Type;
    CThingFilter_HasTC(ETCInterfaceType type) : Type(type) { }
    bool operator()(const CThing& thing) const;        // 0x006A80A0
};
class CThingSearchTools {
public:
    template <class T, class C, class F> void GetAllThings(const F& filter, C& things) const;
protected:
    const std::list<CClass*>& PeekClassList() const;   // 0x0049C770
};
// Same-TU body of 0x004ACD70 (the CWorldMap TU instantiates the template; retail calls it out of line): every
// scan-visible thing of the thing manager's class list that passes the filter (the CThing type-mask test folds away).
#pragma auto_inline(off)
template <class T, class C, class F> __declspec(noinline) void CThingSearchTools::GetAllThings(const F& filter, C& things) const
{
    const std::list<CClass*>& all = PeekClassList();
    for (std::_List_node<CClass*>* it = all._M_node->_M_next; it != all._M_node; it = it->_M_next) {
        T* thing = (T*)it->_M_data;
        if (thing->IsIncludedInThingScans() && filter(*thing))
            things.push_back(thing);
    }
}
#pragma auto_inline(on)

class CWorld {
public:
    unsigned char _pad[0x20];
    CThingSearchTools* PThingSearchTools;              // +0x20 scoped_ptr<CThingSearchTools>
    const CThingSearchTools& PeekThingSearchTools() const { return *PThingSearchTools; }
};

class CWorldMap {
public:
    virtual ~CWorldMap();
    virtual void PostRegionLoad(long region);
    long        f4;                                    // +0x04
    CWorld*     World;                                 // +0x08
    unsigned char _pad0c[0x4c - 0x0c];
    bool        SoundThemesDisabled;                   // +0x4C
    void PopulateRegionWithVillagers(CThing& village); // 0x00502E90
};

void CWorldMap::PostRegionLoad(long region)
{
    std::list<CThing*> villages;
    World->PeekThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(TC_VILLAGE), villages);
    for (std::_List_node<CThing*>* it = villages._M_node->_M_next; it != villages._M_node; it = it->_M_next)
    {
        NProgressDisplay::DisplayProgress("World Map: Post Region Load Villages", -1.0f, false, false);
        CThing* thing = it->_M_data;
        CTCVillage* village = 0;
        thing->GetTC(&village);
        if (!village->PopulatedOnce)
        {
            PopulateRegionWithVillagers(*thing);
            village->PopulatedOnce = true;
        }
        else
            village->CreateDummyVillagersIfNecessary();
        CTCCrateStack::FillStacks(*village, 2);
        std::vector<long> from;
        from.push_back(2);
        std::vector<long> to;
        to.push_back(0);
        to.push_back(3);
        village->AddCrateTasksFromAndTo(from, to);
        NProgressDisplay::DisplayProgress("World Map: Post Region Load Villages End", -1.0f, false, false);
    }
    SoundThemesDisabled = false;
}