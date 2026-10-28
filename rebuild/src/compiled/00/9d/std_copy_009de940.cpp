// std::__copy(Input first, Input last, Output result, const random_access_iterator_tag& tag, int* distance) -- assign the input range forwards.
// Leaf: ??$__copy@PAVCQuickDrawTriInfo@@PAV1@H@std@@YIPAVCQuickDrawTriInfo@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Identity: proven; CQuickDrawTriInfo::operator=0x9DE6F0 own landed memberwise implementation; vector erase0x9E15E0 calls;60-byte PDB record.
class C2DBoxF { public: float TLX,TLY,BRX,BRY; };
class CWideString { public: void* PData; };
#include "engine/CTexture.h"
class CRGBColour { public: unsigned char B,G,R,A; };
class CFontBank;
class CQuickDrawTriInfo { public: unsigned char Line:1; unsigned char IsText:1; C2DBoxF Window; CWideString String; CTexture UploadedTexture; union { struct {unsigned long BlendMode; float ZAverage;}; struct {float StringX,StringY;}; }; float StringZ,StringScale; unsigned long StringFlags; CRGBColour Colour; CFontBank* PFont; CQuickDrawTriInfo& operator=(const CQuickDrawTriInfo&); };
namespace std {
struct input_iterator_tag {}; struct forward_iterator_tag : input_iterator_tag {}; struct bidirectional_iterator_tag : forward_iterator_tag {}; struct random_access_iterator_tag : bidirectional_iterator_tag {};
template<class Input,class Output,class Distance> Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*) {
 for(Distance n=last-first;n>0;--n) { *result=*first; ++first; ++result; } return result;
}
}
template CQuickDrawTriInfo* __fastcall std::__copy<CQuickDrawTriInfo*,CQuickDrawTriInfo*,int>(CQuickDrawTriInfo*,CQuickDrawTriInfo*,CQuickDrawTriInfo*,const std::random_access_iterator_tag&,int*);