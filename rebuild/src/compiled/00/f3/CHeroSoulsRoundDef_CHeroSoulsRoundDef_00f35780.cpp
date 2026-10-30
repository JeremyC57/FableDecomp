// CHeroSoulsRoundDef::CHeroSoulsRoundDef(const CHeroSoulsRoundDef&) -- copy the actual base, scalar and typed vector.
// Leaf: ??0CHeroSoulsRoundDef@@QAE@ABV0@@Z
// Identity: proven; own virtual Transfer slot18 literals NumWaves/Waves, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
// Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define NOINLINE __attribute__((noinline))
typedef __SIZE_TYPE__ PlacementSize;
#else
#define NOINLINE __declspec(noinline)
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p){return p;}
enum ENullDef{ENullDef_DescriptiveNull=0};
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:explicit vector(const A& =A());vector(const vector&);~vector();vector& operator=(const vector&);T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CHeroSoulsWaveDef;class CHeroSoulsRoundDef:public CDefClassBase{public:long NumWaves;std::vector<CHeroSoulsWaveDef> Waves;CHeroSoulsRoundDef();CHeroSoulsRoundDef(const CHeroSoulsRoundDef&);virtual ~CHeroSoulsRoundDef();virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);virtual unsigned long GetSizeofClass();};   // def vtable 0x012FB430 (no RTTI): slot 0 0x00F26DF0 (implicit inline dtor), Transfer 0x00F285D0, Copy 0x00F27B90, GetSizeofClass 0x00F26DE0
CHeroSoulsRoundDef::CHeroSoulsRoundDef(const CHeroSoulsRoundDef& other):CDefClassBase(other),NumWaves(other.NumWaves),Waves(other.Waves){}
#ifdef _MSC_VER
typedef char RecordSize[(sizeof(CHeroSoulsRoundDef)==56)?1:-1];
#endif