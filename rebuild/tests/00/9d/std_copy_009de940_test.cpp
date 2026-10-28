#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class C2DBoxF { public: float TLX,TLY,BRX,BRY; };
class CWideString { public: void* PData; };
struct IDirect3DTexture9;
#pragma pack(push,1)
class CTexture { public: IDirect3DTexture9* PD3DTexture; unsigned char ByteLength[3],AllocationSource[1]; };
#pragma pack(pop)
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

typedef CQuickDrawTriInfo Element;

static int calls=0,bad=0; static const Element* input; static Element* output;
CQuickDrawTriInfo& CQuickDrawTriInfo::operator=(const CQuickDrawTriInfo& other) { if(&other!=input+calls || this!=output+calls) bad=1; ++calls; memcpy(this,&other,sizeof(Element)); return *this; }
int main() { Element src[4],dst[4]; unsigned char* bytes=(unsigned char*)src; unsigned i; for(i=0;i<sizeof(src);++i) bytes[i]=(unsigned char)(i*7+3); memset(dst,0,sizeof(dst));
 input=src; output=dst; Element* end=std::__copy(src,src+3,dst,std::random_access_iterator_tag(),(int*)0);
 if(bad || calls!=3 || end!=dst+3 || memcmp(src,dst,3*sizeof(Element))) goto fail;
 calls=0; end=std::__copy(src,src,dst,std::random_access_iterator_tag(),(int*)0); if(bad || calls || end!=dst) goto fail;
 calls=0; end=std::__copy(src,src+1,dst,std::random_access_iterator_tag(),(int*)0); if(bad || calls!=1 || end!=dst+1 || memcmp(src,dst,sizeof(Element))) goto fail;
 puts("009de940_TEST PASS"); return 0;
fail: puts("009de940_TEST FAIL"); return 1;
}