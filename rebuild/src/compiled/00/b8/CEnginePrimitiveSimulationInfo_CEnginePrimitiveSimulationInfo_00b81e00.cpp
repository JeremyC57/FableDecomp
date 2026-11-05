// CEnginePrimitiveSimulationInfo::CEnginePrimitiveSimulationInfo() -- intrusive count 1, vtable, empty Simulations array
// Leaf: ??0CEnginePrimitiveSimulationInfo@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
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

#include "engine/CArray.h"

#include "engine/CCountedPointer.h"
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