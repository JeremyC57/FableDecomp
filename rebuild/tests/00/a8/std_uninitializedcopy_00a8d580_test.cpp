#include <stdio.h>
#include <stdlib.h>
// std::__uninitialized_copy(first,last,result,tag) -- Copy-construct counted pointers into raw storage.
// Leaf: ??$__uninitialized_copy@PAV?$CCountedPointer@VCUnprovenPointee_00a8ddc0@@@@PAV1@@std@@YIPAV?$CCountedPointer@VCUnprovenPointee_00a8ddc0@@@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Identity: descriptive pointee identity retained from own overflow00a8ddc0 and established helper binding. Own two copy callsites and count-block increment prove CCountedPointer, but no recovered pointee name is claimed.
// Layout: Data0/Info4, control RefCount0; actual eight-byte source and destination stride.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CUnprovenPointee_00a8ddc0;class CCPPointerInfo{public:unsigned long RefCount;void (__fastcall* DeleteFunc)(void*);void* Data;};
template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}__forceinline CCountedPointer(const CCountedPointer& other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;}~CCountedPointer();};
namespace std{struct __false_type{__false_type(){}};template<class T,class U>__forceinline void _Construct(T* p,const U& value){if(p)new(p)T(value);}template<class Input,class Output>Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&){Output current=result;for(;first!=last;++first,++current)_Construct(current,*first);return current;}}
typedef CCountedPointer<CUnprovenPointee_00a8ddc0> Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);
template<class T>CCountedPointer<T>::~CCountedPointer(){if(Info)--Info->RefCount;}
static bool run(unsigned int count){CCPPointerInfo shared;shared.RefCount=1;Element input[3];input[0].Info=&shared;input[2].Info=&shared;++shared.RefCount;int tokens[3];for(int i=0;i<3;++i)input[i].Data=(CUnprovenPointee_00a8ddc0*)(tokens+i);Element* output=(Element*)malloc(3*sizeof(Element));Element* result=std::__uninitialized_copy(input,input+count,output,std::__false_type());bool ok=result==output+count&&shared.RefCount==2+(count>0?1:0)+(count>2?1:0);for(unsigned int j=0;j<count;++j){ok=ok&&output[j].Data==input[j].Data&&output[j].Info==input[j].Info;output[j].~Element();}ok=ok&&shared.RefCount==2;free(output);return ok;}
int main(){bool ok=run(0)&&run(1)&&run(3);puts(ok?"00a8d580_TEST PASS":"00a8d580_TEST FAIL");return ok?0:1;}