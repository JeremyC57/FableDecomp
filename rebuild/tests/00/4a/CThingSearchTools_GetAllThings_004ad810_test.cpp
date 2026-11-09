// Behaviour test for 0x004AD810 CThingSearchTools::GetAllThings<const CThing, vector<const CThing*>, CThingFilter_None>: the row's code with a stub class list.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __declspec(x)
#endif
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
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
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
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
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
public:
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
                            bool __atend = false);   // 0x0049C370 for vector<const CThing*>
};
// STLport 4.6 _vector.c, out of line in retail; the body stays visible in the TU (see Shape).
#pragma auto_inline(off)
template <class _Tp, class _Alloc> __declspec(noinline)
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                                             bool __atend)
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
#pragma auto_inline(on)
} // namespace std

class CClass { };
// Retail CThing (Ego_r.pdb offsets - 0x10 from ThingTypeFlags on): ThingTypeFlags +0x6C, the state bit byte +0x91.
class CThing : public CClass {
public:
    enum { ThingTypeMask = 0 };                          // every thing matches the CThing type mask
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
// The type test: the mask is copied into a local, so the CThing (mask 0) test folds late (retail 0x004AD84E `xor al,al ; jne`).
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
// The thing as the searched type, returned by value (a fresh temporary for push_back's const T&).
template <class T> inline T* ThingCast(CThing* thing) { return (T*)thing; }

class CThingFilter_None {
public:
    bool operator()(const CThing&) const { return true; }
};

class CThingSearchTools {                                // only `this` is passed through here
public:
    template <class TThing, class TContainer, class TFilter>
    void GetAllThings(const TFilter& filter, TContainer& result) const;
public:
    const std::list<CClass*>& PeekClassList() const;     // 0x0049C770
};

// Every scan-visible thing of the class list that is of type TThing and passes the filter.
template <class TThing, class TContainer, class TFilter>
void CThingSearchTools::GetAllThings(const TFilter& filter, TContainer& result) const
{
    const std::list<CClass*>& things = PeekClassList();
    for (std::list<CClass*>::const_iterator it = things.begin(); it != things.end(); ++it) {
        CThing* thing = (CThing*)*it;
        if (thing->IsIncludedInThingScans() && IsThingOfType<TThing>(thing) && filter(*(TThing*)thing))
            result.push_back(ThingCast<TThing>(thing));
    }
}

template void CThingSearchTools::GetAllThings<const CThing, std::vector<const CThing*>, CThingFilter_None>(
    const CThingFilter_None&, std::vector<const CThing*>&) const;
static std::list<CClass*> g_list;
const std::list<CClass*>& CThingSearchTools::PeekClassList() const { return g_list; }
struct VecView { const CThing** s; const CThing** f; const CThing** e; };
int main()
{
    static CThing t[6];
    unsigned char bits[6] = { 0x0c, 0x0d, 0x04, 0x2c, 0x4c, 0x08 };
    std::_List_node<CClass*> head, n[6];
    std::_List_node_base* prev = &head;
    for (int i = 0; i < 6; ++i) { t[i].StateBits = bits[i]; t[i].ThingTypeFlags = (unsigned long)i; n[i]._M_data = &t[i]; prev->_M_next = &n[i]; n[i]._M_prev = prev; prev = &n[i]; }
    prev->_M_next = &head; head._M_prev = prev;
    g_list._M_node._M_data = &head;
    CThingSearchTools tools; CThingFilter_None none; std::vector<const CThing*> out;
    VecView* v = (VecView*)&out; v->s = v->f = v->e = 0;
    tools.GetAllThings<const CThing, std::vector<const CThing*>, CThingFilter_None>(none, out);
    int fail = !(v->f - v->s == 2 && v->s[0] == &t[0] && v->s[1] == &t[4]);
    tools.GetAllThings<const CThing, std::vector<const CThing*>, CThingFilter_None>(none, out);   // appends again (growth path)
    fail |= !(v->f - v->s == 4 && v->s[2] == &t[0] && v->s[3] == &t[4] && v->e - v->s >= 4);
    free(v->s);
    puts(fail ? "004ad810_TEST FAIL" : "004ad810_TEST PASS");
    return fail;
}