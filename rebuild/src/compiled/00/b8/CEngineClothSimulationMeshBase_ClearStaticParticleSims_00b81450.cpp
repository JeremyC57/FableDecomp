// CEngineClothSimulationMeshBase::ClearStaticParticleSims() -- drop every shared particle simulation from the static pool whose render data is gone or no longer busy
// Leaf: ?ClearStaticParticleSims@CEngineClothSimulationMeshBase@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B81450, 85 bytes. Static __fastcall (no arguments), plain `ret`.
// Callees: vector<CCountedPointer<CParticleSimulation> >::erase(iterator) @ 0x00B835D0 (__fastcall on the static pool
//   0x014378B8), CParticleSimulationRenderData::IsBusy() (virtual slot 1).
// Identity: FableWin.pdb `?ClearStaticParticleSims@CEngineClothSimulationMeshBase@@SIXXZ` (public static __fastcall) and
//   the static member `CEngineClothSimulationMeshBase::StaticParticleSims` (a dynamic initializer / atexit pair) at
//   0x014378B8; the manifest label matches.
// Shape: `for (i = 0; i < StaticParticleSims.size(); ++i) if (!sim->IsRenderDataBusy())
//   { StaticParticleSims.erase(StaticParticleSims.begin() + i); --i; }` -- the size is re-read every pass, the start is
//   re-read after the virtual call. The busy test is a bool-returning inline (`RenderData.Get() != 0 &&
//   RenderData->IsBusy()`, descriptive name): written inline in the loop condition VC7.1 keeps the element pointer in
//   edx and the vtable in eax (85v85 register-only diff); through the bool helper it uses ecx / edx as retail does.
//   CParticleSimulation keeps a vptr at +0 and RenderData (CCountedPointer<CParticleSimulationRenderData>, Data +4 /
//   Info +8) next. The generated engine headers are flattened data-only
//   records, so the polymorphic render data and the STLport containers are modelled locally.
class CCPPointerInfo;

template <class T> class CCountedPointer {             // retail 8 bytes
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    T* Get() const { return Data; }
    T* operator->() const { return Data; }
};

class CParticleSimulationRenderData {                   // vtable 0x012A56B0
public:
    virtual ~CParticleSimulationRenderData();           // slot 0
    virtual bool IsBusy() const;                        // slot 1
};

class CParticleSimulation {                             // vtable: deleting destructor at slot 0
public:
    virtual ~CParticleSimulation();
    CCountedPointer<CParticleSimulationRenderData> RenderData;   // +0x04
    bool IsRenderDataBusy() const { return RenderData.Get() != 0 && RenderData->IsBusy(); }
};

namespace _STL {
template <class _Tp> class vector {                     // STLport 12-byte vector
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    iterator begin() { return _M_start; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
    iterator erase(iterator __position);                // 0x00B835D0
};
}

template <class T> class CArray : public _STL::vector<T> {
};

class CEngineClothSimulationMeshBase {
public:
    static CArray<CCountedPointer<CParticleSimulation> > StaticParticleSims;   // 0x014378B8
    static void __fastcall ClearStaticParticleSims();
};

void __fastcall CEngineClothSimulationMeshBase::ClearStaticParticleSims()
{
    for (unsigned long i = 0; i < StaticParticleSims.size(); ++i) {
        if (!StaticParticleSims[i]->IsRenderDataBusy()) {
            StaticParticleSims.erase(StaticParticleSims.begin() + i);
            --i;
        }
    }
}