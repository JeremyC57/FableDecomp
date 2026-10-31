// CLoadingMemoryFailureHandler::UnregisterBank(CBankFileAsync* bank) -- under the handler's critical section, find the async bank in the registered-bank array (CArray::Find) and erase it
// Leaf: ?UnregisterBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5A30, 89 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb name set `?UnregisterBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z`; the landed
//   CBankFileAsync::SetLoadingMemoryFailureHandler 0x009D66A0 calls this on the old handler before RegisterBank 0x009D6420
//   (landed) on the new one; the other caller is 0x009D653B. Catalog label "CBankFile::PopLoadErrorQueue" is wrong.
// Callees: CCriticalSectionLock(_RTL_CRITICAL_SECTION&, bool) @ 0x00A62920 (PDB/bound spelling), ~CCriticalSectionLock
//   @ 0x00A629A0, std::__find<CBankFileAsync**, CBankFileAsync*> @ 0x009D6E80 (landed; same symbol), memmove @ 0x00BFEAE6.
// Layout: Banks (Ego_r.pdb CArray<CBankFileAsync*>; retail CArray = the 12-byte STLport 4.6 vector, as in the landed
//   RegisterBank 0x009D6420) +0x04, CriticalSection +0x10.
// Shape / lever (chunk 604): the PDB has `CArray<CBankFileAsync*>::Find(CBankFileAsync* const&)` (Ego_r name set), so the
//   body is `it = Banks.Find(bank); if (it != Banks.end()) Banks.erase(it);` with Find = inline std::find. Three things
//   together give retail's bytes: (1) the real STLport 4.6 vector::erase over `__copy_trivial` (its `__last == __first`
//   test CSEs with `__position + 1 != end()` and gives retail's `cmp ecx,edx` operand order; the parked hand erase gave
//   `cmp edx,ecx`); (2) the real STLport __find body in the TU as a noinline COMDAT (the empty random_access_iterator_tag
//   is then homed on the live `bank` slot, &tag == &val, frame 8); (3) the find reached through the inline CArray::Find
//   (with the bare std::find call VC7.1 kept Banks._M_finish in edi across the call; through Find it reloads [esi+8]
//   after the call and loads first/last between the pushes, as retail does).
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
// Layout: engine/CCriticalSectionLock.h matches the data (+0 CriticalSection, +4 Locked) but declares no constructor/destructor, which this body calls; modelled here with the same members.
class CCriticalSectionLock {
public:
    _RTL_CRITICAL_SECTION* CriticalSection;
    bool                   Locked;
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                       // 0x00A629A0
};
class CBankFileAsync;
namespace std {
struct __true_type { __true_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> inline random_access_iterator_tag __iterator_category(_Tp* const&) { return random_access_iterator_tag(); }
template <class _RandomAccessIter, class _Tp>
// STLport 4.6 _algobase.c __find (random-access, unrolled by four), the body landed at 0x009D6E80; a same-TU noinline COMDAT here.
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val, const random_access_iterator_tag&)   // 0x009D6E80
{
    long __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}

template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, __iterator_category(__first));
}
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    typedef __true_type _TrivialAss;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_trivial(__position + 1, this->_M_finish, __position);
        --this->_M_finish;
        return __position;
    }
protected:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x08
};
}
template <class T> class CArray : public std::vector<T> {      // retail CArray: the 12-byte STLport vector
public:
    typename std::vector<T>::iterator Find(const T& item) { return std::find(this->begin(), this->end(), item); }
};
class CFailedAllocationHandler {                   // vtable 0x0129B874
public:
    virtual bool HandleFailedAllocation(unsigned long size);
};
class CLoadingMemoryFailureHandler : public CFailedAllocationHandler {   // vtable 0x0129B8C4
public:
    CArray<CBankFileAsync*> Banks;                 // +0x04
    _RTL_CRITICAL_SECTION   CriticalSection;       // +0x10
    virtual bool HandleFailedAllocation(unsigned long size);
    void RegisterBank(CBankFileAsync* bank);
    void UnregisterBank(CBankFileAsync* bank);
};

void CLoadingMemoryFailureHandler::UnregisterBank(CBankFileAsync* bank)
{
    CCriticalSectionLock lock(CriticalSection, false);
    CArray<CBankFileAsync*>::iterator it = Banks.Find(bank);
    if (it != Banks.end())
        Banks.erase(it);
}