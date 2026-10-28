// std::__uninitialized_fill_n(Element* first,unsigned int count,const Element& value,const __false_type&) -- Construct copies in uninitialised storage.
// Leaf: ??$__uninitialized_fill_n@PAVCVertexShaderConstant@@IV1@@std@@YIPAVCVertexShaderConstant@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Identity: Own overflow00bdb8e0 shares uninitialized-copy00bdb6d0 with CEngineVSConstantLayoutBasic constructor00bdbb70; ReservedPresets vector at+320 copied through00bdbb60/00bdb9f0; PDB CVertexShaderConstant X/Y/Z/W sixteen-byte class.
// Layout: original four-float class16, implicit copy constructor.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CVertexShaderConstant{public:float X,Y,Z,W;};
namespace std {
struct __false_type{};
template<class T,class U>__forceinline void _Construct(T* p,const U& value){new(p)T(value);}
template<class Iter,class Size,class T>Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&){for(;count>0;--count,++first)_Construct(first,value);return first;}
}
typedef CVertexShaderConstant Element;
template Element* __fastcall std::__uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&,const std::__false_type&);