// CUserProfileManager::IsTutorialCategoryGiven(ETutorialCategory) -- true when the category is already in GivenTutorialCategories
// Leaf: ?IsTutorialCategoryGiven@CUserProfileManager@@QBE_NW4ETutorialCategory@@@Z
// Retail: Fable.exe @ 0x00407330, 43 bytes, `ret 4`. Callee: std::__find 0x0040E170 (STLport random-access
// find, __fastcall: first ecx, last edx, val and the iterator tag by reference on the stack; the tag
// temporary is homed in the dead argument slot, so both pushes are &category).
// Identity: Ego_r.pdb / FableWin.pdb `?IsTutorialCategoryGiven@CUserProfileManager@@QBE_NW4ETutorialCategory@@@Z`;
// callers ProcessPendingTutorials 0x0040BB30 (landed, names this address) and 0x0062A5F4. The catalog label
// CHitParameters::IsDivineWrathStrike is propagated across a byte-similar body.
// Layout: GivenTutorialCategories is Ego_r +0x98; retail +0x8C (three STLport vectors of 12 bytes before it,
// the donor's Dinkumware vectors are 16): _M_start +0x8C, _M_finish +0x90.
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE = 0 };

namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
template <class _Tp> class allocator { public: };
#pragma auto_inline(off)
template <class RandomAccessIter, class T>
__declspec(noinline) RandomAccessIter __fastcall __find(RandomAccessIter first, RandomAccessIter last, const T& val, const random_access_iterator_tag&)   // 0x0040E170
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

class CUserProfileManager {
public:
    virtual ~CUserProfileManager();
    unsigned char _pad04[0x88];
    std::vector<ETutorialCategory> GivenTutorialCategories;      // +0x8C
    bool IsTutorialCategoryGiven(ETutorialCategory category) const;
};

bool CUserProfileManager::IsTutorialCategoryGiven(ETutorialCategory category) const
{
    return std::find(GivenTutorialCategories.begin(), GivenTutorialCategories.end(), category) != GivenTutorialCategories.end();
}