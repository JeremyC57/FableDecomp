// NPlayerGui::CDrawAbilityButtonsPC::GetAbilityPositionMapping(std::vector<long>& mapping) -- fill the PC ability-button layout table: clear the vector and append 1, 2, -1
// Leaf: ?GetAbilityPositionMapping@CDrawAbilityButtonsPC@NPlayerGui@@MAEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006579e0, 209 bytes, thiscall, `ret 4` (`this` unused).
// Identity (probable): vtable 0x0125A7D8 (NPlayerGui::CDrawAbilityButtonsPC) slot 5; Ego_r.pdb gives CDrawAbilityButtonsPC two protected virtual overrides, GetAbilityButtonMapping and GetAbilityPositionMapping; this one returns 1, 2, -1 -- the -1-terminated form its expression twin GetExpressionPositionMapping 0x00657BA0 (0, 1, -1) shares -- so the position mapping (the button/position split is by those values).
// Callees: memmove 0x00BFEAE6 (clear's __copy_trivial), std::vector<long>::_M_insert_overflow(pointer, const long&,
//   const __true_type&, size_type, bool) 0x00440990 (the bound protected IAE spelling).
// Shape (family L025): STLport 4.6 `clear()` = erase(begin(), end()) through __copy_trivial (`last == first ? result :
//   memmove(..) + n`), then three inline push_backs; the pushed temporaries and the empty __true_type tag are homed on the
//   dead `mapping` argument slot, which needs the same-TU noinline growth-path body below.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __true_type { __true_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp> inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator, _ForwardIterator) { }
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);                                     // 0x00440990 for long
};
// Same-TU noinline body of the POD growth path (STLport 4.6 vector.c; not graded here): with it VC7.1 homes the pushed
// value and the empty tag on the dead `mapping` argument slot as retail does.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
                                                                  size_type __fill_len, bool __atend)
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
}
namespace NPlayerGui {
class CDrawAbilityButtons {
protected:
    virtual void GetAbilityButtonMapping(std::vector<long>& mapping);
    virtual void GetAbilityPositionMapping(std::vector<long>& mapping);
};
class CDrawAbilityButtonsPC : public CDrawAbilityButtons {
protected:
    virtual void GetAbilityButtonMapping(std::vector<long>& mapping);
    virtual void GetAbilityPositionMapping(std::vector<long>& mapping);
    void GetExpressionButtonMapping(std::vector<long>& mapping);
    void GetExpressionPositionMapping(std::vector<long>& mapping);
};
}
void NPlayerGui::CDrawAbilityButtonsPC::GetAbilityPositionMapping(std::vector<long>& mapping)
{
    mapping.clear();
    mapping.push_back(1);
    mapping.push_back(2);
    mapping.push_back(-1);
}