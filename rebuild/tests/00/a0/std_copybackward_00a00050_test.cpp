#include <stdio.h>
#include <string.h>
// std::__copy_backward(first,last,result,tag,distance) -- Copy plain records backward with overlap support.
// Leaf: ??$__copy_backward@PAVCUnprovenCopyRecord_00a00050@@PAV1@H@std@@YIPAVCUnprovenCopyRecord_00a00050@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Identity: descriptive original element type unproven; actual SGI/STLport __copy_backward random-access five-argument ABI. CGraphicInfo is a plausible neighboring/PDB instantiation, but no callable xref identifies this body. Manifest string/dummy pair is inconsistent with POD block copying.
// Layout: own reciprocal stride44 and rep movsd count11 prove the copied span only; record fields deliberately unnamed.
#ifndef _MSC_VER
#define __fastcall
#endif
class CUnprovenCopyRecord_00a00050{public:unsigned char UnprovenBytes[44];};
namespace std {
struct random_access_iterator_tag {random_access_iterator_tag(){}};
template<class Input,class Output,class Distance>Output __fastcall __copy_backward(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){for(Distance count=last-first;count>0;--count)*--result=*--last;return result;}
}
typedef CUnprovenCopyRecord_00a00050 Element;
template Element* __fastcall std::__copy_backward<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);

int main(){Element records[7],before[7];for(int i=0;i<7;++i)for(unsigned int j=0;j<sizeof(Element);++j)records[i].UnprovenBytes[j]=(unsigned char)(i*17+j);memcpy(before,records,sizeof(records));Element* result=std::__copy_backward(records,records+4,records+6,std::random_access_iterator_tag(),(int*)0);bool ok=result==records+2;for(int k=0;k<4;++k)ok=ok&&memcmp(records+k+2,before+k,sizeof(Element))==0;ok=ok&&memcmp(records,before,2*sizeof(Element))==0&&memcmp(records+6,before+6,sizeof(Element))==0;memcpy(before,records,sizeof(records));result=std::__copy_backward(records,records,records+3,std::random_access_iterator_tag(),(int*)0);ok=ok&&result==records+3&&memcmp(records,before,sizeof(records))==0;result=std::__copy_backward(records,records+7,records+7,std::random_access_iterator_tag(),(int*)0);ok=ok&&result==records&&memcmp(records,before,sizeof(records))==0;puts(ok?"00a00050_TEST PASS":"00a00050_TEST FAIL");return ok?0:1;}