// Test for 0x00AB1F30 C3DAnimationBuilder::BuildBoneInfoTree: four sequence chunks (two roots, a chain of two children) build
// four named, indexed bones with parent child lists and counts; the stale bone list is cleared first; the first root returns.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#define __cdecl
#endif
class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0x0
    void (__fastcall* DeleteFunc)(void*);                   // +0x4
    void* Data;                                             // +0x8
    CCPPointerInfo(unsigned long refCount, void (__fastcall* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
    T* Get() const { return Data; }
    static void __fastcall DeleteData(void* data) { delete static_cast<T*>(data); }
};
#include <stdlib.h>
#include <new>

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

// ---- test doubles -------------------------------------------------------------------------------------------------------
class CCharStringData { public: char* m_pData; long m_length; long m_refCount; };
static int g_liveStrings = 0;
static CCharStringData* MakeData(const char* text)
{
    CCharStringData* d = (CCharStringData*)malloc(sizeof(CCharStringData));
    d->m_length = (long)strlen(text); d->m_pData = (char*)malloc(d->m_length + 1); strcpy(d->m_pData, text); d->m_refCount = 1;
    g_liveStrings++;
    return d;
}
static void Release(CCharStringData* d) { if (d && --d->m_refCount == 0) { free(d->m_pData); free(d); g_liveStrings--; } }
CCharString::CCharString() : PStringData(0) { }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { if (PStringData) ++PStringData->m_refCount; }
CCharString::~CCharString() { Release(PStringData); PStringData = 0; }
const CCharString& CCharString::operator=(const CCharString& other)
{
    if (other.PStringData) ++other.PStringData->m_refCount;
    Release(PStringData); PStringData = other.PStringData; return *this;
}
static int g_boneInfosDeleted = 0;
C3DAnimationBuilder::CBoneInfo::~CBoneInfo() { g_boneInfosDeleted++; }
static const char* Text(const CCharString& s) { return s.PStringData ? s.PStringData->m_pData : ""; }

int main(void)
{
    int ok = 1;
    typedef C3DAnimationBuilder::CBoneInfo CBoneInfo;
    const char* names[4] = { "Root", "Spine", "Head", "Prop" };
    long parents[4] = { -1, 0, 1, -1 };
    CArray<CCountedPointer<C3DAnimFileSequenceChunk> > sequences;
    for (int k = 0; k < 4; k++) {
        C3DAnimFileSequenceChunk* chunk = new C3DAnimFileSequenceChunk;
        memset(chunk, 0, sizeof(*chunk));
        chunk->BoneIndex = k; chunk->ParentIndex = parents[k]; chunk->BoneName.PStringData = MakeData(names[k]);
        sequences.push_back(CCountedPointer<C3DAnimFileSequenceChunk>(chunk));
    }
    CArray<CCountedPointer<CBoneInfo> > bones;
    bones.push_back(CCountedPointer<CBoneInfo>(new CBoneInfo));     // stale entry, cleared by the call
    {
        C3DAnimationBuilder builder;
        CCountedPointer<CBoneInfo> root = builder.BuildBoneInfoTree(sequences, bones);
        if (g_boneInfosDeleted != 1) ok = 0;
        if (bones.size() != 4) ok = 0;
        for (unsigned long i = 0; ok && i < 4; i++) {
            CBoneInfo* bone = bones[i].Get();
            if (!bone || strcmp(Text(bone->Name), names[i]) != 0 || bone->Index != i) ok = 0;
            if (bone->ExtensionChild || bone->ExtensionParent || bone->AnimSource.Get() != 0) ok = 0;
        }
        if (ok) {
            if (bones[0]->OriginalNoChildren != 1 || bones[0]->Children.size() != 1 || bones[0]->Children[0].Get() != bones[1].Get()) ok = 0;
            if (bones[1]->OriginalNoChildren != 1 || bones[1]->Children.size() != 1 || bones[1]->Children[0].Get() != bones[2].Get()) ok = 0;
            if (bones[2]->OriginalNoChildren != 0 || bones[2]->Children.size() != 0) ok = 0;
            if (bones[3]->OriginalNoChildren != 0 || bones[3]->Children.size() != 0) ok = 0;
            if (root.Get() != bones[0].Get() || root.Info != bones[0].Info) ok = 0;
            /* bones[0]: the list and the returned root (roots was destroyed); bones[1]: the list and bone 0's child list */
            if (bones[0].Info->RefCount != 2 || bones[1].Info->RefCount != 2 || bones[3].Info->RefCount != 1) ok = 0;
        }
    }
    bones.clear();
    if (g_boneInfosDeleted != 5) ok = 0;
    sequences.clear();
    if (g_liveStrings != 0) ok = 0;
    printf(ok ? "00ab1f30_TEST PASS\n" : "00ab1f30_TEST FAIL\n");
    return 0;
}