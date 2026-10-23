// 006c2d40 _STL::vector<CLevelLoader::CObject::CLevelFiles>::operator=
// Flags: /O2 /Oy /W3
// Leaf: ??4?$vector@UCLevelFiles@@@_STL@@
#include <stdlib.h>
#include <new>
struct CCPPointerInfo{unsigned RefCount;void(__cdecl*DeleteFunc)(void*);void*Data;};
struct CAFile;
template<class T>struct CCountedPointer{T*Data;CCPPointerInfo*Info;
 CCountedPointer():Data(0),Info(0){}
 CCountedPointer(const CCountedPointer&x):Data(x.Data),Info(x.Info){if(Info)++Info->RefCount;}
 ~CCountedPointer(){if(Info&&!--Info->RefCount){Info->DeleteFunc(Info->Data);delete Info;}}
 CCountedPointer&operator=(const CCountedPointer&x){if(this!=&x){CCountedPointer tmp(x);T*p=Data;CCPPointerInfo*i=Info;Data=tmp.Data;Info=tmp.Info;tmp.Data=p;tmp.Info=i;}return*this;}
};
// CLevelFiles is CLevelLoader::CObject::CLevelFiles in the PDB (0x1c bytes: LevelID + three counted file pointers);
// it is declared at file scope because engine/CLevelLoader.h is a flattened layout without the nested types.
// Shape: the STLport 4.x operator= -- the grow arm sets _M_start then _M_end_of_storage = _M_start + n and all
// three arms share the trailing `_M_finish = _M_start + n` (retail tail-duplicates it per arm).
// Callees: __uninitialized_copy 0x006C2AA0, vector::_M_clear 0x005196A0, __copy (assign) 0x006C2B10, _Destroy 0x00518B80.
struct CLevelFiles{long LevelID;CCountedPointer<CAFile>PTopologyFile,PNavigationFile,PThingFile;};
namespace _STL{
struct __false_type{__false_type(){}};
template<class Tag>__declspec(noinline) CLevelFiles*__fastcall uninitialized_copy(CLevelFiles*a,CLevelFiles*b,CLevelFiles*c,const Tag&){while(a!=b){if(c)new(c)CLevelFiles(*a);++a;++c;}return c;}
template<class Tag>__declspec(noinline) CLevelFiles*__fastcall copy(CLevelFiles*a,CLevelFiles*b,CLevelFiles*c,const Tag&,int){while(a!=b)*c++=*a++;return c;}
__declspec(noinline) void __fastcall _Destroy(CLevelFiles*a,CLevelFiles*b){while(a!=b)(a++)->~CLevelFiles();}
template<class T>struct vector{T*_M_start,*_M_finish,*_M_end_of_storage;void _M_clear();__forceinline T*_M_allocate_and_copy(unsigned,T*,T*);vector&operator=(const vector&);};
template<class T>__declspec(noinline)void vector<T>::_M_clear(){_Destroy(_M_start,_M_finish);free(_M_start);}
template<class T>__forceinline T*vector<T>::_M_allocate_and_copy(unsigned n,T*a,T*b){T*fresh=n?(T*)malloc(n*sizeof(T)):0;uninitialized_copy(a,b,fresh,__false_type());return fresh;}
template<class T>vector<T>&vector<T>::operator=(const vector&rhs){
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
template struct vector<CLevelFiles>;
}