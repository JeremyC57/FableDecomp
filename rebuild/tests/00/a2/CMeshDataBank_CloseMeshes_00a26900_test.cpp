// Behaviour test for CMeshDataBank::CloseMeshes (0x00A26900): every table ends empty (storage kept), the counted
// pointers in the dynamic buffer are destroyed, and the file name becomes "".
#include <cstdio>
#include <cstring>

static const char* g_assigned = 0;
static int g_cpDestroyed = 0;
static int g_outOfLineErases = 0;

class CCharString {
public:
    const char* Text;
    CCharString& operator=(const char* text) { Text = text; g_assigned = text; return *this; }
};

class CMeshDataBankEntry { public: int Id; };
class C3DMeshInfo { public: int Id; };
class CPhysicsMeshInfo { public: int Id; };
class C3DAnimationInfo { public: int Id; };
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer() { ++g_cpDestroyed; }
};

namespace _STL {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)std::memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = (_Distance)(__last - __first); __n > 0; --__n, ++__result, ++__first)
        *__result = *__first;
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&) { return (_Tp*)__copy_trivial(__first, __last, __result); }
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&) { return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0); }
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first) __first->~CCountedPointer<CMeshDataBankEntry>();
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) {}
template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, typename __type_traits<_Tp>::has_trivial_destructor()); }

template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, typename __type_traits<_Tp>::has_trivial_assignment_operator());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
template <> C3DMeshInfo* vector<C3DMeshInfo>::erase(C3DMeshInfo* __first, C3DMeshInfo* __last) { ++g_outOfLineErases; if (__last == _M_finish) _M_finish = __first; return __first; }
template <> CPhysicsMeshInfo* vector<CPhysicsMeshInfo>::erase(CPhysicsMeshInfo* __first, CPhysicsMeshInfo* __last) { ++g_outOfLineErases; if (__last == _M_finish) _M_finish = __first; return __first; }
template <> C3DAnimationInfo* vector<C3DAnimationInfo>::erase(C3DAnimationInfo* __first, C3DAnimationInfo* __last) { ++g_outOfLineErases; if (__last == _M_finish) _M_finish = __first; return __first; }
}

class CMeshDataBank {
public:
    CCharString Filename;
    _STL::vector<CMeshDataBankEntry*>                     MeshesAndAnimations;
    _STL::vector<C3DMeshInfo>                             MeshInfoBuffer;
    _STL::vector<CPhysicsMeshInfo>                        PhysicsMeshInfoBuffer;
    _STL::vector<C3DAnimationInfo>                        AnimationInfoBuffer;
    _STL::vector<CCountedPointer<CMeshDataBankEntry> >    DynamicInfoBuffer;
    void CloseMeshes();
};

void CMeshDataBank::CloseMeshes()
{
    MeshesAndAnimations.clear();
    MeshInfoBuffer.clear();
    PhysicsMeshInfoBuffer.clear();
    AnimationInfoBuffer.clear();
    DynamicInfoBuffer.clear();
    Filename = "";
}

template <class T> static void Fill(_STL::vector<T>& v, T* storage, int n, int cap) { v._M_start = storage; v._M_finish = storage + n; v._M_end_of_storage._M_data = storage + cap; }

int main()
{
    int ok = 1;
    CMeshDataBankEntry e[3];
    CMeshDataBankEntry* entries[4] = { &e[0], &e[1], &e[2], 0 };
    C3DMeshInfo meshes[2]; CPhysicsMeshInfo phys[2]; C3DAnimationInfo anims[2];
    unsigned char raw[3 * sizeof(CCountedPointer<CMeshDataBankEntry>)];
    CCountedPointer<CMeshDataBankEntry>* dyn = (CCountedPointer<CMeshDataBankEntry>*)raw;
    CMeshDataBank bank;
    bank.Filename.Text = "meshes.big";
    Fill(bank.MeshesAndAnimations, entries, 3, 4);
    Fill(bank.MeshInfoBuffer, meshes, 2, 2);
    Fill(bank.PhysicsMeshInfoBuffer, phys, 1, 2);
    Fill(bank.AnimationInfoBuffer, anims, 2, 2);
    Fill(bank.DynamicInfoBuffer, dyn, 3, 3);

    bank.CloseMeshes();
    if (bank.MeshesAndAnimations._M_finish != entries || bank.MeshesAndAnimations._M_end_of_storage._M_data != entries + 4) ok = 0;
    if (bank.MeshInfoBuffer._M_finish != meshes || bank.PhysicsMeshInfoBuffer._M_finish != phys || bank.AnimationInfoBuffer._M_finish != anims) ok = 0;
    if (g_outOfLineErases != 3) ok = 0;
    if (bank.DynamicInfoBuffer._M_finish != dyn || g_cpDestroyed != 3) ok = 0;
    if (g_assigned == 0 || g_assigned[0] != 0 || bank.Filename.Text != g_assigned) ok = 0;

    if (!ok) { std::printf("00a26900_TEST FAIL\n"); return 1; }
    std::printf("00a26900_TEST PASS\n");
    return 0;
}