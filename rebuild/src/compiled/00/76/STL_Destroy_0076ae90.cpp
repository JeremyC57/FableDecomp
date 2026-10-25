// std::_Destroy(ParticleAndEndPoints* first, ParticleAndEndPoints* last) -- destroy a run of 20-byte augmentation particle records: each record's two point-name CCharStrings, the second first
// Leaf: ??$_Destroy@PAUParticleAndEndPoints@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076AE90, 41 bytes (+7 int3), plain `ret` (link-time __fastcall: first in ecx,
//   last in edx; the two-iterator `_Destroy` takes no tag argument).
// Identity: sole caller vector<ParticleAndEndPoints>::operator= 0x0076B1E0 (its `_Destroy(i, _M_finish)` of the surplus;
//   the inner vector of CTCObjectAugmentations::ParticleEffects, vector<vector<ParticleAndEndPoints> >, cleared by
//   CTCObjectAugmentations::ClearAllParticleEffects 0x007674A0). The landed vector dtor 0x0076B1A0 destroys the same
//   20-byte element (+4 then +0).
// Callees: CCharString::~CCharString 0x0099EAE0 (twice per record).
// Layout: Ego_r.pdb struct ParticleAndEndPoints (20 bytes, PeriodicFrames +0x10; the first 16 bytes are unnamed there):
//   the destructor calls show CCharStrings at +0 and +4 and POD words at +8 / +0xC (member names descriptive).
// Shape: STLport 4.x `_Destroy(first, last)` -> `__destroy(first, last, value_type)` -> `__destroy_aux(first, last,
//   __false_type())`, all inline in the out-of-line two-iterator `_Destroy`; the record's implicit destructor runs its
//   string members in reverse order.
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the element is
//   ParticleAndEndPoints.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};

struct ParticleAndEndPoints {                        // Ego_r.pdb struct, 20 bytes
    CCharString   StartPoint;                        // +0x00 (descriptive)
    CCharString   EndPoint;                          // +0x04 (descriptive)
    long          ParticleData[2];                   // +0x08 (descriptive POD words)
    long          PeriodicFrames;                    // +0x10
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator>
void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}

template void _Destroy<ParticleAndEndPoints*>(ParticleAndEndPoints*, ParticleAndEndPoints*);
}