// NHeroInformationScreens::CManager::InitialiseScrollBarIndices(long count) -- reset ScrollBarIndices to 0, 1, ..., count-1
// Leaf: ?InitialiseScrollBarIndices@CManager@NHeroInformationScreens@@QAEXJ@Z
// Retail: Fable.exe @ 0x0066EA85, 48 bytes, `ret 4`; Ego_r.pdb ?InitialiseScrollBarIndices@CManager@NHeroInformationScreens@@QAEXJ@Z;
// four callers in the NHeroInformationScreens::CManager block (0x0066EB04 .. 0x0066EC04). Callees are the STLport
// vector<long> members erase(first, last) (0x00440590, via clear()) and resize(n) (0x004433F0, which forwards to
// resize(n, long())); then ScrollBarIndices[i] = i, re-reading _M_start every pass.
// Layout: retail NHeroInformationScreens::CManager -- World +0, Screens +4, InventoryScreens +0x10, ScrollBarIndices +0x1c
// (12-byte STLport vectors; Ego_r.pdb has 16-byte vectors, ScrollBarIndices +0x24); engine/CManager.h is the
// unrelated 0x100-byte CManager.
// Opt: size TU (unaligned entry; clear() is inlined but erase/resize stay calls).
#pragma optimize("s", on)
class CWorld;
namespace NHeroInformationScreens {
class CBase;
template <class T> class CVector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    T* erase(T* first, T* last);                         // 0x00440590 for long
    void resize(unsigned int new_size);                  // 0x004433F0 for long
    void clear() { erase(_M_start, _M_finish); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
class CManager {
public:
    CWorld*               World;                         // +0x00
    CVector<const CBase*> Screens;                       // +0x04
    CVector<const CBase*> InventoryScreens;              // +0x10
    CVector<long>         ScrollBarIndices;              // +0x1c
    void InitialiseScrollBarIndices(long count);
};
void CManager::InitialiseScrollBarIndices(long count)
{
    ScrollBarIndices.clear();
    ScrollBarIndices.resize(count);
    for (long i = 0; i < count; ++i)
        ScrollBarIndices[i] = i;
}
}