#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class C3DVector { public: float X, Y, Z; };
class CQuadric { public: double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD; double _pad; };   // Ego_r 88 bytes
namespace QuadricOptimiserInternals {
class COptimisedVertex {                                // retail 0xB8 (Ego_r 192 with 16-byte set/maps; STLport's are 12)
public:
    C3DVector     Position;                             // +0x00
    C3DVector     Normal;                               // +0x0C
    C3DVector     TextureCoord;                         // +0x18
    CQuadric      PositionError;                        // +0x28 (8-aligned)
    float         Cost;                                 // +0x80
    unsigned char FaceList[0xC];                        // +0x84 CIndexList (12-byte STLport tree)
    bool          Exists;                               // +0x90
    unsigned long UniqueIndex;                          // +0x94
    unsigned long OutputIndex;                          // +0x98
    unsigned char Groups[0xC];                          // +0x9C map<unsigned long, float>
    unsigned char PairContractions[0xC];                // +0xA8 map<unsigned long, CContractionReference>
    COptimisedVertex& operator=(const COptimisedVertex& other);   // 0x00AD7050
};
}
namespace std {
struct input_iterator_tag {}; struct forward_iterator_tag : input_iterator_tag {}; struct bidirectional_iterator_tag : forward_iterator_tag {}; struct random_access_iterator_tag : bidirectional_iterator_tag {};
template<class Input,class Output,class Distance> Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*) {
 for(Distance n=last-first;n>0;--n) { *result=*first; ++first; ++result; } return result;
}
}
template QuadricOptimiserInternals::COptimisedVertex* __fastcall std::__copy<QuadricOptimiserInternals::COptimisedVertex*,QuadricOptimiserInternals::COptimisedVertex*,int>(QuadricOptimiserInternals::COptimisedVertex*,QuadricOptimiserInternals::COptimisedVertex*,QuadricOptimiserInternals::COptimisedVertex*,const std::random_access_iterator_tag&,int*);

typedef QuadricOptimiserInternals::COptimisedVertex Element;

static int calls=0,bad=0; static const Element* input; static Element* output;
QuadricOptimiserInternals::COptimisedVertex& QuadricOptimiserInternals::COptimisedVertex::operator=(const QuadricOptimiserInternals::COptimisedVertex& other) { if(&other!=input+calls || this!=output+calls) bad=1; ++calls; memcpy(this,&other,sizeof(Element)); return *this; }
int main() { Element src[4],dst[4]; unsigned char* bytes=(unsigned char*)src; unsigned i; for(i=0;i<sizeof(src);++i) bytes[i]=(unsigned char)(i*7+3); memset(dst,0,sizeof(dst));
 input=src; output=dst; Element* end=std::__copy(src,src+3,dst,std::random_access_iterator_tag(),(int*)0);
 if(bad || calls!=3 || end!=dst+3 || memcmp(src,dst,3*sizeof(Element))) goto fail;
 calls=0; end=std::__copy(src,src,dst,std::random_access_iterator_tag(),(int*)0); if(bad || calls || end!=dst) goto fail;
 calls=0; end=std::__copy(src,src+1,dst,std::random_access_iterator_tag(),(int*)0); if(bad || calls!=1 || end!=dst+1 || memcmp(src,dst,sizeof(Element))) goto fail;
 puts("00ad7930_TEST PASS"); return 0;
fail: puts("00ad7930_TEST FAIL"); return 1;
}