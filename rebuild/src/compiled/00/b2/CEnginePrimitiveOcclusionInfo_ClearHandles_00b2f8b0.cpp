// CEnginePrimitiveOcclusionInfo::ClearHandles() -- clear the trivial handle vector.
// Leaf: ?ClearHandles@CEnginePrimitiveOcclusionInfo@@AAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: proven; own CheckVisibilityResult0xB2F8E0 calls at0xB2F9D4 on its info argument; PDB private ClearHandles and OcclusionTestHandles+12.
// Layout: no target generated header. Real RTTI base chain retains the secondary vptr at+12 for appearance callbacks; donor16-byte vector becomes retail12-byte vector. The compiler supplies the secondary receiver convention.
#include <stddef.h>
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" void* __cdecl memmove(void*,const void*,size_t);
#include "engine/CArray.h"
class CIVCountedPointeeBase { public: virtual ~CIVCountedPointeeBase() {} mutable unsigned long IVRefCount; }; class CEnginePrimitiveOcclusionInfo:public CIVCountedPointeeBase { public: long InvisibleFrames; CArray<unsigned long> OcclusionTestHandles; private: void ClearHandles(); };
void CEnginePrimitiveOcclusionInfo::ClearHandles() { OcclusionTestHandles.clear(); }