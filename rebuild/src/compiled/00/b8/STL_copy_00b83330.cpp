// std::__copy(CCountedPointer<CParticleSimulation>* first, CCountedPointer<CParticleSimulation>* last, CCountedPointer<CParticleSimulation>* result, const random_access_iterator_tag&, int*) -- STLport random-access copy: assign [first, last) onto result through the inline counted-pointer assignment and return the advanced result
// Leaf: ??$__copy@PAV?$CCountedPointer@VCParticleSimulation@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCParticleSimulation@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B83330, length 115 bytes, `__fastcall` (first ecx, last edx; result, the
//   tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity: the landed CArray<CCountedPointer<CParticleSimulation> >::DeallocateMemory 0x00B83D30 (FableWin.pdb name) binds
//   this address as `std::__copy<CCountedPointer<CParticleSimulation>*, ...>` (vector::erase -> copy -> __copy_aux ->
//   __copy_ptrs -> this out-of-line random-access __copy); every caller (0x00B83440, 0x00B835D0 ParticleSims erase in
//   CEngineClothSimulationMeshBase::ClearStaticParticleSims, 0x00B83820, 0x00B83970, 0x00B83D30, 0x00B83FC0) sits in the
//   cloth-simulation TU over CEngineClothSimulationMeshBase::ParticleSims (Ego_r CArray<CCountedPointer<CParticleSimulation> >).
//   The catalog name Move_CountedPointer and the manifest Dinkumware `_Move<..>` were a shape name and a propagation.
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first;
//   ++__result; }`; the signed count lives in the `push ecx` frame word. Each assignment is the inline
//   CCountedPointer::operator=: read the source's info/data first (kept in edi/ebp), and when the info blocks differ release
//   the destination's (RefCount at +0, the delete function at +4 called with Data (+8) in ecx, then operator delete
//   @ 0x00BFE9BC), adopt the source's pair and bump its RefCount.
class CCPPointerInfo {                                  // retail 12 bytes (Ego_r CCPPointerInfo)
public:
    long  RefCount;                                     // +0x00
    void (__fastcall* DeleteFunc)(void*);               // +0x04 (retail passes Data in ecx)
    void* Data;                                         // +0x08
};

template <class T> class CCountedPointer {             // retail 8 bytes
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;              // the source's fields are read first
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CParticleSimulation;
typedef CCountedPointer<CParticleSimulation> CParticleSimulationPtr;

namespace std {
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}

template CParticleSimulationPtr* __fastcall std::__copy<CParticleSimulationPtr*, CParticleSimulationPtr*, int>(
    CParticleSimulationPtr*, CParticleSimulationPtr*, CParticleSimulationPtr*, const std::random_access_iterator_tag&, int*);
