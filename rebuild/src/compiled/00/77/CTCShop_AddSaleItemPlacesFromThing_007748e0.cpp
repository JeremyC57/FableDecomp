// CTCShop::AddSaleItemPlacesFromThing(const CThing* thing) -- add the goods places of a shop thing: recurse into every object the thing owns (CTCThingOwner), then read its "GoodsPlace1".."GoodsPlace10" (or "GOODS_PLACE_01".."GOODS_PLACE_10") dummy objects from its graphic appearance into GoodsPlaces
// Leaf: ?AddSaleItemPlacesFromThing@CTCShop@@QAEXPBVCThing@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007748E0, 596 bytes, thiscall `ret 4`.
// Identity (proven): Ego_r.pdb ?AddSaleItemPlacesFromThing@CTCShop@@QAEXPBVCThing@@@Z (CTCShop TU); the landed
//   CTCShop::GetFreeSaleItemPlaces 0x00774B40 calls it on the parent thing right after erasing GoodsPlaces (it named the
//   callee "InitGoodsPlaces": NAME_FIXES); the GoodsPlaces vector (+0x28; Ego_r +0x2c) receives the places; literals
//   "GoodsPlace" 0x0126A9CC and "GOODS_PLACE_" 0x0126A9BC. The catalog "CTCGraphicAppearance_InitGoodsPlaces" is descriptive.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (inlined const GetTC of 0x21 = CTCThingOwner and
//   0x5b = CTCGraphicAppearance), CBaseIntelligentPointer::GetPItem const 0x00A01B50, itself 0x007748E0, malloc 0x00BFEA0E /
//   memmove 0x00BFEAE6 / free 0x00BFEA14 (the inlined STLport string constructor and destructor), basic_string<char>::
//   push_back 0x00777F80, basic_string<char>::reserve 0x00777DB0 (inside the one inlined push_back), basic_string<char>::
//   assign(const char*, const char*) 0x005693B0 (operator=(const char*)), CCharString(const char*, long) 0x0099EBF0,
//   CCharString::operator=(const char*) 0x0099EFE0, ~CCharString 0x0099EAE0, NGraphicAppearance::CTCGraphicAppearance::
//   GetDummyObject 0x004C08C0, vector<CTCShop::GoodsPlace>::push_back 0x00778210.
// Layout: CThing interface bitset +0x20, map +0x44, Dead bit 0 of +0x91; CTCThingOwner ObjectList +0x0c (STLport list header
//   node of CIntelligentPointer<CThing>, item at node +8); CTCShop::GoodsPlace {C3DVector pos; CRightHandedSet rhset} 36
//   bytes (Ego_r.pdb); CTCShop GoodsPlaces +0x28.
// Shape: the thing-owner recursion first, then `for (long i = 0; i < 10; i++)` with `number = i + 1` (VC7.1 keeps the
//   substituted induction variable in the frame beside a 10-down counter; `for (i = 1; i <= 10)` compares instead)
//   building a STLport std::string name ("GoodsPlace" + the decimal digits of number, the tens digit only when non-zero),
//   a CCharString copy of it handed to GetDummyObject with the place's position and orientation; on failure the name is
//   rebuilt as "GOODS_PLACE_" + two zero-padded digits and retried. VC7.1 inlines only the second push_back (the others
//   call 0x00777F80), as retail. The lookup keys, the CCharString and the dead `thing` argument share one frame word; the
//   same-TU bodies of reserve and assign keep the string's address from escaping (no dead constructor stores).
#include "fable_compiler.h"
#include <string.h>

extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);    // 0x00BFEAE6
extern "C" int __cdecl puts(const char* text);                                        // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();                                // 0x00BFEAEC

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
#include "engine/C3DVector.h"
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
private:
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