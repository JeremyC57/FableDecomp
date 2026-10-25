// std::vector::_M_insert_overflow(CQuestDesc* position, const CQuestDesc& x, const __false_type&, unsigned int fillLength, bool atEnd) -- (vector<CHeroLogBook::CQuestDesc>) STLport growth path of the hero log book's quest list: allocate size + max(size, n), copy-construct the head, n copies of x and (unless at the end) the tail, destroy and free the old block
// Leaf: ?_M_insert_overflow@?$vector@VCQuestDesc@CHeroLogBook@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047CCD0, 255 bytes, thiscall `ret 14h` (position, x, the tag, fillLength, atEnd); /O2.
// Identity: stride 24 (signed divide by 24: 0x2AAAAAAB, `sar edx,2`) with the landed out-of-line helpers of this
//   element, __uninitialized_copy 0x0047C290, __uninitialized_fill_n 0x0047C2E0 and __destroy_aux 0x0047C800 (landed as
//   "DestroyAux_SNamedListRecord18"); single caller, the quest push_back 0x0047CE32. The catalog label
//   "Std_Vector_Insert_Range" is wrong: this is _M_insert_overflow.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy 0x0047C290 (head and tail), CCharString(const CCharString&) 0x0099EC30
//   and vector<CQuestObjectiveDesc>(const vector&) 0x0047BA50 (the inlined n == 1 copy), __uninitialized_fill_n 0x0047C2E0,
//   __destroy_aux(first, last, const __false_type&) 0x0047C800, free 0x00BFEA14.
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` as 0x0047C140 (out-of-line __destroy_aux, every empty
//   tag homed on the live `atEnd` slot through the same-TU helper bodies).
// Model: Ego_r.pdb CHeroLogBook::CQuestDesc {ScriptID, TextDescription, QuestStatus, QuestSubObjectives}, as the landed
//   0x0047C290; CCharString is the engine's one-pointer string handle; STLport's three vector words.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CCharStringData;

class CCharString {
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
private:
    CCharStringData* PStringData;                         // +0x00
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)   // 0x0047C290
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)                     // 0x0047C2E0
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIter>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)   // 0x0047C800
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    vector(const vector<_Tp>& __x);                       // 0x0047BA50 for vector<CQuestObjectiveDesc>
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void _M_clear() { _Destroy(_M_start, _M_finish); if (_M_start != 0) free(_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                                     bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = allocate(__len);
    pointer __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

}

class CHeroLogBook {
public:
    class CQuestObjectiveDesc;
    enum EQuestStatus { QUEST_STATUS_NONE };
    class CQuestDesc {                                    // Ego_r.pdb (retail 24 bytes: STLport's 12-byte vector)
    public:
        unsigned long                             ScriptID;              // +0x00
        CCharString                               TextDescription;       // +0x04
        EQuestStatus                              QuestStatus;           // +0x08
        std::vector<CQuestObjectiveDesc>         QuestSubObjectives;    // +0x0C
    };
};

namespace std {

template void vector<CHeroLogBook::CQuestDesc>::_M_insert_overflow(CHeroLogBook::CQuestDesc*, const CHeroLogBook::CQuestDesc&, const __false_type&,
                                                          unsigned int, bool);

}