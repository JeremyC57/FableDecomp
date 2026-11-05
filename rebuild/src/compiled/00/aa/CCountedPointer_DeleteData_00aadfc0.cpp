// CCountedPointer::DeleteData(void* object) -- Delete the typed pointee and its owned plain-element array.
// Leaf: ?DeleteData@?$CCountedPointer@VCWaveFile@@@@SIXPAX@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Own LoadWaveFile00aad650 allocates24 bytes, Channels/SampleLength/SamplingFrequency and AudioData at12; Reset00aadff0 stores this callback. Exact original CCountedPointer<CWaveFile>::DeleteData PDB name.
// Layout: genuine original CArray/std::vector/base/allocator-proxy. All owned array elements are trivial. Original fields retained, no active pointee header.
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
class CWaveFile{public:unsigned long Channels,SampleLength,SamplingFrequency;CArray<short> AudioData;};
template void __fastcall CCountedPointer<CWaveFile>::DeleteData(void*);