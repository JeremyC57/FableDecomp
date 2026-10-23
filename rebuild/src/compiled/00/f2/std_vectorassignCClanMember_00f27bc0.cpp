// 00f27bc0 N00f27bc0::vector<CHeroSoulsWaveDef>::operator= (STLport; catalog vector_assign_CClanMember is wrong: element layout is PDB CHeroSoulsWaveDef)
// Flags: /O2 /Oy /W3
// Leaf: ??4
// Layout: PDB42943-42983 HeroSoulsWaveDef60/debug ->56/retail, padded CDefClassBase37->40, NumWaveCreatures40, vector44; nested CreatureDef52 has two CCharString wrappers40/44 and count48. f27d10 _M_clear writes no vector fields; f27d50 allocation also leaves receiver unchanged.
#include <new>
#include "engine/CDefClassBase.h"
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
namespace N00f27bc0 {
struct __false_type{};
struct CCharStringData;
struct CCharString {CCharStringData* Data;CCharString();CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
// Layout: vector receiver has no active header. CHeroSoulsWaveDef base vptr0, payload4..24, aligned prefix40.
// Layout: use compiler-generated virtual destruction; actual base payload agrees with CDefClassBase offsets.
#pragma pack(push,1)
struct DefPayload {unsigned char _base_0x04[24];CDefinitionManager* PDefManager;unsigned GlobalIndex;unsigned char DefaultValsApplied[1];};
struct DefClassStorage {virtual ~DefClassStorage();DefPayload Base;unsigned char Padding[3];DefClassStorage();DefClassStorage(const DefClassStorage&);DefClassStorage& operator=(const DefClassStorage&);};
#pragma pack(pop)
struct CHeroSoulsCreatureDef:DefClassStorage {CCharString CreatureType,Location;int NumCreatures;CHeroSoulsCreatureDef();CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef&);~CHeroSoulsCreatureDef();CHeroSoulsCreatureDef& operator=(const CHeroSoulsCreatureDef&);};
struct CreatureVector {CHeroSoulsCreatureDef* _M_start;CHeroSoulsCreatureDef* _M_finish;CHeroSoulsCreatureDef* _M_end_of_storage;CreatureVector();CreatureVector(const CreatureVector&);~CreatureVector();CreatureVector& operator=(const CreatureVector&);};
struct CHeroSoulsWaveDef:DefClassStorage {int NumWaveCreatures;CreatureVector Creatures;CHeroSoulsWaveDef();CHeroSoulsWaveDef(const CHeroSoulsWaveDef&);~CHeroSoulsWaveDef();CHeroSoulsWaveDef& operator=(const CHeroSoulsWaveDef&);};
#pragma auto_inline(off)
CHeroSoulsWaveDef* __fastcall __copy_ptrs(const CHeroSoulsWaveDef*,const CHeroSoulsWaveDef*,CHeroSoulsWaveDef*,const __false_type&);
__declspec(noinline) void __fastcall _Construct(CHeroSoulsWaveDef* out,const CHeroSoulsWaveDef* input){if(out)new(out)CHeroSoulsWaveDef(*input);}
#pragma auto_inline(on)
template<class T>struct vector {
 T* _M_start;T* _M_finish;T* _M_end_of_storage;
 T* _M_allocate_and_copy(unsigned int,T*,T*);void _M_clear();vector& operator=(const vector&);
};
#pragma auto_inline(off)
template<class T>__declspec(noinline) T* vector<T>::_M_allocate_and_copy(unsigned int n,T* first,T* last){T* allocation=n?(T*)malloc(n*sizeof(T)):0;T* out=allocation;for(;first!=last;++first,++out)_Construct(out,first);return allocation;}
template<class T>__declspec(noinline) void vector<T>::_M_clear(){T* first=_M_start;T* last=_M_finish;for(;first!=last;++first)first->~T();T* allocation=_M_start;if(allocation)free(allocation);}
#pragma auto_inline(on)
template<class T> __forceinline void _Destroy(T* first,T* last){for(;first!=last;++first)first->~T();}
template<class T>vector<T>& vector<T>::operator=(const vector& other){
 if(&other!=this){
  const unsigned int n=other._M_finish-other._M_start;
  T* first=_M_start;
  if(n>(unsigned int)(_M_end_of_storage-first)){
   T* p=_M_allocate_and_copy(n,other._M_start,other._M_finish);_M_clear();_M_start=p;_M_end_of_storage=_M_start+n;
  }else if((unsigned int)(_M_finish-first)>=n){
   T* p=__copy_ptrs(other._M_start,other._M_finish,first,__false_type());
   _Destroy(p,_M_finish);
  }else{
   __copy_ptrs(other._M_start,other._M_start+(_M_finish-first),first,__false_type());
   T* last=other._M_finish;T* out=_M_finish;T* p=other._M_start+(out-_M_start);
   for(;p!=last;++p,++out)_Construct(out,p);
  }
  _M_finish=_M_start+n;
 }
 return *this;
}
template vector<CHeroSoulsWaveDef>& vector<CHeroSoulsWaveDef>::operator=(const vector&);
}