// 0052bff0 std::vector<NThingManager::CEntityRuntimePersistence::CEntityScriptInfo>::operator=
// Flags: /O2 /Oy /W3 
// Leaf: ??4?$vector@UCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@
#include <stdlib.h>
#include <new>
namespace X_0052bff0 {
// Layout: 0052b5e0 assigns CCharString+0, nested vector+0x04, scalar+0x10,
// bool+0x14, stride0x18; vector pointers+0x00,+0x04,+0x08.
// Nested byte-vector verified by 005295b0 byte-count allocation and memmove.
struct CCharString{void*p;CCharString();CCharString(const CCharString&);~CCharString();CCharString&operator=(const CCharString&);};
template<class T>struct CArray;
template<>struct CArray<unsigned char>{unsigned char*first,*last,*limit;CArray();CArray(const CArray&);~CArray();CArray&operator=(const CArray&);};
typedef CArray<unsigned char> ByteVector;
namespace NThingManager {struct CEntityRuntimePersistence {struct CEntityScriptInfo{CCharString Script;ByteVector SaveData;long UncompressedSize;bool Valid;CEntityScriptInfo():UncompressedSize(0),Valid(false){} CEntityScriptInfo(const CEntityScriptInfo&x):Script(x.Script),SaveData(x.SaveData),UncompressedSize(x.UncompressedSize),Valid(x.Valid){} CEntityScriptInfo&operator=(const CEntityScriptInfo&x){Script=x.Script;SaveData=x.SaveData;UncompressedSize=x.UncompressedSize;Valid=x.Valid;return *this;}};};}
typedef NThingManager::CEntityRuntimePersistence::CEntityScriptInfo CEntityScriptInfo;
struct __false_type{__false_type(){}};
template<class EmptyTag> __declspec(noinline) CEntityScriptInfo* __fastcall uninitialized_copy(CEntityScriptInfo*a,CEntityScriptInfo*b,CEntityScriptInfo*out,const EmptyTag&){while(a!=b){new(out)CEntityScriptInfo(*a);++a;++out;}return out;}
template<class EmptyTag> __declspec(noinline) CEntityScriptInfo* __fastcall copy(CEntityScriptInfo*a,CEntityScriptInfo*b,CEntityScriptInfo*out,const EmptyTag&,bool){int n=b-a;while(n>0){*out=*a;++a;++out;--n;}return out;}
template<class EmptyTag> __declspec(noinline) void __fastcall destroy(CEntityScriptInfo*a,CEntityScriptInfo*b,const EmptyTag&){while(a!=b){a->~CEntityScriptInfo();++a;}}
namespace std {
template<class T>struct vector{T*first,*last,*limit;vector&operator=(const vector&);};
template<class T>vector<T>&vector<T>::operator=(const vector&rhs){
 if(&rhs!=this){T*end=rhs.last;T*begin=rhs.first;unsigned n=end-begin;
 if(n>unsigned(limit-first)){T*p=n?(T*)malloc(n*sizeof(T)):0;__false_type t;uninitialized_copy(begin,end,p,t);destroy(first,last,t);if(first)free(first);first=p;limit=p+n;}
 else {
 unsigned old=last-first;
 if(old>=n){__false_type t;T*p=copy(begin,end,first,t,false);destroy(p,last,t);}
 else {__false_type t;copy(begin,begin+old,first,t,false);uninitialized_copy(rhs.first+(last-first),rhs.last,last,t);}
 }
 last=first+n;
 }return *this;
}
template struct vector<CEntityScriptInfo>;
}
using std::vector;
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
namespace X_0052bff0 {
struct Storage{int refs;unsigned value;};static int liveStrings,liveArrays;
CCharString::CCharString():p(0){} CCharString::CCharString(const CCharString&x):p(x.p){if(p)++((Storage*)p)->refs;}CCharString::~CCharString(){if(p&&!--((Storage*)p)->refs){delete(Storage*)p;--liveStrings;}}
CCharString&CCharString::operator=(const CCharString&x){if(p!=x.p){if(p&&!--((Storage*)p)->refs){delete(Storage*)p;--liveStrings;}p=x.p;if(p)++((Storage*)p)->refs;}return *this;}
CArray<unsigned char>::CArray():first(0),last(0),limit(0){} CArray<unsigned char>::CArray(const ByteVector&x):first(0),last(0),limit(0){*this=x;}CArray<unsigned char>::~CArray(){if(first){free(first);--liveArrays;}}
CArray<unsigned char>&CArray<unsigned char>::operator=(const ByteVector&x){if(this!=&x){unsigned n=x.last-x.first;unsigned char*p=n?(unsigned char*)malloc(n):0;for(unsigned i=0;i<n;++i)p[i]=x.first[i];if(first){free(first);--liveArrays;}first=p;last=limit=p?p+n:0;if(p)++liveArrays;}return *this;}
static void Init(CEntityScriptInfo&e,unsigned n){Storage*p=new Storage;p->refs=1;p->value=n;e.Script.p=p;++liveStrings;e.SaveData.first=(unsigned char*)malloc(3);e.SaveData.last=e.SaveData.limit=e.SaveData.first+3;e.SaveData.first[0]=n*3;e.SaveData.first[1]=n*7;e.SaveData.first[2]=0xe0+n;++liveArrays;e.UncompressedSize=n+100;e.Valid=(n&1)!=0;}
static void Check(const vector<CEntityScriptInfo>&a,const vector<CEntityScriptInfo>&b){assert(a.last-a.first==b.last-b.first);for(int i=0;i<a.last-a.first;++i){assert(a.first[i].Script.p==b.first[i].Script.p);assert(((Storage*)a.first[i].Script.p)->refs==2);assert(a.first[i].SaveData.first!=b.first[i].SaveData.first);assert(a.first[i].SaveData.first[0]==b.first[i].SaveData.first[0]);assert(a.first[i].SaveData.first[1]==b.first[i].SaveData.first[1]);assert(a.first[i].SaveData.last-a.first[i].SaveData.first==3);assert(a.first[i].SaveData.first[2]==b.first[i].SaveData.first[2]);assert(a.first[i].UncompressedSize==b.first[i].UncompressedSize&&a.first[i].Valid==b.first[i].Valid);}}
}
int main(){using namespace X_0052bff0;assert(sizeof(CEntityScriptInfo)==0x18&&offsetof(CEntityScriptInfo,SaveData)==4&&offsetof(CEntityScriptInfo,Valid)==0x14);{CEntityScriptInfo x[3];for(unsigned i=0;i<3;++i)Init(x[i],i+7);vector<CEntityScriptInfo>a,b;a.first=a.last=a.limit=0;b.first=x;b.last=b.limit=x+3;a.operator=(b);Check(a,b);a.operator=(a);Check(a,b);b.last=x+1;a.operator=(b);Check(a,b);assert(((Storage*)x[1].Script.p)->refs==1);b.last=x+3;a.operator=(b);Check(a,b);b.last=x;a.operator=(b);assert(a.first==a.last&&liveArrays==3);free(a.first);}assert(liveStrings==0&&liveArrays==0);puts("0052bff0_TEST PASS");}