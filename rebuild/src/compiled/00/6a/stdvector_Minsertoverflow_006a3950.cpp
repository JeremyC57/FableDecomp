// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser>) STLport growth insert of 9-byte screen-effect bypass records (intelligent thing pointer + flag): allocate size + max(size, fill_len) records, copy the head, x once (or fill_len times), the tail unless atend, destroy the old records, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3950, 282 bytes, thiscall, `ret 14h` (five arguments).
// Identity (proven): the element is 9 bytes (`imul 38E38E39h ; sar edx,1`, `add edi,9`) = Ego_r.pdb
//   NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser {CIntelligentPointer<CThing> thing +0; bool
//   bypassSet +8} (the only 9-byte {CIntelligentPointer<CThing>, bool} record; the PDB's script-side
//   CScreenFilterSThingByPass is 16 bytes, holding a CScriptThing). The inline single copy runs CIntelligentPointer<CThing>'s
//   copy (base ctor 0x00A01B10, vtable 0x01238C6C, SetPItem(GetPItem())), the destroy loop the base destructor 0x00A01C10.
//   Callers: CTCCarrying::ResetAllCarriedThingsAsBypassFilter 0x006A23E0 (0x006A2495), the push_back 0x006A3C70 (0x006A3CC2)
//   and the script interface at 0x008A9941; helpers: the landed CThingGraphicArgPasser __uninitialized_copy 0x006A3130 and
//   __uninitialized_fill_n 0x006A3180 of this TU.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy 0x006A3130 (prefix and suffix, `__fastcall`), CBaseIntelligentPointer::
//   CBaseIntelligentPointer() 0x00A01B10, GetPItem() const 0x00A01B50, SetPItem(CBaseObject*) 0x00A01B90,
//   __uninitialized_fill_n 0x006A3180 (`__fastcall`), ~CBaseIntelligentPointer 0x00A01C10, free 0x00BFEA14.
// Model: this chunk's 0x008AD5F0 (landed twin 0x00B6C120 recipe): STLport 4.6 vector.c with the helpers as same-TU noinline
//   bodies (tags on the dead atend slot) and _M_clear inline; the record is byte-packed (pack(1)).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CBaseObject;
class CThing;

#pragma pack(push, 1)
class CBaseIntelligentPointer {                             // 8 bytes, 1-aligned in retail
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    void* PData;                                            // +0x04 CBaseObjectPointer*
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for T = CThing
public:
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CThingGraphicArgPasser {                         // Ego_r.pdb, 9 bytes
        CIntelligentPointer<CThing> thing;                  // +0x00
        bool                        bypassSet;              // +0x08
    };
};
}
#pragma pack(pop)

namespace std {

struct __false_type { __false_type() { } };

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
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x006A3130
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x006A3180
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

typedef NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser CThingGraphicArgPasser;
template void vector<CThingGraphicArgPasser>::_M_insert_overflow(CThingGraphicArgPasser*, const CThingGraphicArgPasser&,
                                                                const __false_type&, unsigned int, bool);

}