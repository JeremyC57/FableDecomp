#ifndef FABLE_ENGINE_NSHAPE_STLHELPERS_H
#define FABLE_ENGINE_NSHAPE_STLHELPERS_H
#include "engine/NShape_CSubdivisionTriangle.h"
#include "engine/CCountedPointer.h"
// Native00764bc0/00764ee0 retain these genuine STLport helper instantiations only.
namespace std {
template<> __declspec(noinline) CCountedPointer<NShape::CSubdivisionTriangle>* FABLE_FASTCALL __copy(CCountedPointer<NShape::CSubdivisionTriangle>* first,CCountedPointer<NShape::CSubdivisionTriangle>* last,CCountedPointer<NShape::CSubdivisionTriangle>* result,const random_access_iterator_tag&,int*) {
 for(int n=last-first;n>0;--n){*result=*first;++first;++result;}return result;
}
template<> __declspec(noinline) void FABLE_FASTCALL __destroy_aux(CCountedPointer<NShape::CSubdivisionTriangle>* first,CCountedPointer<NShape::CSubdivisionTriangle>* last,const __false_type&) {
 for(;first!=last;++first)_Destroy(first);
}
}
#endif
