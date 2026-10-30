// CLoadingMemoryFailureHandler::UnregisterBank(CBankFileAsync* bank) -- under the handler's critical section, find the async bank in the registered-bank array and erase it (memmove the tail down one slot)
// Grade: DIFFER(89v89) (retry1; was 92v89) -- with __find extern (0x009D6E80) the Banks.last reload after the call is retail's and the length matches; residue: the empty iterator tag gets its own frame byte ([esp+7], frame 0xC) where retail homes it on the `bank` argument slot (&tag == &val, frame 8). With the __find body in the TU (before or after the caller: VC7.1 compiles the callee first either way) VC shares the slot but also keeps Banks.last in edi across the call (it knows __find writes no memory) -- retail has the slot sharing without that mod/ref knowledge, which no single-TU form gave | tried: extern + by-value category helper, body after the caller | next: a callee body that VC cannot prove read-only but that does not read the tag
// Leaf: ?UnregisterBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5A30, 89 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb name set `?UnregisterBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z`; the landed
//   CBankFileAsync::SetLoadingMemoryFailureHandler 0x009D66A0 calls this on the old handler ("remove bank") before
//   RegisterBank 0x009D6420 (landed) on the new one; the other caller is 0x009D653B. Catalog label
//   "CBankFile::PopLoadErrorQueue" is wrong (label correction).
// Callees: CCriticalSectionLock(_RTL_CRITICAL_SECTION&, bool) @ 0x00A62920 (PDB spelling), ~CCriticalSectionLock @ 0x00A629A0,
//   the 4-byte find @ 0x009D6E80 (landed as Find_009d6e80, __fastcall, tag by reference), memmove @ 0x00BFEAE6.
// Layout: the landed siblings' retail model: Banks (12-byte STLport vector) +0x04, CriticalSection +0x10.
// Shape: lock first; find(first, last, bank) with the iterator tag homed in the dead `bank` slot; STLport
//   vector::erase(pos): `if (pos + 1 != end()) memmove(pos, pos + 1, bytes); --_M_finish;`.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
struct CCriticalSectionLock {                      // 8 bytes on the stack
    _RTL_CRITICAL_SECTION* CriticalSection;
    bool                   Locked;
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                       // 0x00A629A0
};
class CBankFileAsync;

struct CIteratorTag { CIteratorTag() { } };   // user ctor: no zero store, the tag is homed in the dead `bank` slot
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val, const random_access_iterator_tag&);   // 0x009D6E80
template <class _Tp> inline random_access_iterator_tag __iterator_category(_Tp* const&) { return random_access_iterator_tag(); }
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, __iterator_category(__first));
}
}

template <class T> struct CRetailArray {           // retail CArray: {first, last, end}
    T* first;
    T* last;
    T* end_of_storage;
    T* begin() { return first; }
    T* end() { return last; }
    T* erase(T* position)
    {
        T* next = position + 1;
        if (next != last)
            memmove(position, next, (const char*)last - (const char*)next);
        --last;
        return position;
    }
};
class CFailedAllocationHandler { public:                  // vtable 0x0129B874
    virtual bool HandleFailedAllocation(unsigned long size);
};
class CLoadingMemoryFailureHandler : public CFailedAllocationHandler { public:   // vtable 0x0129B8C4
    CRetailArray<CBankFileAsync*> Banks;           // +0x04
    _RTL_CRITICAL_SECTION         CriticalSection; // +0x10
    void UnregisterBank(CBankFileAsync* bank);
};

void CLoadingMemoryFailureHandler::UnregisterBank(CBankFileAsync* bank)
{
    CCriticalSectionLock lock(CriticalSection, false);
    CBankFileAsync** it = std::find(Banks.begin(), Banks.end(), bank);
    if (it != Banks.end())
        Banks.erase(it);
}
