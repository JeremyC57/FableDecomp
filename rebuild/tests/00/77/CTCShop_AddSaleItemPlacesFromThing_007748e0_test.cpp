// Test for 0x007748E0 CTCShop::AddSaleItemPlacesFromThing: a shop thing owning a dead thing and a stall whose graphic
// appearance answers four of the twenty dummy-object names; checks recursion, the name sequence and the collected places.
#include "fable_compiler.h"
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#endif

namespace std {

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };

template <class _CharT> struct char_traits;
template <> struct char_traits<char> {
public:
    typedef char char_type;
    static void assign(char& __c1, const char& __c2) { __c1 = __c2; }
    static unsigned int length(const char* __s) { return strlen(__s); }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
          ((const char*)__last - (const char*)__first);
}
inline char* uninitialized_copy(const char* __first, const char* __last, char* __result)
{
    return (char*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc> class _String_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    unsigned int max_size() const { return ((unsigned int)-1 / sizeof(_Tp)) - 1; }
    void _M_allocate_block(unsigned int __n)
    {
        if ((__n <= (max_size() + 1)) && (__n > 0)) {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        }
        else
            _M_throw_length_error();
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    __declspec(noreturn) void _M_throw_length_error() const;                           // 0x00565A30
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    _String_base(const allocator_type& __a, unsigned int __n)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
    {
        _M_allocate_block(__n);
    }
    ~_String_base() { _M_deallocate_block(); }
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    typedef typename _String_base<_CharT, _Alloc>::allocator_type allocator_type;
    enum { _DEFAULT_SIZE = 8 };

    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    basic_string() : _String_base<_CharT, _Alloc>(allocator_type())                   // STLport 4.6 _string.c
    {
        this->_M_start = this->_M_end_of_storage.allocate(8);
        this->_M_finish = this->_M_start;
        this->_M_end_of_storage._M_data = this->_M_start + 8;
        _M_terminate_string();
    }
    basic_string(const _Self& __s) : _String_base<_CharT, _Alloc>(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }
    basic_string(const _CharT* __s) : _String_base<_CharT, _Alloc>(allocator_type())
    {
        _M_range_initialize(__s, __s + _Traits::length(__s));
    }
    ~basic_string() { }
    _Self& operator=(const _CharT* __s) { return assign(__s, __s + _Traits::length(__s)); }
    _Self& assign(const _CharT* __f, const _CharT* __l);                                   // 0x005693B0
    const _CharT* c_str() const { return this->_M_start; }

    size_type size() const { return this->_M_finish - this->_M_start; }
    const _CharT& operator[](size_type __n) const { return *(this->_M_start + __n); }
    void reserve(size_type __res_arg = 0);                                             // 0x00777DB0 (same-TU stand-in below)
    void push_back(_CharT __c)
    {
        if (this->_M_finish + 1 == this->_M_end_of_storage._M_data)
            reserve(size() + (max)(size(), (size_type)1));
        _M_construct_null(this->_M_finish + 1);
        _Traits::assign(*(this->_M_finish), __c);
        ++this->_M_finish;
    }
    _Self& operator+=(_CharT __c) { push_back(__c); return *this; }

    template <class _ForwardIter>
    void _M_range_initialize(_ForwardIter __f, _ForwardIter __l, const forward_iterator_tag&)
    {
        int __n = __l - __f;
        this->_M_allocate_block(__n + 1);
        this->_M_finish = uninitialized_copy(__f, __l, this->_M_start);
        _M_terminate_string();
    }
    template <class _InputIter> void _M_range_initialize(_InputIter __f, _InputIter __l)
    {
        _M_range_initialize(__f, __l, forward_iterator_tag());
    }
    void _M_construct_null(_CharT* __p) { *__p = _CharT(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
};

#pragma auto_inline(off)
template <class _Tp, class _Alloc>
void _String_base<_Tp, _Alloc>::_M_throw_length_error() const                         // 0x00565A30 (same-TU stand-in)
{
    puts("basic_string");
    abort();
}
template <class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::reserve(size_type __res_arg)              // 0x00777DB0 (landed body)
{
    if (__res_arg > this->max_size())
        this->_M_throw_length_error();
    size_type __n = (max)(__res_arg, size()) + 1;
    _CharT* __new_start = this->_M_end_of_storage.allocate(__n);
    _CharT* __new_finish = __new_start;
    __new_finish = uninitialized_copy(this->_M_start, this->_M_finish, __new_start);
    _M_construct_null(__new_finish);
    this->_M_deallocate_block();
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __n;
}
// Same-TU stand-in for basic_string<char>::assign(const char*, const char*) 0x005693B0 (STLport: copy over the prefix, then
// erase the tail or append the rest): with a body VC7.1 knows the only escape of the string's address is to its own members,
// as retail's LTCG did, so the dead constructor stores go and the storage pointers live in registers.
template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT, _Traits, _Alloc>& basic_string<_CharT, _Traits, _Alloc>::assign(const _CharT* __f, const _CharT* __l)
{
    unsigned int __n = __l - __f;
    if (__n <= size()) {
        memmove(this->_M_start, __f, __n);
        this->_M_finish = this->_M_start + __n;
    } else {
        if (this->_M_end_of_storage._M_data - this->_M_start <= (int)__n)
            reserve(__n);
        memmove(this->_M_start, __f, __n);
        this->_M_finish = this->_M_start + __n;
    }
    _M_terminate_string();
    return *this;
}
#pragma auto_inline(on)

typedef basic_string<char> string;
}

class CTCBase;
class CCharString {
public:
    CCharString(const char* text, long length);                 // 0x0099EBF0
    ~CCharString();                                             // 0x0099EAE0
    const CCharString& operator=(const char* text);             // 0x0099EFE0
    void* PStringData;
};
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& set,
                        float scale) const;                     // 0x004C08C0
};
}
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound const 0x004365B0 (STLport lower_bound over the 8-byte entries; the landed
// 0x00848730 idiom): VC7.1 then knows the key's address does not escape and homes it in the dead argument slot like retail.
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset (bitset<274>)
    CInterfaceMap m_interfaces;                         // +0x44 Components
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                             // +0x91 bit 0
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                         // vtable + PData (Ego_r: 8 bytes)
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
public:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CObjectNode {                                    // STLport list node of CIntelligentPointer<CThing>
    CObjectNode* _M_next;
    CObjectNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;                // +0x08
};
class CTCThingOwner { public:                           // interface 0x21
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CObjectNode* ObjectList;                            // +0x0c (header node)
};
struct GoodsPlaceVector {                               // STLport vector<CTCShop::GoodsPlace>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
};
class CTCShop { public:
    struct GoodsPlace {                                 // 36 bytes (Ego_r.pdb)
        C3DVector       pos;                            // +0x00
        CRightHandedSet rhset;                          // +0x0c
    };
    struct CGoodsPlaceVector : GoodsPlaceVector {
        void push_back(const GoodsPlace& place);        // 0x00778210
    };
    virtual void v00();
    CThing*           ParentThing;                      // +0x04 (CTCBase)
    unsigned char     _pad08[0x28 - 0x08];
    CGoodsPlaceVector GoodsPlaces;                      // +0x28
    void AddSaleItemPlacesFromThing(const CThing* thing);
};

void CTCShop::AddSaleItemPlacesFromThing(const CThing* thing)
{
    if (thing == 0 || thing->Dead)
        return;
    if (thing->HasTC(0x21)) {
        const CTCThingOwner* owner = (const CTCThingOwner*)thing->GetTC(0x21);
        for (CObjectNode* node = owner->ObjectList->_M_next; node != owner->ObjectList; node = node->_M_next)
            AddSaleItemPlacesFromThing(node->_M_data.GetPItem());
    }
    if (thing->HasTC(0x5b)) {
        const NGraphicAppearance::CTCGraphicAppearance* appearance =
            (const NGraphicAppearance::CTCGraphicAppearance*)thing->GetTC(0x5b);
        for (long i = 0; i < 10; i++) {
            long number = i + 1;
            std::string name("GoodsPlace");
            if (number / 10)
                name.push_back((char)('0' + (number / 10) % 10));
            name.push_back((char)('0' + number % 10));
            CCharString placeName(name.c_str(), -1);
            GoodsPlace place;
            if (appearance->GetDummyObject(placeName, 0, 0, place.pos, place.rhset, 1.0f)) {
                GoodsPlaces.push_back(place);
            } else {
                name = "GOODS_PLACE_";
                name.push_back((char)('0' + (number / 10) % 10));
                name.push_back((char)('0' + number % 10));
                placeName = name.c_str();
                if (appearance->GetDummyObject(placeName, 0, 0, place.pos, place.rhset, 1.0f))
                    GoodsPlaces.push_back(place);
            }
        }
    }
}

// ---- test doubles -------------------------------------------------------------------------------------------------------
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
CCharString::CCharString(const char* text, long length) { PStringData = strdup(text); if (length != -1) ((char*)PStringData)[length] = 0; }
CCharString::~CCharString() { free(PStringData); }
const CCharString& CCharString::operator=(const char* text) { free(PStringData); PStringData = strdup(text); return *this; }
static char g_requested[64][20]; static int g_requestCount = 0;
bool NGraphicAppearance::CTCGraphicAppearance::GetDummyObject(const CCharString& name, long index, unsigned long flags,
                                                               C3DVector& pos, CRightHandedSet& set, float scale) const
{
    const char* text = (const char*)name.PStringData;
    if (g_requestCount < 64) strcpy(g_requested[g_requestCount], text);
    g_requestCount++;
    if (index != 0 || flags != 0 || scale != 1.0f) return false;
    float which = 0.0f;
    if (strcmp(text, "GoodsPlace1") == 0) which = 1.0f;
    else if (strcmp(text, "GOODS_PLACE_02") == 0) which = 2.0f;
    else if (strcmp(text, "GoodsPlace3") == 0) which = 3.0f;
    else if (strcmp(text, "GoodsPlace10") == 0) which = 10.0f;
    else return false;
    pos.X = which; pos.Y = which * 2.0f; pos.Z = which * 3.0f;
    set.Up.X = 0.0f; set.Up.Y = 1.0f; set.Up.Z = 0.0f;
    set.Forward.X = which; set.Forward.Y = 0.0f; set.Forward.Z = 0.0f;
    return true;
}
static CTCShop::GoodsPlace g_places[16]; static int g_placeCount = 0;
void CTCShop::CGoodsPlaceVector::push_back(const GoodsPlace& place) { if (g_placeCount < 16) g_places[g_placeCount] = place; g_placeCount++; }
void CTCThingOwner::v00() { }
void CTCShop::v00() { }

static void SetInterface(CThing& thing, CInterfaceMap::SEntry* entries, int count, long type, void* component)
{
    thing.m_presentBits[type >> 5] |= (1UL << (type & 31));
    entries[count].type = type; entries[count].component = (CTCBase*)component;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + count + 1;
}

int main(void)
{
    int ok = 1;
    static CThing shop, deadThing, stall;
    static CTCThingOwner owner;
    static NGraphicAppearance::CTCGraphicAppearance appearance;
    static CInterfaceMap::SEntry shopEntries[2], stallEntries[2];
    static CObjectNode header, nodeDead, nodeStall;
    static CTCShop tcShop;
    memset(&shop, 0, sizeof(shop)); memset(&deadThing, 0, sizeof(deadThing)); memset(&stall, 0, sizeof(stall));
    memset(&tcShop, 0, sizeof(tcShop));
    deadThing.Dead = 1;
    // the shop owns the dead thing and the stall (sorted interface maps: 0x21 only on the shop, 0x5b only on the stall)
    SetInterface(shop, shopEntries, 0, 0x21, &owner);
    SetInterface(stall, stallEntries, 0, 0x5b, &appearance);
    header._M_next = &nodeDead; header._M_prev = &nodeStall;
    nodeDead._M_next = &nodeStall; nodeDead._M_prev = &header; nodeDead._M_data.PData = (CBaseObjectPointer*)&deadThing;
    nodeStall._M_next = &header; nodeStall._M_prev = &nodeDead; nodeStall._M_data.PData = (CBaseObjectPointer*)&stall;
    owner.ObjectList = &header;

    tcShop.AddSaleItemPlacesFromThing(0);
    tcShop.AddSaleItemPlacesFromThing(&deadThing);
    if (g_requestCount != 0 || g_placeCount != 0) ok = 0;
    tcShop.AddSaleItemPlacesFromThing(&shop);
    // 10 first tries + 7 retries (the misses 2, 4, 5, 6, 7, 8, 9; 2 is found on the retry)
    if (g_requestCount != 17) ok = 0;
    const char* expected[17] = { "GoodsPlace1", "GoodsPlace2", "GOODS_PLACE_02", "GoodsPlace3", "GoodsPlace4", "GOODS_PLACE_04",
        "GoodsPlace5", "GOODS_PLACE_05", "GoodsPlace6", "GOODS_PLACE_06", "GoodsPlace7", "GOODS_PLACE_07", "GoodsPlace8",
        "GOODS_PLACE_08", "GoodsPlace9", "GOODS_PLACE_09", "GoodsPlace10" };
    for (int i = 0; i < 17 && i < g_requestCount; i++)
        if (strcmp(g_requested[i], expected[i]) != 0) ok = 0;
    if (g_placeCount != 4) ok = 0;
    float which[4] = { 1.0f, 2.0f, 3.0f, 10.0f };
    for (int j = 0; j < 4 && j < g_placeCount; j++) {
        if (g_places[j].pos.X != which[j] || g_places[j].pos.Y != which[j] * 2.0f || g_places[j].pos.Z != which[j] * 3.0f) ok = 0;
        if (g_places[j].rhset.Up.Y != 1.0f || g_places[j].rhset.Forward.X != which[j]) ok = 0;
    }
    printf(ok ? "007748e0_TEST PASS\n" : "007748e0_TEST FAIL\n");
    return 0;
}