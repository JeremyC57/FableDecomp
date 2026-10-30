// CTCRandomAppearanceMorph::OnAppearanceClear() -- clear the trivial handle vector.
// Leaf: ?OnAppearanceClear@CTCRandomAppearanceMorph@@UAEXXZ
// Identity: proven; RTTI secondary CAGraphicAppearanceComponent+12 vtable0x1266520 slot1, PDB method and PrimHandles+28 (receiver-relative+16).
// Layout: no target generated header. Real RTTI base chain retains the secondary vptr at+12 for appearance callbacks; donor16-byte vector becomes retail12-byte vector. The compiler supplies the secondary receiver convention.
#include <stddef.h>
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" void* __cdecl memmove(void*,const void*,size_t);
namespace std {
inline void* __copy_trivial(const void* first,const void* last,void* result) { return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first); }
template<class T> class allocator {};
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; T* erase(T* first,T* last) { T* result=(T*)__copy_trivial(last,_M_finish,first); _M_finish=result; return first; } void clear() { erase(_M_start,_M_finish); } };
}
template<class T> class CArray:public std::vector<T> {};
class CThing; class CCPPointerInfo;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
 class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
namespace NGraphicAppearance { class CPrimitive; class CAGraphicAppearanceComponent { public: virtual ~CAGraphicAppearanceComponent() {} virtual void OnAppearanceClear() {} }; }
class CTCAppearanceMorphBase:public CTCBase { public: class CMeshMorphSet; };
template<class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };
class CTCRandomAppearanceMorph:public CTCAppearanceMorphBase,public NGraphicAppearance::CAGraphicAppearanceComponent { public: CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> PMeshMorphSet; unsigned long Seed; std::vector<NGraphicAppearance::CPrimitive*> PrimHandles; virtual void OnAppearanceClear(); };
void CTCRandomAppearanceMorph::OnAppearanceClear() { PrimHandles.clear(); }