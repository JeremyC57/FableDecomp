#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CAFile; class CCPPointerInfo;
template<class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };
class CLevelLoader { public: class CObject { public: class CLevelFiles { public: long LevelID; CCountedPointer<CAFile> PTopologyFile,PNavigationFile,PThingFile; CLevelFiles& operator=(const CLevelFiles&); }; }; };
namespace std {
struct input_iterator_tag {}; struct forward_iterator_tag : input_iterator_tag {}; struct bidirectional_iterator_tag : forward_iterator_tag {}; struct random_access_iterator_tag : bidirectional_iterator_tag {};
template<class Input,class Output,class Distance> Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*) {
 for(Distance n=last-first;n>0;--n) { *result=*first; ++first; ++result; } return result;
}
}
template CLevelLoader::CObject::CLevelFiles* __fastcall std::__copy<CLevelLoader::CObject::CLevelFiles*,CLevelLoader::CObject::CLevelFiles*,int>(CLevelLoader::CObject::CLevelFiles*,CLevelLoader::CObject::CLevelFiles*,CLevelLoader::CObject::CLevelFiles*,const std::random_access_iterator_tag&,int*);

typedef CLevelLoader::CObject::CLevelFiles Element;

static int calls=0,bad=0; static const Element* input; static Element* output;
CLevelLoader::CObject::CLevelFiles& CLevelLoader::CObject::CLevelFiles::operator=(const CLevelLoader::CObject::CLevelFiles& other) { if(&other!=input+calls || this!=output+calls) bad=1; ++calls; memcpy(this,&other,sizeof(Element)); return *this; }
int main() { Element src[4],dst[4]; unsigned char* bytes=(unsigned char*)src; unsigned i; for(i=0;i<sizeof(src);++i) bytes[i]=(unsigned char)(i*7+3); memset(dst,0,sizeof(dst));
 input=src; output=dst; Element* end=std::__copy(src,src+3,dst,std::random_access_iterator_tag(),(int*)0);
 if(bad || calls!=3 || end!=dst+3 || memcmp(src,dst,3*sizeof(Element))) goto fail;
 calls=0; end=std::__copy(src,src,dst,std::random_access_iterator_tag(),(int*)0); if(bad || calls || end!=dst) goto fail;
 calls=0; end=std::__copy(src,src+1,dst,std::random_access_iterator_tag(),(int*)0); if(bad || calls!=1 || end!=dst+1 || memcmp(src,dst,sizeof(Element))) goto fail;
 puts("006c2b10_TEST PASS"); return 0;
fail: puts("006c2b10_TEST FAIL"); return 1;
}