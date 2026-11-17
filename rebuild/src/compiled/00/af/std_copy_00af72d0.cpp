// std::__copy<CArray<long>*, CArray<long>*, int>(first, last, result, const random_access_iterator_tag&, int*) -- STLport random-access copy: the signed element count once, each CArray<long> assigned (`*result = *first`), the advanced result returned
// Leaf: ??$__copy@PAV?$CArray@J@@PAV1@H@std@@YIPAV?$CArray@J@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF72D0, oracle length 66 bytes. __fastcall (first in ecx, last in edx; result, the tag
//   reference and the null distance pointer on the stack, `ret 0Ch`).
// Identity (NF48, round 7): the element is CArray<long> (12 bytes, assigned through vector<long>::operator= 0x00454886, CArray's implicit
//   operator=): its landed caller vector<CArray<long> >::erase 0x00AF7910 (CParticle::PrimIDList / CDecal::PrimIDList, Ego_r.pdb
//   CArray<CArray<long> >) binds this address as this exact symbol (reloc_bindings). Name descriptive-by-library: both PDBs are Dinkumware
//   builds; the spelling is STLport 4.6.2 _algobase.h __copy as stlport/fable_containers.h declares it. Was the address-named
//   Copy_CElement12_00454886_00af72d0 over a 12-byte stand-in element whose operator= named 0x00454886 CElement12_00454886::operator=.
// Shape: SGI-STL `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;`, the shared
//   header's inline body, emitted by an explicit instantiation.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "engine/CArray.h"

template CArray<long>* __fastcall std::__copy<CArray<long>*, CArray<long>*, int>(CArray<long>*, CArray<long>*, CArray<long>*,
                                                                                 const std::random_access_iterator_tag&, int*);
