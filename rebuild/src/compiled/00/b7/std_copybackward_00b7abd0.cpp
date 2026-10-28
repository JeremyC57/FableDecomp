// std::__copy_backward(first,last,result,tag,distance) -- Copy plain records backward with overlap support.
// Leaf: ??$__copy_backward@PAVCUnprovenCopyRecord_00b7abd0@@PAV1@H@std@@YIPAVCUnprovenCopyRecord_00b7abd0@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Identity: descriptive original element type unproven; actual SGI/STLport __copy_backward random-access five-argument ABI. CPreTransposedBoneMatrix and other size48 records remain possible; no callable xref identifies this body. Manifest string/dummy pair is inconsistent with POD block copying.
// Layout: own reciprocal stride48 and rep movsd count12 prove the copied span only; record fields deliberately unnamed.
#ifndef _MSC_VER
#define __fastcall
#endif
class CUnprovenCopyRecord_00b7abd0{public:unsigned char UnprovenBytes[48];};
namespace std {
struct random_access_iterator_tag {random_access_iterator_tag(){}};
template<class Input,class Output,class Distance>Output __fastcall __copy_backward(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){for(Distance count=last-first;count>0;--count)*--result=*--last;return result;}
}
typedef CUnprovenCopyRecord_00b7abd0 Element;
template Element* __fastcall std::__copy_backward<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);