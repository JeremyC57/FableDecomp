// Native virtual destructor, identified by deleting-dtor slot0 RTTI.
// Actual quarantined donor retained. Native packed parent39 vs donor3d;
// native vectors12 vs donor16. novtable views emit no production vtable.
#pragma optimize("s",on)
#define CSpecialAbilitiesBerserkDef CSpecialAbilitiesBerserkDefDonor
#include "engine/_quarantine/CSpecialAbilitiesBerserkDef.h"
#undef CSpecialAbilitiesBerserkDef
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
struct CCharString;struct CPersistContext;struct CDataInputStream;struct CDataOutputStream;struct CValidateDefinitionInitBase;struct CDefinitionManager;struct CResourceList;
struct CDefString {S32 TablePos;};
#ifdef _MSC_VER
typedef unsigned long U32;
#else
typedef unsigned int U32;
#endif
template<class K,class V>struct CKeyPairCompareLess{};
template<class K,class V,class Compare>struct CVectorMap {void* Start;void* Finish;void* End;Compare CompareKeys;unsigned char Dirty;};
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
 private:CDefClassBase();friend struct CParentDefClassBase;
 public:struct CSubDefInfo;
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
struct __declspec(novtable) CSpecialAbilitiesBerserkDef:CParentDefClassBase {
    float         SpeedMultiplier;
    S32          FramesToFadeOut;
    float         LowerBerserkPercent;
    float         TimeForThemeTransition;
    float         BlurCenterFromFloor;
    float         BerserkThreshold;
    float         BerserkLength;
    float         BerserkIntensity;
    float         BerserkFade;
    float         BerserkRange;
    float         BerserkTwist;
    float         TimeForRadialBlurToFadeOut;
    float         CameraStartDist;
    float         CameraEndDist;
    float         CameraStartDeltaHeight;
    float         CameraEndDeltaHeight;
 NativeVector<float> BerserkDuration;
 NativeVector<float> BerserkDamageToEnemyMultiplier;
    float         BerserkDamageToEnemyMultiplierAdditionPerHit;
 NativeVector<float> BerserkDamageToMeMultiplier;
 NativeVector<float> MaxIncreaseInStrength;
 NativeVector<float> BerserkScale;
    float         BerserkBlastRadius;
 NativeVector<float> CamDist;
 NativeVector<float> CamHeight;
 NativeVector<float> CamFOV;
 NativeVector<float> CamSwitchTime;
 NativeVector<float> RumbleLevel;
 NativeVector<float> RumbleSmoothness;
 NativeVector<float> RumbleAttack;
 NativeVector<float> RumbleDecay;
    float         RumbleReleaseLevelDelta;
    float         RumbleReleaseSmoothnessDelta;
    float         RumbleReleaseAttackDelta;
    float         RumbleReleaseDecayDelta;
    float         CrazyActionRadius;
    float         CrazyActionDamage;
 NativeVector<float> DecapitateMaxProb;
    float         TargetNearestEnemyDistance;
    S32          WeaponTrailAttack;
    S32          WeaponTrailKnockDown;
 NativeVector<float> LevelUpTimings;
 NativeVector<S32> TintColorR;
 NativeVector<S32> TintColorG;
 NativeVector<S32> TintColorB;
    bool          CanBlockWhileBerserked;
    float         CrazyIdleIntervalSecs;
 CSpecialAbilitiesBerserkDef(); // genuine public default constructor
 virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);virtual U32 GetSizeofClass();
 virtual ~CSpecialAbilitiesBerserkDef();
};
CSpecialAbilitiesBerserkDef::~CSpecialAbilitiesBerserkDef() {}