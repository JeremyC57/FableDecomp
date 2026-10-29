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
} // (hoisted to global scope: naming wave 9)
class CCharString{ public:X_004fe630::CCharStringData*p;X_004fe630::CCharStringData*AllocStringData(const char*,int);__declspec(noinline) void UnassignString(){if(p&&--p->RefCount<=0)delete p;p=0;}__declspec(noinline) CCharString():p(0){++X_004fe630::CCharStringInstances;}__declspec(noinline) CCharString(const char*s,long n=-1):p(0){++X_004fe630::CCharStringInstances;if(s&&*s)p=AllocStringData(s,n);}__declspec(noinline) CCharString(const CCharString&s);__declspec(noinline) ~CCharString(){UnassignString();--X_004fe630::CCharStringInstances;}__declspec(noinline) const CCharString&operator=(const CCharString&s){if(this!=&s){if(p)UnassignString();if(s.p){p=s.p;++p->RefCount;}}return *this;}__declspec(noinline) void LoadFromFile(X_004fe630::CAFile*,void*,int);__forceinline bool Equals(const char*s)const{bool equal;if(p){equal=p->Data==s;}else{equal=strcmp(s,"")==0;}return equal;}};
namespace X_004fe630 {
// Layout: 004b9370 tree node parent+4,left+8,right+c,key CCharString+10.
} void CCharString::LoadFromFile(X_004fe630::CAFile*file,void*context,int length){UnassignString();if(length==-1)length=file->GetFileSize()-file->GetFilePosition();p=new X_004fe630::CCharStringData(length,' ');if(p&&length>0)file->ReadRaw(p->Data.PData,length,context);} namespace X_004fe630 {
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