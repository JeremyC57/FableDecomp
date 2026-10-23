// 00cb6fd0 _STL::vector<_STL::pair<CCharString,CScriptInfo> >::_M_insert_overflow
// Flags: /O2 /Oy /W3
// Leaf: _M_insert_overflow
#include <new>
#include <stdlib.h>
namespace ScriptVector {
struct CCharStringData;
// Layout: CCharString is one refcounted data pointer; retail copy99ec30. CScriptInfo PDB size20 offsets0,4,8,12,16. Pair is24 bytes.
struct CCharString{CCharStringData*PStringData;CCharString();CCharString(const CCharString&);~CCharString();};
struct CScriptBase;struct CScriptDataBase;struct CGameScriptInterfaceBase;
struct CScriptInfo{CScriptBase*(__cdecl*AllocFunc)(CScriptDataBase*,CGameScriptInterfaceBase*);CScriptDataBase*(__cdecl*MasterScriptDataAlloc)();CCharString Name;long ID;bool MasterScript;CScriptInfo(){}__forceinline CScriptInfo(const CScriptInfo&r):AllocFunc(r.AllocFunc),MasterScriptDataAlloc(r.MasterScriptDataAlloc),Name(r.Name),ID(r.ID),MasterScript(r.MasterScript){}};
namespace _STL{template<class A,class B>struct pair{A first;B second;pair(){}__forceinline pair(const pair&r):first(r.first),second(r.second){}};}
typedef _STL::pair<CCharString,CScriptInfo> ScriptPair;
struct __false_type{__false_type(){}};
template<class Tag>__declspec(noinline) ScriptPair*__fastcall uninitialized_copy(ScriptPair*a,ScriptPair*b,ScriptPair*c,const Tag&){while(a!=b){if(c)new(c)ScriptPair(*a);++a;++c;}return c;}
template<class Tag>__declspec(noinline) ScriptPair*__fastcall uninitialized_fill_n(ScriptPair*a,unsigned n,const ScriptPair&v,const Tag&){while(n--){if(a)new(a)ScriptPair(v);++a;}return a;}
template<class T>inline const T& max(const T&a,const T&b){return a<b?b:a;}
namespace _STL{
// Layout: retail STLport vector start0, finish4, storage8.
template<class T>struct vector{T*_M_start,*_M_finish,*_M_end_of_storage;void _M_clear();void _M_insert_overflow(T*,const T&,const __false_type&,unsigned,bool);};
template<class T>void vector<T>::_M_insert_overflow(T*pos,const T&value,const __false_type&,unsigned n,bool atEnd){
 const unsigned old=_M_finish-_M_start;const unsigned capacity=old+max(old,n);
 T*buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T*out=uninitialized_copy(_M_start,pos,buffer,__false_type());
 if(n==1){if(out)new(out)T(value);++out;}else out=uninitialized_fill_n(out,n,value,__false_type());
 if(!atEnd)out=uninitialized_copy(pos,_M_finish,out,__false_type());
 _M_clear();_M_start=buffer;_M_finish=out;_M_end_of_storage=buffer+capacity;
}
template struct vector<ScriptPair>;
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace ScriptVector{
struct CCharStringData{unsigned refs;char text[16];};
CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString&r):PStringData(r.PStringData){if(PStringData)++PStringData->refs;}CCharString::~CCharString(){if(PStringData&&!--PStringData->refs)delete PStringData;}
namespace _STL{template<>void vector<ScriptPair>::_M_clear(){for(ScriptPair*p=_M_start;p!=_M_finish;++p)p->~ScriptPair();free(_M_start);}}
struct CScriptBase{int value;};struct CScriptDataBase{int value;};struct CGameScriptInterfaceBase{int value;};
CScriptBase*__cdecl Allocate(CScriptDataBase*a,CGameScriptInterfaceBase*b){static CScriptBase s;s.value=a->value+b->value;return &s;}CScriptDataBase*__cdecl Master(){static CScriptDataBase d;d.value=13;return &d;}
void Set(CCharString&s,const char*t){s.PStringData=new CCharStringData;s.PStringData->refs=1;strcpy(s.PStringData->text,t);}
}
int main(){using namespace ScriptVector;ScriptPair seed;Set(seed.first,"registered");Set(seed.second.Name,"master");seed.second.ID=7;seed.second.MasterScript=true;seed.second.AllocFunc=Allocate;seed.second.MasterScriptDataAlloc=Master;_STL::vector<ScriptPair>v;memset(&v,0,sizeof(v));__false_type tag;
v._M_insert_overflow(v._M_start,seed,tag,1,false);assert(v._M_finish-v._M_start==1&&seed.first.PStringData->refs==2&&seed.second.Name.PStringData->refs==2);
v._M_insert_overflow(v._M_start,seed,tag,2,false);assert(v._M_finish-v._M_start==3&&seed.first.PStringData->refs==4);assert(v._M_start[2].second.ID==7&&v._M_start[2].second.MasterScript&&strcmp(v._M_start[2].second.Name.PStringData->text,"master")==0);CGameScriptInterfaceBase api;api.value=2;assert(v._M_start[1].second.AllocFunc(v._M_start[1].second.MasterScriptDataAlloc(),&api)->value==15);
v._M_insert_overflow(v._M_finish,seed,tag,0,true);assert(v._M_finish-v._M_start==3&&seed.first.PStringData->refs==4);v._M_insert_overflow(v._M_finish,seed,tag,1,true);assert(v._M_finish-v._M_start==4&&seed.second.Name.PStringData->refs==5);v._M_clear();assert(seed.first.PStringData->refs==1&&seed.second.Name.PStringData->refs==1);puts("00cb6fd0_TEST PASS");return 0;}