// 006772d0 CThingBaseDef::CThingBaseDef
// Flags:
// Leaf: ??0CThingBaseDef@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Layout: shared flattened header supplies the 0x70 extent and native member offsets;
// typed inheritance below restores the original PDB classes and STLport members.
#define CThingBaseDef CThingBaseDefLayout
#include "engine/_quarantine/CThingBaseDef.h"
#undef CThingBaseDef
#define CDefClassBase CDefClassBaseLayout
#include "engine/CDefClassBase.h"
#undef CDefClassBase
#define CParentDefClassBase CParentDefClassBaseLayout
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointeeBase.h"
class CCharString;class CDefinitionManager;class CPersistContext;class CDataInputStream;class CDataOutputStream;class CValidateDefinitionInitBase;
class CDefString{long TablePos;public:CDefString():TablePos(-1){}void Clear(){TablePos=-1;}};
#include "engine/CArray.h"
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{C Compare;bool Dirty;public:CVectorMap():Dirty(false){}};
template<class T>class CDefPointer;
#pragma pack(push,1)
// Layout: shared CDefClassBase37, parent57, typed original container hierarchy.
class CDefClassBase : public CDefPointeeBase {
private:friend class CParentDefClassBase;
 CDefClassBase():PDefManager(0){Setup=0;Template=0;DefaultValsApplied=0;}
public:
    class CSubDefInfo {                                                  
    public:
        long DefIndex;                                                   
        long OriginalParentDefIndex;                                     
    };
    CDefClassBase(const CDefClassBase&);                                 
    CDefClassBase& operator=(const CDefClassBase&);                      
    template <class T> bool GetSubDef(CDefPointer<const T>*) const;      
    bool AreDefaultValsApplied() const { return DefaultValsApplied; }    
    void SetDefaultValsApplied(bool value) { DefaultValsApplied = value; }   
    virtual ~CDefClassBase() { }                                         
    virtual bool UnloadResource();                                       
    virtual CDefString GetInstantiationName() const;                     
    virtual void SetInstantiationName(CDefString);                       
    virtual void AddSubDef(const CCharString&, long, long);              
    virtual void RemoveSubDef(const CCharString&);                       
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; 
    virtual bool IsParentDef() const;                                    
    virtual void PostTransfer();                                         
    virtual void Transfer(CPersistContext&) = 0;                         
    virtual void Copy(const CDefClassBase*) = 0;                         
    virtual unsigned long GetSizeofClass() = 0;                          
    virtual void InternalSaveBinary(CDataOutputStream&);                 
    virtual void InternalLoadBinary(CDataInputStream&);                  
    virtual void Validate(const CValidateDefinitionInitBase&, const CCharString&) const;  
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             
protected:
    CDefinitionManager* PDefManager;                                     
    unsigned long       GlobalIndex;                                     
    unsigned char       Setup : 1;                                       
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CParentDefClassBase : public CDefClassBase {
public:
    CParentDefClassBase(){};                                               
    CParentDefClassBase(const CParentDefClassBase&);                     
    CParentDefClassBase& operator=(const CParentDefClassBase&);          
    virtual ~CParentDefClassBase(){};                                      
    virtual CDefString GetInstantiationName() const;                     
    virtual void SetInstantiationName(CDefString);                       
    virtual void AddSubDef(const CCharString&, long, long);              
    virtual void RemoveSubDef(const CCharString&);                       
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; 
    virtual bool IsParentDef() const;                                    
    virtual void InternalSaveBinary(CDataOutputStream&);                 
    virtual void InternalLoadBinary(CDataInputStream&);                  
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             
protected:CDefString InstantiationName;
 CVectorMap<unsigned long,CSubDefInfo,CKeyPairCompareLess<unsigned long,CSubDefInfo> >SubDefInfoMap;
};

#pragma pack(pop)

template<class T,int N>class CSmallVector{protected:T*Start;signed char Size;public:CSmallVector():Start(0),Size(0){}};
class CThingComponentSet{
#pragma pack(push,1)
public:class CEntry{public:CDefString Name;unsigned long DriverType;bool Override;};
#pragma pack(pop)
private:CSmallVector<CEntry,8>Entries;
};
#pragma pack(push,1)
class CThingBaseDef :public CParentDefClassBase {
public:    
    CThingComponentSet Components;          
    bool AvailableInEditor : 1;             
    bool AlwaysFrameUpdate : 1;
    bool StartKillLocked : 1;
    bool LevelLess : 1;
    bool IsDrawable : 1;
    bool IsDrawableInEditor : 1;
    bool ForceNoSerialise : 1;
    bool InMapSearches : 1;
    bool IsLoadableGlobal : 1;              
    bool GameStatePersisted : 1;
    bool CanComeBetweenCameraAndObservedThing : 1;
    bool DrawWeaponTrails : 1;
    bool DrawProjectileWeaponTrails : 1;
    bool IsVulnerableToMelee : 1;
    bool Damageable : 1;
    bool AlwaysIncludeInObjectStrikeScans : 1;
    bool DisableOcclusionTesting : 1;       
    bool DrawAfterWater : 1;
    bool ForceRenderedLastFrameOverride : 1;
    bool AddToComboMultiplierOnHit : 1;
    bool GiveHeroStatChangesOnBeingHit : 1;
    long GroupDef;                          
    float RenderFadeDistance;               
    long PersistenceFlags;                  
    float Health;                           
    float MinHealth;                        
    float MeshHeightOffset;                 
    float MeshHeight;                       
    float MeshRadius;                       
    float ApproxMaxMeshHeight;              
    CDefString DefaultScriptName;           
    CDefString DefaultScriptData;           
    virtual void Transfer(CPersistContext&);virtual void Copy(const CDefClassBase*);virtual unsigned long GetSizeofClass();
    CThingBaseDef();
};
#pragma pack(pop)

CThingBaseDef::CThingBaseDef()
 :AvailableInEditor(false),
  AlwaysFrameUpdate(false),
  StartKillLocked(false),
  LevelLess(false),
  IsDrawable(true),
  IsDrawableInEditor(false),
  ForceNoSerialise(false),
  InMapSearches(false),
  IsLoadableGlobal(false),
  GameStatePersisted(false),
  CanComeBetweenCameraAndObservedThing(false),
  DrawWeaponTrails(false),
  IsVulnerableToMelee(false),
  Damageable(false),
  AlwaysIncludeInObjectStrikeScans(false),
  ForceRenderedLastFrameOverride(false),
  AddToComboMultiplierOnHit(false),
  GiveHeroStatChangesOnBeingHit(false),
  GroupDef(0),
  RenderFadeDistance(0),
  PersistenceFlags(0),
  Health(0),
  MinHealth(0),
  MeshHeightOffset(0),
  MeshHeight(0),
  MeshRadius(0),
  ApproxMaxMeshHeight(0)
{}

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CThingBaseDef)==sizeof(CThingBaseDefLayout));
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef,Components)==offsetof(CThingBaseDefLayout,Components));
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef,DefaultScriptName)==offsetof(CThingBaseDefLayout,DefaultScriptName_TablePos));
#endif