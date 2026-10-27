// std::__copy_ptrs(CQuake* first, CQuake* last, CQuake* result, const __false_type&) -- STLport assignment copy of a camera-quake range (used by vector::erase); returns the end of the destination
// Leaf: ??$__copy_ptrs@PAVCQuake@CGameCameraManager@@PAV12@@std@@YIPAVCQuake@CGameCameraManager@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe @ 0x0069C8B0, 60 bytes, /Gr `__fastcall` (first ecx, last edx; result and the empty __false_type tag on
// the stack, `ret 8`).
// Identity (probable): the only caller 0x0069CD18 is vector<CGameCameraManager::CQuake>::erase(iterator) 0x0069CD00
// (`if (pos + 1 != _M_finish) copy(pos + 1, _M_finish, pos); _M_finish -= 0x30;`) passing the tag homed in its own
// dead argument slot; the 0x30-byte element (/48 reciprocal 0x2AAAAAAB, sar 3) is the CQuake of the landed neighbours
// std::_Construct 0x0069C860 and std::__uninitialized_copy 0x0069C8F0. STLport 4.6 `__copy_ptrs(.., __false_type)` forwards to
// `__copy(.., random_access_iterator_tag(), (ptrdiff_t*)0)`, inlined here as the counted loop.
// Shape: `*__result = *__first` is CQuake's implicit assignment, a 12-dword `rep movsd` (C3DVector has a user copy ctor
// but no user operator=).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CGameCameraManager { public:
    class CQuake { public:                          // Ego_r.pdb CGameCameraManager::CQuake, 0x30 bytes
        C3DVector Position;                         // +0x00
        C3DVector Axis;                             // +0x0C
        long      Frame;                            // +0x18
        float     Amplitude;                        // +0x1C
        float     Duration;                         // +0x20
        float     Frequency;                        // +0x24
        float     Phase;                            // +0x28
        bool      Global;                           // +0x2C
    };
};
namespace std {
struct __false_type { };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                     const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CGameCameraManager::CQuake* __fastcall __copy_ptrs(CGameCameraManager::CQuake*, CGameCameraManager::CQuake*,
                                                            CGameCameraManager::CQuake*, const __false_type&);
}