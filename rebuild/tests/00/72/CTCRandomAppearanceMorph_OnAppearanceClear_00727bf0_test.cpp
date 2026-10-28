#include <stdio.h>
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
class CBaseClass { public: virtual ~CBaseClass() {} }; class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
namespace NGraphicAppearance { class CPrimitive; class CAGraphicAppearanceComponent { public: virtual ~CAGraphicAppearanceComponent() {} virtual void OnAppearanceClear() {} }; }
class CTCAppearanceMorphBase:public CTCBase { public: class CMeshMorphSet; };
template<class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };
class CTCRandomAppearanceMorph:public CTCAppearanceMorphBase,public NGraphicAppearance::CAGraphicAppearanceComponent { public: CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> PMeshMorphSet; unsigned long Seed; std::vector<NGraphicAppearance::CPrimitive*> PrimHandles; virtual void OnAppearanceClear(); };
void CTCRandomAppearanceMorph::OnAppearanceClear() { PrimHandles.clear(); }
int main() { CTCRandomAppearanceMorph object; NGraphicAppearance::CPrimitive* data[3]; object.PrimHandles._M_start=data; object.PrimHandles._M_finish=data+3; object.PrimHandles._M_end_of_storage._M_data=data+3; object.OnAppearanceClear(); if(object.PrimHandles._M_finish!=data || object.PrimHandles._M_start!=data || object.PrimHandles._M_end_of_storage._M_data!=data+3) goto fail; object.OnAppearanceClear(); if(object.PrimHandles._M_finish!=data) goto fail; puts("00727bf0_TEST PASS"); return 0; fail: puts("00727bf0_TEST FAIL"); return 1; }