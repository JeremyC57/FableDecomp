// std::__uninitialized_copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*>(first, last, result, const __false_type&) -- STLport 4.6 copy-construct [first, last) into raw storage at result (Data and Info copied, Info->RefCount bumped when set), return the end
// Leaf: ??$__uninitialized_copy@PAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@@std@@YIPAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB0FC0, 50 bytes, `__fastcall` (first in ecx, last in edx, result and the tag on the stack, `ret 8`).
// Identity (naming wave 21, chunk 689 NAME_FIXES): the out-of-line head / tail copy of vector<CCountedPointer<C3DAnimationBuilder::CBoneInfo> >::
//   _M_insert_overflow 0x00AB3300 (landed; binds exactly this spelling). Text: that row's STLport model (the element's own __fastcall COMDAT),
//   explicitly instantiated. Was a free `Uninit_copy` stand-in over an `Elem` record (one of 31 bodies of this shape, template 0x004477C0).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void* Data;                                             // +0x08
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer();
};

class C3DAnimationBuilder { public: class CBoneInfo; };

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template CCountedPointer<C3DAnimationBuilder::CBoneInfo>* __fastcall __uninitialized_copy(CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*,
    CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, const __false_type&);
}
