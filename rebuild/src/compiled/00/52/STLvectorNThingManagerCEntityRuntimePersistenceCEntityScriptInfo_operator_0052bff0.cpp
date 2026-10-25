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