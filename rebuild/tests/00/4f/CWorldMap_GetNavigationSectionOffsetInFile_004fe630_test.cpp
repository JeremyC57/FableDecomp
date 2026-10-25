// 004fe630 CWorldMap::GetNavigationSectionOffsetInFile
// Flags: /O2 /Oy /W3 
// Leaf: GetNavigationSectionOffsetInFile
#include <string.h>
#include <stdlib.h>
#include "engine/CWorldMap.h"
namespace X_004fe630 {
struct CAFile{virtual void V0()=0;virtual void V1()=0;virtual void V2()=0;virtual void ReadRaw(void*,unsigned,void*)=0;virtual void V4()=0;virtual void V5()=0;virtual void V6()=0;virtual int GetFilePosition()=0;virtual void V8()=0;virtual int GetFileSize()=0;virtual void V10()=0;virtual void V11()=0;virtual void V12()=0;virtual int Alignment()=0;__declspec(noinline) void ReadVar(int*out,void*context){if(Alignment()<=1){ReadRaw(out,4,context);return;}unsigned alignment=Alignment();unsigned size=(alignment+3)/alignment*alignment;char*data=(char*)malloc(size);memset(data,0,size);ReadRaw(data,4,context);*out=*(int*)data;free(data);}};
#pragma pack(push,1)
template<class Char>struct CBasicString{Char*PData;unsigned StringLength,DataLength;bool UseFastExtend;__declspec(noinline) bool operator==(const Char*s)const{const signed char*a=(const signed char*)PData,*b=(const signed char*)s;while(*a&&*a==*b){++a;++b;}return *a==*b;}};
struct CCharStringData{CBasicString<char>Data;int RefCount;CCharStringData(){} CCharStringData(int,char);~CCharStringData();};
#pragma pack(pop)
extern int CCharStringInstances;
struct CCharString{CCharStringData*p;CCharStringData*AllocStringData(const char*,int);__declspec(noinline) void UnassignString(){if(p&&--p->RefCount<=0)delete p;p=0;}__declspec(noinline) CCharString():p(0){++CCharStringInstances;}__declspec(noinline) CCharString(const char*s,int n=-1):p(0){++CCharStringInstances;if(s&&*s)p=AllocStringData(s,n);}__declspec(noinline) CCharString(const CCharString&s);__declspec(noinline) ~CCharString(){UnassignString();--CCharStringInstances;}__declspec(noinline) CCharString&operator=(const CCharString&s){if(this!=&s){if(p)UnassignString();if(s.p){p=s.p;++p->RefCount;}}return *this;}__declspec(noinline) void LoadFromFile(CAFile*,void*,int);__forceinline bool Equals(const char*s)const{bool equal;if(p){equal=p->Data==s;}else{equal=strcmp(s,"")==0;}return equal;}};
// Layout: 004b9370 tree node parent+4,left+8,right+c,key CCharString+10.
void CCharString::LoadFromFile(CAFile*file,void*context,int length){UnassignString();if(length==-1)length=file->GetFileSize()-file->GetFilePosition();p=new CCharStringData(length,' ');if(p&&length>0)file->ReadRaw(p->Data.PData,length,context);}
struct Node{int color;Node*parent;Node*left;Node*right;CCharString value;};
__forceinline bool Less(const CCharString&a,const CCharString&b){if(a.p==b.p)return false;if(!b.p)return false;if(!a.p)return true;return strcmp(a.p->Data.PData,b.p->Data.PData)<0;}
namespace std {template<class T>struct set{Node*head;__declspec(noinline) Node*find(const T&key)const{Node*y=head;Node*x=head->parent;while(x){if(!Less(x->value,key)){y=x;x=x->left;}else x=x->right;}if(y==head||Less(key,y->value))return head;return y;}};}

// Layout: active CWorldMap header supplies receiver storage; method adapter adds no fields. PDB004d24e0 names return pair<CCharString,long> and CAFile/set arguments; receiver unused.
// Layout: result is string pointer+0 and index+4, both observed in epilogue.
namespace std {template<class A,class B>struct pair{A first;B second;__forceinline pair(const A&a,const B&b):first(a),second(b){}__forceinline pair(const pair&p):first(p.first),second(p.second){}};}
// Layout: int models retail32-bit long in pair while the fixture remains portable to 64-bit hosts.
typedef std::pair<CCharString,int> NavigationSection;
struct CWorldMapMethods: ::CWorldMap {NavigationSection GetNavigationSectionOffsetInFile(CAFile&file,const std::set<CCharString>&map)const;};
NavigationSection CWorldMapMethods::GetNavigationSectionOffsetInFile(CAFile&file,const std::set<CCharString>&map)const{NavigationSection fallback("",-1);NavigationSection found("",-1);int count;file.ReadVar(&count,0);for(int i=0;i<count;++i){int length;file.ReadVar(&length,0);CCharString name;name.LoadFromFile(&file,0,length);int index;file.ReadVar(&index,0);bool fallbackName;if(!name.p){fallbackName=strcmp("NULL","")==0||strcmp("","")==0;}else{fallbackName=name.p->Data=="NULL"||name.p->Data=="";}if(!fallbackName){if(map.find(name)!=map.head){found.first=name;found.second=index;break;}}else{fallback.first=name;fallback.second=index;}}
 if(found.second==-1){found.first=fallback.first;found.second=fallback.second;}return NavigationSection(found.first,found.second);
}
}

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace X_004fe630 {
#pragma pack(push,1)
typedef CCharStringData Actual;
#pragma pack(pop)
static int values[16],cursor;static const char*names[8];static int ni,live,findCalls;static Node hit;
static CCharStringData*Make(const char*s){if(!s)return 0;Actual*p=new Actual;p->Data.StringLength=(unsigned)strlen(s);p->Data.DataLength=p->Data.StringLength+1;p->Data.UseFastExtend=true;p->Data.PData=new char[p->Data.DataLength];strcpy(p->Data.PData,s);p->RefCount=1;++live;return(CCharStringData*)p;}
int CCharStringInstances=0;CCharStringData::~CCharStringData(){delete[]Data.PData;Data.PData=0;--live;}
static bool pendingString;static int alignment=1;
CCharStringData::CCharStringData(int n,char fill){assert(n>=0);Data.StringLength=n;Data.DataLength=n+1;Data.UseFastExtend=true;Data.PData=new char[n+1];memset(Data.PData,fill,n);Data.PData[n]=0;RefCount=1;++live;++ni;pendingString=n>0;}struct TestFile:CAFile{void V0(){}void V1(){}void V2(){}void V4(){}void V5(){}void V6(){}int GetFilePosition(){return 0;}void V8(){}int GetFileSize(){return 0;}void V10(){}void V11(){}void V12(){}int Alignment(){return alignment;}void ReadRaw(void*out,unsigned n,void*context){assert(!context);if(pendingString){assert(n==strlen(names[ni-1]));memcpy(out,names[ni-1],n);pendingString=false;}else{assert(n==4);*(int*)out=values[cursor++];}}};
CCharStringData* CCharString::AllocStringData(const char*s,int n){assert(n==-1);return Make(s);}
static void Setup(int count,const char*a,const char*b,const char*c){names[0]=a;names[1]=b;names[2]=c;values[0]=count;for(int i=0;i<count;++i){values[1+i*2]=names[i]?strlen(names[i]):0;values[2+i*2]=11+i*13;}cursor=ni=findCalls=0;}
}
int main(){using namespace X_004fe630;CWorldMapMethods world;TestFile file;assert(sizeof(Actual)==17);std::set<CCharString> map;Node sentinel;map.head=&sentinel;sentinel.parent=&hit;hit.parent=&sentinel;hit.left=hit.right=0;hit.value=CCharString("known");Setup(0,0,0,0);{NavigationSection p=world.GetNavigationSectionOffsetInFile(file,map);assert(p.second==-1&&p.first.Equals(""));NavigationSection copy(p);assert(copy.second==-1&&copy.first.Equals(""));}assert(live==1);Setup(3,"NULL","known","missing");{NavigationSection p=world.GetNavigationSectionOffsetInFile(file,map);assert(p.second==24&&p.first.Equals("known")&&cursor==5&&ni==2);NavigationSection copy(p);assert(((Actual*)p.first.p)->RefCount==2);}assert(live==1);Setup(3,"NULL","missing","");{NavigationSection p=world.GetNavigationSectionOffsetInFile(file,map);assert(p.second==37&&p.first.Equals(""));}assert(live==1);Setup(2,0,"missing",0);{NavigationSection p=world.GetNavigationSectionOffsetInFile(file,map);assert(p.second==11&&p.first.Equals(""));}assert(live==1);Setup(1,"missing",0,0);{NavigationSection p=world.GetNavigationSectionOffsetInFile(file,map);assert(p.second==-1&&p.first.Equals(""));}assert(live==1);alignment=8;Setup(1,"known",0,0);{NavigationSection p=world.GetNavigationSectionOffsetInFile(file,map);assert(p.second==11&&p.first.Equals("known"));}assert(live==1);hit.value=CCharString("");assert(!live);puts("004fe630_TEST PASS");}

namespace X_004fe630 {CCharString::CCharString(const CCharString&s):p(0){++CCharStringInstances;if(this!=&s&&s.p){p=s.p;++p->RefCount;}}}