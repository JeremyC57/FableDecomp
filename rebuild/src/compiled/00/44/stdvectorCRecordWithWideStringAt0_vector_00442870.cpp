// std::vector<CRecordWithWideStringAt0>::~vector() -- destroy each 0x14-byte record (its CWideString), then free the storage
// Leaf: ??1?$vector@
// Retail: Fable.exe @ 0x00442870, 49 bytes, plain `ret`; caller 0x00492358 (a local vector at [ebp+1Ch], filled in a
// stride-0x14 loop just before). Catalog label CDefendingCombatantInfo::CCombatWheel::ResetRings is wrong: the body is
// STLport ~vector with _Destroy inlined -- ~CWideString (0x0099B510) on each element's +0x00 from _M_start to
// _M_finish, then ~_Vector_base: `if (_M_start) free(_M_start)` (0x00BFEA14), _M_start re-read.
// Layout: STLport vector {start, finish, end_of_storage}; the element is the 0x14-byte record of 0x00440BA0 (CWideString
// at +0x00 and 16 trivially destructible bytes), named behaviourally there.
// Shape: /O2 (aligned loop head behind `mov edi,edi`); the explicit instantiation emits the destructor.
#include <stdlib.h>
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
struct CRecordWithWideStringAt0 {
    CWideString Text;                                    // +0x00
    long        Data[4];                                 // +0x04
};
namespace std {
struct __false_type {};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter>
inline void __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class T> inline void _Destroy(T* first, T* last) { __destroy_aux(first, last, __false_type()); }
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
template <class T> class vector : public _Vector_base<T> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template class vector<CRecordWithWideStringAt0>;
}