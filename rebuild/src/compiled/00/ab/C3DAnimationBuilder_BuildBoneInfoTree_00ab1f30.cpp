// C3DAnimationBuilder::BuildBoneInfoTree(const CArray<CCountedPointer<C3DAnimFileSequenceChunk>>& sequences, CArray<CCountedPointer<CBoneInfo>>& bones) -- rebuild the bone list from the animation file's sequence chunks (one CBoneInfo per chunk, named and indexed, parented through the chunk's parent index with the parent's child count bumped, parentless ones collected as roots) and return the first root
// Leaf: ?BuildBoneInfoTree@C3DAnimationBuilder@@AAE?AV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@ABV?$CArray@V?$CCountedPointer@VC3DAnimFileSequenceChunk@@@@@@AAV?$CArray@V?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB1F30, 556 bytes, thiscall (`this` unused), hidden CCountedPointer result,
//   `ret 0Ch`.
// Identity: Ego_r.pdb `?BuildBoneInfoTree@C3DAnimationBuilder@@AAE?AV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@ABV?$CArray@V?$CCountedPointer@VC3DAnimFileSequenceChunk@@@@@@AAV?$CArray@V?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@@@@Z`
//   (the sequence-chunk overload; the C3DMesh2 / C3DAnimationSequence overloads are its siblings). Callers: the two sites
//   0x00AB21B4 / 0x00AB21CD of C3DAnimationBuilder::GetIntersection 0x00AB2160 (`?GetIntersection@C3DAnimationBuilder@@QAEKAAV?$CArray@V?$CCountedPointer@VC3DAnimFileSequenceChunk@@@@@@0@Z`),
//   which pass its two sequence arrays. The catalog's "_global::GetIntersection_CollectEntities" is a descriptive stand-in.
// Callees: STLport std::__copy(random access, (ptrdiff_t*)0) 0x00AB1480 and std::__destroy_aux 0x00AB2BD0 (the inlined
//   erase(begin(), end()) of CArray::clear and the roots destructor; the catalog's `_Move<...CGuiVarTransferBase>` /
//   `_Destroy_range<...CEntityScriptBindingBase>` are propagated COMDAT twins), operator new 0x00BFEA1A, CCharString::CCharString()
//   0x0099E4B0, CCharString(const CCharString&) 0x0099EC30, CCharString::operator=(const CCharString&) 0x0099EFB0,
//   ~CCharString 0x0099EAE0, vector<CCountedPointer<CBoneInfo>>::_M_insert_overflow 0x00AB3300 (landed), operator delete
//   0x00BFE9BC, free 0x00BFEA14; the count block's deleter is CCountedPointer<CBoneInfo>::DeleteData 0x00AB3640 (the catalog's
//   "CTCMapwho::OnDie" is propagated: `delete static_cast<CBoneInfo*>(data)`, ~CBoneInfo 0x00AB34C0).
// Layout: C3DAnimationBuilder::CBoneInfo (Ego_r 0x28, retail 0x24: its CArray is 12 bytes): Name +0, Index +4,
//   OriginalNoChildren +8, Children +0xc, AnimSource +0x18, ExtensionChild +0x20, ExtensionParent +0x21. Retail
//   C3DAnimFileSequenceChunk (Ego_r minus 8: the super chunk's two CArrays): BoneIndex +0x2c, ParentIndex +0x30, BoneName
//   +0x34 (the inline accessors GetParentIndex / GetBoneName -- the latter returns `const CCharString` by value, hence the
//   temporary copy before operator=).
// Shape: `roots` is declared before `bones.clear()` (its zero stores interleave with the __copy call setup); the bone
//   pointer is a CCountedPointer local built from `new CBoneInfo` (count block stores RefCount 1, DeleteData, Data behind
//   the operator-new null test), its destructor inlined at the end of each iteration (trailing zero stores kept); the
//   parent bone is read once into a named CBoneInfo* (an unnamed `bones[parent]->` is re-read after the push_back);
//   every STLport tag temporary is homed in the sequences argument slot, which needs the same-TU noinline __copy /
//   __destroy_aux bodies (bare declarations give the tag a frame byte). The counted pointer's Data / Info stay in esi / edi
//   across the CCharString calls only when the one callee that receives the local's ADDRESS (_M_insert_overflow 0x00AB3300)
//   has a same-TU body that merely reads it (the landed 0x00848730 idiom, as retail's LTCG knew): with a bare declaration
//   the local is address-taken and reloaded from its frame words after every call (619v556). Same-TU bodies for the
//   CCharString members themselves are WRONG here: VC7.1 then uses their register facts (ecx preserved by the default
//   constructor) and keeps the new CBoneInfo in ecx instead of retail's `mov esi,eax` (566v556).
// CCountedPointer modelled locally: engine/CCountedPointer.h builds the count block from `(void*)Data` (re-read after the
// operator new call, which reloads the local); retail passes the constructor argument (`mov [eax+8],esi`) -- header debt.
#include "engine/CCountedPointer.h"

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E (the overflow stand-in's storage)
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                         // 0x0099E4B0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
// Same-TU bodies of the two range helpers (retail instantiates them out of line next to this body: 0x00AB1480 / 0x00AB2BD0).
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc> class vector {           // retail STLport 4.6 vector (12 bytes)
public:
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef unsigned long size_type;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08

    vector() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~vector()
    {
        __destroy_aux(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
    size_type size() const { return size_type(end() - begin()); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    const _Tp& operator[](size_type __n) const { return *(begin() + __n); }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend);   // 0x00AB3300
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy(__last, _M_finish, __first, random_access_iterator_tag(), (long*)0);
        __destroy_aux(__i, _M_finish, __false_type());
        _M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

#pragma auto_inline(off)
// Same-TU stand-in for vector<CCountedPointer<CBoneInfo>>::_M_insert_overflow 0x00AB3300 (landed): grow the storage, copy
// the old elements, construct x at the position; it only reads x, so VC7.1 knows the caller's pointer is not modified.
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                                              bool __atend)
{
    size_type __old = size();
    size_type __len = __old + (__old > __fill_len ? __old : __fill_len);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = _M_start; __p != __position; ++__p, ++__new_finish)
        _Construct(__new_finish, *__p);
    for (size_type __k = 0; __k < __fill_len; ++__k, ++__new_finish)
        _Construct(__new_finish, __x);
    if (!__atend)
        for (_Tp* __p = __position; __p != _M_finish; ++__p, ++__new_finish)
            _Construct(__new_finish, *__p);
    __destroy_aux(_M_start, _M_finish, __false_type());
    if (_M_start)
        free(_M_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)

}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };

class C3DAnimFileSequenceChunk { public:                   // retail: CChunkedFileSuperChunk base 0x2c bytes
    unsigned char _pad_0x00[0x2c];
    long          BoneIndex;                               // +0x2c
    long          ParentIndex;                             // +0x30
    CCharString   BoneName;                                // +0x34
    long GetParentIndex() const { return ParentIndex; }    // ?GetParentIndex@C3DAnimFileSequenceChunk@@QBEJXZ (inline)
    const CCharString GetBoneName() const { return BoneName; }   // ?GetBoneName@C3DAnimFileSequenceChunk@@QBE?BVCCharString@@XZ (inline)
};

class C3DAnimationBuilder { public:
    class CBoneInfo { public:                              // retail 0x24 bytes
        CCharString                           Name;                  // +0x00
        unsigned long                         Index;                 // +0x04
        unsigned long                         OriginalNoChildren;    // +0x08
        CArray<CCountedPointer<CBoneInfo> >   Children;              // +0x0c
        CCountedPointer<CBoneInfo>            AnimSource;            // +0x18
        bool                                  ExtensionChild;        // +0x20
        bool                                  ExtensionParent;       // +0x21
        CBoneInfo() : ExtensionChild(false), ExtensionParent(false) { }
        ~CBoneInfo();                                                // 0x00AB34C0
    };
private:                                                   // Ego_r.pdb: a private member (AAE)
    CCountedPointer<CBoneInfo> BuildBoneInfoTree(const CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& sequences,
                                                 CArray<CCountedPointer<CBoneInfo> >& bones);
};

CCountedPointer<C3DAnimationBuilder::CBoneInfo> C3DAnimationBuilder::BuildBoneInfoTree(
    const CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& sequences, CArray<CCountedPointer<CBoneInfo> >& bones)
{
    CArray<CCountedPointer<CBoneInfo> > roots;
    bones.clear();
    for (unsigned long i = 0; i < sequences.size(); ++i) {
        CCountedPointer<CBoneInfo> bone(new CBoneInfo);
        const C3DAnimFileSequenceChunk* sequence = sequences[i].Get();
        bone->Name = sequence->GetBoneName();
        bone->Index = i;
        bone->OriginalNoChildren = 0;
        long parent = sequence->GetParentIndex();
        if (parent >= 0) {
            CBoneInfo* parentBone = bones[parent].Get();
            parentBone->Children.push_back(bone);
            parentBone->OriginalNoChildren++;
        }
        else
            roots.push_back(bone);
        bones.push_back(bone);
    }
    return roots[0];
}