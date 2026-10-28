// C3DAnimationBuilder::BuildBoneInfoTree(const C3DMesh2& mesh, CArray<CCountedPointer<CBoneInfo> >& bones) -- build the builder's bone tree from a mesh skeleton: one counted CBoneInfo per mesh bone (name from the bone-name table, index, original child count), linked under its parent or kept as a root, every bone also listed in `bones`; returns the first root
// Grade: DIFFER(612v612, probe) -- the two empty iterator-tag temporaries of the inline bones.clear() (__copy's random_access_iterator_tag, __destroy_aux's __false_type) are homed on the dead `bones` argument slot in ours (`lea edx,[esp+3Ch]` / `lea eax,[esp+38h]`) and on the `mesh` slot in retail ([esp+38h] / [esp+34h]); every other instruction identical
// Leaf: ?BuildBoneInfoTree@C3DAnimationBuilder@@AAE?AV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@ABVC3DMesh2@@AAV?$CArray@V?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB1CC0, 612 bytes, thiscall `ret 0Ch` (hidden result, mesh, bones; `this` unused).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?BuildBoneInfoTree@C3DAnimationBuilder@@AAE?AV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@ABVC3DMesh2@@AAV?$CArray@V?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@@@@Z
//   -- the overload taking (const C3DMesh2&, CArray&): retail reads the mesh's skeleton (C3DMesh2 BoneCount / Bones /
//   BoneNameIndices / BoneNameData) and the 0x24-byte retail CBoneInfo (Ego_r 40 bytes with a 16-byte CArray); it sits
//   in the C3DAnimationBuilder TU after CBoneInfo::FindChildIndex 0x00AB15F0 and the constructor 0x00AB1660; caller
//   0x00AB254F. The catalog's "BuildEntityScriptBindings_FromVertexIndices" is a placeholder.
// Callees: std::__copy<CCountedPointer<CBoneInfo>*> 0x00AB1480 and std::__destroy_aux 0x00AB2BD0 (both __fastcall; the
//   inline bones.clear()), operator new 0x00BFEA1A, CCharString() 0x0099E4B0 / CCharString(const char*, long) 0x0099EBF0 /
//   operator= 0x0099EFB0 / ~CCharString 0x0099EAE0, vector<CCountedPointer<CBoneInfo> >::_M_insert_overflow 0x00AB3300,
//   operator delete 0x00BFE9BC, free 0x00BFEA14; CCountedPointer<CBoneInfo>::DeleteData 0x00AB3640 (stored in the
//   pointer info).
// Shape: _M_insert_overflow gets a same-TU noinline body (its retail COMDAT 0x00AB3300 sits in this TU): with only a
//   declaration VC7.1 treats the loop's counted pointer as escaped and reloads Data / Info from the frame after every
//   call (0x24 frame, +59 bytes); with the body it keeps them in edi / ebp as retail. CCountedPointer(T*) initialises
//   Info with `data ? new CCPPointerInfo(...) : 0` (an `if` in the body stores a 0 first); the bone name is assigned
//   through the implicit `const char*` conversion (an explicit CCharString(...) temporary passes the constructor's eax).
// Tried (4 grades): i at function scope (before / after `roots`) leaves the tag slots unchanged.
// Layout: C3DMesh2 BoneCount +0x98, Bones +0x9c (60-byte C3DBone2: Parent +4, OriginalNoChildren +8), BoneNameIndices
//   +0xa4, BoneNameData +0xa8 (retail = Ego_r.pdb - 8); no engine header for C3DMesh2 / CBoneInfo (local models).
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);          // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
void* __cdecl operator new(size_t size);               // 0x00BFEA1A
void __cdecl operator delete(void* p);                 // 0x00BFE9BC
inline void* __cdecl operator new(size_t, void* where) { return where; }

class CCharString {
public:
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

class CCPPointerInfo {                                 // Ego_r.pdb, 12 bytes
public:
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void* Data;                                        // +0x08
    CCPPointerInfo(unsigned long ref_count, void (__fastcall* delete_func)(void*), void* data)
        : RefCount(ref_count), DeleteFunc(delete_func), Data(data) { }
};

template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data), Info(data ? new CCPPointerInfo(1, DeleteData, data) : 0) { }
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
    static void __fastcall DeleteData(void* data);     // 0x00AB3640 (C3DAnimationBuilder::CBoneInfo)
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x00AB1480 (CCountedPointer<CBoneInfo>)
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00AB2BD0
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result)
{
    for (; __first != __last; ++__first, ++__result)
        _Construct(&*__result, *__first);
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    for (; __n > 0; --__n, ++__first)
        _Construct(&*__first, __x);
    return __first;
}
template <class _Tp> class allocator {
public:
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, size_t) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                     // +0x00
    _Tp* _M_finish;                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
protected:
    void _M_clear() { this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    // 0x00AB3300 (CCountedPointer<CBoneInfo>): same-TU noinline body of the retail COMDAT (STLport 4.6 _vector.c), so
    // VC7.1 sees the pushed counted pointer's address does not escape through it.
    __declspec(noinline) void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                                 size_type __fill_len, bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start);
        if (__fill_len == 1) {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish);
        _Destroy(this->_M_start, this->_M_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
}   // namespace std

template <class T> class CArray : public std::vector<T> {
public:
    T& operator[](unsigned long index) { return *(this->_M_start + index); }
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            std::_Construct(this->_M_finish, x);
            ++this->_M_finish;
        }
        else
            this->_M_insert_overflow(this->_M_finish, x, std::__false_type(), 1UL, true);
    }
};

class CMatrix3x4 { public: float M[12]; };
class C3DBone2 {                                       // Ego_r.pdb, 60 bytes
public:
    unsigned long NameCRC;                             // +0x00
    long          Parent;                              // +0x04
    long          OriginalNoChildren;                  // +0x08
    CMatrix3x4    LocalizationMatrix;                  // +0x0c
};
class C3DMesh2 {                                       // retail view (Ego_r.pdb - 8 here)
public:
    unsigned char   _pad00[0x98];
    unsigned long   BoneCount;                         // +0x98
    C3DBone2*       Bones;                             // +0x9c
    void*           BoneKeyframes;                     // +0xa0
    unsigned short* BoneNameIndices;                   // +0xa4
    char*           BoneNameData;                      // +0xa8
};

class C3DAnimationBuilder {
public:
    class CBoneInfo {                                  // retail 0x24 bytes
    public:
        CCharString                         Name;                 // +0x00
        unsigned long                       Index;                // +0x04
        unsigned long                       OriginalNoChildren;   // +0x08
        CArray<CCountedPointer<CBoneInfo> > Children;             // +0x0c
        CCountedPointer<CBoneInfo>          AnimSource;           // +0x18
        bool                                ExtensionChild;       // +0x20
        bool                                ExtensionParent;      // +0x21
        CBoneInfo() : ExtensionChild(false), ExtensionParent(false) { }
    };
private:
    CCountedPointer<CBoneInfo> BuildBoneInfoTree(const C3DMesh2& mesh, CArray<CCountedPointer<CBoneInfo> >& bones);
};

CCountedPointer<C3DAnimationBuilder::CBoneInfo> C3DAnimationBuilder::BuildBoneInfoTree(const C3DMesh2& mesh,
                                                                                        CArray<CCountedPointer<CBoneInfo> >& bones)
{
    CArray<CCountedPointer<CBoneInfo> > roots;
    bones.clear();
    for (unsigned long i = 0; i < mesh.BoneCount; ++i) {
        CCountedPointer<CBoneInfo> bone(new CBoneInfo);
        const C3DBone2& mesh_bone = mesh.Bones[i];
        bone->Name = mesh.BoneNameData + mesh.BoneNameIndices[i];
        bone->Index = i;
        bone->OriginalNoChildren = mesh_bone.OriginalNoChildren;
        long parent = mesh_bone.Parent;
        if (parent >= 0)
            bones[parent]->Children.push_back(bone);
        else
            roots.push_back(bone);
        bones.push_back(bone);
    }
    if (roots.size() == 0)
        return CCountedPointer<CBoneInfo>();
    return roots[0];
}
