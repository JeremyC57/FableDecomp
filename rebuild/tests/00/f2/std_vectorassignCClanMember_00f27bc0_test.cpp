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

#include <cstdio>
#include <cstring>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace N00f27bc0 {
__declspec(noinline) Element* __fastcall Copy(const Element* first,const Element* last,Element* out,const FalseTag&){for(;first!=last;++first,++out)*out=*first;return out;}
#pragma pack(push,1)
struct StringData {char* Text;unsigned char Other[9];int Refs;};
#pragma pack(pop)
static int liveBlocks,liveCreatures,liveWaves,liveCreatureStorage;
String::String():Data(0){}String::String(const String& o):Data(o.Data){if(Data)++Data->Refs;}
void Release(StringData* p){if(p&&--p->Refs<=0){delete[]p->Text;delete p;--liveBlocks;}}
String::~String(){Release(Data);}String& String::operator=(const String& o){if(Data!=o.Data){if(o.Data)++o.Data->Refs;Release(Data);Data=o.Data;}return *this;}
void Set(String& s,const char* text){String fresh;fresh.Data=new StringData;std::memset(fresh.Data,0,sizeof *fresh.Data);unsigned int n=(unsigned int)std::strlen(text);fresh.Data->Text=new char[n+1];std::memcpy(fresh.Data->Text,text,n+1);fresh.Data->Refs=1;++liveBlocks;s=fresh;}
DefClassStorage::~DefClassStorage(){}
DefClassStorage::DefClassStorage(){std::memset(&Base,0,sizeof Base);std::memset(Padding,0,sizeof Padding);}
DefClassStorage::DefClassStorage(const DefClassStorage& o){std::memset(&Base,0,sizeof Base);std::memcpy(Base._base_0x04,o.Base._base_0x04,sizeof Base._base_0x04);Base.PDefManager=o.Base.PDefManager;Base.GlobalIndex=o.Base.GlobalIndex;Base.DefaultValsApplied[0]=(unsigned char)(o.Base.DefaultValsApplied[0]&7);std::memset(Padding,0,sizeof Padding);}
DefClassStorage& DefClassStorage::operator=(const DefClassStorage& o){std::memcpy(Base._base_0x04,o.Base._base_0x04,sizeof Base._base_0x04);Base.PDefManager=o.Base.PDefManager;Base.GlobalIndex=o.Base.GlobalIndex;Base.DefaultValsApplied[0]=(unsigned char)((Base.DefaultValsApplied[0]&~7)|(o.Base.DefaultValsApplied[0]&7));return *this;}
CreatureDef::CreatureDef():NumCreatures(0){++liveCreatures;}CreatureDef::CreatureDef(const CreatureDef& o):DefClassStorage(o),CreatureType(o.CreatureType),Location(o.Location),NumCreatures(o.NumCreatures){++liveCreatures;}CreatureDef::~CreatureDef(){--liveCreatures;}CreatureDef& CreatureDef::operator=(const CreatureDef& o){DefClassStorage::operator=(o);CreatureType=o.CreatureType;Location=o.Location;NumCreatures=o.NumCreatures;return *this;}
CreatureVector::CreatureVector():Start(0),Finish(0),End(0){}
CreatureVector::CreatureVector(const CreatureVector& o):Start(0),Finish(0),End(0){unsigned int n=o.Start?(unsigned int)(o.Finish-o.Start):0;if(n){Start=(CreatureDef*)malloc(n*sizeof(CreatureDef));Finish=Start;End=Start+n;++liveCreatureStorage;for(CreatureDef* p=o.Start;p!=o.Finish;++p,++Finish)new(Finish)CreatureDef(*p);}}
void Clear(CreatureVector& v){for(CreatureDef* p=v.Start;p!=v.Finish;++p)p->~CreatureDef();if(v.Start){free(v.Start);--liveCreatureStorage;}}
CreatureVector::~CreatureVector(){Clear(*this);}
CreatureVector& CreatureVector::operator=(const CreatureVector& o){if(this!=&o){unsigned int n=o.Start?(unsigned int)(o.Finish-o.Start):0,size=Start?(unsigned int)(Finish-Start):0,capacity=Start?(unsigned int)(End-Start):0;if(n>capacity){CreatureDef* fresh=(CreatureDef*)malloc(n*sizeof(CreatureDef));CreatureDef* end=fresh;for(CreatureDef* p=o.Start;p!=o.Finish;++p,++end)new(end)CreatureDef(*p);Clear(*this);Start=fresh;Finish=end;End=fresh+n;++liveCreatureStorage;}else{unsigned int common=n<size?n:size;for(unsigned int i=0;i<common;++i)Start[i]=o.Start[i];if(n<size){for(unsigned int j=n;j<size;++j)Start[j].~CreatureDef();}else{for(unsigned int k=size;k<n;++k)new(Start+k)CreatureDef(o.Start[k]);}if(Start)Finish=Start+n;}}return *this;}
Element::Element():NumWaveCreatures(0){++liveWaves;}Element::Element(const Element& o):DefClassStorage(o),NumWaveCreatures(o.NumWaveCreatures),Creatures(o.Creatures){++liveWaves;}Element::~Element(){--liveWaves;}Element& Element::operator=(const Element& o){DefClassStorage::operator=(o);NumWaveCreatures=o.NumWaveCreatures;Creatures=o.Creatures;return *this;}
void Init(Element& e,int id){e.Base.PDefManager=0;e.Base.GlobalIndex=100*id;e.Base.DefaultValsApplied[0]=(unsigned char)(id&7);e.NumWaveCreatures=2;e.Creatures.Start=(CreatureDef*)malloc(2*sizeof(CreatureDef));e.Creatures.Finish=e.Creatures.Start;e.Creatures.End=e.Creatures.Start+2;++liveCreatureStorage;for(int i=0;i<2;++i){new(e.Creatures.Finish)CreatureDef;CreatureDef& v=*e.Creatures.Finish;v.Base.PDefManager=0;v.Base.GlobalIndex=1000*id+10*i;v.Base.DefaultValsApplied[0]=(unsigned char)((id+i)&7);Set(v.CreatureType,"creature_type");Set(v.Location,"spawn_location");v.NumCreatures=10*id+i;++e.Creatures.Finish;}}
struct Owner {Vector<Element> V;int BaseID;Owner(int capacity,int size,int id):BaseID(id){V.Start=(Element*)malloc(capacity*sizeof(Element));V.Finish=V.Start;V.End=V.Start+capacity;Resize(size);}~Owner(){V.Tidy();}void Resize(int n){int size=(int)(V.Finish-V.Start);if(n<size){while(V.Finish!=V.Start+n){--V.Finish;V.Finish->~Element();}}else for(int i=size;i<n;++i){new(V.Finish)Element;Init(*V.Finish,BaseID+i);++V.Finish;}}};
bool Same(const Element& a,const Element& b){if(a.NumWaveCreatures!=b.NumWaveCreatures||a.Creatures.Finish-a.Creatures.Start!=2||a.Creatures.Start==b.Creatures.Start)return false;if(a.Base.PDefManager!=b.Base.PDefManager||a.Base.GlobalIndex!=b.Base.GlobalIndex||a.Base.DefaultValsApplied[0]!=b.Base.DefaultValsApplied[0]||std::memcmp(a.Base._base_0x04,b.Base._base_0x04,24))return false;for(int i=0;i<2;++i){const CreatureDef& x=a.Creatures.Start[i];const CreatureDef& y=b.Creatures.Start[i];if(x.NumCreatures!=y.NumCreatures||x.CreatureType.Data!=y.CreatureType.Data||x.Location.Data!=y.Location.Data||x.CreatureType.Data->Refs!=2||x.Location.Data->Refs!=2||std::strcmp(x.CreatureType.Data->Text,"creature_type")||std::strcmp(x.Location.Data->Text,"spawn_location"))return false;if(x.Base.GlobalIndex!=y.Base.GlobalIndex||x.Base.PDefManager!=y.Base.PDefManager||x.Base.DefaultValsApplied[0]!=y.Base.DefaultValsApplied[0]||std::memcmp(x.Base._base_0x04,y.Base._base_0x04,24))return false;}return true;}
}
int main(){using namespace N00f27bc0;
 if(sizeof(DefClassStorage)!=40||sizeof(CreatureDef)!=52||sizeof(Element)!=56||sizeof(StringData)!=17)return 1;
 {Element e;if((char*)&e.Base.PDefManager-(char*)&e!=28||(char*)&e.Base.GlobalIndex-(char*)&e!=32||(char*)&e.Base.DefaultValsApplied-(char*)&e!=36||(char*)&e.NumWaveCreatures-(char*)&e!=40||(char*)&e.Creatures-(char*)&e!=44)return 12;}
 {Owner source(5,5,10),dest(1,1,50);Element* first=dest.V.Start;Vector<Element>* result=&dest.V.Assign(source.V);if(result!=&dest.V||dest.V.Finish-dest.V.Start!=5||dest.V.End-dest.V.Start!=5||dest.V.Start==first)return 2;for(int i=0;i<5;++i)if(!Same(dest.V.Start[i],source.V.Start[i]))return 3;
 first=dest.V.Start;source.Resize(2);dest.V.Assign(source.V);if(dest.V.Start!=first||dest.V.Finish-first!=2||dest.V.End-first!=5)return 4;for(int j=0;j<2;++j)if(!Same(dest.V.Start[j],source.V.Start[j]))return 5;
 source.Resize(4);dest.V.Assign(source.V);if(dest.V.Start!=first||dest.V.Finish-first!=4||dest.V.End-first!=5)return 6;for(int k=0;k<4;++k)if(!Same(dest.V.Start[k],source.V.Start[k]))return 7;
 int blocks=liveBlocks,creatures=liveCreatures,waves=liveWaves;dest.V.Assign(dest.V);if(dest.V.Start!=first||blocks!=liveBlocks||creatures!=liveCreatures||waves!=liveWaves)return 8;
 source.Resize(0);dest.V.Assign(source.V);if(dest.V.Start!=first||dest.V.Finish!=first||dest.V.End!=first+5||liveBlocks||liveCreatures||liveWaves||liveCreatureStorage)return 9;}
 {Vector<Element> v;v.Start=(Element*)malloc(sizeof(Element));v.Finish=v.Start+1;v.End=v.Finish;new(v.Start)Element;Init(*v.Start,77);Element* start=v.Start;Element* finish=v.Finish;v.Tidy();if(v.Start!=start||v.Finish!=finish||v.End!=finish||liveBlocks||liveCreatures||liveWaves||liveCreatureStorage)return 10;}
 if(liveBlocks||liveCreatures||liveWaves||liveCreatureStorage)return 11;std::printf("00f27bc0_TEST PASS\n");return 0;}