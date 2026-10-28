// CEnginePrimitiveSimulationInfo::CEnginePrimitiveSimulationInfo() -- intrusive count 1, vtable, empty Simulations array
// Leaf: ??0CEnginePrimitiveSimulationInfo@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B81E00, 27 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x012A398C (RTTI .?AVCEnginePrimitiveSimulationInfo@@ : CIVCountedPointeeBase);
//   Ego_r.pdb ??0CEnginePrimitiveSimulationInfo@@QAE@XZ; the landed caller
//   CEngineInternalPrimitiveStaticMesh::InitialiseSimulation 0x00B9DE90 already binds 0x00B81E00 to this constructor.
// Layout: CIVCountedPointeeBase (vptr +0, IVRefCount +4), Simulations CArray<CCountedPointer<CEngineSimulationBase> > +8
//   (the 12-byte STLport vector; Ego_r.pdb's 16-byte one makes the PDB size 24, retail is 20).
// Shape: the base's inline constructor sets the count to 1; its own vptr store is dead and dropped, leaving the derived
//   one; the STLport _Vector_base constructor zeroes start/finish/end-of-storage.
class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const;
    mutable unsigned long IVRefCount;                // +0x04
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base();
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}

template <class T> class CArray : public std::vector<T> { };
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
class CEngineSimulationBase;

class CEnginePrimitiveSimulationInfo : public CIVCountedPointeeBase {
public:
    CEnginePrimitiveSimulationInfo();
    virtual ~CEnginePrimitiveSimulationInfo();
    CArray<CCountedPointer<CEngineSimulationBase> > Simulations;   // +0x08
};

CEnginePrimitiveSimulationInfo::CEnginePrimitiveSimulationInfo()
{
}