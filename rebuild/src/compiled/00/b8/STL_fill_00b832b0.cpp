// _STL::fill(CCountedPointer<CParticleSimulation>* first, CCountedPointer<CParticleSimulation>* last, const CCountedPointer<CParticleSimulation>& val) -- assign val to every counted pointer of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B832B0, 94 bytes. __fastcall (first ecx, last edx), `ret 4` (&val on
//   the stack); last is spilled to the `push ecx` slot across the deleter call.
// Callees: the count block's DeleteFunc (CCPPointerInfo +4, data in ecx), operator delete @ 0x00BFE9BC.
// Callers: vector<CCountedPointer<...> >::_M_fill_insert 0x00B83B00 (call 0x00B83BE1) in the
//   CEngineClothSimulationMeshBase TU, whose counted-pointer vectors are CArray<CCountedPointer<CParticleSimulation> >
//   (ParticleSims, StaticParticleSims); the body is the same for any T.
// Identity: the manifest label "_Copy_backward<CCountedPointer<C3DGroup2>*,...>" is propagated and wrong (a fill, one
//   source element); STLport `fill(_ForwardIter, _ForwardIter, const _Tp&)`.
// Shape: `for (; first != last; ++first) *first = val;` with CCountedPointer::operator= inline in the landed 0x00AD1FC0
//   form: the source Data / Info are read into locals first (ebx / edi), and only when the blocks differ the old block is
//   released (`--RefCount` with the member re-read for the zero test, DeleteFunc(Info->Data), `delete Info` re-reading
//   Info), then the pair is stored and the new block's count bumped.
void __cdecl operator delete(void* block);              // 0x00BFE9BC

struct CCPPointerInfo {
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CParticleSimulation;

namespace _STL {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}

template void __fastcall fill<CCountedPointer<CParticleSimulation>*, CCountedPointer<CParticleSimulation> >(
    CCountedPointer<CParticleSimulation>*, CCountedPointer<CParticleSimulation>*, const CCountedPointer<CParticleSimulation>&);
}