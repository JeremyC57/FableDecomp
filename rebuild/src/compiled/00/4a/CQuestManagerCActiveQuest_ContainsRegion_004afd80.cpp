// CQuestManager::CActiveQuest::ContainsRegion(const CRegion&) -- true when the region is in the active quest's region list
// Leaf: ?ContainsRegion@CActiveQuest@CQuestManager@@QBE_NABVCRegion@@@Z
// Retail: Fable.exe @ 0x004AFD80, 41 bytes, `ret 4`. Callee: std::__find 0x004B74E0 (STLport random-access find
// over 4-byte elements, __fastcall: first ecx, last edx, &val and the iterator tag on the stack).
// Identity: Ego_r.pdb `?ContainsRegion@CActiveQuest@CQuestManager@@QBE_NABVCRegion@@@Z`; callers
// CQuestManager::IsRegionStartOfRegionLockingMode 0x004B11A0 (passes its `const CRegion&` on, on the CActiveQuest*
// that GetActiveQuestFromScriptName 0x004AF5B0 returned) and CTCInventoryMap::FindQuestsForActiveRegion
// 0x005FD0F0. The catalog label CHitParameters::IsDivineWrathStrike is propagated.
// Shape: `&region` is a `const CRegion*` temporary bound to find's `const T&`; VC homes it (and the empty tag)
// in the dead argument slot, hence `mov [esp+8],eax` and two pushes of the same address.
// Layout: Regions (vector<const CRegion*>, PDB GetRegions) begins at +0x10: _M_start +0x10, _M_finish +0x14.
// `? true : false` gives retail's bare `setne al` (the plain `!=` form adds `xor ecx,ecx ; setne cl ; mov al,cl`).
// The same-TU __find body (auto_inline off, noinline) is what lets VC home the tag in the argument slot, as retail.
class CRegion;

namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
template <class _Tp> class allocator { public: };
#pragma auto_inline(off)
template <class RandomAccessIter, class T>
__declspec(noinline) RandomAccessIter __fastcall __find(RandomAccessIter first, RandomAccessIter last, const T& val, const random_access_iterator_tag&)   // 0x004B74E0
{
    int trip_count = (last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == val) return first;
        ++first;
    case 2:
        if (*first == val) return first;
        ++first;
    case 1:
        if (*first == val) return first;
    case 0:
    default:
        return last;
    }
}
#pragma auto_inline(on)
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, random_access_iterator_tag());
}
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_iterator;
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
};
}

class CQuestManager {
public:
    class CActiveQuest {
    public:
        unsigned char _pad00[0x10];
        std::vector<const CRegion*> Regions;       // +0x10
        bool ContainsRegion(const CRegion& region) const;
    };
};

bool CQuestManager::CActiveQuest::ContainsRegion(const CRegion& region) const
{
    return std::find(Regions.begin(), Regions.end(), &region) != Regions.end() ? true : false;
}