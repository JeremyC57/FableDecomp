#include <stdio.h>
// std::__copy_backward(first,last,result,tag,distance) -- Assign counted pointers backward with correct ownership.
// Leaf: ??$__copy_backward@PAV?$CCountedPointer@VCMeshDataBankEntry@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCMeshDataBankEntry@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Identity: Own fill-insert00a2d960 calls this; resize00a2dbc0 is called by CMeshDataBank::NotifyEntryAdded00a26d40 for DynamicInfoBuffer. Original CArray<CCountedPointer<CMeshDataBankEntry>>.
// Layout: original CCPPointerInfo RefCount0/DeleteFunc4/Data8; wrapper Data0/Info4. Actual callback consumes its pointer in ECX, so fastcall in this TU. Scalar operator delete at BFE9BC releases the control block.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CMeshDataBankEntry;
class CCPPointerInfo {public:unsigned long RefCount;void (__fastcall* DeleteFunc)(void*);void* Data;};
template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}__forceinline void Release(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);::operator delete(Info);}}}__forceinline ~CCountedPointer(){Release();}__forceinline CCountedPointer& operator=(const CCountedPointer& other){T* data=other.Data;CCPPointerInfo* info=other.Info;if(Info!=info){Release();Data=data;Info=info;if(info)++info->RefCount;}return *this;}};
namespace std{struct random_access_iterator_tag{random_access_iterator_tag(){}};template<class Input,class Output,class Distance>Output __fastcall __copy_backward(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){for(Distance count=last-first;count>0;--count)*--result=*--last;return result;}}
typedef CCountedPointer<CMeshDataBankEntry> Element;
template Element* __fastcall std::__copy_backward<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);
class CMeshDataBankEntry{public:int Id;};static int deleted[4],errors;
static void __fastcall Destroy(void* p){int id=((CMeshDataBankEntry*)p)->Id;if(id<0||id>=4)++errors;else ++deleted[id];}
static bool run(){CMeshDataBankEntry objects[4];Element records[6];CCPPointerInfo* infos[4];for(int i=0;i<4;++i){objects[i].Id=i;infos[i]=new CCPPointerInfo;infos[i]->RefCount=1;infos[i]->DeleteFunc=&Destroy;infos[i]->Data=objects+i;int target=i==3?4:i;records[target].Data=objects+i;records[target].Info=infos[i];}Element* result=std::__copy_backward(records,records+3,records+5,std::random_access_iterator_tag(),(int*)0);bool ok=result==records+2&&deleted[3]==1&&records[2].Data==objects&&records[3].Data==objects+1&&records[4].Data==objects+2&&infos[0]->RefCount==2&&infos[1]->RefCount==2&&infos[2]->RefCount==1;result=std::__copy_backward(records,records+5,records+5,std::random_access_iterator_tag(),(int*)0);ok=ok&&result==records&&infos[0]->RefCount==2&&infos[1]->RefCount==2;result=std::__copy_backward(records,records,records+4,std::random_access_iterator_tag(),(int*)0);ok=ok&&result==records+4;result=std::__copy_backward(records+5,records+6,records+1,std::random_access_iterator_tag(),(int*)0);return ok&&result==records&&!records[0].Data&&!records[0].Info&&infos[0]->RefCount==1;}
int main(){bool ok=run();for(int i=0;i<4;++i)ok=ok&&deleted[i]==1;ok=ok&&!errors;puts(ok?"00a2a8f0_TEST PASS":"00a2a8f0_TEST FAIL");return ok?0:1;}