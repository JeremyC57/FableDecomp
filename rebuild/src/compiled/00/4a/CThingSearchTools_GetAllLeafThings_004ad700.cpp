// CThingSearchTools::GetAllLeafThings(const TFilter& filter, TContainer& result) [TThing = CThingBuilding, TContainer = std::vector<CThingBuilding*>, TFilter = CThingFilter_None] -- append every scan-visible building (type list 3) to the result vector
// Leaf: GetAllLeafThings
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AD700, 121 bytes (+ int3 padding) (`ret 8`).
// Callees: PeekTypeList(3) 0x0049C780, vector<const CThingBuilding*>::_M_insert_overflow 0x004AC530; CThingFilter_None inlines to true
// (the catalog label's CThingFilter_HasTC is propagated: the body calls no filter). Caller CWorld::PreparePrimitivesForEngine.
// Identity: byte-identical to the landed twin 0x006D4600 (CThingBuilding*, list 4) apart from the list id and the overflow callee;
// the same model ported (retry12). The parked hand335 model (filter on a pointer, `(TThing*)thing` pushed directly, an explicit
// specialisation stand-in) left the thing in edx: the by-value ThingCast<T> temporary and the template noinline stand-in give ecx.
// Identity (sweep3): TThing = CThingBuilding, TContainer = std::vector<CThingBuilding*> (the landed row said const CThingBuilding).
//   Ego_r/FableWin name GetAllLeafThings with CThingFilter_None only as <CThingBuilding, vector<CThingBuilding*>> (buildings),
//   <CThingVillage, vector<CThingVillage*>> and <CThingAICreature, list<...>>; type list 3 is CThingBuilding (both CThingBuilding
//   ctors construct CThing(3) before the RTTI vtable 0x0124509C), and the const-element GetAllLeafThings exists only with
//   CThingFilter_HasTC (0x004AD780, overflow 0x004AC610). The overflow 0x004AC530 is therefore vector<CThingBuilding*>'s.
// Model: CThingSearchTools from engine/CThingSearchTools.h (`this` is only passed through to the list getter);
// CThing, the thing lists, the result container and the filters are local retail-layout models.
// Shape: scan test, then the type-mask test (mask copied into a local), then the filter on the thing reference.
// A vector push takes a by-value ThingCast<T> temporary (homed in the dead argument slot and shared with the
// __true_type tag: the same-TU noinline overflow stand-in shows VC7.1 the tag is never read).
#include "engine/CThingSearchTools.h"
#include <stdlib.h>

inline void* operator new(unsigned int, void* where) { return where; }
namespace std { struct __true_type { __true_type() {} }; }
using std::__true_type;

// ---- CThing (retail layout; Ego_r.pdb offsets in brackets) -------------------------------------
// Retail CThing sits 0x10 below the PDB from ThingTypeFlags on: UsingComponents +0x20 [PDB +0x20],
// ThingTypeFlags +0x6c [PDB +0x7c], the state bit byte +0x91 [PDB +0xa1] = Dead 0x01,
// IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20.
class CThing {                                        // PDB def type: class
public:
    enum { ThingTypeMask = 0 };                       // every thing matches the CThing type mask
    unsigned char _pad_0x00[0x20];
    unsigned char UsingComponents[0x24];              // +0x20 bitset<274> of present thing components
    unsigned char _pad_0x44[0x6c - 0x44];
    unsigned long ThingTypeFlags;                     // +0x6c THING_TYPE_* inheritance bits
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                          // +0x91 Dead / IncludeInThingScans / Activated / InLimbo

    // CThing::IsIncludedInThingScans (out of line at 0x0048BC40), inlined here.
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};

// The type test: the mask is copied into a local first, so the CThing (mask 0) test folds late and
// keeps retail's `xor dl,dl ; jne`, and a byte-sized mask compares through the low byte.
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

// The thing as the searched type, returned by value (a fresh temporary for push_back's const T&).
template <class T> inline T* ThingCast(CThing* thing) { return (T*)thing; }

// The thing manager's lists: std::list<CThing*> (STLport: the node pointer at +0, node = next, prev, value).
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

// The result: STLport 4.6 std::vector<CThingBuilding*> (_Vector_base + _STLP_alloc_proxy); push_back's growth path is the
// protected POD `_M_insert_overflow`, defined in the class body in STLport and kept out of line by VC7.1 (0x004AC530). Its
// verbatim body sits in this TU under auto_inline(off) + noinline, so VC7.1 sees that the __true_type tag is never read and
// homes the tag and the pushed value in the dead argument slot as retail does.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
namespace std {
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
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
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
                            bool __atend = false);   // 0x004AC530 for vector<CThingBuilding*>
};
#pragma auto_inline(off)
template <class _Tp, class _Alloc> __declspec(noinline)
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                                             bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
    // handle insertion
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        // copy remainder
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline(on)
}

class CThingBuilding : public CThing { public: enum { ThingListType = 3 }; };   // CThing(3) in both CThingBuilding ctors 0x00529956/0x0052AC28
class CThingFilter_None {
public:
    bool operator()(const CThing&) const { return true; }
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekTypeList(long type);              // 0x0049C780: the per-type thing list
    template <class TThing, class TContainer, class TFilter> void GetAllLeafThings(const TFilter& filter, TContainer& result);
};

// Every scan-visible thing of TThing's own type list that passes the filter.
template <class TThing, class TContainer, class TFilter>
void CThingSearchToolsMethods::GetAllLeafThings(const TFilter& filter, TContainer& result)
{
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
            result.push_back(ThingCast<TThing>(thing));
    }
}

template void CThingSearchToolsMethods::GetAllLeafThings<CThingBuilding, std::vector<CThingBuilding*>, CThingFilter_None >(const CThingFilter_None&, std::vector<CThingBuilding*>&);