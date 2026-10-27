// CThingSearchTools::GetAllThings(const CThingFilter_HasScriptName& filter, std::vector<CThing*>& result) const -- (<CThingMarker>) append every scan-visible CThingMarker passing the filter to the vector
// Leaf: ??$GetAllThings@VCThingMarker@@V?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@VCThingFilter_HasScriptName@@@CThingSearchTools@@QBEXABVCThingFilter_HasScriptName@@AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E8AC0, 153 bytes, thiscall, `ret 8`.
// Identity (proven): the type mask 0x8007 is CThingMarker's, the filter called is CThingFilter_HasScriptName::operator() 0x006A7E30 and its overflow 0x00436AC0 is the landed vector<CThing*>::_M_insert_overflow (family F005); its caller is CModeBriarRoseConcentrate::OnActivate 0x008E8060;
//   Ego_r.pdb/FableWin.pdb ??$GetAllThings@VCThingMarker@@V?$vector@...VCThingFilter_HasScriptName@@@CThingSearchTools@@QBEX...@Z is the only matching
//   instantiation. The manifest's "GetAllThings<...>" spelling is propagated.
// Callees: CThingSearchTools::PeekAllThings() const 0x0049C770, CThingFilter_HasScriptName::operator()(const CThing&) const 0x006A7E30,
//   vector<CThing*>::_M_insert_overflow(pointer, const value_type&, const __true_type&, size_type, bool) 0x00436AC0.
// Shape (family M175, representative 0x006437D0): the landed GetAllThings 0x0062E840 loop (scan test, the type-mask test,
//   the filter, push_back of a by-value ThingCast temporary homed in the dead argument slot); the POD overflow body is the
//   real STLport one kept out of line (visible `__declspec(noinline)` member) so the tag and the pushed value are homed as
//   retail does. Speed TU.
// Header debt: engine/CThingSearchTools.h declares no member functions, so the class is modelled locally.
#include <stdlib.h>
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* where) { return where; }
// ---- CThing (retail layout): ThingTypeFlags +0x6c, the state bit byte +0x91 (Dead 0x01, IncludeInThingScans 0x04,
//      Activated 0x08, InLimbo 0x20), as the landed CThingSearchTools rows.
class CThing {
public:
    enum { ThingTypeMask = 0 };
    unsigned char _pad_0x00[0x6c];
    unsigned long ThingTypeFlags;                         // +0x6c
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                              // +0x91
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
template <class T> inline T* ThingCast(CThing* thing) { return (T*)thing; }
class CThingMarker : public CThing { public: enum { ThingTypeMask = 0x8007 }; };
class CThingFilter_HasScriptName {
public:
    bool operator()(const CThing& thing) const;           // 0x006A7E30
};
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
namespace std {
struct __true_type { __true_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp> inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
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
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
protected:
    void _M_clear() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage._M_data = __e; }
    __declspec(noinline) void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                                                 bool __atend = false)   // 0x00436AC0
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = _M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(_M_start, __position, __new_start);
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            __new_finish = (pointer)__copy_trivial(__position, _M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
}
class CWorldMap;
class CThingManager;
struct CThingMapManager;
class CThingSearchTools {                                 // Ego_r.pdb members (retail 0x18 bytes)
public:
    CThingList* PeekAllThings() const;                    // 0x0049C770
    template <class TThing, class TContainer, class TFilter> void GetAllThings(const TFilter& filter, TContainer& result) const;
    CWorldMap* WorldMap;                                  // +0x00
    CThingMapManager* PThingMapManager;                   // +0x04
    CThingManager* ThingManager;                          // +0x08
    void* AreaMaps[3];                                    // +0x0c vector<NThingMap::CThingMap*>
};
template <class TThing, class TContainer, class TFilter>
void CThingSearchTools::GetAllThings(const TFilter& filter, TContainer& result) const
{
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && IsThingOfType<TThing>(thing) && filter(*(TThing*)thing))
            result.push_back(ThingCast<TThing>(thing));
    }
}
template void CThingSearchTools::GetAllThings<CThingMarker, std::vector<CThing*>, CThingFilter_HasScriptName >(const CThingFilter_HasScriptName&, std::vector<CThing*>&) const;