#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing; class CThingCreatureBase; class CThingObject;
class CBaseObject; class CBaseObjectPointer;
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseObjectPointer* PData; CBaseObject* GetPItem() const; void SetPItem(CBaseObject*); };
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
 T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
 void SetPItem(T* p) { CBaseIntelligentPointer::SetPItem((CBaseObject*)p); }
 CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
namespace std {
struct input_iterator_tag {}; struct forward_iterator_tag: input_iterator_tag {}; struct bidirectional_iterator_tag: forward_iterator_tag {}; struct random_access_iterator_tag: bidirectional_iterator_tag {};
template<class Input,class Output,class Distance> Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*) { for(Distance n=last-first;n>0;--n) { *result=*first; ++first; ++result; } return result; }
}
typedef CIntelligentPointer<const CThing> Element;
template Element* __fastcall std::__copy<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);

class CBaseObjectPointer { public: CBaseObject* Item; };
static int calls=0,bad=0; static Element* in; static Element* out;
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { if(calls%2 || this!=in+calls/2) bad=1; ++calls; return PData->Item; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* p) { if(calls%2!=1 || this!=out+calls/2 || p!=(in+calls/2)->PData->Item) bad=1; PData->Item=p; ++calls; }
int main() { Element src[3],dst[3]; CBaseObjectPointer sb[3],db[3]; int i; for(i=0;i<3;++i) { src[i].PData=&sb[i]; dst[i].PData=&db[i]; sb[i].Item=(CBaseObject*)(i==1?0:0x100+i*8); db[i].Item=0; }
 in=src; out=dst; Element* end=std::__copy(src,src+3,dst,std::random_access_iterator_tag(),(int*)0); if(bad || calls!=6 || end!=dst+3) goto fail;
 for(i=0;i<3;++i) if(sb[i].Item!=db[i].Item) goto fail;
 calls=0; end=std::__copy(src,src,dst,std::random_access_iterator_tag(),(int*)0); if(bad || calls || end!=dst) goto fail;
 puts("00737d50_TEST PASS"); return 0;
fail: puts("00737d50_TEST FAIL"); return 1;
}