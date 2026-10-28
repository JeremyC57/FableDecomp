// std::__copy_backward(first,last,result,tag,distance) -- Assign counted pointers backward with correct ownership.
// Leaf: ??$__copy_backward@PAV?$CCountedPointer@VCEngineSceneGrid@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCEngineSceneGrid@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Identity: Own fill-insert00b97e90/resize00b98170 and CEngineSceneGrid own caller00b949a0 prove original CCountedPointer<CEngineSceneGrid>; same verified type as N060.
// Layout: original CCPPointerInfo RefCount0/DeleteFunc4/Data8; wrapper Data0/Info4. Actual callback consumes its pointer in ECX, so fastcall in this TU. Scalar operator delete at BFE9BC releases the control block.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CEngineSceneGrid;
class CCPPointerInfo {public:unsigned long RefCount;void (__fastcall* DeleteFunc)(void*);void* Data;};
template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}__forceinline void Release(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);::operator delete(Info);}}}__forceinline ~CCountedPointer(){Release();}__forceinline CCountedPointer& operator=(const CCountedPointer& other){T* data=other.Data;CCPPointerInfo* info=other.Info;if(Info!=info){Release();Data=data;Info=info;if(info)++info->RefCount;}return *this;}};
namespace std{struct random_access_iterator_tag{random_access_iterator_tag(){}};template<class Input,class Output,class Distance>Output __fastcall __copy_backward(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){for(Distance count=last-first;count>0;--count)*--result=*--last;return result;}}
typedef CCountedPointer<CEngineSceneGrid> Element;
template Element* __fastcall std::__copy_backward<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);