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

#include <cstdio>
#include <cstring>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace N00f27bc0 {
__declspec(noinline) CHeroSoulsWaveDef* __fastcall __copy_ptrs(const CHeroSoulsWaveDef* first,const CHeroSoulsWaveDef* last,CHeroSoulsWaveDef* out,const __false_type&){for(;first!=last;++first,++out)*out=*first;return out;}
#pragma pack(push,1)
struct CCharStringData {char* Text;unsigned char Other[9];int Refs;};
#pragma pack(pop)
static int liveBlocks,liveCreatures,liveWaves,liveCreatureStorage;
CCharString::CCharString():Data(0){}CCharString::CCharString(const CCharString& o):Data(o.Data){if(Data)++Data->Refs;}
void Release(CCharStringData* p){if(p&&--p->Refs<=0){delete[]p->Text;delete p;--liveBlocks;}}
CCharString::~CCharString(){Release(Data);}CCharString& CCharString::operator=(const CCharString& o){if(Data!=o.Data){if(o.Data)++o.Data->Refs;Release(Data);Data=o.Data;}return *this;}
void Set(CCharString& s,const char* text){CCharString fresh;fresh.Data=new CCharStringData;std::memset(fresh.Data,0,sizeof *fresh.Data);unsigned int n=(unsigned int)std::strlen(text);fresh.Data->Text=new char[n+1];std::memcpy(fresh.Data->Text,text,n+1);fresh.Data->Refs=1;++liveBlocks;s=fresh;}
DefClassStorage::~DefClassStorage(){}
DefClassStorage::DefClassStorage(){std::memset(&Base,0,sizeof Base);std::memset(Padding,0,sizeof Padding);}
DefClassStorage::DefClassStorage(const DefClassStorage& o){std::memset(&Base,0,sizeof Base);std::memcpy(Base._base_0x04,o.Base._base_0x04,sizeof Base._base_0x04);Base.PDefManager=o.Base.PDefManager;Base.GlobalIndex=o.Base.GlobalIndex;Base.DefaultValsApplied[0]=(unsigned char)(o.Base.DefaultValsApplied[0]&7);std::memset(Padding,0,sizeof Padding);}
DefClassStorage& DefClassStorage::operator=(const DefClassStorage& o){std::memcpy(Base._base_0x04,o.Base._base_0x04,sizeof Base._base_0x04);Base.PDefManager=o.Base.PDefManager;Base.GlobalIndex=o.Base.GlobalIndex;Base.DefaultValsApplied[0]=(unsigned char)((Base.DefaultValsApplied[0]&~7)|(o.Base.DefaultValsApplied[0]&7));return *this;}
CHeroSoulsCreatureDef::CHeroSoulsCreatureDef():NumCreatures(0){++liveCreatures;}CHeroSoulsCreatureDef::CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef& o):DefClassStorage(o),CreatureType(o.CreatureType),Location(o.Location),NumCreatures(o.NumCreatures){++liveCreatures;}CHeroSoulsCreatureDef::~CHeroSoulsCreatureDef(){--liveCreatures;}CHeroSoulsCreatureDef& CHeroSoulsCreatureDef::operator=(const CHeroSoulsCreatureDef& o){DefClassStorage::operator=(o);CreatureType=o.CreatureType;Location=o.Location;NumCreatures=o.NumCreatures;return *this;}
CreatureVector::CreatureVector():_M_start(0),_M_finish(0),_M_end_of_storage(0){}
CreatureVector::CreatureVector(const CreatureVector& o):_M_start(0),_M_finish(0),_M_end_of_storage(0){unsigned int n=o._M_start?(unsigned int)(o._M_finish-o._M_start):0;if(n){_M_start=(CHeroSoulsCreatureDef*)malloc(n*sizeof(CHeroSoulsCreatureDef));_M_finish=_M_start;_M_end_of_storage=_M_start+n;++liveCreatureStorage;for(CHeroSoulsCreatureDef* p=o._M_start;p!=o._M_finish;++p,++_M_finish)new(_M_finish)CHeroSoulsCreatureDef(*p);}}
void Clear(CreatureVector& v){for(CHeroSoulsCreatureDef* p=v._M_start;p!=v._M_finish;++p)p->~CHeroSoulsCreatureDef();if(v._M_start){free(v._M_start);--liveCreatureStorage;}}
CreatureVector::~CreatureVector(){Clear(*this);}
CreatureVector& CreatureVector::operator=(const CreatureVector& o){if(this!=&o){unsigned int n=o._M_start?(unsigned int)(o._M_finish-o._M_start):0,size=_M_start?(unsigned int)(_M_finish-_M_start):0,capacity=_M_start?(unsigned int)(_M_end_of_storage-_M_start):0;if(n>capacity){CHeroSoulsCreatureDef* fresh=(CHeroSoulsCreatureDef*)malloc(n*sizeof(CHeroSoulsCreatureDef));CHeroSoulsCreatureDef* end=fresh;for(CHeroSoulsCreatureDef* p=o._M_start;p!=o._M_finish;++p,++end)new(end)CHeroSoulsCreatureDef(*p);Clear(*this);_M_start=fresh;_M_finish=end;_M_end_of_storage=fresh+n;++liveCreatureStorage;}else{unsigned int common=n<size?n:size;for(unsigned int i=0;i<common;++i)_M_start[i]=o._M_start[i];if(n<size){for(unsigned int j=n;j<size;++j)_M_start[j].~CHeroSoulsCreatureDef();}else{for(unsigned int k=size;k<n;++k)new(_M_start+k)CHeroSoulsCreatureDef(o._M_start[k]);}if(_M_start)_M_finish=_M_start+n;}}return *this;}
CHeroSoulsWaveDef::CHeroSoulsWaveDef():NumWaveCreatures(0){++liveWaves;}CHeroSoulsWaveDef::CHeroSoulsWaveDef(const CHeroSoulsWaveDef& o):DefClassStorage(o),NumWaveCreatures(o.NumWaveCreatures),Creatures(o.Creatures){++liveWaves;}CHeroSoulsWaveDef::~CHeroSoulsWaveDef(){--liveWaves;}CHeroSoulsWaveDef& CHeroSoulsWaveDef::operator=(const CHeroSoulsWaveDef& o){DefClassStorage::operator=(o);NumWaveCreatures=o.NumWaveCreatures;Creatures=o.Creatures;return *this;}
void Init(CHeroSoulsWaveDef& e,int id){e.Base.PDefManager=0;e.Base.GlobalIndex=100*id;e.Base.DefaultValsApplied[0]=(unsigned char)(id&7);e.NumWaveCreatures=2;e.Creatures._M_start=(CHeroSoulsCreatureDef*)malloc(2*sizeof(CHeroSoulsCreatureDef));e.Creatures._M_finish=e.Creatures._M_start;e.Creatures._M_end_of_storage=e.Creatures._M_start+2;++liveCreatureStorage;for(int i=0;i<2;++i){new(e.Creatures._M_finish)CHeroSoulsCreatureDef;CHeroSoulsCreatureDef& v=*e.Creatures._M_finish;v.Base.PDefManager=0;v.Base.GlobalIndex=1000*id+10*i;v.Base.DefaultValsApplied[0]=(unsigned char)((id+i)&7);Set(v.CreatureType,"creature_type");Set(v.Location,"spawn_location");v.NumCreatures=10*id+i;++e.Creatures._M_finish;}}
struct Owner {vector<CHeroSoulsWaveDef> V;int BaseID;Owner(int capacity,int size,int id):BaseID(id){V._M_start=(CHeroSoulsWaveDef*)malloc(capacity*sizeof(CHeroSoulsWaveDef));V._M_finish=V._M_start;V._M_end_of_storage=V._M_start+capacity;Resize(size);}~Owner(){V._M_clear();}void Resize(int n){int size=(int)(V._M_finish-V._M_start);if(n<size){while(V._M_finish!=V._M_start+n){--V._M_finish;V._M_finish->~CHeroSoulsWaveDef();}}else for(int i=size;i<n;++i){new(V._M_finish)CHeroSoulsWaveDef;Init(*V._M_finish,BaseID+i);++V._M_finish;}}};
bool Same(const CHeroSoulsWaveDef& a,const CHeroSoulsWaveDef& b){if(a.NumWaveCreatures!=b.NumWaveCreatures||a.Creatures._M_finish-a.Creatures._M_start!=2||a.Creatures._M_start==b.Creatures._M_start)return false;if(a.Base.PDefManager!=b.Base.PDefManager||a.Base.GlobalIndex!=b.Base.GlobalIndex||a.Base.DefaultValsApplied[0]!=b.Base.DefaultValsApplied[0]||std::memcmp(a.Base._base_0x04,b.Base._base_0x04,24))return false;for(int i=0;i<2;++i){const CHeroSoulsCreatureDef& x=a.Creatures._M_start[i];const CHeroSoulsCreatureDef& y=b.Creatures._M_start[i];if(x.NumCreatures!=y.NumCreatures||x.CreatureType.Data!=y.CreatureType.Data||x.Location.Data!=y.Location.Data||x.CreatureType.Data->Refs!=2||x.Location.Data->Refs!=2||std::strcmp(x.CreatureType.Data->Text,"creature_type")||std::strcmp(x.Location.Data->Text,"spawn_location"))return false;if(x.Base.GlobalIndex!=y.Base.GlobalIndex||x.Base.PDefManager!=y.Base.PDefManager||x.Base.DefaultValsApplied[0]!=y.Base.DefaultValsApplied[0]||std::memcmp(x.Base._base_0x04,y.Base._base_0x04,24))return false;}return true;}
}
int main(){using namespace N00f27bc0;
 if(sizeof(DefClassStorage)!=40||sizeof(CHeroSoulsCreatureDef)!=52||sizeof(CHeroSoulsWaveDef)!=56||sizeof(CCharStringData)!=17)return 1;
 {CHeroSoulsWaveDef e;if((char*)&e.Base.PDefManager-(char*)&e!=28||(char*)&e.Base.GlobalIndex-(char*)&e!=32||(char*)&e.Base.DefaultValsApplied-(char*)&e!=36||(char*)&e.NumWaveCreatures-(char*)&e!=40||(char*)&e.Creatures-(char*)&e!=44)return 12;}
 {Owner source(5,5,10),dest(1,1,50);CHeroSoulsWaveDef* first=dest.V._M_start;vector<CHeroSoulsWaveDef>* result=&dest.V.operator=(source.V);if(result!=&dest.V||dest.V._M_finish-dest.V._M_start!=5||dest.V._M_end_of_storage-dest.V._M_start!=5||dest.V._M_start==first)return 2;for(int i=0;i<5;++i)if(!Same(dest.V._M_start[i],source.V._M_start[i]))return 3;
 first=dest.V._M_start;source.Resize(2);dest.V.operator=(source.V);if(dest.V._M_start!=first||dest.V._M_finish-first!=2||dest.V._M_end_of_storage-first!=5)return 4;for(int j=0;j<2;++j)if(!Same(dest.V._M_start[j],source.V._M_start[j]))return 5;
 source.Resize(4);dest.V.operator=(source.V);if(dest.V._M_start!=first||dest.V._M_finish-first!=4||dest.V._M_end_of_storage-first!=5)return 6;for(int k=0;k<4;++k)if(!Same(dest.V._M_start[k],source.V._M_start[k]))return 7;
 int blocks=liveBlocks,creatures=liveCreatures,waves=liveWaves;dest.V.operator=(dest.V);if(dest.V._M_start!=first||blocks!=liveBlocks||creatures!=liveCreatures||waves!=liveWaves)return 8;
 source.Resize(0);dest.V.operator=(source.V);if(dest.V._M_start!=first||dest.V._M_finish!=first||dest.V._M_end_of_storage!=first+5||liveBlocks||liveCreatures||liveWaves||liveCreatureStorage)return 9;}
 {vector<CHeroSoulsWaveDef> v;v._M_start=(CHeroSoulsWaveDef*)malloc(sizeof(CHeroSoulsWaveDef));v._M_finish=v._M_start+1;v._M_end_of_storage=v._M_finish;new(v._M_start)CHeroSoulsWaveDef;Init(*v._M_start,77);CHeroSoulsWaveDef* start=v._M_start;CHeroSoulsWaveDef* finish=v._M_finish;v._M_clear();if(v._M_start!=start||v._M_finish!=finish||v._M_end_of_storage!=finish||liveBlocks||liveCreatures||liveWaves||liveCreatureStorage)return 10;}
 if(liveBlocks||liveCreatures||liveWaves||liveCreatureStorage)return 11;std::printf("00f27bc0_TEST PASS\n");return 0;}