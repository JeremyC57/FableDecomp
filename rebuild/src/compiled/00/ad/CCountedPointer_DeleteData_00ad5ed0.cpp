// CCountedPointer::DeleteData(void* object) -- Delete the typed pointee and its owned plain-element array.
// Leaf: ?DeleteData@?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@SIXPAX@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Own allocation00acbfc7 size40 and field stores Exists0/Unsplit1/SubContractions4/ListIterator36 bind callback00acc015; original nested type/PDB DeleteData. ListIterator unaccessed single-node-pointer projection.
// Layout: genuine original CArray/std::vector/base/allocator-proxy. All owned array elements are trivial. Original fields retained, no active pointee header.
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
namespace QuadricOptimiserInternals{class CSubContraction{public:unsigned long Index1,Index2;};class CVertexPairContraction{public:bool Exists,Unsplit;CArray<CSubContraction> SubContractions;float Weight2,Cost;float ResultingPosition[3];void* ListIterator;};}
template void __fastcall CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction>::DeleteData(void*);