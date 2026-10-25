// std::vector<CDateAndTime>::resize(size_type new_size) -- STLport `resize(new_size, _Tp())`: build a default
// CDateAndTime and grow or shrink to new_size with it
// Leaf: ?resize@?$vector@VCDateAndTime@@V?$allocator@VCDateAndTime@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048623F, 33 bytes, `leave ; ret 4`. A frame-pointer size-optimised module.
// Identity: the value temporary is built by CDateAndTime::CDateAndTime (0x009E9AB0, zeroes the 7 dwords; Ego_r.pdb
// CDateAndTime is 28 bytes) and passed with new_size to resize(size_type, const CDateAndTime&) (0x004861F7: erase the
// tail (0x00411B00) or insert copies through _M_fill_insert 0x0048608C). Callers: CTCInGameMenu GetFileNameList
// (0x00480216) and CFrontEndManager GetSavedGameNames (0x0059810E), the save-file name/time lists (Ego_r.pdb
// CUserProfileManager::GetSortedSaveNamesAndTimes takes a vector<CDateAndTime>&). The earlier descriptive
// CTimedEntryArray_0048623f::ResetEntry label described the same body as an indexed store.
// Shape: `push ebp ; mov ebp,esp ; sub esp,1Ch` for the temporary, whose address (the constructor's eax) is pushed
// straight after `push [ebp+8]`. optimize("y",off) + ("s",on).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CDateAndTime {
public:
    long          Year;                                 // +0x00
    unsigned long Month;                                // +0x04
    unsigned long Day;                                  // +0x08
    unsigned long Hour;                                 // +0x0C
    unsigned long Minute;                               // +0x10
    unsigned long Second;                               // +0x14
    unsigned long Millisecond;                          // +0x18
    CDateAndTime();                                     // 0x009E9AB0
};

namespace std {
template <class _Tp> class allocator;

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x004861F7 for CDateAndTime
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CDateAndTime>::resize(unsigned int);
}
