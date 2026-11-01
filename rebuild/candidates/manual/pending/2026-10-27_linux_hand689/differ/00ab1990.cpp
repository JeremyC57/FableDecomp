// C3DAnimationBuilder::BuildBoneInfoTree(const C3DAnimationSequence* sequences, unsigned long noSequences) -- build the bone tree of an animation from its sequences (one CBoneInfo per sequence, named from the sequence data, parented through ParentIndex) and return the first root
// Grade: DIFFER(812v812) -- inlined __uninitialized_copy loop in the expanded bones push_back overflow addresses [edx+eax] where retail has [eax+edx] (3 instructions, operand order only); 20 probes
// Leaf: ?BuildBoneInfoTree@C3DAnimationBuilder@@AAE?AV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PBVC3DAnimationSequence@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB1990, 812 bytes, thiscall (`this` unused), hidden CCountedPointer result, `ret 0Ch`.
// Identity: Ego_r.pdb ?BuildBoneInfoTree@C3DAnimationBuilder@@AAE?AV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PBVC3DAnimationSequence@@K@Z
//   (the sequence-array overload: the second argument is indexed as an array of 8-byte C3DAnimationSequence records --
//   Data +0 (C3DAnimationSequenceData*, whose +0 is the `char* BoneName`), ParentIndex +4 (short, `movsx`) -- and the third
//   is the unsigned count the loop runs to); sole caller C3DAnimationBuilder::BuildMeshAnimation 0x00AB2520 (Ego_r
//   ?BuildMeshAnimation@C3DAnimationBuilder@@QAEKAAVCOutputAnimation@N3DAnimationBuilder@@PBVC3DAnimationSequence@@KABVC3DMesh2@@_NAAVCSimpleMemoryPool@@@Z,
//   which passes its own sequence pointer and count) at 0x00AB2565. The landed sibling 0x00AB1F30 is the sequence-chunk
//   overload; the catalog's "BuildEntityScriptBindings_FromNameArray" is a propagated stand-in.
// Callees: operator new 0x00BFEA1A, CCharString::CCharString() 0x0099E4B0, CCharString(const char*, long) 0x0099EBF0,
//   CCharString(const CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0, CCharString::operator=(const CCharString&) 0x0099EFB0,
//   vector<CCountedPointer<CBoneInfo>>::_M_insert_overflow 0x00AB3300 (landed; two out-of-line sites, the bones site inlined),
//   malloc 0x00BFEA0E, std::__destroy_aux<CCountedPointer<CBoneInfo>*> 0x00AB2BD0, free 0x00BFEA14, operator delete 0x00BFE9BC;
//   the count block's deleter is CCountedPointer<CBoneInfo>::DeleteData 0x00AB3640.
// Shape: both bone arrays are locals here (roots, then bones: the result is roots[0], then bones and roots are destroyed);
//   the bone name is `BoneName ? CCharString(BoneName) : CCharString("")` -- two conditionally built temporaries (VC7.1's
//   flag byte at [esp+10h]) copied into the ternary's result temporary before operator=; the rest is the landed sibling's
//   recipe (counted pointer local from `new CBoneInfo`, the parent read into a named CBoneInfo*, same-TU noinline
//   __destroy_aux, a plain same-TU _M_insert_overflow body).
// Layout: C3DAnimationBuilder::CBoneInfo (Ego_r 0x28, retail 0x24): Name +0, Index +4, OriginalNoChildren +8, Children +0xc,
//   AnimSource +0x18, ExtensionChild +0x20, ExtensionParent +0x21. C3DAnimationSequence (Ego_r 8): Data +0, ParentIndex +4
//   (short), Shared +6; C3DAnimationSequenceData: BoneName +0 (char*).
// CCountedPointer modelled locally as in the landed sibling (engine/CCountedPointer.h builds the count block from
// `(void*)Data`, re-read after the operator new call; retail passes the constructor argument) -- header debt.
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
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E (the overflow stand-in's storage)
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace std {

struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> class allocator { public:
    _Tp* allocate(unsigned long __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned long) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
    _Tp* allocate(unsigned long __n) { return _MaybeReboundAlloc::allocate(__n); }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
// Same-TU body of the range destroyer (retail instantiates it out of line next to this body: 0x00AB2BD0).
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    while (__first != __last) {
        _Construct(&*__cur, *__first);
        ++__first;
        ++__cur;
    }
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

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
    ~vector() { _M_clear(); }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
    size_type size() const { return size_type(end() - begin()); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    const _Tp& operator[](size_type __n) const { return *(begin() + __n); }
    void _M_clear() { __destroy_aux(_M_start, _M_finish, __false_type()); _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage._M_data = __e; }
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
    // Retail expands _M_insert_overflow at the bones push_back only (the Children / roots sites call the COMDAT 0x00AB3300):
    // the lane recipe is a __forceinline copy of push_back whose growth path carries the body (GOTCHAS: 0x6276F0 / 00917130).
    __forceinline void _M_insert_overflow_expanded(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = _M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
    __forceinline void push_back_expanded(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow_expanded(_M_finish, __x, __false_type(), 1UL, true);
    }
};

// Same-TU body of vector<CCountedPointer<CBoneInfo>>::_M_insert_overflow 0x00AB3300 (landed): the STLport 4.6 text (max of the
// old size and the fill length, allocate, __uninitialized_copy of the prefix, construct x, _M_clear, _M_set). VC7.1's own
// inliner expands it at the last push_back (bones) and calls the COMDAT at the two earlier sites, as retail does.
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                                              bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = _M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };

class C3DAnimationSequenceData { public:                   // Ego_r 0x2c bytes
    char*          BoneName;                               // +0x00
    float          SamplesPerSecond;                       // +0x04
    unsigned short FrameCount;                             // +0x08
    unsigned char  _flags[2];                              // +0x0a bit fields
    unsigned char  _pad_0x0c[0x20];
    const CCharString GetBoneName() const { return BoneName ? CCharString(BoneName) : CCharString(""); }
};
class C3DAnimationSequence { public:                       // Ego_r 8 bytes
    C3DAnimationSequenceData* Data;                        // +0x00
    short                     ParentIndex;                 // +0x04
    bool                      Shared;                      // +0x06
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
    CCountedPointer<CBoneInfo> BuildBoneInfoTree(const C3DAnimationSequence* sequences, unsigned long noSequences);
};

CCountedPointer<C3DAnimationBuilder::CBoneInfo> C3DAnimationBuilder::BuildBoneInfoTree(
    const C3DAnimationSequence* sequences, unsigned long noSequences)
{
    CArray<CCountedPointer<CBoneInfo> > roots;
    CArray<CCountedPointer<CBoneInfo> > bones;
    for (unsigned long i = 0; i < noSequences; ++i) {
        CCountedPointer<CBoneInfo> bone(new CBoneInfo);
        bone->Name = sequences[i].Data->GetBoneName();
        bone->Index = i;
        bone->OriginalNoChildren = 0;
        long parent = sequences[i].ParentIndex;
        if (parent != -1) {
            CBoneInfo* parentBone = bones[parent].Get();
            parentBone->Children.push_back(bone);
            parentBone->OriginalNoChildren++;
        }
        else
            roots.push_back(bone);
        bones.push_back_expanded(bone);
    }
    return roots[0];
}
