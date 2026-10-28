// std::__copy_ptrs(Input first, Input last, Output result, const __false_type& tag) -- copy the record range forwards.
// Leaf: ??$__copy_ptrs@PBVCDataElement@CVertexStreamFormat@@PAV12@@std@@YIPAVCDataElement@CVertexStreamFormat@@PBV12@0PAV12@ABU__false_type@0@@Z
// Identity: proven; own vector op=0xA5E4D0 shares helpers with CVertexStreamFormat::DataElements copy0xA5E0A0, J076 local0xAB3B40; PDB Register/DataType record8bytes; const input from vector assignment.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
enum EVertexStreamDataType {};
class CVertexStreamFormat { public: class CDataElement { public: unsigned long Register; EVertexStreamDataType DataType; }; };
namespace std {
struct __false_type { __false_type() {} };
struct input_iterator_tag {}; struct forward_iterator_tag:input_iterator_tag {}; struct bidirectional_iterator_tag:forward_iterator_tag {}; struct random_access_iterator_tag:bidirectional_iterator_tag {};
template<class Input,class Output,class Distance> __forceinline Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*) { for(Distance n=last-first;n>0;--n) { *result=*first; ++first; ++result; } return result; }
template<class Input,class Output> Output __fastcall __copy_ptrs(Input first,Input last,Output result,const __false_type&) { return __copy(first,last,result,random_access_iterator_tag(),(int*)0); }
}

typedef CVertexStreamFormat::CDataElement Element;
template Element* __fastcall std::__copy_ptrs<const Element*,Element*>(const Element*,const Element*,Element*,const std::__false_type&);