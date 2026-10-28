// std::__uninitialized_fill_n(Element* first,unsigned int count,const Element& value,const __false_type&) -- Construct copies in uninitialised storage.
// Leaf: ??$__uninitialized_fill_n@PAVC4DVector@@IV1@@std@@YIPAVC4DVector@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Identity: Own caller0098d240 is vector<C4DVector> overflow reached by SetPixelShaderConstant0098b390 through push_back0098d820; established exact helper binding and PDB class key V.
// Layout: original four-float class16, implicit copy constructor.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class C4DVector{public:float X,Y,Z,W;};
namespace std {
struct __false_type{};
template<class T,class U>__forceinline void _Construct(T* p,const U& value){new(p)T(value);}
template<class Iter,class Size,class T>Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&){for(;count>0;--count,++first)_Construct(first,value);return first;}
}
typedef C4DVector Element;
template Element* __fastcall std::__uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&,const std::__false_type&);