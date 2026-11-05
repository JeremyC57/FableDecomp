// CEnginePrimitiveSimulationInfo::FindSimulation(unsigned long index) -- the simulation stored at index, or 0 past the end of the array
// Leaf: ?FindSimulation@CEnginePrimitiveSimulationInfo@@QAEPAVCEngineSimulationBase@@K@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B812C0, 33 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?FindSimulation@CEnginePrimitiveSimulationInfo@@QAEPAVCEngineSimulationBase@@K@Z; this+8 is
//   Simulations, CArray<CCountedPointer<CEngineSimulationBase> > (8-byte elements: `sar 3`, `[ecx+eax*8]`), the
//   member the constructor 0x00B81E00 and ClearSimulations 0x00B81E20 work on; unsigned compare (`jae`) = size_type.
// Shape: `if (index < Simulations.size()) return Simulations[index].Data; return 0;` with STLport operator[] as
//   *(begin() + n) (begin re-read after the size test).
#include "engine/CArray.h"

#include "engine/CCountedPointer.h"
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