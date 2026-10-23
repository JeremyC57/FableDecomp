// 0046f06a _STL::vector<CAbilityButtonGuiDef>::operator=
// Flags: /O2 /Oy /W3 /O1 /Oy-
// Leaf: ??4?$vector@UCAbilityButtonGuiDef@@@_STL@@QAEAAU01@ABU01@@Z
#include <stdlib.h>
#include <new>

// Layout: vtable slot0 is a genuine virtual destructor; opaque payload follows vptr. Retail copy assignment and ctor operate on 92-byte CAbilityButtonGuiDef; active header absent; quarantined donor sizeof0x60 conflicts with retail stride0x5c.
struct CAbilityButtonGuiDef {unsigned data[22];CAbilityButtonGuiDef();CAbilityButtonGuiDef(const CAbilityButtonGuiDef&);virtual ~CAbilityButtonGuiDef();CAbilityButtonGuiDef&operator=(const CAbilityButtonGuiDef&);};

namespace _STL {
struct __false_type{__false_type(){}};
template<class Tag> __declspec(noinline) void __fastcall _Destroy(CAbilityButtonGuiDef*a,CAbilityButtonGuiDef*b,const Tag&){while(a!=b){a->~CAbilityButtonGuiDef();++a;}}
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

template struct vector<CAbilityButtonGuiDef>;
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
static int Alive,Copies,Assigns,Destroyed;

CAbilityButtonGuiDef::CAbilityButtonGuiDef(){memset(data,0,sizeof(data));++Alive;}
CAbilityButtonGuiDef::~CAbilityButtonGuiDef(){assert(Alive>0);--Alive;++Destroyed;}
CAbilityButtonGuiDef::CAbilityButtonGuiDef(const CAbilityButtonGuiDef&v){memcpy(data,v.data,sizeof(data));++Alive;++Copies;}
CAbilityButtonGuiDef&CAbilityButtonGuiDef::operator=(const CAbilityButtonGuiDef&v){memcpy(data,v.data,sizeof(data));++Assigns;return *this;}

namespace _STL {
template<>CAbilityButtonGuiDef*vector<CAbilityButtonGuiDef>::_M_allocate_and_copy(unsigned n,CAbilityButtonGuiDef*a,CAbilityButtonGuiDef*b){CAbilityButtonGuiDef*p=(CAbilityButtonGuiDef*)malloc(n*sizeof(CAbilityButtonGuiDef));CAbilityButtonGuiDef*q=p;while(a!=b){new(q)CAbilityButtonGuiDef(*a);++q;++a;}return p;}
template<>void vector<CAbilityButtonGuiDef>::_M_clear(){for(CAbilityButtonGuiDef*p=_M_start;p!=_M_finish;++p)p->~CAbilityButtonGuiDef();free(_M_start);}
}
int main(){
 {CAbilityButtonGuiDef data[5];for(unsigned i=0;i<5;++i)for(unsigned j=0;j<22;++j)data[i].data[j]=100*i+j;
 _STL::vector<CAbilityButtonGuiDef>a,b;memset(&a,0,sizeof(a));b._M_start=data;b._M_finish=data+3;b._M_end_of_storage=data+5;
 a=b;assert(Alive==8&&Copies==3&&Assigns==0&&Destroyed==0);assert(a._M_finish-a._M_start==3&&a._M_end_of_storage-a._M_start==3&&memcmp(a._M_start,data,3*sizeof(CAbilityButtonGuiDef))==0);
 a=a;assert(Copies==3&&Assigns==0&&Destroyed==0);
 b._M_finish=data+1;a=b;assert(Alive==6&&Assigns==1&&Destroyed==2&&a._M_finish-a._M_start==1);
 b._M_finish=data+3;a=b;assert(Alive==8&&Copies==5&&Assigns==2&&Destroyed==2&&a._M_finish-a._M_start==3);assert(memcmp(a._M_start,data,3*sizeof(CAbilityButtonGuiDef))==0);
 b._M_finish=data+5;a=b;assert(Alive==10&&Copies==10&&Destroyed==5&&a._M_finish-a._M_start==5&&a._M_end_of_storage-a._M_start==5);assert(memcmp(a._M_start,data,5*sizeof(CAbilityButtonGuiDef))==0);
 b._M_finish=data;a=b;assert(Alive==5&&Destroyed==10&&a._M_finish==a._M_start);a._M_clear();
 }assert(Alive==0&&Destroyed==15);puts("0046f06a_TEST PASS");}