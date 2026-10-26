// Native virtual destructor, identified by deleting-dtor slot0 RTTI.
// Leaf: ??1CPatchTesselationEdgeStrip@
// Actual quarantined donor retained. Native packed parent39 vs donor3d;
// native vectors12 vs donor16. novtable views emit no production vtable.
#pragma optimize("s",on)
#define CSpecialAbilitiesLightningSpellDef CSpecialAbilitiesLightningSpellDefDonor
#include "engine/_quarantine/CSpecialAbilitiesLightningSpellDef.h"
#undef CSpecialAbilitiesLightningSpellDef
#define CParentDefClassBase CParentDefClassBaseDonor
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#define CDefClassBase CDefClassBaseDonor
#include "engine/CDefClassBase.h"
#undef CDefClassBase
#define CResource CResourceDonor
#include "engine/CResource.h"
#undef CResource
#define CIVCountedPointeeBase CIVCountedPointeeBaseDonor
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#include <stdlib.h>
#ifdef _MSC_VER
typedef long S32;
#else
typedef int S32;
#define __declspec(x)
#endif
// RTTI: derived -> CParentDefClassBase -> CDefClassBase -> CDefPointeeBase
// -> CResource -> CIVCountedPointeeBase; all 25 slots audited at 012341FC/0123460C.
class CCharString;class CPersistContext;class CDataInputStream;class CDataOutputStream;class CValidateDefinitionInitBase;class CDefinitionManager;class CResourceList;
class CDefString { public:S32 TablePos;};
#ifdef _MSC_VER
typedef unsigned long U32;
#else
typedef unsigned int U32;
#endif
template<class K,class V>class CKeyPairCompareLess{ public:};
template<class K,class V,class Compare>class CVectorMap { public:void* Start;void* Finish;void* End;Compare CompareKeys;unsigned char Dirty;};
#pragma pack(push,1)
struct __declspec(novtable) CIVCountedPointeeBase {
 CIVCountedPointeeBase();virtual ~CIVCountedPointeeBase();virtual void Delete() const;
 U32 IVRefCount;
};
struct __declspec(novtable) CResource:CIVCountedPointeeBase {
 CResource();virtual ~CResource();
 virtual bool UnloadResource();virtual U32 GetUnloadableMemory() const;
 virtual bool IsResourceLoaded() const;virtual void OnResourceLoaded();
 virtual bool CanAbortResourceLoad(S32) const;virtual void OnResourceLoadAborted();
 virtual U32 GetDebugMemoryInUse() const;virtual void ResetDebugMemoryInUse();
 CResourceList* ResourceList;CResource* PrevResource;CResource* NextResource;U32 ResourceSize;U32 LastUsedFrame;
};
struct __declspec(novtable) CDefPointeeBase:CResource {CDefPointeeBase();};
struct __declspec(novtable) CDefClassBase:CDefPointeeBase {
 private:CDefClassBase();friend class CParentDefClassBase;
 public:class CSubDefInfo;
 typedef CVectorMap<U32,CSubDefInfo,CKeyPairCompareLess<U32,CSubDefInfo> > SubDefMap;
 virtual ~CDefClassBase();virtual bool UnloadResource();
 virtual CDefString GetInstantiationName() const;virtual void SetInstantiationName(CDefString);
 virtual void AddSubDef(const CCharString&,S32,S32);virtual void RemoveSubDef(const CCharString&);
 virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const;virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);
 virtual bool IsParentDef() const;virtual void PostTransfer();
 virtual void Transfer(CPersistContext&)=0;virtual void Copy(const CDefClassBase*)=0;virtual U32 GetSizeofClass()=0;
 virtual void InternalSaveBinary(CDataOutputStream&);virtual void InternalLoadBinary(CDataInputStream&);
 virtual void Validate(const CValidateDefinitionInitBase&,const CCharString&) const;virtual SubDefMap* GetPSubDefInfoMap();
 CDefinitionManager* PDefManager;U32 GlobalIndex;unsigned char Setup:1;unsigned char Template:1;unsigned char DefaultValsApplied:1;
};
struct __declspec(novtable) CParentDefClassBase:CDefClassBase {
 CParentDefClassBase();virtual ~CParentDefClassBase(); // 00430370 / 00430300
 virtual CDefString GetInstantiationName() const;virtual void SetInstantiationName(CDefString);
 virtual void AddSubDef(const CCharString&,S32,S32);virtual void RemoveSubDef(const CCharString&);
 virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const;virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);
 virtual bool IsParentDef() const;virtual void InternalSaveBinary(CDataOutputStream&);virtual void InternalLoadBinary(CDataInputStream&);
 virtual SubDefMap* GetPSubDefInfoMap();
 S32 InstantiationName_TablePos;SubDefMap SubDefInfoMap;
};
#pragma pack(pop)
template<class T> struct NativeVector {T* Start;T* Finish;T* End;NativeVector():Start(0),Finish(0),End(0){}~NativeVector(){if(Start)free(Start);}};
struct __declspec(novtable) CSpecialAbilitiesLightningSpellDef:CParentDefClassBase {
 NativeVector<float> m_faAreaEffect;
 NativeVector<float> m_faDamagePerHit;
 NativeVector<float> m_faSubBeamDamagePerHit;
 NativeVector<float> m_faActivationTime;
 NativeVector<float> m_faDeActivationTime;
 NativeVector<S32> m_naMinMainBeams;
 NativeVector<S32> m_naMaxMainBeams;
 NativeVector<S32> m_naMinSubBeams;
 NativeVector<S32> m_naMaxSubBeams;
 NativeVector<float> m_faTemporaryBeamLife;
 NativeVector<float> m_faTargettingFOV;
 NativeVector<float> m_faStaminaPerSecond;
    float         m_fRandomBeamMinAngleX;
    float         m_fRandomBeamMaxAngleX;
    float         m_fRandomBeamMinAngleZ;
    float         m_fRandomBeamMaxAngleZ;
 NativeVector<float> m_faMainBeamDecapitationTime;
 NativeVector<float> m_faSubBeamDecapitationTime;
 NativeVector<float> m_faSubBeamDelay;
 NativeVector<float> OnhitEveryNSecs;
    float         CombatMultiplierIncreaseTime;
    S32          InitSound_TablePos;
    S32          LoopingSound_TablePos;
    S32          EndHeroSound_TablePos;
    S32          EndAttackeeSound_TablePos;
    float         LightningThemeTransitionInSecs;
    float         LightningThemeTransitionOutSecs;
 CSpecialAbilitiesLightningSpellDef(); // genuine public default constructor
 virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);virtual U32 GetSizeofClass();
 virtual ~CSpecialAbilitiesLightningSpellDef();
};
CSpecialAbilitiesLightningSpellDef::~CSpecialAbilitiesLightningSpellDef() {}