// CEnginePrimitiveSimulationInfo::FindSimulation(unsigned long index) -- the simulation stored at index, or 0 past the end of the array
// Leaf: ?FindSimulation@CEnginePrimitiveSimulationInfo@@QAEPAVCEngineSimulationBase@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B812C0, 33 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?FindSimulation@CEnginePrimitiveSimulationInfo@@QAEPAVCEngineSimulationBase@@K@Z; this+8 is
//   Simulations, CArray<CCountedPointer<CEngineSimulationBase> > (8-byte elements: `sar 3`, `[ecx+eax*8]`), the
//   member the constructor 0x00B81E00 and ClearSimulations 0x00B81E20 work on; unsigned compare (`jae`) = size_type.
// Shape: `if (index < Simulations.size()) return Simulations[index].Data; return 0;` with STLport operator[] as
//   *(begin() + n) (begin re-read after the size test).
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
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

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const;
    mutable unsigned long IVRefCount;                // +0x04
};

class CEnginePrimitiveSimulationInfo : public CIVCountedPointeeBase {
public:
    virtual ~CEnginePrimitiveSimulationInfo();
    CEngineSimulationBase* FindSimulation(unsigned long index);
    CArray<CCountedPointer<CEngineSimulationBase> > Simulations;   // +0x08
};

CEngineSimulationBase* CEnginePrimitiveSimulationInfo::FindSimulation(unsigned long index)
{
    if (index < Simulations.size())
        return Simulations[index].Data;
    return 0;
}