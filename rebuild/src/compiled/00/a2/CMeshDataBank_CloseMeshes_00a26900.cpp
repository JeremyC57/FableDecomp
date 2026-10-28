// CMeshDataBank::CloseMeshes() -- empty the entry table and the four mesh / physics / animation / dynamic info buffers, and forget the bank file name
// Leaf: ?CloseMeshes@CMeshDataBank@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A26900, 185 bytes, plain `ret`. Callers: CMeshDataBank::Close 0x00A26C50 (its
//   first call) and 0x00A2735B. The catalog label "CMeshDataBank_Close" names the caller.
// Identity: Ego_r.pdb `?CloseMeshes@CMeshDataBank@@AAEXXZ` (private). Retail layout from the PDB order with 12-byte
//   STLport arrays: Filename (CCharString) +0x16C, MeshesAndAnimations +0x380, MeshInfoBuffer +0x38C,
//   PhysicsMeshInfoBuffer +0x398, AnimationInfoBuffer +0x3A4, DynamicInfoBuffer (CCountedPointer<CMeshDataBankEntry>)
//   +0x3B0; the four CResourceBanks (0x7C each) and four counted pointers fill +0x170..+0x380.
// Callees: vector<C3DMeshInfo/CPhysicsMeshInfo/C3DAnimationInfo>::erase(first, last) 0x00A2C910 / 0x00A2CA70 /
//   0x00A2CBD0 (out of line), STLport __copy 0x00A2B340 and __destroy_aux 0x00A2BD20 (__fastcall; manifest labels
//   are identical-body folds), memmove 0x00BFEAE6, CCharString::operator=(const char*) 0x0099EFE0 ("" at 0x0122D70E).
// Shape: every `clear()` is `erase(begin(), end())`. For the pointer vector, __copy_trivial compares _M_finish with
//   itself (`cmp ecx,ecx`, kept by VC7.1). The counted-pointer erase is inline: __copy(last, finish, first, tag, 0),
//   then __destroy_aux(i, finish, __false_type), both tags in the same frame byte. The three info-buffer erases are
//   declared as explicit specializations so they stay out of line, as in retail.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6

class CCharString {
public:
    void* PStringData;
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};

class CMeshDataBankEntry;
class C3DMeshInfo { public: unsigned char _body[0x60]; };
class CPhysicsMeshInfo { public: unsigned char _body[0x10]; };
class C3DAnimationInfo { public: unsigned char _body[0x28]; };
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer();
};

namespace std {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*);   // 0x00A2B340
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&) { return (_Tp*)__copy_trivial(__first, __last, __result); }
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&) { return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00A2BD20
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) {}
template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, typename __type_traits<_Tp>::has_trivial_destructor()); }

template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {      // retail STLport vector, 12 bytes
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
// the three info-buffer erases are out of line in retail
template <> C3DMeshInfo* vector<C3DMeshInfo>::erase(C3DMeshInfo* __first, C3DMeshInfo* __last);                 // 0x00A2C910
template <> CPhysicsMeshInfo* vector<CPhysicsMeshInfo>::erase(CPhysicsMeshInfo* __first, CPhysicsMeshInfo* __last);   // 0x00A2CA70
template <> C3DAnimationInfo* vector<C3DAnimationInfo>::erase(C3DAnimationInfo* __first, C3DAnimationInfo* __last);   // 0x00A2CBD0
}

class CMeshDataBank {
public:
    unsigned char _base[0x16C];
    CCharString Filename;                                              // +0x16C
    unsigned char _banks[0x210];
    std::vector<CMeshDataBankEntry*>                     MeshesAndAnimations;     // +0x380
    std::vector<C3DMeshInfo>                             MeshInfoBuffer;          // +0x38C
    std::vector<CPhysicsMeshInfo>                        PhysicsMeshInfoBuffer;   // +0x398
    std::vector<C3DAnimationInfo>                        AnimationInfoBuffer;     // +0x3A4
    std::vector<CCountedPointer<CMeshDataBankEntry> >    DynamicInfoBuffer;       // +0x3B0
private:
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