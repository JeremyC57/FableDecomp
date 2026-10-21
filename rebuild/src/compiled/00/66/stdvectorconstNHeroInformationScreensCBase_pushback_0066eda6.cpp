// std::vector<const NHeroInformationScreens::CBase*>::push_back(const NHeroInformationScreens::CBase* const&) -- append one pointer, reallocating when full
// Leaf: ?push_back@
// Retail: Fable.exe @ 0x0066EDA6, 49 bytes, `ret 4`; callers NHeroInformationScreens::CManager::GetAllRegisteredScreens
// (0x0066E974) and GetAllRegisteredInventoryScreens (0x0066EA32), whose Ego_r.pdb signatures take
// vector<const NHeroInformationScreens::CBase*>&. Catalog label std::vector::InsertValue is descriptive only: this is
// STLport push_back -- finish != end_of_storage: construct *finish = x (null-checked placement new), ++finish; else
// _M_insert_overflow(finish, x, __true_type(), 1, true) (0x0066ED1D).
// Layout: STLport vector {_M_start +0, _M_finish +4, _M_end_of_storage +8}.
// Opt: size TU with frame pointers (`push ebp / mov ebp,esp`, unaligned entry; the named __true_type tag local lives in a dead
// argument byte, `lea edx,[ebp+0Bh]`; a `__true_type()` temporary is zeroed with stosb instead).
#pragma optimize("s", on)
#pragma optimize("y", off)
#include <new>
namespace NHeroInformationScreens { class CBase; }
namespace std {
struct __true_type {};
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend);  // 0x0066ED1D
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else {
            __true_type is_pod;
            _M_insert_overflow(_M_finish, x, is_pod, 1, true);
        }
    }
};
}
template class std::vector<const NHeroInformationScreens::CBase*>;