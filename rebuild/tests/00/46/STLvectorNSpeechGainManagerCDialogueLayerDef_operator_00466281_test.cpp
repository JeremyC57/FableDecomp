// 00466281 _STL::vector<NSpeechGainManager::CDialogueLayerDef>::operator=
// Flags: /O2 /Oy /W3 /O1 /Oy-
// Leaf: ??4?$vector@UCDialogueLayerDef@NSpeechGainManager@@@_STL@@QAEAAU01@ABU01@@Z
#include <stdlib.h>
#include <new>
namespace NSpeechGainManager {
// Layout: vtable slot0 is a genuine virtual destructor; opaque payload follows vptr. Retail copy assignment and ctor operate on 64-byte CDialogueLayerDef; no engine header exists.
struct CDialogueLayerDef {unsigned data[15];CDialogueLayerDef();CDialogueLayerDef(const CDialogueLayerDef&);virtual ~CDialogueLayerDef();CDialogueLayerDef&operator=(const CDialogueLayerDef&);};
}
namespace _STL {
struct __false_type{__false_type(){}};
template<class Tag> __declspec(noinline) void __fastcall _Destroy(NSpeechGainManager::CDialogueLayerDef*a,NSpeechGainManager::CDialogueLayerDef*b,const Tag&){while(a!=b){a->~CDialogueLayerDef();++a;}}
template<class T>__forceinline T*_Copy(T*first,T*last,T*out){for(int n=last-first;n>0;--n,++first,++out)*out=*first;return out;}
template<class T>__forceinline void _Construct(T*p,const T&v){if(p)new(p)T(v);}
template<class T>__forceinline void _UninitializedCopy(T*out,T*first,T*last){while(first!=last){_Construct(out,*first++);++out;}}
template<class T>struct vector{T*_M_start,*_M_finish,*_M_end_of_storage;
 T*_M_allocate_and_copy(unsigned,T*,T*);void _M_clear();vector&operator=(const vector&);
};
template<class T>vector<T>&vector<T>::operator=(const vector&rhs){
 if(&rhs!=this){unsigned count=rhs._M_finish-rhs._M_start;
  if(count>unsigned(_M_end_of_storage-_M_start)){T*fresh=_M_allocate_and_copy(count,rhs._M_start,rhs._M_finish);_M_clear();_M_start=fresh;_M_end_of_storage=fresh+count;}
  else if(unsigned(_M_finish-_M_start)>=count){_Destroy(_Copy(rhs._M_start,rhs._M_finish,_M_start),_M_finish,__false_type());}
  else{_Copy(rhs._M_start,rhs._M_start+(_M_finish-_M_start),_M_start);_UninitializedCopy(_M_finish,rhs._M_start+(_M_finish-_M_start),rhs._M_finish);}
  _M_finish=_M_start+count;
 }return *this;
}

template struct vector<NSpeechGainManager::CDialogueLayerDef>;
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
static int Alive,Copies,Assigns,Destroyed;
namespace NSpeechGainManager {
CDialogueLayerDef::CDialogueLayerDef(){memset(data,0,sizeof(data));++Alive;}
CDialogueLayerDef::~CDialogueLayerDef(){assert(Alive>0);--Alive;++Destroyed;}
CDialogueLayerDef::CDialogueLayerDef(const CDialogueLayerDef&v){memcpy(data,v.data,sizeof(data));++Alive;++Copies;}
CDialogueLayerDef&CDialogueLayerDef::operator=(const CDialogueLayerDef&v){memcpy(data,v.data,sizeof(data));++Assigns;return *this;}
}
namespace _STL {
template<>NSpeechGainManager::CDialogueLayerDef*vector<NSpeechGainManager::CDialogueLayerDef>::_M_allocate_and_copy(unsigned n,NSpeechGainManager::CDialogueLayerDef*a,NSpeechGainManager::CDialogueLayerDef*b){NSpeechGainManager::CDialogueLayerDef*p=(NSpeechGainManager::CDialogueLayerDef*)malloc(n*sizeof(NSpeechGainManager::CDialogueLayerDef));NSpeechGainManager::CDialogueLayerDef*q=p;while(a!=b){new(q)NSpeechGainManager::CDialogueLayerDef(*a);++q;++a;}return p;}
template<>void vector<NSpeechGainManager::CDialogueLayerDef>::_M_clear(){for(NSpeechGainManager::CDialogueLayerDef*p=_M_start;p!=_M_finish;++p)p->~CDialogueLayerDef();free(_M_start);}
}
int main(){
 {NSpeechGainManager::CDialogueLayerDef data[5];for(unsigned i=0;i<5;++i)for(unsigned j=0;j<15;++j)data[i].data[j]=100*i+j;
 _STL::vector<NSpeechGainManager::CDialogueLayerDef>a,b;memset(&a,0,sizeof(a));b._M_start=data;b._M_finish=data+3;b._M_end_of_storage=data+5;
 a=b;assert(Alive==8&&Copies==3&&Assigns==0&&Destroyed==0);assert(a._M_finish-a._M_start==3&&a._M_end_of_storage-a._M_start==3&&memcmp(a._M_start,data,3*sizeof(NSpeechGainManager::CDialogueLayerDef))==0);
 a=a;assert(Copies==3&&Assigns==0&&Destroyed==0);
 b._M_finish=data+1;a=b;assert(Alive==6&&Assigns==1&&Destroyed==2&&a._M_finish-a._M_start==1);
 b._M_finish=data+3;a=b;assert(Alive==8&&Copies==5&&Assigns==2&&Destroyed==2&&a._M_finish-a._M_start==3);assert(memcmp(a._M_start,data,3*sizeof(NSpeechGainManager::CDialogueLayerDef))==0);
 b._M_finish=data+5;a=b;assert(Alive==10&&Copies==10&&Destroyed==5&&a._M_finish-a._M_start==5&&a._M_end_of_storage-a._M_start==5);assert(memcmp(a._M_start,data,5*sizeof(NSpeechGainManager::CDialogueLayerDef))==0);
 b._M_finish=data;a=b;assert(Alive==5&&Destroyed==10&&a._M_finish==a._M_start);a._M_clear();
 }assert(Alive==0&&Destroyed==15);puts("00466281_TEST PASS");}