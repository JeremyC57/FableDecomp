// std::__uninitialized_copy(first,last,result,tag) -- Copy-construct counted pointers into raw storage.
// Leaf: ??$__uninitialized_copy@PAV?$CCountedPointer@VCUnprovenPointee_00ad68d0@@@@PAV1@@std@@YIPAV?$CCountedPointer@VCUnprovenPointee_00ad68d0@@@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Identity: descriptive pointee identity retained from own overflow00ad68d0 and established helper binding. Own two copy callsites and count-block increment prove CCountedPointer, but no recovered pointee name is claimed.
// Layout: Data0/Info4, control RefCount0; actual eight-byte source and destination stride.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CUnprovenPointee_00ad68d0;class CCPPointerInfo{public:unsigned long RefCount;void (__fastcall* DeleteFunc)(void*);void* Data;};
template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}__forceinline CCountedPointer(const CCountedPointer& other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;}~CCountedPointer();};
namespace std{struct __false_type{__false_type(){}};template<class T,class U>__forceinline void _Construct(T* p,const U& value){if(p)new(p)T(value);}template<class Input,class Output>Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&){Output current=result;for(;first!=last;++first,++current)_Construct(current,*first);return current;}}
typedef CCountedPointer<CUnprovenPointee_00ad68d0> Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);