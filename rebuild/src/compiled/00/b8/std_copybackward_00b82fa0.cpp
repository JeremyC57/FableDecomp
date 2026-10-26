// std::__copy_backward(CCountedPointer<T>* first, CCountedPointer<T>* last, CCountedPointer<T>* result, const random_access_iterator_tag&, int*) -- (CCountedPointer<CParticleSimulation>) assign last-first counted pointers back to front into the range ending at result; returns the new result
// Leaf: ??$__copy_backward@PAV?$CCountedPointer@VCParticleSimulation@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCParticleSimulation@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B82FA0, 119 bytes (+ int3 padding), `__fastcall` (first in ecx, last in edx; result,
//   the tag reference and the null distance pointer on the stack, `ret 0Ch`).
// Identity (proven): std::__copy_backward<CCountedPointer<CParticleSimulation>*, CCountedPointer<CParticleSimulation>*, int> (STLport 4.6 _algobase.h);
//   evidence: its caller 0x00B83B00 is the fill-insert under resize(n, x) 0x00B83FC0 / resize(n) 0x00B840A0, which CEngineClothSimulationStaticMesh::RunSimulation 0x00B81F30 (its vtable 0x012A3998 slot 6) and the AnimatedMesh twin 0x00B82100 run on this+0x0C = Ego_r.pdb CEngineClothSimulationMeshBase::ParticleSims, CArray<CCountedPointer<CParticleSimulation> >, sized to the primitive's ClothPrimitives count.
// Label: the manifest's "_Copy_backward<CCountedPointer<C3DGroup2>*,CCountedPointer<class_C3DGroup2>*>" is propagated and names the wrong element (family J094: five byte-identical bodies,
//   one COMDAT per pointee; retail has no identical-code folding).
// Callees: the count block's DeleteFunc (`call [Info+4]`, Data in ecx) and operator delete 0x00BFE9BC, from the inlined
//   CCountedPointer::operator= (ShareData: nothing when both share one block, else release the old block, then share).
// Body: `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`.
// Layout: CCountedPointer<T> is 8 bytes {Data +0, Info +4}; CCPPointerInfo {RefCount +0, DeleteFunc +4, Data +8} (Ego_r.pdb).
// Model: J051's counted-pointer assignment (agent953) on the real STLport signature.
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0
    void (__fastcall* DeleteFunc)(void* data);              // +4, Data in ecx
    void* Data;                                             // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0
    CCPPointerInfo* Info;                                   // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
};
class CParticleSimulation;
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
typedef CCountedPointer<CParticleSimulation > _Elem_00b82fa0;
template _Elem_00b82fa0* __fastcall __copy_backward<_Elem_00b82fa0*, _Elem_00b82fa0*, int>(_Elem_00b82fa0*, _Elem_00b82fa0*, _Elem_00b82fa0*,
                                                                    const random_access_iterator_tag&, int*);
}   // namespace std