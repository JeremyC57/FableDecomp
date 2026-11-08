// CEnginePrimitiveOcclusionInfo::~CEnginePrimitiveOcclusionInfo() -- Release owned sequence storage and destroy the base.
// Leaf: ??1CEnginePrimitiveOcclusionInfo@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Own deleting-dtor00b2fb10 and full RTTI12a1284; CIVCountedPointeeBase at0, InvisibleFrames+8, handles+12, base vtable12354d0.
#include "fable_compiler.h"
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderLayout
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include "engine/CArray.h"
// Layout: shared base headers describe flattened vptrs; genuine polymorphic hierarchy is required for compiler-generated destruction. Original base fields and offsets retained; no target base reconstruction claim.
class CIVCountedPointeeBase {public:virtual FABLE_FORCEINLINE ~CIVCountedPointeeBase(){}virtual void Delete()const;unsigned long IVRefCount;};
class CEnginePrimitiveOcclusionInfo:public CIVCountedPointeeBase {public:long InvisibleFrames;CArray<unsigned long> OcclusionTestHandles;bool Seen;unsigned long TestFrame;};


void DestroyCandidate(CEnginePrimitiveOcclusionInfo* p){p->CEnginePrimitiveOcclusionInfo::~CEnginePrimitiveOcclusionInfo();}