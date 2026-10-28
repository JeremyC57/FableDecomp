#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static bool run(unsigned int count){Element* storage=(Element*)malloc(5*sizeof(Element));memset(storage,0x5a,5*sizeof(Element));Element value;value.X=1.5f;value.Y=-2.0f;value.Z=4.0f;value.W=0.5f;Element* result=std::__uninitialized_fill_n(storage+1,count,value,std::__false_type());bool ok=result==storage+1+count;for(unsigned int i=0;i<count;++i){Element& item=storage[1+i];ok=ok&&item.X==value.X&&item.Y==value.Y&&item.Z==value.Z&&item.W==value.W;item.~Element();}unsigned char* bytes=(unsigned char*)storage;for(unsigned int j=0;j<sizeof(Element);++j)ok=ok&&bytes[j]==0x5a;for(unsigned int k=(count+1)*sizeof(Element);k<5*sizeof(Element);++k)ok=ok&&bytes[k]==0x5a;free(storage);return ok;}
int main(){bool ok=run(0)&&run(1)&&run(3);puts(ok?"00a51c20_TEST PASS":"00a51c20_TEST FAIL");return ok?0:1;}