// std::__uninitialized_fill_n(Element* first,unsigned int count,const Element& value,const __false_type&) -- Construct copies in uninitialised storage.
// Leaf: ??$__uninitialized_fill_n@PAVCQuaternion@@IV1@@std@@YIPAVCQuaternion@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Identity: Own caller00a54fa0 vector<CQuaternion> overflow and established exact helper binding; original PDB X/Y/Z/W fields and class key V.
// Layout: original four-float class16, implicit copy constructor.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CQuaternion{public:float X,Y,Z,W;};
namespace std {
struct __false_type{};
template<class T,class U>__forceinline void _Construct(T* p,const U& value){new(p)T(value);}
template<class Iter,class Size,class T>Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&){for(;count>0;--count,++first)_Construct(first,value);return first;}
}
typedef CQuaternion Element;
template Element* __fastcall std::__uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&,const std::__false_type&);