// CTCRandomAppearanceMorph::OnAppearanceClear() -- clear the trivial handle vector.
// Leaf: ?OnAppearanceClear@CTCRandomAppearanceMorph@@UAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: proven; RTTI secondary CAGraphicAppearanceComponent+12 vtable0x1266520 slot1, PDB method and PrimHandles+28 (receiver-relative+16).
// Layout: no target generated header. Real RTTI base chain retains the secondary vptr at+12 for appearance callbacks; donor16-byte vector becomes retail12-byte vector. The compiler supplies the secondary receiver convention.
#include <stddef.h>
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" void* __cdecl memmove(void*,const void*,size_t);
#include "engine/CArray.h"
class CThing; class CCPPointerInfo;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
 class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
namespace NGraphicAppearance { class CPrimitive; class CAGraphicAppearanceComponent { public: virtual ~CAGraphicAppearanceComponent() {} virtual void OnAppearanceClear() {} }; }
class CTCAppearanceMorphBase:public CTCBase { public: class CMeshMorphSet; };
template<class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };
class CTCRandomAppearanceMorph:public CTCAppearanceMorphBase,public NGraphicAppearance::CAGraphicAppearanceComponent { public: CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> PMeshMorphSet; unsigned long Seed; std::vector<NGraphicAppearance::CPrimitive*> PrimHandles; virtual void OnAppearanceClear(); };
void CTCRandomAppearanceMorph::OnAppearanceClear() { PrimHandles.clear(); }