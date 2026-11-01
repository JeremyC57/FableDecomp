// 006c2d40 std::vector<CLevelLoader::CObject::CLevelFiles>::operator=
// Flags: /O2 /Oy /W3
// Leaf: ??4?$vector@VCLevelFiles@CObject@CLevelLoader@@V?$allocator@VCLevelFiles@CObject@CLevelLoader@@@std@@@std@@QAEAAV01@ABV01@@Z
#include <stdlib.h>
#include <new>
class CCPPointerInfo{ public:unsigned RefCount;void(__cdecl*DeleteFunc)(void*);void*Data;};
class CAFile;
template<class T>class CCountedPointer{ public:T*Data;CCPPointerInfo*Info;
 CCountedPointer():Data(0),Info(0){}
 CCountedPointer(const CCountedPointer&x):Data(x.Data),Info(x.Info){if(Info)++Info->RefCount;}
 ~CCountedPointer(){if(Info&&!--Info->RefCount){Info->DeleteFunc(Info->Data);delete Info;}}
 CCountedPointer&operator=(const CCountedPointer&x){if(this!=&x){CCountedPointer tmp(x);T*p=Data;CCPPointerInfo*i=Info;Data=tmp.Data;Info=tmp.Info;tmp.Data=p;tmp.Info=i;}return*this;}
};
// CLevelFiles is CLevelLoader::CObject::CLevelFiles in the PDB (0x1c bytes: LevelID + three counted file pointers), nested here
// (naming wave 20, chunk 688: ??1CLevelFiles@CObject@CLevelLoader@@QAE@XZ); the vector carries its allocator parameter (the FableWin spelling).
// Shape: the STLport 4.x operator= -- the grow arm sets _M_start then _M_end_of_storage = _M_start + n and all
// three arms share the trailing `_M_finish = _M_start + n` (retail tail-duplicates it per arm).
// Callees: __uninitialized_copy 0x006C2AA0, vector::_M_clear 0x005196A0, __copy (assign) 0x006C2B10, _Destroy 0x00518B80.
class CLevelLoader{public:class CObject{public:class CLevelFiles{public:long LevelID;CCountedPointer<CAFile>PTopologyFile,PNavigationFile,PThingFile;};};};
typedef CLevelLoader::CObject::CLevelFiles CLevelFiles;
namespace std{
struct __false_type{__false_type(){}};
template<class Tag>__declspec(noinline) CLevelFiles*__fastcall uninitialized_copy(CLevelFiles*a,CLevelFiles*b,CLevelFiles*c,const Tag&){while(a!=b){if(c)new(c)CLevelFiles(*a);++a;++c;}return c;}
template<class Tag>__declspec(noinline) CLevelFiles*__fastcall copy(CLevelFiles*a,CLevelFiles*b,CLevelFiles*c,const Tag&,int){while(a!=b)*c++=*a++;return c;}
__declspec(noinline) void __fastcall _Destroy(CLevelFiles*a,CLevelFiles*b){while(a!=b)(a++)->~CLevelFiles();}
template<class T>class allocator{};
template<class T,class A=allocator<T> >class vector{public:T*_M_start,*_M_finish,*_M_end_of_storage;void _M_clear();__forceinline T*_M_allocate_and_copy(unsigned,T*,T*);vector&operator=(const vector&);};
template<class T,class A>__declspec(noinline)void vector<T,A>::_M_clear(){_Destroy(_M_start,_M_finish);free(_M_start);}
template<class T,class A>__forceinline T*vector<T,A>::_M_allocate_and_copy(unsigned n,T*a,T*b){T*fresh=n?(T*)malloc(n*sizeof(T)):0;uninitialized_copy(a,b,fresh,__false_type());return fresh;}
template<class T,class A>vector<T,A>&vector<T,A>::operator=(const vector&rhs){
 if(&rhs!=this){
  const unsigned count=rhs._M_finish-rhs._M_start;
  if(count>unsigned(_M_end_of_storage-_M_start)){
   T*fresh=_M_allocate_and_copy(count,rhs._M_start,rhs._M_finish);
   _M_clear();
   _M_start=fresh;
   _M_end_of_storage=_M_start+count;
  }
  else if(unsigned(_M_finish-_M_start)>=count){
   T*copied=copy(rhs._M_start,rhs._M_finish,_M_start,__false_type(),0);
   _Destroy(copied,_M_finish);
  }
  else{
   copy(rhs._M_start,rhs._M_start+(_M_finish-_M_start),_M_start,__false_type(),0);
   uninitialized_copy(rhs._M_start+(_M_finish-_M_start),rhs._M_finish,_M_finish,__false_type());
  }
  _M_finish=_M_start+count;
 }
 return*this;
}
template class vector<CLevelFiles>;
}