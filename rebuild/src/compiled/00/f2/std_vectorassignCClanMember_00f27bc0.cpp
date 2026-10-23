// 00f27bc0 Vector::Assign
// Flags: /O2 /Oy /W3
// Leaf: Assign
// Layout: PDB42943-42983 HeroSoulsWaveDef60/debug ->56/retail, padded CDefClassBase37->40, NumWaveCreatures40, vector44; nested CreatureDef52 has two String wrappers40/44 and count48. f27d10 Tidy writes no vector fields; f27d50 allocation also leaves receiver unchanged.
#include <new>
#include "engine/CDefClassBase.h"
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
namespace N00f27bc0 {
struct FalseTag{};
struct StringData;
struct String {StringData* Data;String();String(const String&);~String();String& operator=(const String&);};
// Layout: vector receiver has no active header. Element base vptr0, payload4..24, aligned prefix40.
// Layout: use compiler-generated virtual destruction; actual base payload agrees with CDefClassBase offsets.
#pragma pack(push,1)
struct DefPayload {unsigned char _base_0x04[24];CDefinitionManager* PDefManager;unsigned GlobalIndex;unsigned char DefaultValsApplied[1];};
struct DefClassStorage {virtual ~DefClassStorage();DefPayload Base;unsigned char Padding[3];DefClassStorage();DefClassStorage(const DefClassStorage&);DefClassStorage& operator=(const DefClassStorage&);};
#pragma pack(pop)
struct CreatureDef:DefClassStorage {String CreatureType,Location;int NumCreatures;CreatureDef();CreatureDef(const CreatureDef&);~CreatureDef();CreatureDef& operator=(const CreatureDef&);};
struct CreatureVector {CreatureDef* Start;CreatureDef* Finish;CreatureDef* End;CreatureVector();CreatureVector(const CreatureVector&);~CreatureVector();CreatureVector& operator=(const CreatureVector&);};
struct Element:DefClassStorage {int NumWaveCreatures;CreatureVector Creatures;Element();Element(const Element&);~Element();Element& operator=(const Element&);};
#pragma auto_inline(off)
Element* __fastcall Copy(const Element*,const Element*,Element*,const FalseTag&);
__declspec(noinline) void __fastcall Construct(Element* out,const Element* input){if(out)new(out)Element(*input);}
#pragma auto_inline(on)
template<class T>struct Vector {
 T* Start;T* Finish;T* End;
 T* AllocateCopy(unsigned int,T*,T*);void Tidy();Vector& Assign(const Vector&);
};
#pragma auto_inline(off)
template<class T>__declspec(noinline) T* Vector<T>::AllocateCopy(unsigned int n,T* first,T* last){T* allocation=n?(T*)malloc(n*sizeof(T)):0;T* out=allocation;for(;first!=last;++first,++out)Construct(out,first);return allocation;}
template<class T>__declspec(noinline) void Vector<T>::Tidy(){T* first=Start;T* last=Finish;for(;first!=last;++first)first->~T();T* allocation=Start;if(allocation)free(allocation);}
#pragma auto_inline(on)
template<class T> __forceinline void DestroyRange(T* first,T* last){for(;first!=last;++first)first->~T();}
template<class T>Vector<T>& Vector<T>::Assign(const Vector& other){
 if(&other!=this){
  const unsigned int n=other.Finish-other.Start;
  T* first=Start;
  if(n>(unsigned int)(End-first)){
   T* p=AllocateCopy(n,other.Start,other.Finish);Tidy();Start=p;End=Start+n;
  }else if((unsigned int)(Finish-first)>=n){
   T* p=Copy(other.Start,other.Finish,first,FalseTag());
   DestroyRange(p,Finish);
  }else{
   Copy(other.Start,other.Start+(Finish-first),first,FalseTag());
   T* last=other.Finish;T* out=Finish;T* p=other.Start+(out-Start);
   for(;p!=last;++p,++out)Construct(out,p);
  }
  Finish=Start+n;
 }
 return *this;
}
template Vector<Element>& Vector<Element>::Assign(const Vector&);
}