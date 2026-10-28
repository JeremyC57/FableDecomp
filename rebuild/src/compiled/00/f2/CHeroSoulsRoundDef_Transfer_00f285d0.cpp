// CHeroSoulsRoundDef::Transfer(CPersistContext&) -- transfer the scalar and typed sub-component vector.
// Leaf: ?Transfer@CHeroSoulsRoundDef@@UAEXAAVCPersistContext@@@Z
// Identity: proven; own virtual Transfer slot18 literals NumWaves/Waves, original donor named records/types and PDB class-V/Copy/Transfer/copy-ctor signatures; original CDefClassBase copy ctor0046644c/operator=00431f10, true scalar Transfer<long>(name,long&,const long&)00410be0. Catalog CClanMember/CAbilityTrack labels are propagated and false. STLport _Construct is the genuine placement-copy template matching ECX destination/EDX source.
// Layout: original CIV/CResource/CDefPointee/CDefClassBase packed37, derived scalar+40 and genuine STLport vector+44 giving56. Forward-only vector element class identities retained; no unrelated element layout invented. Unknown unused definition slots2–17 explicit, actual Transfer18/Copy19. Available shared base headers retained as aliases. Source names anchored by actual record producer/consumer, PDB addresses used only as namesets.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CResource CResource_HeaderEvidence
#include "engine/CResource.h"
#undef CResource
#define CDefClassBase CDefClassBase_HeaderEvidence
#include "engine/CDefClassBase.h"
#undef CDefClassBase
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
class CIVCountedPointeeBase{public:CIVCountedPointeeBase():IVRefCount(1){}virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable unsigned long IVRefCount;void AddRef()const{++IVRefCount;}long GetRefCount()const{return (long)IVRefCount;}long Release()const{long result=(long)--IVRefCount;if(result==0)Delete();return result;}};
class CResourceList;class CResource:public CIVCountedPointeeBase{public:CResourceList* ResourceList;CResource* PrevResource;CResource* NextResource;unsigned long ResourceSize,LastUsedFrame;CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}virtual ~CResource();};class CDefPointeeBase:public CResource{public:virtual void Delete()const;};enum ENullDef{ENullDef_DescriptiveNull=0};
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:explicit vector(const A& =A());vector(const vector&);~vector();vector& operator=(const vector&);T& operator[](unsigned int n){return this->_M_start[n];}const T& operator[](unsigned int n)const{return this->_M_start[n];}};}
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#pragma pack(push,1)
class CDefClassBase:public CDefPointeeBase{public:CDefinitionManager* PDefManager;unsigned long GlobalIndex;unsigned char Setup:1;unsigned char Template:1;unsigned char DefaultValsApplied:1;void SetDefaultValsApplied(bool value){DefaultValsApplied=value;}private:CDefClassBase();friend class CHeroSoulsRoundDef;public:CDefClassBase(const CDefClassBase&);CDefClassBase& operator=(const CDefClassBase&);virtual void UnknownDefinitionSlot2(){}virtual void UnknownDefinitionSlot3(){}virtual void UnknownDefinitionSlot4(){}virtual void UnknownDefinitionSlot5(){}virtual void UnknownDefinitionSlot6(){}virtual void UnknownDefinitionSlot7(){}virtual void UnknownDefinitionSlot8(){}virtual void UnknownDefinitionSlot9(){}virtual void UnknownDefinitionSlot10(){}virtual void UnknownDefinitionSlot11(){}virtual void UnknownDefinitionSlot12(){}virtual void UnknownDefinitionSlot13(){}virtual void UnknownDefinitionSlot14(){}virtual void UnknownDefinitionSlot15(){}virtual void UnknownDefinitionSlot16(){}virtual void UnknownDefinitionSlot17(){} virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);};
#pragma pack(pop)
class CHeroSoulsWaveDef;class CHeroSoulsRoundDef:public CDefClassBase{public:long NumWaves;std::vector<CHeroSoulsWaveDef> Waves;CHeroSoulsRoundDef();CHeroSoulsRoundDef(const CHeroSoulsRoundDef&);virtual ~CHeroSoulsRoundDef();virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);};
class CSymbolMap;class CStringParser;class CMemoryDataInputStream;class CDataOutputStream;class CPersistContext{public:CDefinitionManager* PDefinitionManager;CSymbolMap* PSymbolMap;CStringParser* PParser;void (__cdecl *ErrorCallback)(CCharString*);CCharString* PStringToClean;bool UseDefaults,UseCRCCheck,SingleTransfer,Finished;enum EMode{EMode_DescriptiveZero=0};EMode Mode;bool Binary;CCharString* PStringToSaveTo;CMemoryDataInputStream* PLoadStream;CDataOutputStream* PSaveStream;template<class T>void Transfer(const char*,T&,const T& =T());template<class T>void TransferVectorOfSubComponents(const char*,std::vector<T>&);};
void CHeroSoulsRoundDef::Transfer(CPersistContext& context){context.Transfer("NumWaves",NumWaves);context.TransferVectorOfSubComponents("Waves",Waves);}
#ifdef _MSC_VER
typedef char RecordSize[(sizeof(CHeroSoulsRoundDef)==56)?1:-1];
#endif