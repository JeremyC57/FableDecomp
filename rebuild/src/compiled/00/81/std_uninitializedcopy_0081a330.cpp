// std::__uninitialized_copy(pair<unsigned long, CSkeletalMorphs::CEntry>* first, pair<...>* last, pair<...>* result, const __false_type&) -- copy-construct [first, last) into raw storage at result, return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@KVCEntry@CSkeletalMorphs@@@std@@PAU12@@std@@YIPAU?$pair@KVCEntry@CSkeletalMorphs@@@0@PAU10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081a330, 53 bytes, /Gr `__fastcall` (first ecx, last edx, result and the tag on the stack), `ret 8`.
// Identity (proven): callee of vector<pair<unsigned long, CSkeletalMorphs::CEntry> >::_M_fill_insert 0x0081CE50 (family L059, proven through resize 0x0081D9F0's binding) at 0x0081CED0 / 0x0081CF3B, and of its _M_insert_overflow 0x0081CA90; 12-byte stride.
// Body: STLport 4.6 `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` -- the placement new
//   null-tests the destination; pair's user copy constructor copies the key and the 8-byte value through &first->second
//   (`lea esi,[ecx+4]`).
// Shape (family L070): byte twin of the landed 0x00673C60 (pair<unsigned long, CTextureMorph>).
class CDefString {                                         // Ego_r.pdb, 4 bytes
public:
    long TablePos;                                        // +0x00
};
class CSkeletalMorphs {
public:
    class CEntry {                                         // Ego_r.pdb, 8 bytes
    public:
        CDefString MorphName;                             // +0x00
        short Priority;                                   // +0x04
        bool SupressesLowerMorphs;                        // +0x06
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6's user copy constructor
};
}
inline void* operator new(unsigned int, void* __p) { return __p; }
namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new ((void*)__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template pair<unsigned long, CSkeletalMorphs::CEntry>* __fastcall __uninitialized_copy(pair<unsigned long, CSkeletalMorphs::CEntry>*, pair<unsigned long, CSkeletalMorphs::CEntry>*, pair<unsigned long, CSkeletalMorphs::CEntry>*, const __false_type&);
}   // namespace std