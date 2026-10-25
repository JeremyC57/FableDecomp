// 00cb6fd0 std::vector<std::pair<CCharString,CScriptInfo> >::_M_insert_overflow
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
namespace std{template<class A,class B>struct pair{A first;B second;pair(){}__forceinline pair(const pair&r):first(r.first),second(r.second){}};}
typedef std::pair<CCharString,CScriptInfo> ScriptPair;
struct __false_type{__false_type(){}};
template<class Tag>__declspec(noinline) ScriptPair*__fastcall uninitialized_copy(ScriptPair*a,ScriptPair*b,ScriptPair*c,const Tag&){while(a!=b){if(c)new(c)ScriptPair(*a);++a;++c;}return c;}
template<class Tag>__declspec(noinline) ScriptPair*__fastcall uninitialized_fill_n(ScriptPair*a,unsigned n,const ScriptPair&v,const Tag&){while(n--){if(a)new(a)ScriptPair(v);++a;}return a;}
template<class T>inline const T& max(const T&a,const T&b){return a<b?b:a;}
namespace std{
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