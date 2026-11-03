// Standalone C++03 behaviour fixture: exact selected function and original headers pasted.
// Visibility relaxed only in tests; unused virtual calls fail. Host long uses int32.
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>
#ifdef _MSC_VER
typedef unsigned __int64 FixtureUInt64;
#else
typedef unsigned long FixtureUInt64;
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define long int
#endif
#define FABLE_FASTCALL __fastcall
#define private public
#define protected public
static int randomValues[2],randomCalls;
static int fixture_rand(){assert(randomCalls<2);return randomValues[randomCalls++];}
#define rand fixture_rand


#include <stdlib.h>

class CCharString;
class CWideStringData;
class CWideString {
public:
    ~CWideString();
protected:
    CWideStringData* PStringData;
};


class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    inline void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    inline const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    inline const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    inline const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    inline bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    inline float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    inline float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    inline const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    inline const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef inline

class CRGBColour {
public:
    union {
#ifdef _MSC_VER
        struct { unsigned char B,G,R,A; };
#else
        __extension__ struct { unsigned char B,G,R,A; };
#endif
        unsigned long IntValue;
    };
    CRGBColour() { }
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
    CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
};



#include <stddef.h>







template<class T> class CBasicString {
protected:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
public:
    const T* PeekData() const { return PData; }
    unsigned long GetLength() const { return StringLength; }
};
class CCharStringData {
public:
    CBasicString<char> Data;
protected:
    long NoRefs;
};

class CCharStringData;
class CCharString {
public:
    long GetLength() const { if(!PStringData)return 0; return PStringData->Data.GetLength(); }
    void Truncate(unsigned long);
    const CCharString& operator=(const CCharString&);
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};










































class CCPPointerInfo {
public:
    unsigned long RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                             
    CCPPointerInfo* Info;                                                

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
    void Validate() const;                                               
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              
    }
    void reset(T* data) { Reset(data); }                                 
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    
};
























class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};








class C2DCoordI;
class C2DVector;
class C3DVector;
class CCharString;
class CCounter;
class CRGBColour;
class CRGBFloatColour;
class CRightHandedSet;
class CScriptGameResourceObjectMovieBase;
class CScriptGameResourceObjectScriptedThingBase;
class CScriptThing;
class CThing;
class CTimer;
class CWideString;
class CPersistContext;
class CScriptBase;
class CSpawnedFuncBase;
class CEntityScriptBindingBase;
class CScriptDataBase;
class CActiveEntityScriptBase;
struct CScreenFilterSThingByPass;
class CHeroLogBook;
template <class T> class CArray;
template <class T> class CCountedPointer;
namespace NScript { class CPredicateBase; }
namespace std {
template <class T> class allocator;
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T, class A> class vector;
template <class T, class A> class list;
template <class K, class C, class A> class set;
template <class K, class V, class C, class A> class map;
}
typedef FixtureUInt64 FableNScriptUInt64;
enum EAIStateGroupType { };
enum ECameraOp { };
enum EClothingCoversArea { };
enum ECreatureGroup { };
enum ECrime { };
enum ECutsceneBehaviour { };
enum EDoorTriggerType { };
enum EGameAction { };
enum EHeroAbility { };
enum EHeroTitle { };
enum EHeroTrainableStatType { };
enum EMiniGameType { };
enum EMorality { };
enum EMusicSetType { };
enum EOpinion { };
enum EOpinionAttitudeType { };
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };
enum EOpinionReactionType { };
enum EScriptAIPriority { };
enum EScriptVillageAttitude { };
enum EScriptingStateGroups { };
enum ESex { };
enum ETargetingType { };
enum ETextGroupSelectionMethod { };
enum ETutorialCategory { };
namespace NEntityEvents { enum EEventType { }; }

class CScriptThing : public CBaseClass {
public:
    CScriptThing();                                                      
    CScriptThing(const CScriptThing&);                                   
    virtual ~CScriptThing();                                             
    virtual const CCharString& GetName() const;                                                              
    virtual CCharString GetDefName() const;                                                                  
    virtual CCharString GetDataString() const;                                                               
    virtual void SetDataString(const CCharString&);                                                          
    virtual C3DVector GetFocalPos() const;                                                                   
    virtual const C3DVector& GetPos() const;                                                                 
    virtual C3DVector GetHomePos() const;                                                                    
    virtual CCharString GetCurrentMapName() const;                                                           
    virtual CCharString GetHomeMapName() const;                                                              
    virtual float GetAngleXY() const;                                                                        
    virtual CThing* GetPThing() const;                                                                       
    virtual FableNScriptUInt64 GetPThingUniqueID() const;                                                     
    virtual EAIStateGroupType GetCurrentStateGroupType() const;                                              
    virtual EScriptAIPriority GetCurrentScriptPriority() const;                                              
    virtual bool IsSneaking() const;                                                                         
    virtual bool IsAwareOfHero() const;                                                                      
    virtual bool MsgIsTriggeredBy(const CCharString&) const;                                                 
    virtual bool MsgIsKilledBy(const CCharString&) const;                                                    
    virtual CScriptThing MsgWhoKilledMe() const;                                                             
    virtual bool MsgIsUsedBy(const CCharString&) const;                                                      
    virtual bool MsgIsHitBy(const CCharString&) const;                                                       
    virtual bool MsgIsHitByWithFlourish(const CCharString&) const;                                           
    virtual bool MsgIsHitByWithDecapitate(const CCharString&) const;                                         
    virtual bool MsgIsHitByWithWeapon(const CCharString&, const CCharString&) const;                         
    virtual bool MsgIsHitByWithProjectileWeapon(const CCharString&, float&) const;                           
    virtual CScriptThing MsgWhoHitMe() const;                                                                
    virtual bool MsgIsTalkedToBy(const CCharString&) const;                                                  
    virtual CScriptThing MsgWhoTalkedToMe() const;                                                           
    virtual bool MsgExpressionPerformedTo(CCharString&) const;                                               
    virtual CScriptThing MsgWhoExpressedToMe() const;                                                        
    virtual long MsgHowLongWasExpressionPerformed() const;                                                   
    virtual bool MsgIsKnockedOutBy(const CCharString&) const;                                                
    virtual CScriptThing MsgWhoKnockedMeOut() const;                                                         
    virtual bool MsgReceivedMoney(long&) const;                                                              
    virtual bool MsgIsPresentedWithItem(CCharString&) const;                                                 
    virtual bool MsgReceivedInventoryItem() const;                                                           
    virtual bool MsgPerformedSpecialAbility(EHeroAbility) const;                                             
    virtual bool MsgPerformedAnySpecialAbility() const;                                                      
    virtual bool MsgPerformedAnyAggressiveSpecialAbility() const;                                            
    virtual bool MsgPerformedAnyNonAggressiveSpecialAbility() const;                                         
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility, const CCharString&) const;                       
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const;                                  
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const;                        
    virtual bool MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const;                     
    virtual bool MsgOpenedChest() const;                                                                     
    virtual bool MsgHitEnemyWithMeleeWeapon() const;                                                         
    virtual bool MsgHitFriendWithMeleeWeapon() const;                                                        
    virtual bool MsgHitEnemyWithRangedWeapon() const;                                                        
    virtual bool MsgHitFriendWithRangedWeapon() const;                                                       
    virtual bool MsgHitEnemyWithBareHands() const;                                                           
    virtual bool MsgHitFriendWithBareHands() const;                                                          
    virtual bool MsgAttackedWithMeleeWeaponWithoutHittingAnything() const;                                   
    virtual bool MsgAttackedWithBareHandsWithoutHittingAnything() const;                                     
    virtual bool MsgTalkedToAnyone() const;                                                                  
    virtual bool MsgGetThingsKilled(std::vector<unsigned long,std::allocator<unsigned long> >&) const;       
    virtual bool MsgPerformedFlourish() const;                                                               
    virtual bool MsgPerformedSuccessfulFlourish() const;                                                     
    virtual bool MsgOnMoralityChanged(long&) const;                                                          
    virtual bool MsgOnCutSceneAnimEvent(CCharString&) const;                                                 
    virtual bool MsgIsKicked() const;                                                                        
    virtual bool IsUnconscious() const;                                                                      
    virtual bool IsUsable() const;                                                                           
    virtual bool IsBeingCarriedBy(const CCharString&) const;                                                 
    virtual bool IsOpenDoor() const;                                                                         
    virtual bool IsSummonedCreature() const;                                                                 
    virtual void SetAsUsable(bool);                                                                          
    virtual void SetFriendsWithEverythingFlag(bool);                                                         
    virtual bool GetActivationTriggerStatus() const;                                                         
    virtual void SetActivationTriggerStatus(bool);                                                           
    virtual void SetToKillOnLevelUnload(bool);                                                               
    virtual void UpdateThingAttachment();                                                                    
    virtual void IncrementScriptCounter();                                                                   
    virtual void DecrementScriptCounter();                                                                   
    virtual long GetScriptCounter();                                                                         
    virtual bool IsAlive() const;                                                                            
    virtual bool IsDead() const;                                                                             
    virtual bool IsNull() const;                                                                             
    protected: virtual bool IsEqualTo(const CScriptThing&) const; public:                                    
    CScriptThing& operator=(const CScriptThing&);
    bool operator==(const CScriptThing&) const;                          
    bool operator!=(const CScriptThing&) const;                          
    void SetImplementation(CCountedPointer<CScriptThing>);               
private:
    CCountedPointer<CScriptThing> PImp;                                  
protected:
    static C3DVector NullCoord;
    static CCharString NullString;                                       
private:
    void ValidatePImp() const;                                           
};

















class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};



class CGameScriptInterfaceBase : public CBaseClassNonCopyable {
public:
    virtual ~CGameScriptInterfaceBase();                                 
    virtual void Error(const CCharString&, const CCharString&, const CCharString&, unsigned long) const = 0; 
    virtual void TraceMessage(const CCharString&) const = 0;                                             
    virtual void Validate() const = 0;                                                                   
    virtual void SetDebugCameraType(long) const = 0;                                                     
    virtual void DeactivateBoastUi() const = 0;                                                          
    virtual bool IsXbox() const = 0;                                                                     
    virtual void NewScriptFrame() const = 0;                                                             
    virtual bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority) = 0; 
    virtual bool IsEntityUnderScriptedControl(const CScriptThing&) const = 0;                            
    virtual bool IsActiveThreadTerminating() const = 0;                                                  
    virtual bool IsLevelLoaded(const CCharString&) const = 0;                                            
    virtual bool IsRegionLoaded(const CCharString&) const = 0;                                           
    virtual bool IsRegionLoadedAndPreloaded(const CCharString&) const = 0;                               
    virtual bool IsRegionDefLoaded(const CCharString&) const = 0;                                        
    virtual const CCharString& GetRegionName() const = 0;                                                
    virtual bool MsgIsLevelLoaded(const CCharString&) const = 0;                                         
    virtual bool MsgIsLevelUnloaded(const CCharString&) const = 0;                                       
    virtual bool MsgOnLevelLoaded(std::list<CCharString,std::allocator<CCharString> >&) const = 0;       
    virtual bool MsgOnLevelUnloaded(std::list<CCharString,std::allocator<CCharString> >&) const = 0;     
    virtual bool MsgIsRegionLoaded(const CCharString&) const = 0;                                        
    virtual bool MsgIsRegionUnloaded(const CCharString&) const = 0;                                      
    virtual bool MsgOnRegionLoaded(CCharString&) const = 0;                                              
    virtual bool MsgOnRegionUnloaded(CCharString&) const = 0;                                            
    virtual bool MsgOnRegionPreunload(CCharString&) const = 0;                                           
    virtual bool MsgOnQuestCompleted(const CCharString&) const = 0;                                      
    virtual bool MsgOnAnyQuestCompleted(CCharString&) const = 0;                                         
    virtual bool MsgOnQuestFailed(const CCharString&) const = 0;                                         
    virtual bool MsgOnQuestCompletedBeforeScreenShown(const CCharString&) const = 0;                     
    virtual bool MsgOnQuestFailedBeforeScreenShown(const CCharString&) const = 0;                        
    virtual bool MsgOnQuestAccepted(CCharString&) const = 0;                                             
    virtual bool MsgOnFeatAccepted(long&) const = 0;                                                     
    virtual bool MsgIsBoastMade(long) const = 0;                                                         
    virtual bool MsgOnBoastMade(long&, CCharString&) const = 0;                                          
    virtual bool MsgOnBoastsMade(std::vector<std::pair<long,CCharString>,std::allocator<std::pair<long,CCharString> > >&) const = 0; 
    virtual void RemoveBoastMessage() const = 0;                                                         
    virtual bool IsQuestStartScreenActive(CCharString&) const = 0;                                       
    virtual bool MsgOnLeavingQuestStartScreen(CCharString&) const = 0;                                   
    virtual bool MsgOnLeavingExperienceSpendingScreen() const = 0;                                       
    virtual long MsgIsQuestionAnsweredYesOrNo() const = 0;                                               
    virtual bool MsgIsGameInfoClickedPast() const = 0;                                                   
    virtual bool MsgIsTutorialClickedPast() const = 0;                                                   
    virtual bool MsgIsActionModeButtonPressed() const = 0;                                               
    virtual bool MsgOnExpressionPerformed(CCharString&) const = 0;                                       
    virtual bool MsgIsCutSceneSkipped() const = 0;                                                       
    virtual void RemoveAllCutSceneSkippedMessages() const = 0;                                           
    virtual bool MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString&) const = 0;                  
    virtual bool MsgOnHeroUsedTeleporter(CCharString&) const = 0;                                        
    virtual bool MsgOnHeroUsedGuildSeal() const = 0;                                                     
    virtual bool MsgOnGameSavedManually() const = 0;                                                     
    virtual bool MsgOnHeroSlept(bool&) const = 0;                                                        
    virtual bool MsgOnHeroFiredRangedWeapon() const = 0;                                                 
    virtual bool MsgOnHeroCastSpell(EHeroAbility&) const = 0;                                            
    virtual bool MsgOnHeroPickedPocket(CScriptThing&) const = 0;                                         
    virtual bool MsgOnHeroPickedLock(CScriptThing&) const = 0;                                           
    virtual bool MsgOnFishingGameFinished(CScriptThing&) const = 0;                                      
    virtual bool MsgOnTavernGameFinished(const CScriptThing&) const = 0;                                 
    virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString&) const = 0;                                 
    virtual bool MsgOnChestOpeningCancelled() const = 0;                                                 
    virtual void DontPopulateNextLoadedRegion() const = 0;                                               
    virtual void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString&) const = 0;             
    virtual void GetWanderingPopulationScriptDefNameInRegion(const CCharString&, CCharString&) const = 0; 
    virtual bool IsHeroAllowedHenchmenInCurrentRegion() const = 0;                                       
    virtual bool IsHeroAllowedHenchmenInRegion(const CCharString&) const = 0;                            
    virtual void PostAddScriptedEntities() const = 0;                                                    
    virtual bool IsPlayerHoldingUnsheatheRangedWeaponButton() const = 0;                                 
    virtual bool IsPlayerHoldingLockTargetButton() const = 0;                                            
    virtual bool IsPlayerHoldingFireRangedWeaponButton() const = 0;                                      
    virtual bool IsPlayerHoldingFirstPersonTargetingButton() const = 0;                                  
    virtual bool IsHeroInProjectileWeaponMode() const = 0;                                               
    virtual CScriptThing& GetHero() = 0;                                                                 
    virtual CScriptThing GetHeroTargetedThing() const = 0;                                               
    virtual CScriptThing GetThingWithScriptName(const CCharString&, const CCharString&) const = 0;       
    virtual CScriptThing GetThingWithScriptName(const CCharString&) const = 0;                           
    virtual CScriptThing GetRandomThingWithScriptName(const CCharString&) const = 0;                     
    virtual long GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual long GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString&) const = 0;    
    virtual CScriptThing GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const = 0;   
    virtual long GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual long GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const = 0;       
    virtual CScriptThing GetFurthestWithDefName(const CScriptThing&, const CCharString&) const = 0;      
    virtual CScriptThing GetThingWithUID(unsigned __int64) const = 0;                                    
    virtual long GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual long GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual bool IsThingWithThisUIDAlive(unsigned __int64) const = 0;                                    
    virtual long RegisterTimer() = 0;                                                                    
    virtual void DeregisterTimer(long) = 0;                                                              
    virtual void SetTimer(long, long) = 0;                                                               
    virtual long GetTimer(long) const = 0;                                                               
    virtual CScriptThing CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) const = 0; 
    virtual CScriptThing CreateCreatureNearby(const CCharString&, const C3DVector&, float, const CCharString&, bool) const = 0; 
    virtual CScriptThing CreateCreatureOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const = 0; 
    virtual CScriptThing TurnCreatureInto(const CScriptThing&, const CCharString&) const = 0;            
    virtual void SetCreatureCreationDelayFrames(long) = 0;                                               
    virtual void ResetCreatureCreationDelayFrames() = 0;                                                 
    virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, const CCharString&) const = 0; 
    virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, float, const CCharString&) const = 0; 
    virtual CScriptThing CreateObjectOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const = 0; 
    virtual CScriptThing CreateEffect(const CCharString&, const CScriptThing&, const CCharString&, const CCharString&, bool, bool) const = 0; 
    virtual CScriptThing CreateEffect(const CCharString&, const C3DVector&, const CCharString&, float, bool, bool) const = 0; 
    virtual CScriptThing CreateLight(const C3DVector&, const CRGBColour&, const CCharString&, float, float, bool) const = 0; 
    virtual CScriptThing CreateExperienceOrb(const C3DVector&, long) const = 0;                          
    virtual CScriptThing CreateExplosion(const CCharString&, const C3DVector&, const C3DVector*, CCharString) const = 0; 
    virtual CScriptThing CreatePhysicalBarrier(float, const C3DVector&, const C3DVector*, CCharString) const = 0; 
    virtual CScriptThing CreateRumble(const C3DVector&, float, float, CCharString) const = 0;            
    virtual void ClearAllRumbles() const = 0;                                                            
    virtual void RemoveThing(const CScriptThing&, bool, bool) const = 0;                                 
    virtual void ShowOnScreenMessage(const CCharString&, float) const = 0;                               
    virtual void ShowOnScreenMessage(const CCharString&, const CCharString&, const CCharString&, float) const = 0; 
    virtual void ShowOnScreenMessage(const C2DVector&, const CCharString&, const CRGBColour&, const CCharString&) const = 0; 
    virtual void AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const = 0;              
    virtual void AddScreenTitleMessage(const CCharString&, float, bool) const = 0;                       
    virtual void GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) const = 0; 
    virtual void DisplayGameInfo(const CCharString&) const = 0;                                          
    virtual void DisplayGameInfoText(const CWideString&) const = 0;                                      
    virtual bool IsSafeToDisplayGameInfo() const = 0;                                                    
    virtual bool DisplayTutorial(ETutorialCategory) const = 0;                                           
    virtual bool IsTutorialSystemEnabled() const = 0;                                                    
    virtual void GiveHeroWeapon(const CCharString&, bool) const = 0;                                     
    virtual void GiveHeroObject(const CCharString&, long, bool) const = 0;                               
    virtual void SetWeaponAsHerosActiveWeapon(const CCharString&) const = 0;                             
    virtual void GiveHeroItem(const CScriptThing&) const = 0;                                            
    virtual bool GiveHeroItemsFromContainer(const CScriptThing&, bool) const = 0;                        
    virtual void TakeObjectFromHero(const CCharString&) const = 0;                                       
    virtual void GiveHeroGold(long) const = 0;                                                           
    virtual long GetHeroGold() const = 0;                                                                
    virtual void GiveHeroExperience(long) const = 0;                                                     
    virtual void SetHeroAbleToGainExperience(bool) const = 0;                                            
    virtual void SheatheHeroWeapons() const = 0;                                                         
    virtual void SetHeroWillAsUsable(bool) const = 0;                                                    
    virtual void SetHeroWeaponsAsUsable(bool) const = 0;                                                 
    virtual void SetWeaponOutCrimeEnabled(bool) const = 0;                                               
    virtual void SetGuardsIgnoreCrimes(bool) const = 0;                                                  
    virtual void RemoveAllHeroWeapons() const = 0;                                                       
    virtual bool IsReportedOrUnreportedCrimeKnown(const CScriptThing&) const = 0;                        
    virtual void ConfiscateAllHeroItems() const = 0;                                                     
    virtual void ConfiscateAllHeroWeapons() const = 0;                                                   
    virtual void ConfiscateItemsOfTypeFromHero(const CCharString&) const = 0;                            
    virtual void ReturnAllConfiscatedItemsToHero() const = 0;                                            
    virtual bool GiveHeroTutorial(ETutorialCategory) const = 0;                                          
    virtual void MakeHeroCarryItemInHand(const CCharString&) const = 0;                                  
    virtual void MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) const = 0;                     
    virtual void AddTattooToHero(const CCharString&) const = 0;                                          
    virtual void GiveHeroAbility(EHeroAbility, bool) const = 0;                                          
    virtual bool IsPlayerZTargetingThing(const CScriptThing&) const = 0;                                 
    virtual bool IsPlayerCreatureBlocking() const = 0;                                                   
    virtual bool IsPlayerCreatureReadyToFireProjectileWeapon(float&) const = 0;                          
    virtual long GetPlayerCreatureCombatMultiplier() const = 0;                                          
    virtual long GetPlayerCreatureCombatMultiplierRunningNumHits() const = 0;                            
    virtual void ResetPlayerCreatureCombatMultiplier() const = 0;                                        
    virtual bool IsPlayerCreatureFlourishEnabled() const = 0;                                            
    virtual void SetPlayerCreatureOnlyTarget(const CScriptThing&) const = 0;                             
    virtual void ResetPlayerCreatureOnlyTarget() const = 0;                                              
    virtual void RespawnHero(bool) const = 0;                                                            
    virtual void GiveHeroMorality(float) const = 0;                                                      
    virtual float GetHeroMorality() const = 0;                                                           
    virtual EMorality GetHeroMoralityCategory() const = 0;                                               
    virtual void ChangeHeroMoralityDueToTheft() const = 0;                                               
    virtual void ChangeHeroMoralityDueToPicklock() const = 0;                                            
    virtual void GiveHeroRenownPoints(long) const = 0;                                                   
    virtual long GetHeroRenownLevel() const = 0;                                                         
    virtual bool IsHeroRenownLevelFull() const = 0;                                                      
    virtual void IncreaseHeroRenownLevel() const = 0;                                                    
    virtual long GetHeroStrengthLevel() const = 0;                                                       
    virtual long GetHeroSkillLevel() const = 0;                                                          
    virtual long GetHeroWillLevel() const = 0;                                                           
    virtual long GetHeroStatLevel(EHeroTrainableStatType) const = 0;                                     
    virtual long GetHeroStatMax(EHeroTrainableStatType) const = 0;                                       
    virtual void SetHeroAge(float) const = 0;                                                            
    virtual float GetHeroAge() const = 0;                                                                
    virtual void SetHeroAsTeenager(bool) const = 0;                                                      
    virtual void SetHeroAsApprentice(bool) const = 0;                                                    
    virtual float GetDistanceHeroCanBeHeardFrom() const = 0;                                             
    virtual long GetHeroRoughExperienceLevel() const = 0;                                                
    virtual long GetHeroExperienceAvailableToSpend() const = 0;                                          
    virtual float GetHeroFatness() const = 0;                                                            
    virtual float GetHeroScariness() const = 0;                                                          
    virtual float GetHeroAttractiveness() const = 0;                                                     
    virtual float GetHeroWillEnergyLevel() const = 0;                                                    
    virtual void SetHeroWillEnergyLevel(float) const = 0;                                                
    virtual void SetHeroWillEnergyAsAbleToRefill(bool) const = 0;                                        
    virtual long GetNumberOfItemsOfTypeInInventory(const CCharString&) const = 0;                        
    virtual bool IsObjectInThingsPossession(const CCharString&, const CScriptThing&) const = 0;          
    virtual bool IsHeroHandLampLit() const = 0;                                                          
    virtual void SetHeroHandLampAsLit(bool) const = 0;                                                   
    virtual bool IsWearingClothingItem(const CScriptThing&, const CCharString&) const = 0;               
    virtual bool IsHeroNaked() const = 0;                                                                
    virtual void RemoveHeroClothing() const = 0;                                                         
    virtual void SetHeroAsWearing(const CCharString&) const = 0;                                         
    virtual void ChangeHeroHairstyle(const CCharString&) const = 0;                                      
    virtual void RemoveHeroHairstyle() const = 0;                                                        
    virtual bool IsWearingHairstyle(CScriptThing&, const CCharString&) const = 0;                        
    virtual bool IsPlayerCarryingItemOfType(const CCharString&) const = 0;                               
    virtual bool IsPlayerWieldingWeapon() const = 0;                                                     
    virtual bool IsEntityWieldingWeapon(const CScriptThing&) const = 0;                                  
    virtual bool IsEntityWieldingMeleeWeapon(const CScriptThing&) const = 0;                             
    virtual bool IsEntityWieldingRangedWeapon(const CScriptThing&) const = 0;                            
    virtual bool GetPreviouslyWieldedMeleeWeaponName(CCharString&) const = 0;                            
    virtual bool GetPreviouslyWieldedRangedWeaponName(CCharString&) const = 0;                           
    virtual bool IsEntityAbleToAttack(const CScriptThing&) const = 0;                                    
    virtual CScriptThing EntityGetThingInPrimarySlot(const CScriptThing&) const = 0;                     
    virtual void ApplyHeroPenaltyForDeath() const = 0;                                                   
    virtual void GiveHeroTitle(const CCharString&) const = 0;                                            
    virtual EHeroTitle GetHeroTitle() const = 0;                                                         
    virtual void EntitySetAsMarryable(const CScriptThing&, bool) const = 0;                              
    virtual void EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const = 0;          
    virtual void EntityForceMarriageToHero(const CScriptThing&, bool) const = 0;                         
    virtual bool IsEntityMarriedToHero(const CScriptThing&) const = 0;                                   
    virtual bool IsEntityMarriable(const CScriptThing&) const = 0;                                       
    virtual bool GetHeroHasMarried() const = 0;                                                          
    virtual bool GetHeroHasCurrentMarriage() const = 0;                                                  
    virtual bool GetHeroHasDivorcedMarriage() const = 0;                                                 
    virtual bool GetHeroHasChildren() const = 0;                                                         
    virtual bool GetHeroHasMurderedWife() const = 0;                                                     
    virtual bool IsHeroChild() const = 0;                                                                
    virtual void CancelHeroTeleportEffects() const = 0;                                                  
    virtual long GetNumberOfTimesHeroHasHadSex() const = 0;                                              
    virtual void SetNumberOfTimesHeroHasHadSex(long) const = 0;                                          
    virtual void SetHeroAsHavingHadSex(bool) const = 0;                                                  
    virtual void SetHeroAsHavingHadGaySex(bool) const = 0;                                               
    virtual void GiveThingHeroRewardItem(const CScriptThing&, const CCharString&, const CCharString&) const = 0; 
    virtual void GiveThingItemInHand(const CScriptThing&, const CCharString&, bool) const = 0;           
    virtual void GiveThingItemInSlot(const CScriptThing&, const CCharString&, const CCharString&) const = 0; 
    virtual void GiveHeroExpression(const CCharString&, long, bool) const = 0;                           
    virtual bool HeroHasExpression(const CCharString&) const = 0;                                        
    virtual bool IsHeroPerformingExpression(const CCharString&) const = 0;                               
    virtual bool IsDPadButtonHeldForExpression(const CCharString&) const = 0;                            
    virtual void EntityFollowThing(const CScriptThing&, const CScriptThing&, float, bool) const = 0;     
    virtual void EntityStopFollowing(const CScriptThing&) const = 0;                                     
    virtual bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual bool GetHeroSummonedCreaturesList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual bool GetPerceivingHeroEntityList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const = 0; 
    virtual bool IsEntityFollowingHero(const CScriptThing&) const = 0;                                   
    virtual void EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) const = 0;                    
    virtual void EntitySetAsAllowedToChangeRegionFollowingState(const CScriptThing&, bool) const = 0;    
    virtual void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing&, bool) const = 0;   
    virtual void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const CScriptThing&, bool) const = 0; 
    virtual void TeleportAllFollowersToHeroPosition() const = 0;                                         
    virtual void EntityTeleportToHeroPosition(const CScriptThing&) const = 0;                            
    virtual void SendEntityEvent(NEntityEvents::EEventType, const CScriptThing&, const CScriptThing&, CThing*) const = 0; 
    virtual float GetWaterHeightAtPosition(const C3DVector&) const = 0;                                  
    virtual bool IsFishingSpotEnabled(const CScriptThing&) const = 0;                                    
    virtual void DisableFishingSpot(const CScriptThing&) const = 0;                                      
    virtual void UpdateFishWeight(const CScriptThing&) const = 0;                                        
    virtual float GetBestFishWeight(const CCharString&) const = 0;                                       
    virtual void HeroGoFishing(bool) const = 0;                                                          
    virtual long GetHeroFishingLevel() const = 0;                                                        
    virtual void HeroGoDigging() const = 0;                                                              
    virtual void HeroStopDigging() const = 0;                                                            
    virtual void HeroPlayOracleMinigame() const = 0;                                                     
    virtual bool IsHeroPlayingOracleMinigame() const = 0;                                                
    virtual bool HasHeroWonOracleMinigame() const = 0;                                                   
    virtual void HeroPlayFireheartMinigame() const = 0;                                                  
    virtual void HeroQuitFireheartMinigame() const = 0;                                                  
    virtual bool HasHeroForceQuitFireheartMinigame() const = 0;                                          
    virtual float GetHeroHealth() const = 0;                                                             
    virtual float GetHeroHealthMax() const = 0;                                                          
    virtual float GetHeroHealthPercentage() const = 0;                                                   
    virtual long GetHeroWillEnergy() const = 0;                                                          
    virtual long GetHeroWillEnergyMax() const = 0;                                                       
    virtual long GetHeroWillEnergyPercentage() const = 0;                                                
    virtual void ChangeHeroHealthBy(float, bool, bool) const = 0;                                        
    virtual float GetHealth(const CScriptThing&) const = 0;                                              
    virtual void ModifyThingHealth(const CScriptThing&, float, bool) const = 0;                          
    virtual void EntitySetMaxHealth(const CScriptThing&, float, bool) const = 0;                         
    virtual void SetThingAsKilled(const CScriptThing&) const = 0;                                        
    virtual void GiveHeroNewQuestObjective(const CCharString&, unsigned long) const = 0;                 
    virtual void TellHeroQuestObjectiveCompleted(unsigned long) const = 0;                               
    virtual void TellHeroQuestObjectiveFailed(unsigned long) const = 0;                                  
    virtual void AddQuestRegion(const CCharString&, const CCharString&) const = 0;                       
    virtual void SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&) const = 0;                 
    virtual bool IsHeroOnQuest() const = 0;                                                              
    virtual void HeroReceiveMessageFromGuildMaster(const CCharString&, const CCharString&, bool, bool) const = 0; 
    virtual void SetGuildMasterMessages(bool) const = 0;                                                 
    virtual void ActivateQuest(const CCharString&) const = 0;                                            
    virtual void ActivateMultipleQuests(const CArray<CCharString>&) const = 0;                           
    virtual void ActivateQuestWithoutLoadingResources(const CCharString&) const = 0;                     
    virtual void ActivateMultipleQuestsWithoutLoadingResources(const CArray<CCharString>&) const = 0;    
    virtual void DeactivateQuest(const CCharString&, unsigned long) const = 0;                           
    virtual void DeactivateQuestLater(const CCharString&, unsigned long) const = 0;                      
    virtual void PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) const = 0;                   
    virtual void PrepareQuestsWhenFinalQuestIsCompleted() const = 0;                                     
    virtual bool IsQuestActive(const CCharString&) const = 0;                                            
    virtual bool IsQuestRegistered(const CCharString&) const = 0;                                        
    virtual bool IsQuestCompleted(const CCharString&) const = 0;                                         
    virtual bool IsQuestFailed(const CCharString&) const = 0;                                            
    virtual void SetQuestAsCompleted(const CCharString&, bool, bool, bool) const = 0;                    
    virtual void SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const = 0;         
    virtual void SetQuestAsPersistent(const CCharString&, bool) const = 0;                               
    virtual const CCharString& GetExclusiveQuestScriptName() const = 0;                                  
    virtual void AddQuestCard(const CCharString&, const CCharString&, bool, bool) const = 0;             
    virtual void RemoveQuestCardFromGuild(const CCharString&) const = 0;                                 
    virtual void RemoveQuestCardFromHero(const CCharString&) const = 0;                                  
    virtual void GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) const = 0;      
    virtual void SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) const = 0; 
    virtual void SetQuestCardGoldReward(const CCharString&, long) const = 0;                             
    virtual void SetQuestCardRenownReward(const CCharString&, long) const = 0;                           
    virtual void RemoveAllAvailableQuestCardsFromGuild() const = 0;                                      
    virtual void FailAllActiveQuests() const = 0;                                                        
    virtual void GetAllActiveQuestInfo(std::vector<CCharString,std::allocator<CCharString> >&, std::vector<CCharString,std::allocator<CCharString> >&) const = 0; 
    virtual void AddFeatCard(long, const CCharString&, const CCharString&) const = 0;                    
    virtual void AddBoast(const CCharString&, long, long, long, bool, const CCharString&, long) const = 0; 
    virtual void RemoveBoast(long, const CCharString&) const = 0;                                        
    virtual void SetBoastAsFailed(long, const CCharString&) const = 0;                                   
    virtual void SetBoastAsCompleted(long, const CCharString&) const = 0;                                
    virtual bool IsBoastTaken(long, const CCharString&) const = 0;                                       
    virtual void AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, long) const = 0;
    virtual void KickOffQuestStartScreen(const CCharString&, bool, bool) const = 0;                      
    virtual void KickOffDeathScreen() const = 0;                                                         
    virtual void KickOffCreditsScreen(CCharString&) const = 0;                                           
    virtual void SetPreferredQuickAccessItem(const CCharString&, long, long) const = 0;                  
    virtual CCharString GetDeathRecoveryMarkerName() const = 0;                                          
    virtual void SetDeathRecoveryMarkerName(const CCharString&) const = 0;                               
    virtual void ResetDeathRecoveryMarkerNameToDefault() const = 0;                                      
    virtual bool IsToFailQuestOnDeath() const = 0;                                                       
    virtual void SetWhetherToFailQuestOnDeath(bool) const = 0;                                           
    virtual void ResetWhetherToFailQuestOnDeathToDefault() const = 0;                                    
    virtual CScriptThing GetMostRecentValidUsedTarget() const = 0;                                       
    virtual CCharString GetMostRecentValidUsedTargetName() const = 0;                                    
    virtual void DisplayQuestInfo(bool) const = 0;                                                       
    virtual void SetQuestInfoName(const char*) const = 0;                                                
    virtual void SetQuestInfoText(const char*) const = 0;                                                
    virtual long AddQuestInfoBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) const = 0; 
    virtual long AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour&, const CCharString&, float) const = 0; 
    virtual long AddQuestInfoTimer(const CTimer&, const CCharString&, float) const = 0;                  
    virtual long AddQuestInfoCounter(const CCounter&, const CCharString&, float) const = 0;              
    virtual long AddQuestInfoCounter(const CCharString&, long, float) const = 0;                         
    virtual long AddQuestInfoCounterList(const CCharString&, long, float) const = 0;                     
    virtual long AddQuestInfoTick(const CCharString&, bool, float) const = 0;                            
    virtual long AddQuestInfoTick(EGameAction, bool, float) const = 0;                                   
    virtual void UpdateQuestInfoBar(long, float, float, float) const = 0;                                
    virtual void ChangeQuestInfoBarColour(long, const CRGBColour&, const CRGBColour&) const = 0;         
    virtual void UpdateQuestInfoTimer(long, float) const = 0;                                            
    virtual void UpdateQuestInfoCounter(long, long, long) const = 0;                                     
    virtual void UpdateQuestInfoCounterList(long, long, long) const = 0;                                 
    virtual void UpdateQuestInfoTick(long, bool) const = 0;                                              
    virtual void RemoveQuestInfoElement(long) const = 0;                                                 
    virtual void RemoveAllQuestInfoElements() const = 0;                                                 
    virtual void DisplayTime(bool) const = 0;                                                            
    virtual void DisplayMoneyBag(bool) const = 0;                                                        
    virtual void DisplayMiniGameInfo(bool, EMiniGameType) const = 0;                                     
    virtual void UpdateMiniGameInfoBar(float) const = 0;                                                 
    virtual bool IsEntityPickPocketable(const CScriptThing&) const = 0;                                  
    virtual bool IsEntityPickLockable(const CScriptThing&) const = 0;                                    
    virtual bool IsEntityStealable(const CScriptThing&) const = 0;                                       
    virtual void EntitySetAsPickPocketed(const CScriptThing&) const = 0;                                 
    virtual void EntitySetAsPickLocked(const CScriptThing&) const = 0;                                   
    virtual void EntitySetAsStolen(const CScriptThing&) const = 0;                                       
    virtual void MiniMapAddMarker(const CScriptThing&, const CCharString&) const = 0;                    
    virtual void MiniMapSetMarkerGraphic(const CScriptThing&, const CCharString&) const = 0;             
    virtual void MiniMapRemoveMarker(const CScriptThing&) const = 0;                                     
    virtual void MiniMapRemoveAllMarkers() const = 0;                                                    
    virtual void MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) const = 0; 
    virtual void MiniMapSetAsEnabled(bool) const = 0;                                                    
    virtual void EntitySetAsHiddenOnMiniMap(const CScriptThing&, bool) const = 0;                        
    virtual void SetHUDEnabled(bool) const = 0;                                                          
    virtual bool TextEntryExists(const CCharString&) const = 0;                                          
    virtual CCharString GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const = 0; 
    virtual void SetThingHasInformation(const CScriptThing&, bool, bool, bool) const = 0;                
    virtual void ClearThingHasInformation(const CScriptThing&) const = 0;                                
    virtual void EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const = 0;        
    virtual void EntityResetAsPureAINarrator(const CScriptThing&) const = 0;                             
    virtual long AddNewConversation(const CScriptThing&, bool, bool) const = 0;                          
    virtual void AddPersonToConversation(long, const CScriptThing&) const = 0;                           
    virtual void AddLineToConversation(long, const CCharString&, bool, const CScriptThing&, const CScriptThing&) const = 0; 
    virtual void RemoveConversation(long, bool) const = 0;                                               
    virtual bool IsConversationActive(long) const = 0;                                                   
    virtual void PlayAVIMovie(const CCharString&) const = 0;                                             
    virtual void StartMovieSequence(const CCharString&, CScriptGameResourceObjectMovieBase&) = 0;        
    virtual void FixMovieSequenceCamera(bool) const = 0;                                                 
    virtual void FadeScreenOutUntilNextCallToFadeScreenIn(float, float) = 0;                             
    virtual bool FadeScreenOut(float, float, CRGBColour) = 0;                                            
    virtual void FadeScreenIn() = 0;                                                                     
    virtual bool IsScreenFadingOut() const = 0;                                                          
    virtual void Pause(float) const = 0;                                                                 
    virtual void EndCutFade() = 0;                                                                       
    protected: virtual void EndLetterBox() = 0; public:                                                  
    virtual void PauseAllNonScriptedEntities(bool) = 0;                                                  
    virtual void PauseAllEntities(bool) = 0;                                                             
    virtual void SetAllowScreenFadingOnNextRegionChange(bool) = 0;                                       
    virtual void SetAllowScreenFadingIfAlreadyFaded(bool) = 0;                                           
    virtual void SetToKeepHeroAbilitiesDuringCutscenes(bool) const = 0;                                  
    virtual void SetToDisplayTutorialsDuringCutscenes(bool) const = 0;                                   
    virtual void SetCutsceneMode(bool, bool) const = 0;                                                  
    virtual bool IsInCutscene() const = 0;                                                               
    virtual void SetCutsceneSkippable(bool) const = 0;                                                   
    virtual void SetCutsceneSkippableWhilePaused(bool) const = 0;                                        
    virtual void SetCutsceneActionMode(bool, const CCharString&) const = 0;                              
    virtual void PreloadNewScene(float) const = 0;                                                       
    virtual void StartProgressDisplay() const = 0;                                                       
    virtual void StopProgressDisplay() const = 0;                                                        
    virtual void SetScreenMessagesEnabled(bool) const = 0;                                               
    virtual bool IsHeroControlledByPlayer() const = 0;                                                   
    virtual bool IsInMovieSequence() const = 0;                                                          
    virtual void CancelAbilitiesForCutscene() const = 0;                                                 
    virtual void ResumeAbilitiesForCutscene() const = 0;                                                 
    virtual void CancelUsingAbility(EHeroAbility) const = 0;                                             
    virtual void SetAbilityAvailability(EHeroAbility, bool) const = 0;                                   
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool) const = 0;                                    
    virtual void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const = 0;                   
    virtual void RemoveDeadCreature(const CCharString&) const = 0;                                       
    virtual void CameraSetCameraPreloadFlag(bool) const = 0;                                             
    virtual void CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const = 0;        
    virtual void CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const = 0;             
    virtual void CameraMoveToPosAndLookAtPos(const C3DVector&, const C3DVector&, float) const = 0;       
    virtual void CameraMoveToPosAndLookAtThing(const C3DVector&, const CScriptThing&, float) const = 0;  
    virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const C3DVector&, float, float) const = 0; 
    virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const CScriptThing&, float, float) const = 0; 
    virtual void CameraMoveBetweenLookFromAndLookTo(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, float) const = 0; 
    virtual void CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, long, long) const = 0; 
    virtual void CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, long, long) const = 0; 
    virtual void CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, long, long) const = 0; 
    virtual void CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, long, long) const = 0; 
    virtual void CameraDoConversation(const CScriptThing&, const CScriptThing&, ECameraOp, bool) const = 0; 
    virtual void CameraDefault() = 0;                                                                    
    virtual void CameraResetToViewBehindHero(float) = 0;                                                 
    virtual bool IsCameraInScriptedMode() const = 0;                                                     
    virtual void CameraUseScreenEffect(float, float, float) const = 0;                                   
    virtual void CameraCancelScreenEffect() const = 0;                                                   
    virtual bool IsCameraPosOnScreen(const C3DVector&) const = 0;                                        
    virtual float GetGameAngleXY() const = 0;                                                            
    virtual void CameraEarthquakeIntensityAtPos(const C3DVector&, float, float) const = 0;               
    virtual void CameraShake(float, float) const = 0;                                                    
    virtual void OpenDoor(const CScriptThing&) const = 0;                                                
    virtual void CloseDoor(const CScriptThing&) const = 0;                                               
    virtual void OpenHouseDoors(const CScriptThing&) const = 0;                                          
    virtual void CloseHouseDoors(const CScriptThing&) const = 0;                                         
    virtual void JamDoor(const CScriptThing&) const = 0;                                                 
    virtual void SetDoorTriggerType(const CScriptThing&, EDoorTriggerType) const = 0;                    
    virtual void OverrideAutomaticHouseLocking(const CScriptThing&, bool) const = 0;                     
    virtual void SetHouseOwnedByPlayer(const CScriptThing&, bool, bool) const = 0;                       
    virtual void SetBuyableHouseAsScripted(const CScriptThing&, bool) const = 0;                         
    virtual bool IsChestOpen(const CScriptThing&) const = 0;                                             
    virtual bool OpenChest(const CScriptThing&, bool) const = 0;                                         
    virtual void CloseChest(const CScriptThing&) const = 0;                                              
    virtual long GetNumberOfKeysNeededToUnlockChest(const CScriptThing&, CCharString&) const = 0;        
    virtual void DisplayLockedChestMessage(const CScriptThing&) const = 0;                               
    virtual void SetTrophyAsMountable(const CScriptThing&, bool) const = 0;                              
    virtual void SetVillageLimbo(const CScriptThing&, bool) const = 0;                                   
    virtual void SetCreatureNotReload(const CScriptThing&) const = 0;                                    
    virtual bool IsSleepingTime(const CScriptThing&) const = 0;                                          
    virtual void EnableGuards(const CScriptThing&, bool) const = 0;                                      
    virtual void EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const = 0;        
    virtual CScriptThing TryToRespawnDefNamed(const CScriptThing&, const CCharString&, const C3DVector&) const = 0; 
    virtual void ClearHeroEnemyOfGuards(const CScriptThing&) const = 0;                                  
    virtual void SetThingAsUsable(const CScriptThing&, bool) const = 0;                                  
    virtual void SetThingHomeBuilding(const CScriptThing&, const CScriptThing&) const = 0;               
    virtual void SetVillageAttitude(const CScriptThing&, EScriptVillageAttitude) const = 0;              
    virtual void AddCrimeCommitted(const CScriptThing&, ECrime, bool, const CScriptThing*, const CScriptThing*, EOpinionPostDeedType) const = 0; 
    virtual void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) const = 0;           
    virtual void ClearThingBestEnemyTarget(const CScriptThing&) const = 0;                               
    virtual void EntitySetInLimbo(const CScriptThing&, bool, bool) const = 0;                            
    virtual bool IsEntityInLimbo(const CScriptThing&) const = 0;                                         
    virtual bool EntityGetShotStrikePos(const CScriptThing&, C3DVector&) const = 0;                      
    virtual void EntitySetNegateAllHits(const CScriptThing&, bool) const = 0;                            
    virtual void EntitySetEvadeAllHits(const CScriptThing&, bool) const = 0;                             
    virtual void EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) const = 0;                  
    virtual void EntitySetAlwaysBlockAttacksFromThing(const CScriptThing&, const CScriptThing&, bool) const = 0; 
    virtual void EntitySetAttackThingImmediately(const CScriptThing&, const CScriptThing&, bool, bool) const = 0; 
    virtual void EntitySetCombatType(const CScriptThing&, const CCharString&) const = 0;                 
    virtual void EntityResetCombatTypeToDefault(const CScriptThing&) const = 0;                          
    virtual void EntitySetMaxNumberOfAttackers(const CScriptThing&, long) const = 0;                     
    virtual void EntityClearMaxNumberOfAttackers(const CScriptThing&) const = 0;                         
    virtual void EntityAttachToScript(const CScriptThing&, const CCharString&) const = 0;                
    virtual void EntitySetCombatAbility(const CScriptThing&, float) const = 0;                           
    virtual void EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const = 0;              
    virtual void EntityClearRangedTarget(const CScriptThing&) const = 0;                                 
    virtual void EntitySetTargetable(const CScriptThing&, bool) const = 0;                               
    virtual void EntitySetTargetingType(const CScriptThing&, ETargetingType) const = 0;                  
    virtual void EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const = 0;           
    virtual void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const = 0;        
    virtual void EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const = 0; 
    virtual void EntitySetFacingAngle(const CScriptThing&, float, bool) const = 0;                       
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const = 0; 
    virtual void EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const = 0;       
    virtual void SetThingPersistent(const CScriptThing&, bool) const = 0;                                
    virtual void EntitySetThingAsWantingMoney(const CScriptThing&, long) const = 0;                      
    virtual void EntitySetAppearanceMorphSeed(const CScriptThing&, long) const = 0;                      
    virtual void SetEntityAsRegionFollowing(const CScriptThing&, const CScriptThing&, bool) const = 0;   
    virtual void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool) const = 0;        
    virtual void EntitySetAppearanceSeed(const CScriptThing&, unsigned long) const = 0;                  
    virtual void EntityGetAppearanceSeed(const CScriptThing&, unsigned long&) const = 0;                 
    virtual void EntitySetAsForSale(const CScriptThing&, bool) const = 0;                                
    virtual void EntitySetStockItemPrice(const CScriptThing&, long) const = 0;                           
    virtual long EntityGetStockItemPrice(const CScriptThing&) const = 0;                                 
    virtual void EntityPlayObjectAnimation(const CScriptThing&, const CCharString&, bool) const = 0;     
    virtual void EntitySetMaxRunningSpeed(const CScriptThing&, float) const = 0;                         
    virtual void EntitySetMaxWalkingSpeed(const CScriptThing&, float) const = 0;                         
    virtual void EntityResetMaxRunningSpeed(const CScriptThing&) const = 0;                              
    virtual void EntityResetMaxWalkingSpeed(const CScriptThing&) const = 0;                              
    virtual void EntityAttachToVillage(const CScriptThing&, const CScriptThing&) const = 0;              
    virtual void EntitySetAsSittingOnFloor(const CScriptThing&, bool) const = 0;                         
    virtual void EntitySetAsHavingBoundHands(CScriptThing&, bool) const = 0;                             
    virtual void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing&) const = 0;                     
    virtual void EntitySetAsScared(const CScriptThing&, bool) const = 0;                                 
    virtual void EntitySetAsDrunk(CScriptThing&, bool) const = 0;                                        
    virtual void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) const = 0;           
    virtual void EntityForceToLookAtCamera(const CScriptThing&, bool) const = 0;                         
    virtual void EntityForceToLookAtNothing(const CScriptThing&) const = 0;                              
    virtual void EntityResetForceToLookAt(const CScriptThing&) const = 0;                                
    virtual void EntitySetShotAccuracyPercentage(const CScriptThing&, long) const = 0;                   
    virtual CScriptThing EntityGetStandingOnThing(const CScriptThing&) const = 0;                        
    virtual CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) const = 0;                 
    virtual void EntityDropGenericBox(const CScriptThing&) const = 0;                                    
    virtual void EntitySheatheWeapons(const CScriptThing&, bool) const = 0;                              
    virtual void EntityUnsheatheWeapons(const CScriptThing&, bool) const = 0;                            
    virtual void EntityUnsheatheMeleeWeapon(const CScriptThing&, bool) const = 0;                        
    virtual void EntityUnsheatheRangedWeapon(const CScriptThing&, bool) const = 0;                       
    virtual void EntitySetAlpha(const CScriptThing&, float, bool) const = 0;                             
    virtual void EntitySetAsDrawable(const CScriptThing&, bool) const = 0;                               
    virtual void EntitySetCutsceneBehaviour(const CScriptThing&, ECutsceneBehaviour) const = 0;          
    virtual ESex EntityGetSex(const CScriptThing&) const = 0;                                            
    virtual void EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing&, bool) const = 0;          
    virtual void EntitySetAsRespondToHit(const CScriptThing&, bool) const = 0;                           
    virtual void EntitySetAsDamageable(const CScriptThing&, bool) const = 0;                             
    virtual void EntitySetAsKillable(const CScriptThing&, bool, bool) const = 0;                         
    virtual void EntitySetAsLocked(const CScriptThing&, bool) const = 0;                                 
    virtual void EntityDecapitate(const CScriptThing&) const = 0;                                        
    virtual void EntityGiveGold(const CScriptThing&, long) const = 0;                                    
    virtual void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const = 0;                    
    virtual long EntityGetBossPhase(const CScriptThing&) const = 0;                                      
    virtual void EntitySetBossPhase(const CScriptThing&, long) const = 0;                                
    virtual void EntityResetCreatureMode(const CScriptThing&) const = 0;                                 
    virtual bool EntitySetAsReceivingEvents(const CScriptThing&, bool) const = 0;                        
    virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool) const = 0;          
    virtual void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool) const = 0;              
    virtual void EntityLeaveCombatStance(const CScriptThing&) const = 0;                                 
    virtual void EntitySetAsUseMovementInActions(const CScriptThing&, bool) const = 0;                   
    virtual void EntitySetAsDisplayingEmoteIcon(const CScriptThing&, bool) const = 0;                    
    virtual void EntitySetAsCollidableToThings(const CScriptThing&, bool) const = 0;                     
    virtual void EntityEnableGravity(const CScriptThing&, bool) const = 0;                               
    virtual void EntitySetLightAsOn(const CScriptThing&, bool) const = 0;                                
    virtual void EntityFadeOut(const CScriptThing&, float) const = 0;                                    
    virtual void EntityFadeIn(const CScriptThing&, float) const = 0;                                     
    virtual void EntityBeginLoadingAnimation(const CScriptThing&, const CCharString&) const = 0;         
    virtual void EntityBeginLoadingBasicAnimations(const CScriptThing&) const = 0;                       
    virtual bool EntityCastForcePush(const CScriptThing&, bool) const = 0;                               
    virtual void EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const = 0;        
    virtual void BeginLoadingMesh(const CCharString&) const = 0;                                         
    virtual bool EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const = 0;       
    virtual void EntityStartScreamerSuperAttackThing(const CScriptThing&, const CScriptThing&) const = 0; 
    virtual void EntityEndScreamerSuperAttackThing(const CScriptThing&) const = 0;                       
    virtual void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool) = 0;                                     
    virtual void SetLightColour(const CScriptThing&, const CRGBColour&) const = 0;                       
    virtual void CreatureGeneratorSetFamily(const CScriptThing&, const CCharString&) const = 0;          
    virtual void CreatureGeneratorTrigger(const CScriptThing&) const = 0;                                
    virtual void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const CScriptThing&, bool) const = 0; 
    virtual bool CreatureGeneratorIsDepleted(const CScriptThing&) const = 0;                             
    virtual bool CreatureGeneratorIsDestroyed(const CScriptThing&) const = 0;                            
    virtual void CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing&, const CCharString&) const = 0; 
    virtual void CreatureGeneratorSetNumTriggers(const CScriptThing&, long) const = 0;                   
    virtual long CreatureGeneratorGetNumGeneratedCreatures(const CScriptThing&) const = 0;               
    virtual bool CreatureGeneratorAreAllCreaturesAlive(const CScriptThing&) const = 0;                   
    virtual void CreatureGeneratorAddTriggerer(const CScriptThing&, const CScriptThing&) const = 0;      
    virtual void CreatureGeneratorRemoveTriggerer(const CScriptThing&, const CScriptThing&) const = 0;   
    virtual void SetCreatureGeneratorEnabled(const CScriptThing&, bool) const = 0;                       
    virtual void SetCreatureGeneratorsEnabled(const CCharString&, bool) const = 0;                       
    virtual void SetCreatureGeneratorsEnabledDuringScript(const CCharString&, bool) const = 0;           
    virtual void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const = 0;            
    virtual bool IsCreatureGenerationEnabledForRegion(const CCharString&) const = 0;                     
    virtual bool IsCreatureFlying(const CScriptThing&) const = 0;                                        
    virtual void SetTeleporterAsActive(const CScriptThing&, bool) const = 0;                             
    virtual bool IsTeleporterActive(const CScriptThing&) const = 0;                                      
    virtual void SetTeleportingAsActive(bool) const = 0;                                                 
    virtual bool IsTeleportingActive() const = 0;                                                        
    virtual void SetRegionExitAsActive(const CScriptThing&, bool) const = 0;                             
    virtual void SetRegionEntranceAsActive(const CScriptThing&, bool) const = 0;                         
    virtual void SetRegionTextDisplayAsActive(bool) const = 0;                                           
    virtual void SetHeroSleepingAsEnabled(bool) const = 0;                                               
    virtual bool IsHeroSleepingEnabled() const = 0;                                                      
    virtual void SetExperienceSpendingAsEnabled(bool) const = 0;                                         
    virtual void SetMoralityChangingAsEnabled(bool) const = 0;                                           
    virtual void SetSummonerDeathExplosionAffectsHero(bool) const = 0;                                   
    virtual CScriptThing GetNearestEnabledDiggingSpot(const CScriptThing&) const = 0;                    
    virtual bool IsDiggingSpotEnabled(const CScriptThing&) const = 0;                                    
    virtual bool IsDiggingSpotHidden(const CScriptThing&) const = 0;                                     
    virtual void SetDiggingSpotAsHidden(const CScriptThing&, bool) const = 0;                            
    virtual bool CheckForCameraMessage(const CCharString&) const = 0;                                    
    virtual void WaitForCameraMessage(const CCharString&) const = 0;                                     
    virtual void SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const = 0;           
    virtual void SetFireToThing(const CScriptThing&) const = 0;                                          
    virtual void ExtinguishFiresOnThing(const CScriptThing&) const = 0;                                  
    virtual bool IsThingOnFire(const CScriptThing&) const = 0;                                           
    virtual void AddItemToContainer(const CScriptThing&, const CCharString&) const = 0;                  
    virtual void RemoveItemFromContainer(const CScriptThing&, const CCharString&) const = 0;             
    virtual void EntitySetDeathContainerAsEnabled(const CScriptThing&, bool) const = 0;                  
    virtual void GetItemDefNamesFromContainer(const CScriptThing&, std::vector<CCharString,std::allocator<CCharString> >&) const = 0; 
    virtual void SetCreatureBrain(const CScriptThing&, const CCharString&) const = 0;                    
    virtual void EntitySetStategroupEnabled(const CScriptThing&, const CCharString&, bool) const = 0;    
    virtual void EntitySetAllStategroupsEnabled(const CScriptThing&, bool) const = 0;                    
    virtual void EntitySetCombatEnabled(const CScriptThing&, bool) const = 0;                            
    virtual void EntitySetSleepEnabled(const CScriptThing&, bool) const = 0;                             
    virtual void EntitySetOpinionReactionsEnabled(const CScriptThing&, bool) const = 0;                  
    virtual void EntitySetDeedReactionsEnabled(const CScriptThing&, bool) const = 0;                     
    virtual void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned long,std::less<unsigned long>,std::allocator<unsigned long> >&) const = 0; 
    virtual void EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const = 0;       
    virtual void EntityUnsetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const = 0;     
    virtual void EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const = 0;        
    virtual void EntityUnsetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const = 0;      
    virtual void EntitySetInFaction(const CScriptThing&, const CCharString&) const = 0;                  
    virtual void SetFactionAsAlliedToFaction(const CCharString&, const CCharString&) const = 0;          
    virtual void SetFactionAsNeutralToFaction(const CCharString&, const CCharString&) const = 0;         
    virtual void SetFactionAsEnemyToFaction(const CCharString&, const CCharString&) const = 0;           
    virtual bool AreEntitiesEnemies(const CScriptThing&, const CScriptThing&) const = 0;                 
    virtual EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const = 0;          
    virtual void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString&) const = 0;               
    virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const = 0;        
    virtual void EntityGetOpinionAttitudeToPlayerAsString(const CScriptThing&, CCharString&) const = 0;  
    virtual float EntityGetOpinionOfPlayer(const CScriptThing&, EOpinion) const = 0;                     
    virtual void EntitySetOpinionReactionMask(const CScriptThing&, long) const = 0;                      
    virtual void EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const = 0;        
    virtual void EntitySetOpinionDeedMask(const CScriptThing&, long) const = 0;                          
    virtual void EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const = 0;            
    virtual void EntitySetOpinionDeedTypeEnabled(const CScriptThing&, EOpinionDeedType, bool) const = 0; 
    virtual void EntitySetOpinionAttitudeEnabled(const CScriptThing&, EOpinionAttitudeType, bool) const = 0; 
    virtual void EntitySetOpinionReactionEnabled(const CScriptThing&, EOpinionReactionType, bool) const = 0; 
    virtual void EntitySetPersonalityOverride(const CScriptThing&, const CCharString&) const = 0;        
    virtual void EntitySetPersonalityOverride(const CScriptThing&, long) const = 0;                      
    virtual void EntityClearPersonalityOverride(const CScriptThing&) const = 0;                          
    virtual void EntitySetAsOpinionSource(const CScriptThing&, const CCharString&) const = 0;            
    virtual void EntitySetAsOpinionSource(const CScriptThing&, long) const = 0;                          
    virtual void EntityUnsetAsOpinionSource(const CScriptThing&, bool) const = 0;                        
    virtual void OpinionSourceSetAsExclusive(const CScriptThing&, bool) const = 0;                       
    virtual void OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool) const = 0;               
    virtual void EntityPostOpinionDeedToAll(const CScriptThing&, EOpinionDeedType) const = 0;            
    virtual void EntityPostOpinionDeedToRecipient(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const = 0; 
    virtual void EntityPostOpinionDeedToRecipientVillage(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const = 0; 
    virtual long EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const = 0; 
    virtual void RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing&, long) const = 0;       
    virtual bool IsDeedWitnessed(long) const = 0;                                                        
    virtual bool CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0;       
    virtual bool CanThingBe_NearlySeen_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0; 
    virtual bool CanThingBe_Smelled_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0;    
    virtual bool CanThingBe_Heard_ByOtherThing(const CScriptThing&, const CScriptThing&) const = 0;      
    virtual bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const = 0;   
    virtual void EntitySetAsAwareOfThing(const CScriptThing&, const CScriptThing&) const = 0;            
    virtual void EntitySetSoundRadius(const CScriptThing&, float) const = 0;                             
    virtual void EntitySetSmellRadius(const CScriptThing&, float) const = 0;                             
    virtual void EntitySetSightRadius(const CScriptThing&, float) const = 0;                             
    virtual void EntitySetExtendedSightRadius(const CScriptThing&, float) const = 0;                     
    virtual void EntitySetGiveUpChaseRadius(const CScriptThing&, float) const = 0;                       
    virtual float EntityGetHearingRadius(const CScriptThing&) const = 0;                                 
    virtual bool ManuallyTriggerTrap(const CScriptThing&) const = 0;                                     
    virtual bool ManuallyResetTrap(const CScriptThing&) const = 0;                                       
    virtual void SetTimeOfDay(float) const = 0;                                                          
    virtual long GetTimeOfDay() const = 0;                                                               
    virtual void SetTimeAsStopped(bool, long&) const = 0;                                                
    virtual void FastForwardTimeTo(float, float) const = 0;                                              
    virtual bool IsTimeOfDayBetween(long, long) const = 0;                                               
    virtual long GetDayOfWeek() const = 0;                                                               
    virtual long GetDayCount() const = 0;                                                                
    virtual long GetWorldFrame() const = 0;                                                              
    virtual long GetConstantFPS() const = 0;                                                             
    virtual CCharString GetActiveQuestName() const = 0;                                                  
    virtual void TransitionToTheme(const CCharString&, float) const = 0;                                 
    virtual void ResetToDefaultTheme(float) const = 0;                                                   
    virtual void TransitionToThemeAllInternals(const CCharString&, float) const = 0;                     
    virtual void ResetToDefaultThemeAllInternals(float) const = 0;                                       
    virtual void TransitionToThemeExternals(const CCharString&, float) const = 0;                        
    virtual void ResetToDefaultThemeExternals(float) const = 0;                                          
    virtual void SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const = 0;              
    virtual void SetEnvironmentThemeWeightAllInternals(const CCharString&, float) const = 0;             
    virtual void SetEnvironmentThemeWeightExternals(const CCharString&, float) const = 0;                
    virtual void SetSoundThemesAsEnabledForRegion(const CCharString&, bool) const = 0;                   
    virtual void SetAllSoundsAsMuted(bool) const = 0;                                                    
    virtual void* RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const = 0;   
    virtual void* RadialBlurFadeTo(float, float, float, float, float, float, float) const = 0;           
    virtual void RadialBlurFadeOut(float, void*) const = 0;                                              
    virtual bool IsRadialBlurFadeActive() const = 0;                                                     
    virtual void CancelRadialBlurFade() const = 0;                                                       
    virtual void RadialBlurSetCenterWorldPos(void*, const C3DVector&) const = 0;                         
    virtual void* DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const = 0;     
    virtual void DisplacementMonochromeEffectColourFadeOut(float, void*) const = 0;                      
    virtual void* ScreenFilterFadeTo(float, float, float, float, float, const CRGBFloatColour&, const std::vector<CScreenFilterSThingByPass,std::allocator<CScreenFilterSThingByPass> >&) const = 0; 
    virtual void ScreenFilterFadeOut(float, void*) const = 0;                                            
    virtual void SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&, void*) const = 0;       
    virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const = 0;            
    virtual bool IsGiftRomantic(const CCharString&) const = 0;                                           
    virtual bool IsGiftFriendly(const CCharString&) const = 0;                                           
    virtual bool IsGiftOffensive(const CCharString&) const = 0;                                          
    virtual bool IsThingABed(const CScriptThing&) const = 0;                                             
    virtual bool IsThingAChest(const CScriptThing&) const = 0;                                           
    virtual bool IsThingADoor(const CScriptThing&) const = 0;                                            
    virtual bool IsThingSmashable(const CScriptThing&) const = 0;                                        
    virtual bool IsThingSearchable(const CScriptThing&) const = 0;                                       
    virtual void ApplyScriptBrush(const CCharString&) const = 0;                                         
    virtual void EnableDecals(bool) const = 0;                                                           
    virtual unsigned long PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) const = 0;   
    virtual unsigned long PlaySoundOnThing(const CScriptThing&, const CCharString&) const = 0;           
    virtual unsigned long PlaySoundAtPos(const C3DVector&, const CCharString&) const = 0;                
    virtual unsigned long Play2DSound(const CCharString&) const = 0;                                     
    virtual bool IsSoundPlaying(unsigned long) const = 0;                                                
    virtual void EnableSounds(bool) const = 0;                                                           
    virtual void StopSound(unsigned long) const = 0;                                                     
    virtual void OverrideMusic(EMusicSetType, bool, bool) const = 0;                                     
    virtual void StopOverrideMusic(bool) const = 0;                                                      
    virtual void CacheMusicSet(EMusicSetType) const = 0;                                                 
    virtual void EnableDangerMusic(bool) const = 0;                                                      
    virtual bool IsDangerMusicEnabled() const = 0;                                                       
    virtual void StartCountdownTimer(float) const = 0;                                                   
    virtual float GetCountdownTimer() const = 0;                                                         
    virtual void AutoSaveCheckPoint() const = 0;                                                         
    virtual void AutoSaveQuestStart() const = 0;                                                         
    virtual void AutoSave() const = 0;                                                                   
    virtual void SetSavingAsEnabled(bool) const = 0;                                                     
    virtual bool IsSavingEnabled() const = 0;                                                            
    virtual void SetSaveGameMarkerPos(const C3DVector&) const = 0;                                       
    virtual void ResetToFrontEnd() const = 0;                                                            
    virtual void SetGuildSealRecallLocation(const C3DVector&, float) const = 0;                          
    virtual C3DVector GetGuildSealRecallPos() const = 0;                                                 
    virtual float GetGuildSealRecallAngleXY() const = 0;                                                 
    virtual void SetReadableObjectText(const CScriptThing&, const CWideString&) const = 0;               
    virtual void SetReadableObjectTextTag(const CScriptThing&, const CCharString&) const = 0;            
    virtual CWideString GetFormattedString(const CCharString&, const std::vector<CWideString,std::allocator<CWideString> >&) const = 0; 
    virtual CWideString GetTextString(const CCharString&) const = 0;                                     
    virtual void AddRumourCategory(const CCharString&) const = 0;                                        
    virtual void AddNewRumourToCategory(const CCharString&, const CCharString&) const = 0;               
    virtual void RemoveRumourCategory(const CCharString&) const = 0;                                     
    virtual void SetCategoryActivity(const CCharString&, bool) const = 0;                                
    virtual void AddGossipVillage(const CCharString&, const CCharString&) const = 0;                     
    virtual void AddGossipFactionToCategory(const CCharString&, const CCharString&) const = 0;           
    virtual void SetIsGossipForPlayer(const CCharString&, bool) const = 0;                               
    virtual void SetIsGossipForPlayer(CCharString, bool) = 0;                                            
    virtual void UpdateOnlineScore_Archery(float) const = 0;                                             
    virtual void UpdateOnlineScore_ChickenKick(float) const = 0;                                         
    virtual void UpdateOnlineScore_ChapelOrTemple(float) const = 0;                                      
    virtual void UpdateOnlineScore_FishingCompo(float) const = 0;                                        
    virtual void UpdateScore_FishingCompetition(float) const = 0;                                        
    virtual float GetBestTimePairs() const = 0;                                                          
    virtual float GetBestTimeSorting() const = 0;                                                        
    virtual long GetBestScoreBlackjack() const = 0;                                                      
    virtual long GetBestScoreCoinGolfOakVale() const = 0;                                                
    virtual long GetBestScoreCoinGolfSnowSpire() const = 0;                                              
    virtual long GetBestScoreShoveHaPenny() const = 0;                                                   
    virtual float GetBestTimeGuessTheAddition() const = 0;                                               
    virtual bool IsHeroInTavernGame() const = 0;                                                         
    virtual long GetNumHousesOwned() const = 0;                                                          
    virtual void StartSneaking() const = 0;                                                              
    virtual long GetStealDuration(const CScriptThing&) const = 0;                                        
    virtual void SetUseableByHero(const CScriptThing&, bool) const = 0;                                  
    virtual void SetOwnedByHero(const CScriptThing&, bool) const = 0;                                    
    virtual void SetTavernTableAvailableForUse(const CScriptThing&, bool) const = 0;                     
    virtual void SetIsThingTurncoatable(const CScriptThing&, bool) const = 0;                            
    virtual void SetIsThingForcePushable(const CScriptThing&, bool) const = 0;                           
    virtual void SetIsThingLightningable(const CScriptThing&, bool) const = 0;                           
    virtual void SetIsThingEpicSpellable(const CScriptThing&, bool) const = 0;                           
    virtual bool IsThingTurncoated(const CScriptThing&) const = 0;                                       
    virtual void AddCreatureScriptedMode(const CScriptThing&, const CCharString&) const = 0;             
    virtual void RemoveCreatureScriptedMode(const CScriptThing&) const = 0;                              
    virtual void ForceShipsVisible() const = 0;                                                          
    virtual bool GetSleepingPositionAndOrientationFromBed(const CScriptThing&, const CScriptThing&, C3DVector&, C3DVector&) = 0; 
    virtual void SetBedAvailability(const CScriptThing&, bool) = 0;                                      
    virtual void RepopulateVillage(const CScriptThing&) const = 0;                                       
    virtual void SmashAllWindowsWithinRadiusOfPoint(const C3DVector&, float) = 0;                        
    virtual CScriptThing SetResidency(const CScriptThing&, bool) = 0;                                    
    virtual void SetThankingPhrase(CScriptThing, unsigned long) = 0;                                     
    virtual unsigned long GetThankingPhrase(CScriptThing) = 0;                                           
    virtual void ResetThankingPhrase(CScriptThing) = 0;                                                  
    virtual void SetIgnoringPhrase(CScriptThing, unsigned long) = 0;                                     
    virtual unsigned long GetIgnoringPhrase(CScriptThing) = 0;                                           
    virtual void ResetIgnoringPhrase(CScriptThing) = 0;                                                  
    virtual void SetWanderCentrePoint(CScriptThing, C3DVector) = 0;                                      
    virtual C3DVector GetWanderCentrePoint(CScriptThing) = 0;                                            
    virtual void ResetWanderCentrePoint(CScriptThing) = 0;                                               
    virtual void SetWanderMinDistance(CScriptThing, float) = 0;                                          
    virtual float GetWanderMinDistance(CScriptThing) = 0;                                                
    virtual void ResetWanderMinDistance(CScriptThing) = 0;                                               
    virtual void SetWanderMaxDistance(CScriptThing, float) = 0;                                          
    virtual float GetWanderMaxDistance(CScriptThing) = 0;                                                
    virtual void ResetWanderMaxDistance(CScriptThing) = 0;                                               
    virtual void SetGossipCounter(CScriptThing, long) = 0;                                               
    virtual long GetGossipCounter(CScriptThing) = 0;                                                     
    virtual void ResetGossipCounter(CScriptThing) = 0;                                                   
    virtual void SetMaxGossipPhrase(CScriptThing, long) = 0;                                             
    virtual long GetMaxGossipPhrase(CScriptThing) = 0;                                                   
    virtual void ResetMaxGossipPhrase(CScriptThing) = 0;                                                 
    virtual void SetWarningPhrase(CScriptThing, unsigned long) = 0;                                      
    virtual unsigned long GetWarningPhrase(CScriptThing) = 0;                                            
    virtual void ResetWarningPhrase(CScriptThing) = 0;                                                   
    virtual void SetBeerRequestPhrase(CScriptThing, unsigned long) = 0;                                  
    virtual unsigned long GetBeerRequestPhrase(CScriptThing) = 0;                                        
    virtual void ResetBeerRequestPhrase(CScriptThing) = 0;                                               
    virtual void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups) = 0;                        
    virtual EScriptingStateGroups GetScriptingStateGroup(CScriptThing) = 0;                              
    virtual void ResetScriptingStateGroup(CScriptThing) = 0;                                             
    virtual void SetMaxHeroReactionDistance(CScriptThing, float) = 0;                                    
    virtual float GetMaxHeroReactionDistance(CScriptThing) = 0;                                          
    virtual void ResetMaxHeroReactionDistance(CScriptThing) = 0;                                         
    virtual void SetActionFrequency(CScriptThing, long) = 0;                                             
    virtual long GetActionFrequency(CScriptThing) = 0;                                                   
    virtual void ResetActionFrequency(CScriptThing) = 0;                                                 
    virtual void SetActionFrequencyVariation(CScriptThing, float) = 0;                                   
    virtual float GetActionFrequencyVariation(CScriptThing) = 0;                                         
    virtual void ResetActionFrequencyVariation(CScriptThing) = 0;                                        
    virtual void SetAction(CScriptThing, CCharString) = 0;                                               
    virtual CCharString GetAction(CScriptThing) = 0;                                                     
    virtual void ResetAction(CScriptThing) = 0;                                                          
    virtual void SetFaceHeroForAction(CScriptThing, bool) = 0;                                           
    virtual bool GetFaceHeroForAction(CScriptThing) = 0;                                                 
    virtual void ResetFaceHeroForAction(CScriptThing) = 0;                                               
    virtual void SetTargetName(CScriptThing, CCharString) = 0;                                           
    virtual CCharString GetTargetName(CScriptThing) = 0;                                                 
    virtual void ResetTargetName(CScriptThing) = 0;                                                      
    virtual void SetFollowDistance(CScriptThing, float) = 0;                                             
    virtual float GetFollowDistance(CScriptThing) = 0;                                                   
    virtual void ResetFollowDistance(CScriptThing) = 0;                                                  
    virtual void SetAttackHeroOnSight(CScriptThing, bool) = 0;                                           
    virtual bool GetAttackHeroOnSight(CScriptThing) = 0;                                                 
    virtual void ResetAttackHeroOnSight(CScriptThing) = 0;                                               
    virtual void SetTimeToSpendHarassingHero(CScriptThing, long) = 0;                                    
    virtual long GetTimeToSpendHarassingHero(CScriptThing) = 0;                                          
    virtual void ResetTimeToSpendHarassingHero(CScriptThing) = 0;                                        
    virtual void SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float) = 0;                      
    virtual float GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) = 0;                            
    virtual void ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) = 0;                           
    virtual void SetCombatNearbyBreakOffRange(CScriptThing, float) = 0;                                  
    virtual float GetCombatNearbyBreakOffRange(CScriptThing) = 0;                                        
    virtual void ResetCombatNearbyBreakOffRange(CScriptThing) = 0;                                       
    virtual void SetStealStealableItems(CScriptThing, bool) = 0;                                         
    virtual bool GetStealStealableItems(CScriptThing) = 0;                                               
    virtual void ResetStealStealableItems(CScriptThing) = 0;                                             
    virtual void SetRecoverStealableItems(CScriptThing, bool) = 0;                                       
    virtual bool GetRecoverStealableItems(CScriptThing) = 0;                                             
    virtual void ResetRecoverStealableItems(CScriptThing) = 0;                                           
    virtual void SetTakeStealableItemToRandomDestination(CScriptThing, bool) = 0;                        
    virtual bool GetTakeStealableItemToRandomDestination(CScriptThing) = 0;                              
    virtual void ResetTakeStealableItemToRandomDestination(CScriptThing) = 0;                            
    virtual void SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool) = 0;            
    virtual bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) = 0;                  
    virtual void ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) = 0;                
    virtual void SetAllowedToFollow(CScriptThing, bool) = 0;                                             
    virtual bool GetAllowedToFollow(CScriptThing) = 0;                                                   
    virtual void ResetAllowedToFollow(CScriptThing) = 0;                                                 
    virtual void SetTableName(CScriptThing, CCharString) = 0;                                            
    virtual CCharString GetTableName(CScriptThing) = 0;                                                  
    virtual void ResetTableName(CScriptThing) = 0;                                                       
    virtual void SetSeatName(CScriptThing, CCharString) = 0;                                             
    virtual CCharString GetSeatName(CScriptThing) = 0;                                                   
    virtual void ResetSeatName(CScriptThing) = 0;                                                        
    virtual void SetDisableHeadLooking(CScriptThing, bool) = 0;                                          
    virtual bool GetDisableHeadLooking(CScriptThing) = 0;                                                
    virtual void ResetDisableHeadLooking(CScriptThing) = 0;                                              
    virtual void SetIsPushableByHero(CScriptThing, bool) = 0;                                            
    virtual bool GetIsPushableByHero(CScriptThing) = 0;                                                  
    virtual void ResetIsPushableByHero(CScriptThing) = 0;                                                
    virtual void SetLookForFiniteTime(CScriptThing, bool) = 0;                                           
    virtual bool GetLookForFiniteTime(CScriptThing) = 0;                                                 
    virtual void ResetLookForFiniteTime(CScriptThing) = 0;                                               
    virtual void SetAvoidRegionExits(CScriptThing, bool) = 0;                                            
    virtual bool GetAvoidRegionExits(CScriptThing) = 0;                                                  
    virtual void ResetAvoidRegionExits(CScriptThing) = 0;                                                
    virtual void SetTargetingDistanceOffset(CScriptThing, float) = 0;                                    
    virtual float GetTargetingDistanceOffset(CScriptThing) = 0;                                          
    virtual void ResetTargetingDistanceOffset(CScriptThing) = 0;                                         
    virtual void SetPlayerUsingMeleeDummies(bool) = 0;                                                   
    virtual bool GetPlayerUsingMeleeDummies() = 0;                                                       
    virtual void SetPlayerUsingRangedDummies(bool) = 0;                                                  
    virtual bool GetPlayerUsingRangedDummies() = 0;                                                      
    virtual void SetPlayerUsingWillDummies(bool) = 0;                                                    
    virtual bool GetPlayerUsingWillDummies() = 0;                                                        
    virtual void SetCheapHeadLooking(bool) = 0;                                                          
    virtual bool GetCheapHeadLooking() = 0;                                                              
    virtual void SetQuitTavernGame(bool) = 0;                                                            
    virtual bool GetQuitTavernGame() = 0;                                                                
    virtual void SetPrizeTavernTable(bool) = 0;                                                          
    virtual bool GetPrizeTavernTable() = 0;                                                              
    virtual void SetBettingActive(bool) = 0;                                                             
    virtual bool GetBettingActive() = 0;                                                                 
    virtual void SetBettingAccept(bool) = 0;                                                             
    virtual bool GetBettingAccept() = 0;                                                                 
    virtual void SetBettingAmount(long) = 0;                                                             
    virtual long GetBettingAmount() = 0;                                                                 
    virtual void SetCountBetMoneyDown(bool) = 0;                                                         
    virtual bool GetCountBetMoneyDown() = 0;                                                             
    virtual void SetSpotTheAdditionBeaten(bool) = 0;                                                     
    virtual bool GetSpotTheAdditionBeaten() = 0;                                                         
    virtual void SetGlobalTargetingDistanceOffset(float) = 0;                                            
    virtual float GetGlobalTargetingDistanceOffset() = 0;                                                
    virtual void SetTradingPriceMult(float) = 0;                                                         
    virtual float GetTradingPriceMult() = 0;                                                             
    virtual void SetBoastingEnabled(bool) = 0;                                                           
    virtual bool GetBoastingEnabled() = 0;                                                               
    virtual void SetActiveGossipCategories(CCharString, bool) = 0;                                       
    virtual bool* GetActiveGossipCategories(CCharString) = 0;                                            
    virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetActiveGossipCategories() = 0; 
    virtual int GetActiveGossipCategoriesSize() = 0;                                                     
    virtual void ClearActiveGossipCategories() = 0;                                                      
    virtual bool* GetIsGossipForPlayer(CCharString) = 0;                                                 
    virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetIsGossipForPlayer() = 0; 
    virtual int GetIsGossipForPlayerSize() = 0;                                                          
    virtual void ClearIsGossipForPlayer() = 0;                                                           
    virtual void SetGossip(CCharString, CCharString, long) = 0;                                          
    virtual CCharString GetGossip(CCharString, long) = 0;                                                
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossip(CCharString) = 0;     
    virtual int GetGossipSize(CCharString) = 0;                                                          
    virtual void ClearGossip(CCharString) = 0;                                                           
    virtual void RemoveGossip(CCharString) = 0;                                                          
    virtual void AddGossip(CCharString, CCharString) = 0;                                                
    virtual void AddGossip(CCharString) = 0;                                                             
    virtual void SetGossipVillages(CCharString, CCharString, long) = 0;                                  
    virtual CCharString GetGossipVillages(CCharString, long) = 0;                                        
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipVillages(CCharString) = 0; 
    virtual int GetGossipVillagesSize(CCharString) = 0;                                                  
    virtual void ClearGossipVillages(CCharString) = 0;                                                   
    virtual void RemoveGossipVillages(CCharString) = 0;                                                  
    virtual void AddGossipVillages(CCharString, CCharString) = 0;                                        
    virtual void AddGossipVillages(CCharString) = 0;                                                     
    virtual void SetGossipFactions(CCharString, CCharString, long) = 0;                                  
    virtual CCharString GetGossipFactions(CCharString, long) = 0;                                        
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipFactions(CCharString) = 0; 
    virtual int GetGossipFactionsSize(CCharString) = 0;                                                  
    virtual void ClearGossipFactions(CCharString) = 0;                                                   
    virtual void RemoveGossipFactions(CCharString) = 0;                                                  
    virtual void AddGossipFactions(CCharString, CCharString) = 0;                                        
    virtual void AddGossipFactions(CCharString) = 0;                                                     
};






































class CScriptBase : public CBaseClassNonCopyable {
public:
    CScriptBase();                                                       
    virtual ~CScriptBase();                                              
protected:
    virtual void RegisterMain();                                         
    virtual void Main();                                                 
    virtual void Init();                                                 
public:
    virtual void OnPersist(CPersistContext&);                            
    void Activate();
    void SetPredicate(const NScript::CPredicateBase&);                   
    void RemovePredicate();
    bool IsActiveThreadTerminating();                                    
    CSpawnedFuncBase* GetPActiveThread();
    unsigned long GetThreadID();
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);     
    void KillSpawnedFunction(const CCharString&, unsigned long);
    void KillAllThreads();
    void RunSubThreads();
    void AddEntityScriptBinding(CEntityScriptBindingBase*);              
    bool IsEntityScriptBound(const CCharString&) const;
    void PostAddScriptedEntities();                                      
    void OnRegionLoaded(const CCharString&);
    void OnRegionUnloaded(const CCharString&);
    void OnScriptedEntityActivated(FableNScriptUInt64, CScriptThing&);
    void OnScriptedEntityDeactivated(FableNScriptUInt64, CScriptThing&, bool);
    void OnScriptedEntityInterrupted(FableNScriptUInt64);
    void LoadGameState(const CCharString&);
    void SaveGameState(CCharString&);
protected:
    bool UpdateSpawnedFunction(CSpawnedFuncBase*);
    void RunSpawnedFunctions();
    void RunEntityThreads();
    void DoScriptDataPersist(CCharString&, bool);
    void DoEntityScriptPersist(CActiveEntityScriptBase&, CCharString&, bool);
    void LoadEntityScript(FableNScriptUInt64, CActiveEntityScriptBase&);
    void SaveEntityScript(FableNScriptUInt64, CActiveEntityScriptBase&);
public:
private:
    unsigned char     Threads[0x4];                                      
    unsigned char     EntityScripts[0x10];                               
    unsigned char     EntityScriptBindings[0x10];                        
    unsigned long     LastWorldUpdateFrame;                              
    CSpawnedFuncBase* PActiveThread;                                     
    unsigned char     RuntimeScriptedEntityPersist[0x10];                
};

class CGameScriptInterfaceBase;

template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data,CGameScriptInterfaceBase& game):Game(game),MasterScriptData(*(T*)data) {}
    virtual ~CLevelScriptBase();
    T* GetMasterData();
    CGameScriptInterfaceBase& Game;
    T& MasterScriptData;
};








class CPersistContext;
class CScriptDataBase : public CBaseClassNonCopyable {
public:
    virtual void OnPersist(CPersistContext&)=0;
    virtual void Init()=0;
};





#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type {};
struct __false_type {};
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<class T> inline const T& max(const T& a,const T& b) { return a < b ? b : a; }
template<class A,class B> struct pair {
    typedef A first_type; typedef B second_type;
    A first; B second;
    pair():first(A()),second(B()){}
    pair(const A& a,const B& b):first(a),second(b){}
};
template<class A,class B> inline pair<A,B> make_pair(const A& a,const B& b) {return pair<A,B>(a,b);}
template<class T> class allocator {
public:
    allocator() {}
    T* allocate(size_t n,const void* = 0) { return n ? (T*)malloc(n*sizeof(T)) : 0; }
    void deallocate(T* p,size_t) { if(p) free(p); }
};
template<class V,class T,class A> class _STLP_alloc_proxy : public A {
public: V _M_data;
    _STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class T,class U> inline void _Construct(T* p,const U& x) { new(p) T(x); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class T> inline void __destroy_aux(T* first,T* last,const __false_type&) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline void __destroy_aux(T*,T*,const __true_type&) {}
template<class T> inline void _Destroy(T* first,T* last) { __destroy_aux(first,last,typename __type_traits<T>::has_trivial_destructor()); }
inline void* __copy_trivial(const void* first,const void* last,void* out) {
 return last==first ? out : (char*)memmove(out,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O out,const __false_type&) {
 O cur=out; for(;first!=last;++first,++cur) _Construct(&*cur,*first); return cur;
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O out,const __true_type&) {
 return (O)__copy_trivial(first,last,out);
}
template<class T> inline T* __uninitialized_fill_n(T* first,size_t n,const T& value,const __false_type&) { T* cur=first;for(;n>0;--n,++cur)_Construct(&*cur,value);return cur;}
template<class T> inline T* fill_n(T* first,size_t n,const T& value) { for(;n>0;--n,++first)*first=value;return first; }
template<class T,class A> class _Vector_base {
public: typedef A allocator_type;
 _Vector_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0) {
  _M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;
 }
 ~_Vector_base(){if(_M_start!=0)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:
 T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {
public:
 typedef T value_type;typedef T* pointer;typedef const T* const_pointer;typedef T* iterator;typedef const T* const_iterator;typedef size_t size_type;typedef A allocator_type;
 typedef typename __type_traits<T>::has_trivial_assignment_operator _IsPODType;
 explicit vector(const A& a=A()):_Vector_base<T,A>(a){}
 vector(const vector& x):_Vector_base<T,A>(x.size(),x.get_allocator()){
 this->_M_finish=__uninitialized_copy((const_pointer)x._M_start,(const_pointer)x._M_finish,this->_M_start,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 A get_allocator()const{return (const A&)this->_M_end_of_storage;}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 void push_back(const T& x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
 vector& operator=(const vector&);
 iterator insert(iterator,const T&);
 T& operator[](size_type n){return this->_M_start[n];}
 const T& operator[](size_type n)const{return this->_M_start[n];}
 void clear(){erase(begin(),end());}
 iterator erase(iterator pos){if(pos+1!=end()) __copy_trivial(pos+1,this->_M_finish,pos);--this->_M_finish;_Destroy(this->_M_finish);return pos;}
 iterator erase(iterator first,iterator last){pointer i=first;for(;last!=this->_M_finish;++last,++i)*i=*last;_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
protected:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(pointer s,pointer f,pointer e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 void _M_insert_overflow(pointer pos,const T& x,const __false_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=start;
 finish=__uninitialized_copy(this->_M_start,pos,start,__false_type());
 if(fill_len==1){_Construct(finish,x);++finish;}else finish=__uninitialized_fill_n(finish,fill_len,x,__false_type());
 if(!atend)finish=__uninitialized_copy(pos,this->_M_finish,finish,__false_type());
 _M_clear();_M_set(start,finish,start+len);
 }
 void _M_insert_overflow(pointer pos,const T& x,const __true_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=(pointer)__copy_trivial(this->_M_start,pos,start);
 finish=fill_n(finish,fill_len,x);if(!atend)finish=(pointer)__copy_trivial(pos,this->_M_finish,finish);
 _M_clear();_M_set(start,finish,start+len);
 }
};
}
namespace NScript {
class CQ_SunnyvaleMasterData : public CScriptDataBase {
public:
    unsigned char PostSavePosition[4]; 
    long HenchmanCommentaryState; 
    long HenchmanOverrideState; 
    bool HenchmanSacrificedBodge; 
    bool HauntedBarrowFieldsCompleted; 
    long RandomEntityVillagerMax; 
    long RandomEntityGuardMax; 
    long RandomEntityBanditMax; 
    long RandomEntityVillagerCount; 
    long RandomEntityGuardCount; 
    long RandomEntityBanditCount; 
    float RandomEntityVillagerPercentage; 
    float RandomEntityGuardPercentage; 
    float RandomEntityBanditPercentage; 
    bool RandomEntitySheriffAlive; 
    bool RandomEntityLieutenantAlive; 
    bool BanditCampTwinbladeKilled; 
    unsigned char ArcheryStateCurrent[4]; 
    unsigned char ArcheryStateRequired[4]; 
    long ArcheryHighScore; 
    bool FriendOfForeman; 
    bool BridgeOpened; 
    bool GrannyMemoryReturned; 
    bool IsLunaHuman; 
    bool CondemnedManDead; 
    bool CondemnedManForgiven; 
    bool CondemnedManMeetsBodyGuard; 
    bool CondemnedManMeetsBodyGuardCutSceneStart; 
    bool CondemnedManMeetsBodyGuardCutSceneFinished; 
    CCharString TeddySolution; 
    long OrchardFarmRaidLastCompleted; 
    long OrchardFarmTraderEscortCounter; 
    bool SeenAbbeyMotherAtGuild; 
    bool DefeatedThunder; 
    bool LostToThunder; 
    bool KilledThunder; 
    bool CollectedSoulFromArena; 
    bool KilledBriar; 
    bool CollectedSoulFromMother; 
    bool KilledGM; 
    bool CollectedSoulFromNostro; 
    long DeliveredSoul; 
    unsigned char CurrentHeroSoulsPosition[4]; 
    bool WhisperKilledByHero; 
    bool ArenaFinished; 
    bool GatesRequireClosing; 
    bool GatesRequireOpening; 
    long HeroDrunkness; 
    bool TrophyDealerHeroSpokenToDemonDoors; 
    float VillagerAngryRating; 
    bool MotherDodgingKraken; 
    long JackBossBattleResult; 
    bool JackBossBattleHeroGoodAtEnd; 
    bool WifeLeavingYou; 
    bool WifeLeftYou; 
    long CurrentScene; 
    bool StartNextScene; 
    long CurrentChapter; 
    bool ScorpionsDestroyed; 
    bool ScorpionsDestroyedCutscenePlayed; 
    bool SkillTrainingStarted; 
    bool WillTrainingStarted; 
    bool MovingDummiesNeeded; 
    long SkillScore; 
    long HighestSkillScore; 
    long WillScore; 
    bool MeleeApprenticeNeededForCutscene; 
    long GlobalMeleeGrade; 
    long GlobalSkillGrade; 
    long GlobalWillGrade; 
    bool GuildWarningOccuring; 
    bool SkillTestOccuring; 
    bool WillTestOccuring; 
    bool SkillRepeating; 
    bool SkillRepeatKnown; 
    bool SkillDummyReset; 
    bool HeroTakingGuildTest; 
    bool SwordInTheStoneComplete; 
    bool AmbushTradersAllGuardsDead; 
    bool AmbushTradersAllTradersDead; 
    long AmbushTradersKillCount; 
    long AmbushTradersBanditHireCount; 
    bool AmbushTradersSpyDead; 
    bool StruckDealWithLadyGrey; 
    bool HeroExposedLadyGrey; 
    bool HeroMarriedLadyGrey; 
    bool BountyHuntWithinTimeLimit; 
    bool BountyHuntDecapitation; 
    bool BountyHuntTimeLimitExceeded; 
    unsigned long AchievementsWorthyOfSong; 
    bool BreakSiegeFinished; 
    bool WhiteBalverineFinished; 
    bool MadBomberFinished; 
    bool BodyGuardsMustStandAndWait; 
    bool BodyGuardsInLimbo; 
    bool WifeHeroFlirting; 
    long PrisonRaceNumber; 
    bool PrisonRaceWonByHero; 
    bool WasPrisonRaceEverWonByHero; 
    bool PrisonKeyStolenByHero; 
    bool BooksPreviouslyOpened[3]; 
    bool PrisonTorturedBefore; 
    bool PrisonGameDoneBefore; 
    bool HeroDollsScriptUsingTeacher; 
    long MaxChickenKickingScore; 
    unsigned long StoryTellerSpecialStories; 
    unsigned long StoryTellerToldSpecialStories; 
    unsigned char GHeroName[4]; 
    bool TimeAdvancePointTriggered; 
    bool OFBRCratesStolen; 
    bool OFBR_NoCratesWereStolen; 
    bool DarkwoodAllTradersAlive; 
    bool MadBomberNoBombsExplode; 
    bool HobbeCavePerpertratorKilled; 
    long HobbeCaveInnocentSacrificed; 
    bool HobbeCaveBoyUnharmed; 
    bool BanditCampKillNoBandits; 
    bool BreakSiegeNoAssistance; 
    bool BreakSiegeKilledLeader; 
    bool OrchardFarmBanditKilled; 
    bool OrchardFarmGuardKilled; 
    bool BanditCampKillManyBandits; 
    bool HobbeContestKillMoreHobbes; 
    bool GraveyardKillAllInnocents; 
    long DarkwoodPickpocketedAllTraders; 
    bool HangingTreeBanditKilled; 
    bool HangingTreeGuardKilled; 
    bool TCGKillNoBandits; 
    bool TCGMadeTimeLimit; 
    bool TCEKeepBanditFollowerAlive; 
    bool TCEMadeTimeLimit; 
    bool RansomVictimVictimNoDamage; 
    bool RansomVictimKidnappersKilled; 
    bool RansomVictimSaveVictim; 
    bool RansomVictimHaveVictimKilled; 
    bool MinionCampBriarNoDamage; 
    bool FireHeartFreeAllPrisoners; 
    bool FireHeartKillAllPrisoners; 
    bool TCETimeLimitBoastTaken; 
    bool TCGTimeLimitBoastTaken; 
    bool HeroWoreMask; 
    std::vector<CCharString> BoastRegions; 
    bool SingingStonesInSync; 
    virtual void OnPersist(CPersistContext&);
    virtual void Init();
};
}


class CCounter {
public:
    CCounter();
    bool operator<=(float) const;
    bool operator>=(float) const;
    bool operator<(float) const;
    bool operator>(float) const;
    float GetCounter() const;
private:
    long GetFrameDelta() const;
    long RelevantWorldFrame;
};









class CTimer {
public:
    CTimer();                                                            
    ~CTimer();                                                           
    CTimer& operator=(long);
    bool operator==(long) const;
    bool operator!=(long) const;
    bool operator<(long) const;
    bool operator>(long) const;
    bool operator<=(long) const;
    bool operator>=(long) const;
    long GetTime() const;
    long GetIndex() const;
    void Pause();
    void Unpause();
private:
    long TimerIndex;                                                     
};

namespace NScript {
class CV_BodyGuardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_BodyGuardScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    long TotalWages;
    bool HeroCannotAffordHelp;
    bool HeroWieldingWeapon;
    CCounter HeroStoodStillCounter;
    CTimer HenchmanLastIdleChatTimer;
    CTimer HenchmanLastBoredChatTimer;
    CTimer CommentaryTimer;
    CTimer HenchmanFreakedOutThePlayerTimer;
    std::vector<CScriptThing> HenchmanList;
    long ConversationIndex;
    enum CONVERSATION_STATES { CS_IDLE=1, CS_COMMENT_SCRIPTED=2 };
    enum EHENCHMAN_COMMENT { HM_COMMENT_NONE=0, HM_COMMENT_MULTIPLE=1, HM_COMMENT_SINGLE=2, HM_COMMENT_MULTIPLE_RANDOM=3 };
    CTimer HenchmanDeadRespawnTimer[4];
    long HenchmanGlobalStatus[4];
    long GetHenchmanTypeFromString(const CCharString&);
    bool MakeHenchmanPerformIdleComment();
    bool MakeHenchmanComment(const CCharString&,long,EHENCHMAN_COMMENT,const CScriptThing& = CScriptThing());
};
}
extern CGameScriptInterfaceBase* pDSTGame;
CCharString __fastcall operator+(const char*,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
CCharString __fastcall operator+(const CCharString&,const CCharString&);
inline CTimer& CTimer::operator=(long seconds) { pDSTGame->SetTimer(TimerIndex,seconds); return *this; }
inline CScriptThing::CScriptThing(const CScriptThing& __that):CBaseClass(__that),PImp(__that.PImp) {}
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
inline bool CScriptThing::operator==(const CScriptThing& rhs) const { return PImp->IsEqualTo(*rhs.PImp); }
inline CCharString CScriptThing::GetDataString() const { if(PImp.IsNull())return NullString;return PImp->GetDataString(); }

bool NScript::CV_BodyGuardScript::MakeHenchmanComment(const CCharString& comment_to_make,long comment_state,EHENCHMAN_COMMENT comment_type,const CScriptThing& specific_bodyguard)
{
    if(HenchmanList.size()==0) return false;
    if(Game.IsConversationActive(ConversationIndex)) return false;
    if(MasterScriptData.HenchmanCommentaryState!=0) return false;
    CommentaryTimer=5;
    MasterScriptData.HenchmanCommentaryState=comment_state;
    long HenchmanID=rand()%HenchmanList.size();
    CScriptThing Henchman=HenchmanList[HenchmanID];
    CScriptThing HenchmanBuddy=HenchmanList[(HenchmanID+1)%HenchmanList.size()];
    bool ResponseAllowed;
    if(comment_type==HM_COMMENT_MULTIPLE_RANDOM) ResponseAllowed=(rand()%3)==0;
    else { ResponseAllowed=true; if(comment_type==HM_COMMENT_SINGLE) ResponseAllowed=false; }
    
    if(specific_bodyguard.IsAlive() && !(specific_bodyguard==Henchman)) {
        HenchmanBuddy=Henchman;
        Henchman=specific_bodyguard;
    }
    CCharString HenchmanMorality;
    if(Henchman.IsAlive()) {
        HenchmanMorality=Henchman.GetDataString();
        HenchmanMorality.Truncate(HenchmanMorality.GetLength()-1);
    }
    if(HenchmanList.size()>1 && ResponseAllowed) {
        long HLStyleConversationIndex=Game.AddNewConversation(Henchman,false,false);
        ConversationIndex=HLStyleConversationIndex;
        Game.AddPersonToConversation(HLStyleConversationIndex,HenchmanBuddy);
        Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
        Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_016_BODYGUARD"+Henchman.GetDataString()+"_"+comment_to_make,false,Henchman,HenchmanBuddy);
        Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_016_BODYGUARD"+HenchmanBuddy.GetDataString()+"_"+comment_to_make+"_RESPONSE"+HenchmanMorality,false,HenchmanBuddy,Henchman);
    } else {
        long HLStyleConversationIndex=Game.AddNewConversation(Henchman,false,false);
        ConversationIndex=HLStyleConversationIndex;
        Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
        Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_016_BODYGUARD"+Henchman.GetDataString()+"_"+comment_to_make,false,Henchman,Game.GetHero());
    }
    return true;
}

static int strings=0;
static const char* text(const CCharString& s){return s.PStringData?s.PStringData->Data.PData:"";}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char* p,long count):PStringData(0){if(count<0)count=(long)strlen(p);PStringData=new CCharStringData;PStringData->Data.PData=(char*)malloc(count+1);memcpy(PStringData->Data.PData,p,count);PStringData->Data.PData[count]=0;PStringData->Data.StringLength=count;PStringData->Data.DataLength=count;PStringData->Data.UseFastExtend=false;PStringData->NoRefs=1;++strings;}
CCharString::CCharString(const CCharString& s):PStringData(s.PStringData){if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){if(PStringData&&!--PStringData->NoRefs){free(PStringData->Data.PData);delete PStringData;--strings;}}
const CCharString& CCharString::operator=(const CCharString& s){if(s.PStringData)++s.PStringData->NoRefs;if(PStringData&&!--PStringData->NoRefs){free(PStringData->Data.PData);delete PStringData;--strings;}PStringData=s.PStringData;return *this;}
void CCharString::Truncate(unsigned long n){assert(n<=strlen(text(*this)));CCharString shortened(text(*this),(long)n);*this=shortened;}
CCharString __fastcall operator+(const CCharString& a,const CCharString& b){char tmp[256];assert(strlen(text(a))+strlen(text(b))<sizeof(tmp));strcpy(tmp,text(a));strcat(tmp,text(b));CCharString result(tmp);return result;}
CCharString __fastcall operator+(const char* a,const CCharString& b){CCharString first(a);return first+b;}
CCharString __fastcall operator+(const CCharString& a,const char* b){CCharString last(b);return a+last;}
CWideString::~CWideString(){}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CScriptBase::CScriptBase(){} CScriptBase::~CScriptBase(){}
void CScriptBase::RegisterMain(){} void CScriptBase::Main(){} void CScriptBase::Init(){} void CScriptBase::OnPersist(CPersistContext&){}
template<class T>CLevelScriptBase<T>::~CLevelScriptBase(){}
CScriptThing::CScriptThing(){} CScriptThing::~CScriptThing(){}
CScriptThing& CScriptThing::operator=(const CScriptThing& s){PImp=s.PImp;return *this;}
CCharString CScriptThing::NullString;
CTimer::CTimer():TimerIndex(0){} CTimer::~CTimer(){}
CCounter::CCounter():RelevantWorldFrame(0){}
CGameScriptInterfaceBase::~CGameScriptInterfaceBase(){}
CGameScriptInterfaceBase* pDSTGame=0;
namespace NScript {
void CQ_SunnyvaleMasterData::OnPersist(CPersistContext&){} void CQ_SunnyvaleMasterData::Init(){}
CV_BodyGuardScript::CV_BodyGuardScript(CScriptDataBase* data,CGameScriptInterfaceBase& game):CLevelScriptBase<CQ_SunnyvaleMasterData>(data,game),ConversationIndex(17){}
void CV_BodyGuardScript::RegisterMain(){} void CV_BodyGuardScript::Main(){} void CV_BodyGuardScript::Init(){} void CV_BodyGuardScript::OnPersist(CPersistContext&){}
}
const CCharString& CScriptThing::GetName() const { assert(!"unexpected CScriptThing call: GetName"); throw 0; }
CCharString CScriptThing::GetDefName() const { assert(!"unexpected CScriptThing call: GetDefName"); throw 0; }
void CScriptThing::SetDataString(const CCharString&) { assert(!"unexpected CScriptThing call: SetDataString"); throw 0; }
C3DVector CScriptThing::GetFocalPos() const { assert(!"unexpected CScriptThing call: GetFocalPos"); throw 0; }
const C3DVector& CScriptThing::GetPos() const { assert(!"unexpected CScriptThing call: GetPos"); throw 0; }
C3DVector CScriptThing::GetHomePos() const { assert(!"unexpected CScriptThing call: GetHomePos"); throw 0; }
CCharString CScriptThing::GetCurrentMapName() const { assert(!"unexpected CScriptThing call: GetCurrentMapName"); throw 0; }
CCharString CScriptThing::GetHomeMapName() const { assert(!"unexpected CScriptThing call: GetHomeMapName"); throw 0; }
float CScriptThing::GetAngleXY() const { assert(!"unexpected CScriptThing call: GetAngleXY"); throw 0; }
CThing* CScriptThing::GetPThing() const { assert(!"unexpected CScriptThing call: GetPThing"); throw 0; }
FableNScriptUInt64 CScriptThing::GetPThingUniqueID() const { assert(!"unexpected CScriptThing call: GetPThingUniqueID"); throw 0; }
EAIStateGroupType CScriptThing::GetCurrentStateGroupType() const { assert(!"unexpected CScriptThing call: GetCurrentStateGroupType"); throw 0; }
EScriptAIPriority CScriptThing::GetCurrentScriptPriority() const { assert(!"unexpected CScriptThing call: GetCurrentScriptPriority"); throw 0; }
bool CScriptThing::IsSneaking() const { assert(!"unexpected CScriptThing call: IsSneaking"); throw 0; }
bool CScriptThing::IsAwareOfHero() const { assert(!"unexpected CScriptThing call: IsAwareOfHero"); throw 0; }
bool CScriptThing::MsgIsTriggeredBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsTriggeredBy"); throw 0; }
bool CScriptThing::MsgIsKilledBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsKilledBy"); throw 0; }
CScriptThing CScriptThing::MsgWhoKilledMe() const { assert(!"unexpected CScriptThing call: MsgWhoKilledMe"); throw 0; }
bool CScriptThing::MsgIsUsedBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsUsedBy"); throw 0; }
bool CScriptThing::MsgIsHitBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitBy"); throw 0; }
bool CScriptThing::MsgIsHitByWithFlourish(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByWithFlourish"); throw 0; }
bool CScriptThing::MsgIsHitByWithDecapitate(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByWithDecapitate"); throw 0; }
bool CScriptThing::MsgIsHitByWithWeapon(const CCharString&, const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByWithWeapon"); throw 0; }
bool CScriptThing::MsgIsHitByWithProjectileWeapon(const CCharString&, float&) const { assert(!"unexpected CScriptThing call: MsgIsHitByWithProjectileWeapon"); throw 0; }
CScriptThing CScriptThing::MsgWhoHitMe() const { assert(!"unexpected CScriptThing call: MsgWhoHitMe"); throw 0; }
bool CScriptThing::MsgIsTalkedToBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsTalkedToBy"); throw 0; }
CScriptThing CScriptThing::MsgWhoTalkedToMe() const { assert(!"unexpected CScriptThing call: MsgWhoTalkedToMe"); throw 0; }
bool CScriptThing::MsgExpressionPerformedTo(CCharString&) const { assert(!"unexpected CScriptThing call: MsgExpressionPerformedTo"); throw 0; }
CScriptThing CScriptThing::MsgWhoExpressedToMe() const { assert(!"unexpected CScriptThing call: MsgWhoExpressedToMe"); throw 0; }
long CScriptThing::MsgHowLongWasExpressionPerformed() const { assert(!"unexpected CScriptThing call: MsgHowLongWasExpressionPerformed"); throw 0; }
bool CScriptThing::MsgIsKnockedOutBy(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsKnockedOutBy"); throw 0; }
CScriptThing CScriptThing::MsgWhoKnockedMeOut() const { assert(!"unexpected CScriptThing call: MsgWhoKnockedMeOut"); throw 0; }
bool CScriptThing::MsgReceivedMoney(long&) const { assert(!"unexpected CScriptThing call: MsgReceivedMoney"); throw 0; }
bool CScriptThing::MsgIsPresentedWithItem(CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsPresentedWithItem"); throw 0; }
bool CScriptThing::MsgReceivedInventoryItem() const { assert(!"unexpected CScriptThing call: MsgReceivedInventoryItem"); throw 0; }
bool CScriptThing::MsgPerformedSpecialAbility(EHeroAbility) const { assert(!"unexpected CScriptThing call: MsgPerformedSpecialAbility"); throw 0; }
bool CScriptThing::MsgPerformedAnySpecialAbility() const { assert(!"unexpected CScriptThing call: MsgPerformedAnySpecialAbility"); throw 0; }
bool CScriptThing::MsgPerformedAnyAggressiveSpecialAbility() const { assert(!"unexpected CScriptThing call: MsgPerformedAnyAggressiveSpecialAbility"); throw 0; }
bool CScriptThing::MsgPerformedAnyNonAggressiveSpecialAbility() const { assert(!"unexpected CScriptThing call: MsgPerformedAnyNonAggressiveSpecialAbility"); throw 0; }
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility, const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitBySpecialAbilityFrom"); throw 0; }
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByAnySpecialAbilityFrom"); throw 0; }
bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByAnyAggressiveSpecialAbilityFrom"); throw 0; }
bool CScriptThing::MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const { assert(!"unexpected CScriptThing call: MsgIsHitByAnyNonAggressiveSpecialAbilityFrom"); throw 0; }
bool CScriptThing::MsgOpenedChest() const { assert(!"unexpected CScriptThing call: MsgOpenedChest"); throw 0; }
bool CScriptThing::MsgHitEnemyWithMeleeWeapon() const { assert(!"unexpected CScriptThing call: MsgHitEnemyWithMeleeWeapon"); throw 0; }
bool CScriptThing::MsgHitFriendWithMeleeWeapon() const { assert(!"unexpected CScriptThing call: MsgHitFriendWithMeleeWeapon"); throw 0; }
bool CScriptThing::MsgHitEnemyWithRangedWeapon() const { assert(!"unexpected CScriptThing call: MsgHitEnemyWithRangedWeapon"); throw 0; }
bool CScriptThing::MsgHitFriendWithRangedWeapon() const { assert(!"unexpected CScriptThing call: MsgHitFriendWithRangedWeapon"); throw 0; }
bool CScriptThing::MsgHitEnemyWithBareHands() const { assert(!"unexpected CScriptThing call: MsgHitEnemyWithBareHands"); throw 0; }
bool CScriptThing::MsgHitFriendWithBareHands() const { assert(!"unexpected CScriptThing call: MsgHitFriendWithBareHands"); throw 0; }
bool CScriptThing::MsgAttackedWithMeleeWeaponWithoutHittingAnything() const { assert(!"unexpected CScriptThing call: MsgAttackedWithMeleeWeaponWithoutHittingAnything"); throw 0; }
bool CScriptThing::MsgAttackedWithBareHandsWithoutHittingAnything() const { assert(!"unexpected CScriptThing call: MsgAttackedWithBareHandsWithoutHittingAnything"); throw 0; }
bool CScriptThing::MsgTalkedToAnyone() const { assert(!"unexpected CScriptThing call: MsgTalkedToAnyone"); throw 0; }
bool CScriptThing::MsgGetThingsKilled(std::vector<unsigned long,std::allocator<unsigned long> >&) const { assert(!"unexpected CScriptThing call: MsgGetThingsKilled"); throw 0; }
bool CScriptThing::MsgPerformedFlourish() const { assert(!"unexpected CScriptThing call: MsgPerformedFlourish"); throw 0; }
bool CScriptThing::MsgPerformedSuccessfulFlourish() const { assert(!"unexpected CScriptThing call: MsgPerformedSuccessfulFlourish"); throw 0; }
bool CScriptThing::MsgOnMoralityChanged(long&) const { assert(!"unexpected CScriptThing call: MsgOnMoralityChanged"); throw 0; }
bool CScriptThing::MsgOnCutSceneAnimEvent(CCharString&) const { assert(!"unexpected CScriptThing call: MsgOnCutSceneAnimEvent"); throw 0; }
bool CScriptThing::MsgIsKicked() const { assert(!"unexpected CScriptThing call: MsgIsKicked"); throw 0; }
bool CScriptThing::IsUnconscious() const { assert(!"unexpected CScriptThing call: IsUnconscious"); throw 0; }
bool CScriptThing::IsUsable() const { assert(!"unexpected CScriptThing call: IsUsable"); throw 0; }
bool CScriptThing::IsBeingCarriedBy(const CCharString&) const { assert(!"unexpected CScriptThing call: IsBeingCarriedBy"); throw 0; }
bool CScriptThing::IsOpenDoor() const { assert(!"unexpected CScriptThing call: IsOpenDoor"); throw 0; }
bool CScriptThing::IsSummonedCreature() const { assert(!"unexpected CScriptThing call: IsSummonedCreature"); throw 0; }
void CScriptThing::SetAsUsable(bool) { assert(!"unexpected CScriptThing call: SetAsUsable"); throw 0; }
void CScriptThing::SetFriendsWithEverythingFlag(bool) { assert(!"unexpected CScriptThing call: SetFriendsWithEverythingFlag"); throw 0; }
bool CScriptThing::GetActivationTriggerStatus() const { assert(!"unexpected CScriptThing call: GetActivationTriggerStatus"); throw 0; }
void CScriptThing::SetActivationTriggerStatus(bool) { assert(!"unexpected CScriptThing call: SetActivationTriggerStatus"); throw 0; }
void CScriptThing::SetToKillOnLevelUnload(bool) { assert(!"unexpected CScriptThing call: SetToKillOnLevelUnload"); throw 0; }
void CScriptThing::UpdateThingAttachment() { assert(!"unexpected CScriptThing call: UpdateThingAttachment"); throw 0; }
void CScriptThing::IncrementScriptCounter() { assert(!"unexpected CScriptThing call: IncrementScriptCounter"); throw 0; }
void CScriptThing::DecrementScriptCounter() { assert(!"unexpected CScriptThing call: DecrementScriptCounter"); throw 0; }
long CScriptThing::GetScriptCounter() { assert(!"unexpected CScriptThing call: GetScriptCounter"); throw 0; }
bool CScriptThing::IsDead() const { assert(!"unexpected CScriptThing call: IsDead"); throw 0; }
bool CScriptThing::IsNull() const { assert(!"unexpected CScriptThing call: IsNull"); throw 0; }
bool CScriptThing::IsEqualTo(const CScriptThing&) const { assert(!"unexpected CScriptThing call: IsEqualTo"); throw 0; }
class FixtureScriptInterface:public CGameScriptInterfaceBase { public:
 virtual void Error(const CCharString&, const CCharString&, const CCharString&, unsigned long) const { assert(!"unexpected interface call: Error"); throw 0; }
 virtual void TraceMessage(const CCharString&) const { assert(!"unexpected interface call: TraceMessage"); throw 0; }
 virtual void Validate() const { assert(!"unexpected interface call: Validate"); throw 0; }
 virtual void SetDebugCameraType(long) const { assert(!"unexpected interface call: SetDebugCameraType"); throw 0; }
 virtual void DeactivateBoastUi() const { assert(!"unexpected interface call: DeactivateBoastUi"); throw 0; }
 virtual bool IsXbox() const { assert(!"unexpected interface call: IsXbox"); throw 0; }
 virtual void NewScriptFrame() const { assert(!"unexpected interface call: NewScriptFrame"); throw 0; }
 virtual bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority) { assert(!"unexpected interface call: StartScriptingEntity"); throw 0; }
 virtual bool IsEntityUnderScriptedControl(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityUnderScriptedControl"); throw 0; }
 virtual bool IsActiveThreadTerminating() const { assert(!"unexpected interface call: IsActiveThreadTerminating"); throw 0; }
 virtual bool IsLevelLoaded(const CCharString&) const { assert(!"unexpected interface call: IsLevelLoaded"); throw 0; }
 virtual bool IsRegionLoaded(const CCharString&) const { assert(!"unexpected interface call: IsRegionLoaded"); throw 0; }
 virtual bool IsRegionLoadedAndPreloaded(const CCharString&) const { assert(!"unexpected interface call: IsRegionLoadedAndPreloaded"); throw 0; }
 virtual bool IsRegionDefLoaded(const CCharString&) const { assert(!"unexpected interface call: IsRegionDefLoaded"); throw 0; }
 virtual const CCharString& GetRegionName() const { assert(!"unexpected interface call: GetRegionName"); throw 0; }
 virtual bool MsgIsLevelLoaded(const CCharString&) const { assert(!"unexpected interface call: MsgIsLevelLoaded"); throw 0; }
 virtual bool MsgIsLevelUnloaded(const CCharString&) const { assert(!"unexpected interface call: MsgIsLevelUnloaded"); throw 0; }
 virtual bool MsgOnLevelLoaded(std::list<CCharString,std::allocator<CCharString> >&) const { assert(!"unexpected interface call: MsgOnLevelLoaded"); throw 0; }
 virtual bool MsgOnLevelUnloaded(std::list<CCharString,std::allocator<CCharString> >&) const { assert(!"unexpected interface call: MsgOnLevelUnloaded"); throw 0; }
 virtual bool MsgIsRegionLoaded(const CCharString&) const { assert(!"unexpected interface call: MsgIsRegionLoaded"); throw 0; }
 virtual bool MsgIsRegionUnloaded(const CCharString&) const { assert(!"unexpected interface call: MsgIsRegionUnloaded"); throw 0; }
 virtual bool MsgOnRegionLoaded(CCharString&) const { assert(!"unexpected interface call: MsgOnRegionLoaded"); throw 0; }
 virtual bool MsgOnRegionUnloaded(CCharString&) const { assert(!"unexpected interface call: MsgOnRegionUnloaded"); throw 0; }
 virtual bool MsgOnRegionPreunload(CCharString&) const { assert(!"unexpected interface call: MsgOnRegionPreunload"); throw 0; }
 virtual bool MsgOnQuestCompleted(const CCharString&) const { assert(!"unexpected interface call: MsgOnQuestCompleted"); throw 0; }
 virtual bool MsgOnAnyQuestCompleted(CCharString&) const { assert(!"unexpected interface call: MsgOnAnyQuestCompleted"); throw 0; }
 virtual bool MsgOnQuestFailed(const CCharString&) const { assert(!"unexpected interface call: MsgOnQuestFailed"); throw 0; }
 virtual bool MsgOnQuestCompletedBeforeScreenShown(const CCharString&) const { assert(!"unexpected interface call: MsgOnQuestCompletedBeforeScreenShown"); throw 0; }
 virtual bool MsgOnQuestFailedBeforeScreenShown(const CCharString&) const { assert(!"unexpected interface call: MsgOnQuestFailedBeforeScreenShown"); throw 0; }
 virtual bool MsgOnQuestAccepted(CCharString&) const { assert(!"unexpected interface call: MsgOnQuestAccepted"); throw 0; }
 virtual bool MsgOnFeatAccepted(long&) const { assert(!"unexpected interface call: MsgOnFeatAccepted"); throw 0; }
 virtual bool MsgIsBoastMade(long) const { assert(!"unexpected interface call: MsgIsBoastMade"); throw 0; }
 virtual bool MsgOnBoastMade(long&, CCharString&) const { assert(!"unexpected interface call: MsgOnBoastMade"); throw 0; }
 virtual bool MsgOnBoastsMade(std::vector<std::pair<long,CCharString>,std::allocator<std::pair<long,CCharString> > >&) const { assert(!"unexpected interface call: MsgOnBoastsMade"); throw 0; }
 virtual void RemoveBoastMessage() const { assert(!"unexpected interface call: RemoveBoastMessage"); throw 0; }
 virtual bool IsQuestStartScreenActive(CCharString&) const { assert(!"unexpected interface call: IsQuestStartScreenActive"); throw 0; }
 virtual bool MsgOnLeavingQuestStartScreen(CCharString&) const { assert(!"unexpected interface call: MsgOnLeavingQuestStartScreen"); throw 0; }
 virtual bool MsgOnLeavingExperienceSpendingScreen() const { assert(!"unexpected interface call: MsgOnLeavingExperienceSpendingScreen"); throw 0; }
 virtual long MsgIsQuestionAnsweredYesOrNo() const { assert(!"unexpected interface call: MsgIsQuestionAnsweredYesOrNo"); throw 0; }
 virtual bool MsgIsGameInfoClickedPast() const { assert(!"unexpected interface call: MsgIsGameInfoClickedPast"); throw 0; }
 virtual bool MsgIsTutorialClickedPast() const { assert(!"unexpected interface call: MsgIsTutorialClickedPast"); throw 0; }
 virtual bool MsgIsActionModeButtonPressed() const { assert(!"unexpected interface call: MsgIsActionModeButtonPressed"); throw 0; }
 virtual bool MsgOnExpressionPerformed(CCharString&) const { assert(!"unexpected interface call: MsgOnExpressionPerformed"); throw 0; }
 virtual bool MsgIsCutSceneSkipped() const { assert(!"unexpected interface call: MsgIsCutSceneSkipped"); throw 0; }
 virtual void RemoveAllCutSceneSkippedMessages() const { assert(!"unexpected interface call: RemoveAllCutSceneSkippedMessages"); throw 0; }
 virtual bool MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString&) const { assert(!"unexpected interface call: MsgOnHeroHairTypeChanged"); throw 0; }
 virtual bool MsgOnHeroUsedTeleporter(CCharString&) const { assert(!"unexpected interface call: MsgOnHeroUsedTeleporter"); throw 0; }
 virtual bool MsgOnHeroUsedGuildSeal() const { assert(!"unexpected interface call: MsgOnHeroUsedGuildSeal"); throw 0; }
 virtual bool MsgOnGameSavedManually() const { assert(!"unexpected interface call: MsgOnGameSavedManually"); throw 0; }
 virtual bool MsgOnHeroSlept(bool&) const { assert(!"unexpected interface call: MsgOnHeroSlept"); throw 0; }
 virtual bool MsgOnHeroFiredRangedWeapon() const { assert(!"unexpected interface call: MsgOnHeroFiredRangedWeapon"); throw 0; }
 virtual bool MsgOnHeroCastSpell(EHeroAbility&) const { assert(!"unexpected interface call: MsgOnHeroCastSpell"); throw 0; }
 virtual bool MsgOnHeroPickedPocket(CScriptThing&) const { assert(!"unexpected interface call: MsgOnHeroPickedPocket"); throw 0; }
 virtual bool MsgOnHeroPickedLock(CScriptThing&) const { assert(!"unexpected interface call: MsgOnHeroPickedLock"); throw 0; }
 virtual bool MsgOnFishingGameFinished(CScriptThing&) const { assert(!"unexpected interface call: MsgOnFishingGameFinished"); throw 0; }
 virtual bool MsgOnTavernGameFinished(const CScriptThing&) const { assert(!"unexpected interface call: MsgOnTavernGameFinished"); throw 0; }
 virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString&) const { assert(!"unexpected interface call: MsgOnHeroRewardedWithItemsFrom"); throw 0; }
 virtual bool MsgOnChestOpeningCancelled() const { assert(!"unexpected interface call: MsgOnChestOpeningCancelled"); throw 0; }
 virtual void DontPopulateNextLoadedRegion() const { assert(!"unexpected interface call: DontPopulateNextLoadedRegion"); throw 0; }
 virtual void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString&) const { assert(!"unexpected interface call: GetWanderingPopulationScriptDefNameInCurrentRegion"); throw 0; }
 virtual void GetWanderingPopulationScriptDefNameInRegion(const CCharString&, CCharString&) const { assert(!"unexpected interface call: GetWanderingPopulationScriptDefNameInRegion"); throw 0; }
 virtual bool IsHeroAllowedHenchmenInCurrentRegion() const { assert(!"unexpected interface call: IsHeroAllowedHenchmenInCurrentRegion"); throw 0; }
 virtual bool IsHeroAllowedHenchmenInRegion(const CCharString&) const { assert(!"unexpected interface call: IsHeroAllowedHenchmenInRegion"); throw 0; }
 virtual void PostAddScriptedEntities() const { assert(!"unexpected interface call: PostAddScriptedEntities"); throw 0; }
 virtual bool IsPlayerHoldingUnsheatheRangedWeaponButton() const { assert(!"unexpected interface call: IsPlayerHoldingUnsheatheRangedWeaponButton"); throw 0; }
 virtual bool IsPlayerHoldingLockTargetButton() const { assert(!"unexpected interface call: IsPlayerHoldingLockTargetButton"); throw 0; }
 virtual bool IsPlayerHoldingFireRangedWeaponButton() const { assert(!"unexpected interface call: IsPlayerHoldingFireRangedWeaponButton"); throw 0; }
 virtual bool IsPlayerHoldingFirstPersonTargetingButton() const { assert(!"unexpected interface call: IsPlayerHoldingFirstPersonTargetingButton"); throw 0; }
 virtual bool IsHeroInProjectileWeaponMode() const { assert(!"unexpected interface call: IsHeroInProjectileWeaponMode"); throw 0; }
 virtual CScriptThing& GetHero() { assert(!"unexpected interface call: GetHero"); throw 0; }
 virtual CScriptThing GetHeroTargetedThing() const { assert(!"unexpected interface call: GetHeroTargetedThing"); throw 0; }
 virtual CScriptThing GetThingWithScriptName(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: GetThingWithScriptName"); throw 0; }
 virtual CScriptThing GetThingWithScriptName(const CCharString&) const { assert(!"unexpected interface call: GetThingWithScriptName"); throw 0; }
 virtual CScriptThing GetRandomThingWithScriptName(const CCharString&) const { assert(!"unexpected interface call: GetRandomThingWithScriptName"); throw 0; }
 virtual long GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllThingsWithScriptName"); throw 0; }
 virtual long GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllCreaturesInAreaWithScriptName"); throw 0; }
 virtual CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: GetNearestWithScriptName"); throw 0; }
 virtual CScriptThing GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: GetFurthestWithScriptName"); throw 0; }
 virtual long GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllThingsWithDefNameByDistanceFrom"); throw 0; }
 virtual long GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllThingsWithDefName"); throw 0; }
 virtual CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: GetNearestWithDefName"); throw 0; }
 virtual CScriptThing GetFurthestWithDefName(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: GetFurthestWithDefName"); throw 0; }
 virtual CScriptThing GetThingWithUID(unsigned __int64) const { assert(!"unexpected interface call: GetThingWithUID"); throw 0; }
 virtual long GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllCreaturesExcludingHero"); throw 0; }
 virtual long GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetAllThingsInLevel"); throw 0; }
 virtual bool IsThingWithThisUIDAlive(unsigned __int64) const { assert(!"unexpected interface call: IsThingWithThisUIDAlive"); throw 0; }
 virtual long RegisterTimer() { assert(!"unexpected interface call: RegisterTimer"); throw 0; }
 virtual void DeregisterTimer(long) { assert(!"unexpected interface call: DeregisterTimer"); throw 0; }
 virtual void SetTimer(long, long) { assert(!"unexpected interface call: SetTimer"); throw 0; }
 virtual long GetTimer(long) const { assert(!"unexpected interface call: GetTimer"); throw 0; }
 virtual CScriptThing CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) const { assert(!"unexpected interface call: CreateCreature"); throw 0; }
 virtual CScriptThing CreateCreatureNearby(const CCharString&, const C3DVector&, float, const CCharString&, bool) const { assert(!"unexpected interface call: CreateCreatureNearby"); throw 0; }
 virtual CScriptThing CreateCreatureOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: CreateCreatureOnEntity"); throw 0; }
 virtual CScriptThing TurnCreatureInto(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: TurnCreatureInto"); throw 0; }
 virtual void SetCreatureCreationDelayFrames(long) { assert(!"unexpected interface call: SetCreatureCreationDelayFrames"); throw 0; }
 virtual void ResetCreatureCreationDelayFrames() { assert(!"unexpected interface call: ResetCreatureCreationDelayFrames"); throw 0; }
 virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, const CCharString&) const { assert(!"unexpected interface call: CreateObject"); throw 0; }
 virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, float, const CCharString&) const { assert(!"unexpected interface call: CreateObject"); throw 0; }
 virtual CScriptThing CreateObjectOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: CreateObjectOnEntity"); throw 0; }
 virtual CScriptThing CreateEffect(const CCharString&, const CScriptThing&, const CCharString&, const CCharString&, bool, bool) const { assert(!"unexpected interface call: CreateEffect"); throw 0; }
 virtual CScriptThing CreateEffect(const CCharString&, const C3DVector&, const CCharString&, float, bool, bool) const { assert(!"unexpected interface call: CreateEffect"); throw 0; }
 virtual CScriptThing CreateLight(const C3DVector&, const CRGBColour&, const CCharString&, float, float, bool) const { assert(!"unexpected interface call: CreateLight"); throw 0; }
 virtual CScriptThing CreateExperienceOrb(const C3DVector&, long) const { assert(!"unexpected interface call: CreateExperienceOrb"); throw 0; }
 virtual CScriptThing CreateExplosion(const CCharString&, const C3DVector&, const C3DVector*, CCharString) const { assert(!"unexpected interface call: CreateExplosion"); throw 0; }
 virtual CScriptThing CreatePhysicalBarrier(float, const C3DVector&, const C3DVector*, CCharString) const { assert(!"unexpected interface call: CreatePhysicalBarrier"); throw 0; }
 virtual CScriptThing CreateRumble(const C3DVector&, float, float, CCharString) const { assert(!"unexpected interface call: CreateRumble"); throw 0; }
 virtual void ClearAllRumbles() const { assert(!"unexpected interface call: ClearAllRumbles"); throw 0; }
 virtual void RemoveThing(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: RemoveThing"); throw 0; }
 virtual void ShowOnScreenMessage(const CCharString&, float) const { assert(!"unexpected interface call: ShowOnScreenMessage"); throw 0; }
 virtual void ShowOnScreenMessage(const CCharString&, const CCharString&, const CCharString&, float) const { assert(!"unexpected interface call: ShowOnScreenMessage"); throw 0; }
 virtual void ShowOnScreenMessage(const C2DVector&, const CCharString&, const CRGBColour&, const CCharString&) const { assert(!"unexpected interface call: ShowOnScreenMessage"); throw 0; }
 virtual void AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const { assert(!"unexpected interface call: AddScreenMessage"); throw 0; }
 virtual void AddScreenTitleMessage(const CCharString&, float, bool) const { assert(!"unexpected interface call: AddScreenTitleMessage"); throw 0; }
 virtual void GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) const { assert(!"unexpected interface call: GiveHeroYesNoQuestion"); throw 0; }
 virtual void DisplayGameInfo(const CCharString&) const { assert(!"unexpected interface call: DisplayGameInfo"); throw 0; }
 virtual void DisplayGameInfoText(const CWideString&) const { assert(!"unexpected interface call: DisplayGameInfoText"); throw 0; }
 virtual bool IsSafeToDisplayGameInfo() const { assert(!"unexpected interface call: IsSafeToDisplayGameInfo"); throw 0; }
 virtual bool DisplayTutorial(ETutorialCategory) const { assert(!"unexpected interface call: DisplayTutorial"); throw 0; }
 virtual bool IsTutorialSystemEnabled() const { assert(!"unexpected interface call: IsTutorialSystemEnabled"); throw 0; }
 virtual void GiveHeroWeapon(const CCharString&, bool) const { assert(!"unexpected interface call: GiveHeroWeapon"); throw 0; }
 virtual void GiveHeroObject(const CCharString&, long, bool) const { assert(!"unexpected interface call: GiveHeroObject"); throw 0; }
 virtual void SetWeaponAsHerosActiveWeapon(const CCharString&) const { assert(!"unexpected interface call: SetWeaponAsHerosActiveWeapon"); throw 0; }
 virtual void GiveHeroItem(const CScriptThing&) const { assert(!"unexpected interface call: GiveHeroItem"); throw 0; }
 virtual bool GiveHeroItemsFromContainer(const CScriptThing&, bool) const { assert(!"unexpected interface call: GiveHeroItemsFromContainer"); throw 0; }
 virtual void TakeObjectFromHero(const CCharString&) const { assert(!"unexpected interface call: TakeObjectFromHero"); throw 0; }
 virtual void GiveHeroGold(long) const { assert(!"unexpected interface call: GiveHeroGold"); throw 0; }
 virtual long GetHeroGold() const { assert(!"unexpected interface call: GetHeroGold"); throw 0; }
 virtual void GiveHeroExperience(long) const { assert(!"unexpected interface call: GiveHeroExperience"); throw 0; }
 virtual void SetHeroAbleToGainExperience(bool) const { assert(!"unexpected interface call: SetHeroAbleToGainExperience"); throw 0; }
 virtual void SheatheHeroWeapons() const { assert(!"unexpected interface call: SheatheHeroWeapons"); throw 0; }
 virtual void SetHeroWillAsUsable(bool) const { assert(!"unexpected interface call: SetHeroWillAsUsable"); throw 0; }
 virtual void SetHeroWeaponsAsUsable(bool) const { assert(!"unexpected interface call: SetHeroWeaponsAsUsable"); throw 0; }
 virtual void SetWeaponOutCrimeEnabled(bool) const { assert(!"unexpected interface call: SetWeaponOutCrimeEnabled"); throw 0; }
 virtual void SetGuardsIgnoreCrimes(bool) const { assert(!"unexpected interface call: SetGuardsIgnoreCrimes"); throw 0; }
 virtual void RemoveAllHeroWeapons() const { assert(!"unexpected interface call: RemoveAllHeroWeapons"); throw 0; }
 virtual bool IsReportedOrUnreportedCrimeKnown(const CScriptThing&) const { assert(!"unexpected interface call: IsReportedOrUnreportedCrimeKnown"); throw 0; }
 virtual void ConfiscateAllHeroItems() const { assert(!"unexpected interface call: ConfiscateAllHeroItems"); throw 0; }
 virtual void ConfiscateAllHeroWeapons() const { assert(!"unexpected interface call: ConfiscateAllHeroWeapons"); throw 0; }
 virtual void ConfiscateItemsOfTypeFromHero(const CCharString&) const { assert(!"unexpected interface call: ConfiscateItemsOfTypeFromHero"); throw 0; }
 virtual void ReturnAllConfiscatedItemsToHero() const { assert(!"unexpected interface call: ReturnAllConfiscatedItemsToHero"); throw 0; }
 virtual bool GiveHeroTutorial(ETutorialCategory) const { assert(!"unexpected interface call: GiveHeroTutorial"); throw 0; }
 virtual void MakeHeroCarryItemInHand(const CCharString&) const { assert(!"unexpected interface call: MakeHeroCarryItemInHand"); throw 0; }
 virtual void MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: MakeHeroCarryItemInHand"); throw 0; }
 virtual void AddTattooToHero(const CCharString&) const { assert(!"unexpected interface call: AddTattooToHero"); throw 0; }
 virtual void GiveHeroAbility(EHeroAbility, bool) const { assert(!"unexpected interface call: GiveHeroAbility"); throw 0; }
 virtual bool IsPlayerZTargetingThing(const CScriptThing&) const { assert(!"unexpected interface call: IsPlayerZTargetingThing"); throw 0; }
 virtual bool IsPlayerCreatureBlocking() const { assert(!"unexpected interface call: IsPlayerCreatureBlocking"); throw 0; }
 virtual bool IsPlayerCreatureReadyToFireProjectileWeapon(float&) const { assert(!"unexpected interface call: IsPlayerCreatureReadyToFireProjectileWeapon"); throw 0; }
 virtual long GetPlayerCreatureCombatMultiplier() const { assert(!"unexpected interface call: GetPlayerCreatureCombatMultiplier"); throw 0; }
 virtual long GetPlayerCreatureCombatMultiplierRunningNumHits() const { assert(!"unexpected interface call: GetPlayerCreatureCombatMultiplierRunningNumHits"); throw 0; }
 virtual void ResetPlayerCreatureCombatMultiplier() const { assert(!"unexpected interface call: ResetPlayerCreatureCombatMultiplier"); throw 0; }
 virtual bool IsPlayerCreatureFlourishEnabled() const { assert(!"unexpected interface call: IsPlayerCreatureFlourishEnabled"); throw 0; }
 virtual void SetPlayerCreatureOnlyTarget(const CScriptThing&) const { assert(!"unexpected interface call: SetPlayerCreatureOnlyTarget"); throw 0; }
 virtual void ResetPlayerCreatureOnlyTarget() const { assert(!"unexpected interface call: ResetPlayerCreatureOnlyTarget"); throw 0; }
 virtual void RespawnHero(bool) const { assert(!"unexpected interface call: RespawnHero"); throw 0; }
 virtual void GiveHeroMorality(float) const { assert(!"unexpected interface call: GiveHeroMorality"); throw 0; }
 virtual float GetHeroMorality() const { assert(!"unexpected interface call: GetHeroMorality"); throw 0; }
 virtual EMorality GetHeroMoralityCategory() const { assert(!"unexpected interface call: GetHeroMoralityCategory"); throw 0; }
 virtual void ChangeHeroMoralityDueToTheft() const { assert(!"unexpected interface call: ChangeHeroMoralityDueToTheft"); throw 0; }
 virtual void ChangeHeroMoralityDueToPicklock() const { assert(!"unexpected interface call: ChangeHeroMoralityDueToPicklock"); throw 0; }
 virtual void GiveHeroRenownPoints(long) const { assert(!"unexpected interface call: GiveHeroRenownPoints"); throw 0; }
 virtual long GetHeroRenownLevel() const { assert(!"unexpected interface call: GetHeroRenownLevel"); throw 0; }
 virtual bool IsHeroRenownLevelFull() const { assert(!"unexpected interface call: IsHeroRenownLevelFull"); throw 0; }
 virtual void IncreaseHeroRenownLevel() const { assert(!"unexpected interface call: IncreaseHeroRenownLevel"); throw 0; }
 virtual long GetHeroStrengthLevel() const { assert(!"unexpected interface call: GetHeroStrengthLevel"); throw 0; }
 virtual long GetHeroSkillLevel() const { assert(!"unexpected interface call: GetHeroSkillLevel"); throw 0; }
 virtual long GetHeroWillLevel() const { assert(!"unexpected interface call: GetHeroWillLevel"); throw 0; }
 virtual long GetHeroStatLevel(EHeroTrainableStatType) const { assert(!"unexpected interface call: GetHeroStatLevel"); throw 0; }
 virtual long GetHeroStatMax(EHeroTrainableStatType) const { assert(!"unexpected interface call: GetHeroStatMax"); throw 0; }
 virtual void SetHeroAge(float) const { assert(!"unexpected interface call: SetHeroAge"); throw 0; }
 virtual float GetHeroAge() const { assert(!"unexpected interface call: GetHeroAge"); throw 0; }
 virtual void SetHeroAsTeenager(bool) const { assert(!"unexpected interface call: SetHeroAsTeenager"); throw 0; }
 virtual void SetHeroAsApprentice(bool) const { assert(!"unexpected interface call: SetHeroAsApprentice"); throw 0; }
 virtual float GetDistanceHeroCanBeHeardFrom() const { assert(!"unexpected interface call: GetDistanceHeroCanBeHeardFrom"); throw 0; }
 virtual long GetHeroRoughExperienceLevel() const { assert(!"unexpected interface call: GetHeroRoughExperienceLevel"); throw 0; }
 virtual long GetHeroExperienceAvailableToSpend() const { assert(!"unexpected interface call: GetHeroExperienceAvailableToSpend"); throw 0; }
 virtual float GetHeroFatness() const { assert(!"unexpected interface call: GetHeroFatness"); throw 0; }
 virtual float GetHeroScariness() const { assert(!"unexpected interface call: GetHeroScariness"); throw 0; }
 virtual float GetHeroAttractiveness() const { assert(!"unexpected interface call: GetHeroAttractiveness"); throw 0; }
 virtual float GetHeroWillEnergyLevel() const { assert(!"unexpected interface call: GetHeroWillEnergyLevel"); throw 0; }
 virtual void SetHeroWillEnergyLevel(float) const { assert(!"unexpected interface call: SetHeroWillEnergyLevel"); throw 0; }
 virtual void SetHeroWillEnergyAsAbleToRefill(bool) const { assert(!"unexpected interface call: SetHeroWillEnergyAsAbleToRefill"); throw 0; }
 virtual long GetNumberOfItemsOfTypeInInventory(const CCharString&) const { assert(!"unexpected interface call: GetNumberOfItemsOfTypeInInventory"); throw 0; }
 virtual bool IsObjectInThingsPossession(const CCharString&, const CScriptThing&) const { assert(!"unexpected interface call: IsObjectInThingsPossession"); throw 0; }
 virtual bool IsHeroHandLampLit() const { assert(!"unexpected interface call: IsHeroHandLampLit"); throw 0; }
 virtual void SetHeroHandLampAsLit(bool) const { assert(!"unexpected interface call: SetHeroHandLampAsLit"); throw 0; }
 virtual bool IsWearingClothingItem(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: IsWearingClothingItem"); throw 0; }
 virtual bool IsHeroNaked() const { assert(!"unexpected interface call: IsHeroNaked"); throw 0; }
 virtual void RemoveHeroClothing() const { assert(!"unexpected interface call: RemoveHeroClothing"); throw 0; }
 virtual void SetHeroAsWearing(const CCharString&) const { assert(!"unexpected interface call: SetHeroAsWearing"); throw 0; }
 virtual void ChangeHeroHairstyle(const CCharString&) const { assert(!"unexpected interface call: ChangeHeroHairstyle"); throw 0; }
 virtual void RemoveHeroHairstyle() const { assert(!"unexpected interface call: RemoveHeroHairstyle"); throw 0; }
 virtual bool IsWearingHairstyle(CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: IsWearingHairstyle"); throw 0; }
 virtual bool IsPlayerCarryingItemOfType(const CCharString&) const { assert(!"unexpected interface call: IsPlayerCarryingItemOfType"); throw 0; }
 virtual bool IsPlayerWieldingWeapon() const { assert(!"unexpected interface call: IsPlayerWieldingWeapon"); throw 0; }
 virtual bool IsEntityWieldingWeapon(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityWieldingWeapon"); throw 0; }
 virtual bool IsEntityWieldingMeleeWeapon(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityWieldingMeleeWeapon"); throw 0; }
 virtual bool IsEntityWieldingRangedWeapon(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityWieldingRangedWeapon"); throw 0; }
 virtual bool GetPreviouslyWieldedMeleeWeaponName(CCharString&) const { assert(!"unexpected interface call: GetPreviouslyWieldedMeleeWeaponName"); throw 0; }
 virtual bool GetPreviouslyWieldedRangedWeaponName(CCharString&) const { assert(!"unexpected interface call: GetPreviouslyWieldedRangedWeaponName"); throw 0; }
 virtual bool IsEntityAbleToAttack(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityAbleToAttack"); throw 0; }
 virtual CScriptThing EntityGetThingInPrimarySlot(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetThingInPrimarySlot"); throw 0; }
 virtual void ApplyHeroPenaltyForDeath() const { assert(!"unexpected interface call: ApplyHeroPenaltyForDeath"); throw 0; }
 virtual void GiveHeroTitle(const CCharString&) const { assert(!"unexpected interface call: GiveHeroTitle"); throw 0; }
 virtual EHeroTitle GetHeroTitle() const { assert(!"unexpected interface call: GetHeroTitle"); throw 0; }
 virtual void EntitySetAsMarryable(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsMarryable"); throw 0; }
 virtual void EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsAbleToRegionFollowWhenMarried"); throw 0; }
 virtual void EntityForceMarriageToHero(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityForceMarriageToHero"); throw 0; }
 virtual bool IsEntityMarriedToHero(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityMarriedToHero"); throw 0; }
 virtual bool IsEntityMarriable(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityMarriable"); throw 0; }
 virtual bool GetHeroHasMarried() const { assert(!"unexpected interface call: GetHeroHasMarried"); throw 0; }
 virtual bool GetHeroHasCurrentMarriage() const { assert(!"unexpected interface call: GetHeroHasCurrentMarriage"); throw 0; }
 virtual bool GetHeroHasDivorcedMarriage() const { assert(!"unexpected interface call: GetHeroHasDivorcedMarriage"); throw 0; }
 virtual bool GetHeroHasChildren() const { assert(!"unexpected interface call: GetHeroHasChildren"); throw 0; }
 virtual bool GetHeroHasMurderedWife() const { assert(!"unexpected interface call: GetHeroHasMurderedWife"); throw 0; }
 virtual bool IsHeroChild() const { assert(!"unexpected interface call: IsHeroChild"); throw 0; }
 virtual void CancelHeroTeleportEffects() const { assert(!"unexpected interface call: CancelHeroTeleportEffects"); throw 0; }
 virtual long GetNumberOfTimesHeroHasHadSex() const { assert(!"unexpected interface call: GetNumberOfTimesHeroHasHadSex"); throw 0; }
 virtual void SetNumberOfTimesHeroHasHadSex(long) const { assert(!"unexpected interface call: SetNumberOfTimesHeroHasHadSex"); throw 0; }
 virtual void SetHeroAsHavingHadSex(bool) const { assert(!"unexpected interface call: SetHeroAsHavingHadSex"); throw 0; }
 virtual void SetHeroAsHavingHadGaySex(bool) const { assert(!"unexpected interface call: SetHeroAsHavingHadGaySex"); throw 0; }
 virtual void GiveThingHeroRewardItem(const CScriptThing&, const CCharString&, const CCharString&) const { assert(!"unexpected interface call: GiveThingHeroRewardItem"); throw 0; }
 virtual void GiveThingItemInHand(const CScriptThing&, const CCharString&, bool) const { assert(!"unexpected interface call: GiveThingItemInHand"); throw 0; }
 virtual void GiveThingItemInSlot(const CScriptThing&, const CCharString&, const CCharString&) const { assert(!"unexpected interface call: GiveThingItemInSlot"); throw 0; }
 virtual void GiveHeroExpression(const CCharString&, long, bool) const { assert(!"unexpected interface call: GiveHeroExpression"); throw 0; }
 virtual bool HeroHasExpression(const CCharString&) const { assert(!"unexpected interface call: HeroHasExpression"); throw 0; }
 virtual bool IsHeroPerformingExpression(const CCharString&) const { assert(!"unexpected interface call: IsHeroPerformingExpression"); throw 0; }
 virtual bool IsDPadButtonHeldForExpression(const CCharString&) const { assert(!"unexpected interface call: IsDPadButtonHeldForExpression"); throw 0; }
 virtual void EntityFollowThing(const CScriptThing&, const CScriptThing&, float, bool) const { assert(!"unexpected interface call: EntityFollowThing"); throw 0; }
 virtual void EntityStopFollowing(const CScriptThing&) const { assert(!"unexpected interface call: EntityStopFollowing"); throw 0; }
 virtual bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetFollowingEntityList"); throw 0; }
 virtual bool GetHeroSummonedCreaturesList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetHeroSummonedCreaturesList"); throw 0; }
 virtual bool GetPerceivingHeroEntityList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { assert(!"unexpected interface call: GetPerceivingHeroEntityList"); throw 0; }
 virtual bool IsEntityFollowingHero(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityFollowingHero"); throw 0; }
 virtual void EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsAllowedToFollowHero"); throw 0; }
 virtual void EntitySetAsAllowedToChangeRegionFollowingState(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsAllowedToChangeRegionFollowingState"); throw 0; }
 virtual void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsRespondingToFollowAndWaitExpressions"); throw 0; }
 virtual void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsMirroringHeroEnemyRelationsWhileFollowing"); throw 0; }
 virtual void TeleportAllFollowersToHeroPosition() const { assert(!"unexpected interface call: TeleportAllFollowersToHeroPosition"); throw 0; }
 virtual void EntityTeleportToHeroPosition(const CScriptThing&) const { assert(!"unexpected interface call: EntityTeleportToHeroPosition"); throw 0; }
 virtual void SendEntityEvent(NEntityEvents::EEventType, const CScriptThing&, const CScriptThing&, CThing*) const { assert(!"unexpected interface call: SendEntityEvent"); throw 0; }
 virtual float GetWaterHeightAtPosition(const C3DVector&) const { assert(!"unexpected interface call: GetWaterHeightAtPosition"); throw 0; }
 virtual bool IsFishingSpotEnabled(const CScriptThing&) const { assert(!"unexpected interface call: IsFishingSpotEnabled"); throw 0; }
 virtual void DisableFishingSpot(const CScriptThing&) const { assert(!"unexpected interface call: DisableFishingSpot"); throw 0; }
 virtual void UpdateFishWeight(const CScriptThing&) const { assert(!"unexpected interface call: UpdateFishWeight"); throw 0; }
 virtual float GetBestFishWeight(const CCharString&) const { assert(!"unexpected interface call: GetBestFishWeight"); throw 0; }
 virtual void HeroGoFishing(bool) const { assert(!"unexpected interface call: HeroGoFishing"); throw 0; }
 virtual long GetHeroFishingLevel() const { assert(!"unexpected interface call: GetHeroFishingLevel"); throw 0; }
 virtual void HeroGoDigging() const { assert(!"unexpected interface call: HeroGoDigging"); throw 0; }
 virtual void HeroStopDigging() const { assert(!"unexpected interface call: HeroStopDigging"); throw 0; }
 virtual void HeroPlayOracleMinigame() const { assert(!"unexpected interface call: HeroPlayOracleMinigame"); throw 0; }
 virtual bool IsHeroPlayingOracleMinigame() const { assert(!"unexpected interface call: IsHeroPlayingOracleMinigame"); throw 0; }
 virtual bool HasHeroWonOracleMinigame() const { assert(!"unexpected interface call: HasHeroWonOracleMinigame"); throw 0; }
 virtual void HeroPlayFireheartMinigame() const { assert(!"unexpected interface call: HeroPlayFireheartMinigame"); throw 0; }
 virtual void HeroQuitFireheartMinigame() const { assert(!"unexpected interface call: HeroQuitFireheartMinigame"); throw 0; }
 virtual bool HasHeroForceQuitFireheartMinigame() const { assert(!"unexpected interface call: HasHeroForceQuitFireheartMinigame"); throw 0; }
 virtual float GetHeroHealth() const { assert(!"unexpected interface call: GetHeroHealth"); throw 0; }
 virtual float GetHeroHealthMax() const { assert(!"unexpected interface call: GetHeroHealthMax"); throw 0; }
 virtual float GetHeroHealthPercentage() const { assert(!"unexpected interface call: GetHeroHealthPercentage"); throw 0; }
 virtual long GetHeroWillEnergy() const { assert(!"unexpected interface call: GetHeroWillEnergy"); throw 0; }
 virtual long GetHeroWillEnergyMax() const { assert(!"unexpected interface call: GetHeroWillEnergyMax"); throw 0; }
 virtual long GetHeroWillEnergyPercentage() const { assert(!"unexpected interface call: GetHeroWillEnergyPercentage"); throw 0; }
 virtual void ChangeHeroHealthBy(float, bool, bool) const { assert(!"unexpected interface call: ChangeHeroHealthBy"); throw 0; }
 virtual float GetHealth(const CScriptThing&) const { assert(!"unexpected interface call: GetHealth"); throw 0; }
 virtual void ModifyThingHealth(const CScriptThing&, float, bool) const { assert(!"unexpected interface call: ModifyThingHealth"); throw 0; }
 virtual void EntitySetMaxHealth(const CScriptThing&, float, bool) const { assert(!"unexpected interface call: EntitySetMaxHealth"); throw 0; }
 virtual void SetThingAsKilled(const CScriptThing&) const { assert(!"unexpected interface call: SetThingAsKilled"); throw 0; }
 virtual void GiveHeroNewQuestObjective(const CCharString&, unsigned long) const { assert(!"unexpected interface call: GiveHeroNewQuestObjective"); throw 0; }
 virtual void TellHeroQuestObjectiveCompleted(unsigned long) const { assert(!"unexpected interface call: TellHeroQuestObjectiveCompleted"); throw 0; }
 virtual void TellHeroQuestObjectiveFailed(unsigned long) const { assert(!"unexpected interface call: TellHeroQuestObjectiveFailed"); throw 0; }
 virtual void AddQuestRegion(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: AddQuestRegion"); throw 0; }
 virtual void SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&) const { assert(!"unexpected interface call: SetQuestWorldMapOffset"); throw 0; }
 virtual bool IsHeroOnQuest() const { assert(!"unexpected interface call: IsHeroOnQuest"); throw 0; }
 virtual void HeroReceiveMessageFromGuildMaster(const CCharString&, const CCharString&, bool, bool) const { assert(!"unexpected interface call: HeroReceiveMessageFromGuildMaster"); throw 0; }
 virtual void SetGuildMasterMessages(bool) const { assert(!"unexpected interface call: SetGuildMasterMessages"); throw 0; }
 virtual void ActivateQuest(const CCharString&) const { assert(!"unexpected interface call: ActivateQuest"); throw 0; }
 virtual void ActivateMultipleQuests(const CArray<CCharString>&) const { assert(!"unexpected interface call: ActivateMultipleQuests"); throw 0; }
 virtual void ActivateQuestWithoutLoadingResources(const CCharString&) const { assert(!"unexpected interface call: ActivateQuestWithoutLoadingResources"); throw 0; }
 virtual void ActivateMultipleQuestsWithoutLoadingResources(const CArray<CCharString>&) const { assert(!"unexpected interface call: ActivateMultipleQuestsWithoutLoadingResources"); throw 0; }
 virtual void DeactivateQuest(const CCharString&, unsigned long) const { assert(!"unexpected interface call: DeactivateQuest"); throw 0; }
 virtual void DeactivateQuestLater(const CCharString&, unsigned long) const { assert(!"unexpected interface call: DeactivateQuestLater"); throw 0; }
 virtual void PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) const { assert(!"unexpected interface call: PrepareQuestsWhenFinalQuestIsActivated"); throw 0; }
 virtual void PrepareQuestsWhenFinalQuestIsCompleted() const { assert(!"unexpected interface call: PrepareQuestsWhenFinalQuestIsCompleted"); throw 0; }
 virtual bool IsQuestActive(const CCharString&) const { assert(!"unexpected interface call: IsQuestActive"); throw 0; }
 virtual bool IsQuestRegistered(const CCharString&) const { assert(!"unexpected interface call: IsQuestRegistered"); throw 0; }
 virtual bool IsQuestCompleted(const CCharString&) const { assert(!"unexpected interface call: IsQuestCompleted"); throw 0; }
 virtual bool IsQuestFailed(const CCharString&) const { assert(!"unexpected interface call: IsQuestFailed"); throw 0; }
 virtual void SetQuestAsCompleted(const CCharString&, bool, bool, bool) const { assert(!"unexpected interface call: SetQuestAsCompleted"); throw 0; }
 virtual void SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const { assert(!"unexpected interface call: SetQuestAsFailed"); throw 0; }
 virtual void SetQuestAsPersistent(const CCharString&, bool) const { assert(!"unexpected interface call: SetQuestAsPersistent"); throw 0; }
 virtual const CCharString& GetExclusiveQuestScriptName() const { assert(!"unexpected interface call: GetExclusiveQuestScriptName"); throw 0; }
 virtual void AddQuestCard(const CCharString&, const CCharString&, bool, bool) const { assert(!"unexpected interface call: AddQuestCard"); throw 0; }
 virtual void RemoveQuestCardFromGuild(const CCharString&) const { assert(!"unexpected interface call: RemoveQuestCardFromGuild"); throw 0; }
 virtual void RemoveQuestCardFromHero(const CCharString&) const { assert(!"unexpected interface call: RemoveQuestCardFromHero"); throw 0; }
 virtual void GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) const { assert(!"unexpected interface call: GiveHeroQuestCardDirectly"); throw 0; }
 virtual void SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) const { assert(!"unexpected interface call: SetQuestCardObjective"); throw 0; }
 virtual void SetQuestCardGoldReward(const CCharString&, long) const { assert(!"unexpected interface call: SetQuestCardGoldReward"); throw 0; }
 virtual void SetQuestCardRenownReward(const CCharString&, long) const { assert(!"unexpected interface call: SetQuestCardRenownReward"); throw 0; }
 virtual void RemoveAllAvailableQuestCardsFromGuild() const { assert(!"unexpected interface call: RemoveAllAvailableQuestCardsFromGuild"); throw 0; }
 virtual void FailAllActiveQuests() const { assert(!"unexpected interface call: FailAllActiveQuests"); throw 0; }
 virtual void GetAllActiveQuestInfo(std::vector<CCharString,std::allocator<CCharString> >&, std::vector<CCharString,std::allocator<CCharString> >&) const { assert(!"unexpected interface call: GetAllActiveQuestInfo"); throw 0; }
 virtual void AddFeatCard(long, const CCharString&, const CCharString&) const { assert(!"unexpected interface call: AddFeatCard"); throw 0; }
 virtual void AddBoast(const CCharString&, long, long, long, bool, const CCharString&, long) const { assert(!"unexpected interface call: AddBoast"); throw 0; }
 virtual void RemoveBoast(long, const CCharString&) const { assert(!"unexpected interface call: RemoveBoast"); throw 0; }
 virtual void SetBoastAsFailed(long, const CCharString&) const { assert(!"unexpected interface call: SetBoastAsFailed"); throw 0; }
 virtual void SetBoastAsCompleted(long, const CCharString&) const { assert(!"unexpected interface call: SetBoastAsCompleted"); throw 0; }
 virtual bool IsBoastTaken(long, const CCharString&) const { assert(!"unexpected interface call: IsBoastTaken"); throw 0; }
 virtual void AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, long) const { assert(!"unexpected interface call: AddLogBookEntry"); throw 0; }
 virtual void KickOffQuestStartScreen(const CCharString&, bool, bool) const { assert(!"unexpected interface call: KickOffQuestStartScreen"); throw 0; }
 virtual void KickOffDeathScreen() const { assert(!"unexpected interface call: KickOffDeathScreen"); throw 0; }
 virtual void KickOffCreditsScreen(CCharString&) const { assert(!"unexpected interface call: KickOffCreditsScreen"); throw 0; }
 virtual void SetPreferredQuickAccessItem(const CCharString&, long, long) const { assert(!"unexpected interface call: SetPreferredQuickAccessItem"); throw 0; }
 virtual CCharString GetDeathRecoveryMarkerName() const { assert(!"unexpected interface call: GetDeathRecoveryMarkerName"); throw 0; }
 virtual void SetDeathRecoveryMarkerName(const CCharString&) const { assert(!"unexpected interface call: SetDeathRecoveryMarkerName"); throw 0; }
 virtual void ResetDeathRecoveryMarkerNameToDefault() const { assert(!"unexpected interface call: ResetDeathRecoveryMarkerNameToDefault"); throw 0; }
 virtual bool IsToFailQuestOnDeath() const { assert(!"unexpected interface call: IsToFailQuestOnDeath"); throw 0; }
 virtual void SetWhetherToFailQuestOnDeath(bool) const { assert(!"unexpected interface call: SetWhetherToFailQuestOnDeath"); throw 0; }
 virtual void ResetWhetherToFailQuestOnDeathToDefault() const { assert(!"unexpected interface call: ResetWhetherToFailQuestOnDeathToDefault"); throw 0; }
 virtual CScriptThing GetMostRecentValidUsedTarget() const { assert(!"unexpected interface call: GetMostRecentValidUsedTarget"); throw 0; }
 virtual CCharString GetMostRecentValidUsedTargetName() const { assert(!"unexpected interface call: GetMostRecentValidUsedTargetName"); throw 0; }
 virtual void DisplayQuestInfo(bool) const { assert(!"unexpected interface call: DisplayQuestInfo"); throw 0; }
 virtual void SetQuestInfoName(const char*) const { assert(!"unexpected interface call: SetQuestInfoName"); throw 0; }
 virtual void SetQuestInfoText(const char*) const { assert(!"unexpected interface call: SetQuestInfoText"); throw 0; }
 virtual long AddQuestInfoBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) const { assert(!"unexpected interface call: AddQuestInfoBar"); throw 0; }
 virtual long AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour&, const CCharString&, float) const { assert(!"unexpected interface call: AddQuestInfoBarHealth"); throw 0; }
 virtual long AddQuestInfoTimer(const CTimer&, const CCharString&, float) const { assert(!"unexpected interface call: AddQuestInfoTimer"); throw 0; }
 virtual long AddQuestInfoCounter(const CCounter&, const CCharString&, float) const { assert(!"unexpected interface call: AddQuestInfoCounter"); throw 0; }
 virtual long AddQuestInfoCounter(const CCharString&, long, float) const { assert(!"unexpected interface call: AddQuestInfoCounter"); throw 0; }
 virtual long AddQuestInfoCounterList(const CCharString&, long, float) const { assert(!"unexpected interface call: AddQuestInfoCounterList"); throw 0; }
 virtual long AddQuestInfoTick(const CCharString&, bool, float) const { assert(!"unexpected interface call: AddQuestInfoTick"); throw 0; }
 virtual long AddQuestInfoTick(EGameAction, bool, float) const { assert(!"unexpected interface call: AddQuestInfoTick"); throw 0; }
 virtual void UpdateQuestInfoBar(long, float, float, float) const { assert(!"unexpected interface call: UpdateQuestInfoBar"); throw 0; }
 virtual void ChangeQuestInfoBarColour(long, const CRGBColour&, const CRGBColour&) const { assert(!"unexpected interface call: ChangeQuestInfoBarColour"); throw 0; }
 virtual void UpdateQuestInfoTimer(long, float) const { assert(!"unexpected interface call: UpdateQuestInfoTimer"); throw 0; }
 virtual void UpdateQuestInfoCounter(long, long, long) const { assert(!"unexpected interface call: UpdateQuestInfoCounter"); throw 0; }
 virtual void UpdateQuestInfoCounterList(long, long, long) const { assert(!"unexpected interface call: UpdateQuestInfoCounterList"); throw 0; }
 virtual void UpdateQuestInfoTick(long, bool) const { assert(!"unexpected interface call: UpdateQuestInfoTick"); throw 0; }
 virtual void RemoveQuestInfoElement(long) const { assert(!"unexpected interface call: RemoveQuestInfoElement"); throw 0; }
 virtual void RemoveAllQuestInfoElements() const { assert(!"unexpected interface call: RemoveAllQuestInfoElements"); throw 0; }
 virtual void DisplayTime(bool) const { assert(!"unexpected interface call: DisplayTime"); throw 0; }
 virtual void DisplayMoneyBag(bool) const { assert(!"unexpected interface call: DisplayMoneyBag"); throw 0; }
 virtual void DisplayMiniGameInfo(bool, EMiniGameType) const { assert(!"unexpected interface call: DisplayMiniGameInfo"); throw 0; }
 virtual void UpdateMiniGameInfoBar(float) const { assert(!"unexpected interface call: UpdateMiniGameInfoBar"); throw 0; }
 virtual bool IsEntityPickPocketable(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityPickPocketable"); throw 0; }
 virtual bool IsEntityPickLockable(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityPickLockable"); throw 0; }
 virtual bool IsEntityStealable(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityStealable"); throw 0; }
 virtual void EntitySetAsPickPocketed(const CScriptThing&) const { assert(!"unexpected interface call: EntitySetAsPickPocketed"); throw 0; }
 virtual void EntitySetAsPickLocked(const CScriptThing&) const { assert(!"unexpected interface call: EntitySetAsPickLocked"); throw 0; }
 virtual void EntitySetAsStolen(const CScriptThing&) const { assert(!"unexpected interface call: EntitySetAsStolen"); throw 0; }
 virtual void MiniMapAddMarker(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: MiniMapAddMarker"); throw 0; }
 virtual void MiniMapSetMarkerGraphic(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: MiniMapSetMarkerGraphic"); throw 0; }
 virtual void MiniMapRemoveMarker(const CScriptThing&) const { assert(!"unexpected interface call: MiniMapRemoveMarker"); throw 0; }
 virtual void MiniMapRemoveAllMarkers() const { assert(!"unexpected interface call: MiniMapRemoveAllMarkers"); throw 0; }
 virtual void MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) const { assert(!"unexpected interface call: MiniMapAllowRouteBetweenRegions"); throw 0; }
 virtual void MiniMapSetAsEnabled(bool) const { assert(!"unexpected interface call: MiniMapSetAsEnabled"); throw 0; }
 virtual void EntitySetAsHiddenOnMiniMap(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsHiddenOnMiniMap"); throw 0; }
 virtual void SetHUDEnabled(bool) const { assert(!"unexpected interface call: SetHUDEnabled"); throw 0; }
 virtual bool TextEntryExists(const CCharString&) const { assert(!"unexpected interface call: TextEntryExists"); throw 0; }
 virtual CCharString GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: GetValidTextEntryNameWithAttitude"); throw 0; }
 virtual void SetThingHasInformation(const CScriptThing&, bool, bool, bool) const { assert(!"unexpected interface call: SetThingHasInformation"); throw 0; }
 virtual void ClearThingHasInformation(const CScriptThing&) const { assert(!"unexpected interface call: ClearThingHasInformation"); throw 0; }
 virtual void EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetWillBeUsingNarrator"); throw 0; }
 virtual void EntityResetAsPureAINarrator(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetAsPureAINarrator"); throw 0; }
 virtual long AddNewConversation(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: AddNewConversation"); throw 0; }
 virtual void AddPersonToConversation(long, const CScriptThing&) const { assert(!"unexpected interface call: AddPersonToConversation"); throw 0; }
 virtual void AddLineToConversation(long, const CCharString&, bool, const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: AddLineToConversation"); throw 0; }
 virtual void RemoveConversation(long, bool) const { assert(!"unexpected interface call: RemoveConversation"); throw 0; }
 virtual bool IsConversationActive(long) const { assert(!"unexpected interface call: IsConversationActive"); throw 0; }
 virtual void PlayAVIMovie(const CCharString&) const { assert(!"unexpected interface call: PlayAVIMovie"); throw 0; }
 virtual void StartMovieSequence(const CCharString&, CScriptGameResourceObjectMovieBase&) { assert(!"unexpected interface call: StartMovieSequence"); throw 0; }
 virtual void FixMovieSequenceCamera(bool) const { assert(!"unexpected interface call: FixMovieSequenceCamera"); throw 0; }
 virtual void FadeScreenOutUntilNextCallToFadeScreenIn(float, float) { assert(!"unexpected interface call: FadeScreenOutUntilNextCallToFadeScreenIn"); throw 0; }
 virtual bool FadeScreenOut(float, float, CRGBColour) { assert(!"unexpected interface call: FadeScreenOut"); throw 0; }
 virtual void FadeScreenIn() { assert(!"unexpected interface call: FadeScreenIn"); throw 0; }
 virtual bool IsScreenFadingOut() const { assert(!"unexpected interface call: IsScreenFadingOut"); throw 0; }
 virtual void Pause(float) const { assert(!"unexpected interface call: Pause"); throw 0; }
 virtual void EndCutFade() { assert(!"unexpected interface call: EndCutFade"); throw 0; }
 virtual void EndLetterBox() { assert(!"unexpected interface call: EndLetterBox"); throw 0; }
 virtual void PauseAllNonScriptedEntities(bool) { assert(!"unexpected interface call: PauseAllNonScriptedEntities"); throw 0; }
 virtual void PauseAllEntities(bool) { assert(!"unexpected interface call: PauseAllEntities"); throw 0; }
 virtual void SetAllowScreenFadingOnNextRegionChange(bool) { assert(!"unexpected interface call: SetAllowScreenFadingOnNextRegionChange"); throw 0; }
 virtual void SetAllowScreenFadingIfAlreadyFaded(bool) { assert(!"unexpected interface call: SetAllowScreenFadingIfAlreadyFaded"); throw 0; }
 virtual void SetToKeepHeroAbilitiesDuringCutscenes(bool) const { assert(!"unexpected interface call: SetToKeepHeroAbilitiesDuringCutscenes"); throw 0; }
 virtual void SetToDisplayTutorialsDuringCutscenes(bool) const { assert(!"unexpected interface call: SetToDisplayTutorialsDuringCutscenes"); throw 0; }
 virtual void SetCutsceneMode(bool, bool) const { assert(!"unexpected interface call: SetCutsceneMode"); throw 0; }
 virtual bool IsInCutscene() const { assert(!"unexpected interface call: IsInCutscene"); throw 0; }
 virtual void SetCutsceneSkippable(bool) const { assert(!"unexpected interface call: SetCutsceneSkippable"); throw 0; }
 virtual void SetCutsceneSkippableWhilePaused(bool) const { assert(!"unexpected interface call: SetCutsceneSkippableWhilePaused"); throw 0; }
 virtual void SetCutsceneActionMode(bool, const CCharString&) const { assert(!"unexpected interface call: SetCutsceneActionMode"); throw 0; }
 virtual void PreloadNewScene(float) const { assert(!"unexpected interface call: PreloadNewScene"); throw 0; }
 virtual void StartProgressDisplay() const { assert(!"unexpected interface call: StartProgressDisplay"); throw 0; }
 virtual void StopProgressDisplay() const { assert(!"unexpected interface call: StopProgressDisplay"); throw 0; }
 virtual void SetScreenMessagesEnabled(bool) const { assert(!"unexpected interface call: SetScreenMessagesEnabled"); throw 0; }
 virtual bool IsHeroControlledByPlayer() const { assert(!"unexpected interface call: IsHeroControlledByPlayer"); throw 0; }
 virtual bool IsInMovieSequence() const { assert(!"unexpected interface call: IsInMovieSequence"); throw 0; }
 virtual void CancelAbilitiesForCutscene() const { assert(!"unexpected interface call: CancelAbilitiesForCutscene"); throw 0; }
 virtual void ResumeAbilitiesForCutscene() const { assert(!"unexpected interface call: ResumeAbilitiesForCutscene"); throw 0; }
 virtual void CancelUsingAbility(EHeroAbility) const { assert(!"unexpected interface call: CancelUsingAbility"); throw 0; }
 virtual void SetAbilityAvailability(EHeroAbility, bool) const { assert(!"unexpected interface call: SetAbilityAvailability"); throw 0; }
 virtual void SetEnvironmentalEffectsAlwaysUpdate(bool) const { assert(!"unexpected interface call: SetEnvironmentalEffectsAlwaysUpdate"); throw 0; }
 virtual void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const { assert(!"unexpected interface call: SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden"); throw 0; }
 virtual void RemoveDeadCreature(const CCharString&) const { assert(!"unexpected interface call: RemoveDeadCreature"); throw 0; }
 virtual void CameraSetCameraPreloadFlag(bool) const { assert(!"unexpected interface call: CameraSetCameraPreloadFlag"); throw 0; }
 virtual void CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const { assert(!"unexpected interface call: CameraCircleAroundThing"); throw 0; }
 virtual void CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const { assert(!"unexpected interface call: CameraCircleAroundPos"); throw 0; }
 virtual void CameraMoveToPosAndLookAtPos(const C3DVector&, const C3DVector&, float) const { assert(!"unexpected interface call: CameraMoveToPosAndLookAtPos"); throw 0; }
 virtual void CameraMoveToPosAndLookAtThing(const C3DVector&, const CScriptThing&, float) const { assert(!"unexpected interface call: CameraMoveToPosAndLookAtThing"); throw 0; }
 virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const C3DVector&, float, float) const { assert(!"unexpected interface call: CameraMoveBetweenLookingAt"); throw 0; }
 virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const CScriptThing&, float, float) const { assert(!"unexpected interface call: CameraMoveBetweenLookingAt"); throw 0; }
 virtual void CameraMoveBetweenLookFromAndLookTo(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, float) const { assert(!"unexpected interface call: CameraMoveBetweenLookFromAndLookTo"); throw 0; }
 virtual void CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, long, long) const { assert(!"unexpected interface call: CameraUseCameraPoint"); throw 0; }
 virtual void CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, long, long) const { assert(!"unexpected interface call: CameraUseCameraPoint"); throw 0; }
 virtual void CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, long, long) const { assert(!"unexpected interface call: CameraUseCameraPoint"); throw 0; }
 virtual void CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, long, long) const { assert(!"unexpected interface call: CameraUseCameraPoint"); throw 0; }
 virtual void CameraDoConversation(const CScriptThing&, const CScriptThing&, ECameraOp, bool) const { assert(!"unexpected interface call: CameraDoConversation"); throw 0; }
 virtual void CameraDefault() { assert(!"unexpected interface call: CameraDefault"); throw 0; }
 virtual void CameraResetToViewBehindHero(float) { assert(!"unexpected interface call: CameraResetToViewBehindHero"); throw 0; }
 virtual bool IsCameraInScriptedMode() const { assert(!"unexpected interface call: IsCameraInScriptedMode"); throw 0; }
 virtual void CameraUseScreenEffect(float, float, float) const { assert(!"unexpected interface call: CameraUseScreenEffect"); throw 0; }
 virtual void CameraCancelScreenEffect() const { assert(!"unexpected interface call: CameraCancelScreenEffect"); throw 0; }
 virtual bool IsCameraPosOnScreen(const C3DVector&) const { assert(!"unexpected interface call: IsCameraPosOnScreen"); throw 0; }
 virtual float GetGameAngleXY() const { assert(!"unexpected interface call: GetGameAngleXY"); throw 0; }
 virtual void CameraEarthquakeIntensityAtPos(const C3DVector&, float, float) const { assert(!"unexpected interface call: CameraEarthquakeIntensityAtPos"); throw 0; }
 virtual void CameraShake(float, float) const { assert(!"unexpected interface call: CameraShake"); throw 0; }
 virtual void OpenDoor(const CScriptThing&) const { assert(!"unexpected interface call: OpenDoor"); throw 0; }
 virtual void CloseDoor(const CScriptThing&) const { assert(!"unexpected interface call: CloseDoor"); throw 0; }
 virtual void OpenHouseDoors(const CScriptThing&) const { assert(!"unexpected interface call: OpenHouseDoors"); throw 0; }
 virtual void CloseHouseDoors(const CScriptThing&) const { assert(!"unexpected interface call: CloseHouseDoors"); throw 0; }
 virtual void JamDoor(const CScriptThing&) const { assert(!"unexpected interface call: JamDoor"); throw 0; }
 virtual void SetDoorTriggerType(const CScriptThing&, EDoorTriggerType) const { assert(!"unexpected interface call: SetDoorTriggerType"); throw 0; }
 virtual void OverrideAutomaticHouseLocking(const CScriptThing&, bool) const { assert(!"unexpected interface call: OverrideAutomaticHouseLocking"); throw 0; }
 virtual void SetHouseOwnedByPlayer(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: SetHouseOwnedByPlayer"); throw 0; }
 virtual void SetBuyableHouseAsScripted(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetBuyableHouseAsScripted"); throw 0; }
 virtual bool IsChestOpen(const CScriptThing&) const { assert(!"unexpected interface call: IsChestOpen"); throw 0; }
 virtual bool OpenChest(const CScriptThing&, bool) const { assert(!"unexpected interface call: OpenChest"); throw 0; }
 virtual void CloseChest(const CScriptThing&) const { assert(!"unexpected interface call: CloseChest"); throw 0; }
 virtual long GetNumberOfKeysNeededToUnlockChest(const CScriptThing&, CCharString&) const { assert(!"unexpected interface call: GetNumberOfKeysNeededToUnlockChest"); throw 0; }
 virtual void DisplayLockedChestMessage(const CScriptThing&) const { assert(!"unexpected interface call: DisplayLockedChestMessage"); throw 0; }
 virtual void SetTrophyAsMountable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetTrophyAsMountable"); throw 0; }
 virtual void SetVillageLimbo(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetVillageLimbo"); throw 0; }
 virtual void SetCreatureNotReload(const CScriptThing&) const { assert(!"unexpected interface call: SetCreatureNotReload"); throw 0; }
 virtual bool IsSleepingTime(const CScriptThing&) const { assert(!"unexpected interface call: IsSleepingTime"); throw 0; }
 virtual void EnableGuards(const CScriptThing&, bool) const { assert(!"unexpected interface call: EnableGuards"); throw 0; }
 virtual void EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const { assert(!"unexpected interface call: EnableVillagerDefTypes"); throw 0; }
 virtual CScriptThing TryToRespawnDefNamed(const CScriptThing&, const CCharString&, const C3DVector&) const { assert(!"unexpected interface call: TryToRespawnDefNamed"); throw 0; }
 virtual void ClearHeroEnemyOfGuards(const CScriptThing&) const { assert(!"unexpected interface call: ClearHeroEnemyOfGuards"); throw 0; }
 virtual void SetThingAsUsable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetThingAsUsable"); throw 0; }
 virtual void SetThingHomeBuilding(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: SetThingHomeBuilding"); throw 0; }
 virtual void SetVillageAttitude(const CScriptThing&, EScriptVillageAttitude) const { assert(!"unexpected interface call: SetVillageAttitude"); throw 0; }
 virtual void AddCrimeCommitted(const CScriptThing&, ECrime, bool, const CScriptThing*, const CScriptThing*, EOpinionPostDeedType) const { assert(!"unexpected interface call: AddCrimeCommitted"); throw 0; }
 virtual void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: GiveThingBestEnemyTarget"); throw 0; }
 virtual void ClearThingBestEnemyTarget(const CScriptThing&) const { assert(!"unexpected interface call: ClearThingBestEnemyTarget"); throw 0; }
 virtual void EntitySetInLimbo(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: EntitySetInLimbo"); throw 0; }
 virtual bool IsEntityInLimbo(const CScriptThing&) const { assert(!"unexpected interface call: IsEntityInLimbo"); throw 0; }
 virtual bool EntityGetShotStrikePos(const CScriptThing&, C3DVector&) const { assert(!"unexpected interface call: EntityGetShotStrikePos"); throw 0; }
 virtual void EntitySetNegateAllHits(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetNegateAllHits"); throw 0; }
 virtual void EntitySetEvadeAllHits(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetEvadeAllHits"); throw 0; }
 virtual void EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAbleToBeEngagedInCombat"); throw 0; }
 virtual void EntitySetAlwaysBlockAttacksFromThing(const CScriptThing&, const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAlwaysBlockAttacksFromThing"); throw 0; }
 virtual void EntitySetAttackThingImmediately(const CScriptThing&, const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: EntitySetAttackThingImmediately"); throw 0; }
 virtual void EntitySetCombatType(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetCombatType"); throw 0; }
 virtual void EntityResetCombatTypeToDefault(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetCombatTypeToDefault"); throw 0; }
 virtual void EntitySetMaxNumberOfAttackers(const CScriptThing&, long) const { assert(!"unexpected interface call: EntitySetMaxNumberOfAttackers"); throw 0; }
 virtual void EntityClearMaxNumberOfAttackers(const CScriptThing&) const { assert(!"unexpected interface call: EntityClearMaxNumberOfAttackers"); throw 0; }
 virtual void EntityAttachToScript(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntityAttachToScript"); throw 0; }
 virtual void EntitySetCombatAbility(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetCombatAbility"); throw 0; }
 virtual void EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntitySetRangedTarget"); throw 0; }
 virtual void EntityClearRangedTarget(const CScriptThing&) const { assert(!"unexpected interface call: EntityClearRangedTarget"); throw 0; }
 virtual void EntitySetTargetable(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetTargetable"); throw 0; }
 virtual void EntitySetTargetingType(const CScriptThing&, ETargetingType) const { assert(!"unexpected interface call: EntitySetTargetingType"); throw 0; }
 virtual void EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetTargetingValidTargetWithoutLOS"); throw 0; }
 virtual void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityTeleportToThing"); throw 0; }
 virtual void EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const { assert(!"unexpected interface call: EntityTeleportToPosition"); throw 0; }
 virtual void EntitySetFacingAngle(const CScriptThing&, float, bool) const { assert(!"unexpected interface call: EntitySetFacingAngle"); throw 0; }
 virtual void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetFacingAngleTowardsThing"); throw 0; }
 virtual void EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const { assert(!"unexpected interface call: EntitySetPerceptionVariables"); throw 0; }
 virtual void SetThingPersistent(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetThingPersistent"); throw 0; }
 virtual void EntitySetThingAsWantingMoney(const CScriptThing&, long) const { assert(!"unexpected interface call: EntitySetThingAsWantingMoney"); throw 0; }
 virtual void EntitySetAppearanceMorphSeed(const CScriptThing&, long) const { assert(!"unexpected interface call: EntitySetAppearanceMorphSeed"); throw 0; }
 virtual void SetEntityAsRegionFollowing(const CScriptThing&, const CScriptThing&, bool) const { assert(!"unexpected interface call: SetEntityAsRegionFollowing"); throw 0; }
 virtual void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetEntityAsFollowingHeroThroughTeleporters"); throw 0; }
 virtual void EntitySetAppearanceSeed(const CScriptThing&, unsigned long) const { assert(!"unexpected interface call: EntitySetAppearanceSeed"); throw 0; }
 virtual void EntityGetAppearanceSeed(const CScriptThing&, unsigned long&) const { assert(!"unexpected interface call: EntityGetAppearanceSeed"); throw 0; }
 virtual void EntitySetAsForSale(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsForSale"); throw 0; }
 virtual void EntitySetStockItemPrice(const CScriptThing&, long) const { assert(!"unexpected interface call: EntitySetStockItemPrice"); throw 0; }
 virtual long EntityGetStockItemPrice(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetStockItemPrice"); throw 0; }
 virtual void EntityPlayObjectAnimation(const CScriptThing&, const CCharString&, bool) const { assert(!"unexpected interface call: EntityPlayObjectAnimation"); throw 0; }
 virtual void EntitySetMaxRunningSpeed(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetMaxRunningSpeed"); throw 0; }
 virtual void EntitySetMaxWalkingSpeed(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetMaxWalkingSpeed"); throw 0; }
 virtual void EntityResetMaxRunningSpeed(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetMaxRunningSpeed"); throw 0; }
 virtual void EntityResetMaxWalkingSpeed(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetMaxWalkingSpeed"); throw 0; }
 virtual void EntityAttachToVillage(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityAttachToVillage"); throw 0; }
 virtual void EntitySetAsSittingOnFloor(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsSittingOnFloor"); throw 0; }
 virtual void EntitySetAsHavingBoundHands(CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsHavingBoundHands"); throw 0; }
 virtual void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing&) const { assert(!"unexpected interface call: EntitySetAsRemoveAllMovementBlockingModes"); throw 0; }
 virtual void EntitySetAsScared(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsScared"); throw 0; }
 virtual void EntitySetAsDrunk(CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsDrunk"); throw 0; }
 virtual void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityForceToLookAtThing"); throw 0; }
 virtual void EntityForceToLookAtCamera(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityForceToLookAtCamera"); throw 0; }
 virtual void EntityForceToLookAtNothing(const CScriptThing&) const { assert(!"unexpected interface call: EntityForceToLookAtNothing"); throw 0; }
 virtual void EntityResetForceToLookAt(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetForceToLookAt"); throw 0; }
 virtual void EntitySetShotAccuracyPercentage(const CScriptThing&, long) const { assert(!"unexpected interface call: EntitySetShotAccuracyPercentage"); throw 0; }
 virtual CScriptThing EntityGetStandingOnThing(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetStandingOnThing"); throw 0; }
 virtual CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetStandingInsideBuilding"); throw 0; }
 virtual void EntityDropGenericBox(const CScriptThing&) const { assert(!"unexpected interface call: EntityDropGenericBox"); throw 0; }
 virtual void EntitySheatheWeapons(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySheatheWeapons"); throw 0; }
 virtual void EntityUnsheatheWeapons(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityUnsheatheWeapons"); throw 0; }
 virtual void EntityUnsheatheMeleeWeapon(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityUnsheatheMeleeWeapon"); throw 0; }
 virtual void EntityUnsheatheRangedWeapon(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityUnsheatheRangedWeapon"); throw 0; }
 virtual void EntitySetAlpha(const CScriptThing&, float, bool) const { assert(!"unexpected interface call: EntitySetAlpha"); throw 0; }
 virtual void EntitySetAsDrawable(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsDrawable"); throw 0; }
 virtual void EntitySetCutsceneBehaviour(const CScriptThing&, ECutsceneBehaviour) const { assert(!"unexpected interface call: EntitySetCutsceneBehaviour"); throw 0; }
 virtual ESex EntityGetSex(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetSex"); throw 0; }
 virtual void EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsAbleToWalkThroughSolidObjects"); throw 0; }
 virtual void EntitySetAsRespondToHit(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsRespondToHit"); throw 0; }
 virtual void EntitySetAsDamageable(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsDamageable"); throw 0; }
 virtual void EntitySetAsKillable(const CScriptThing&, bool, bool) const { assert(!"unexpected interface call: EntitySetAsKillable"); throw 0; }
 virtual void EntitySetAsLocked(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsLocked"); throw 0; }
 virtual void EntityDecapitate(const CScriptThing&) const { assert(!"unexpected interface call: EntityDecapitate"); throw 0; }
 virtual void EntityGiveGold(const CScriptThing&, long) const { assert(!"unexpected interface call: EntityGiveGold"); throw 0; }
 virtual void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAllowBossPhaseChanges"); throw 0; }
 virtual long EntityGetBossPhase(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetBossPhase"); throw 0; }
 virtual void EntitySetBossPhase(const CScriptThing&, long) const { assert(!"unexpected interface call: EntitySetBossPhase"); throw 0; }
 virtual void EntityResetCreatureMode(const CScriptThing&) const { assert(!"unexpected interface call: EntityResetCreatureMode"); throw 0; }
 virtual bool EntitySetAsReceivingEvents(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsReceivingEvents"); throw 0; }
 virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsToAddToComboMultiplierWhenHit"); throw 0; }
 virtual void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsToAddToStatChangesWhenHit"); throw 0; }
 virtual void EntityLeaveCombatStance(const CScriptThing&) const { assert(!"unexpected interface call: EntityLeaveCombatStance"); throw 0; }
 virtual void EntitySetAsUseMovementInActions(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsUseMovementInActions"); throw 0; }
 virtual void EntitySetAsDisplayingEmoteIcon(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsDisplayingEmoteIcon"); throw 0; }
 virtual void EntitySetAsCollidableToThings(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAsCollidableToThings"); throw 0; }
 virtual void EntityEnableGravity(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityEnableGravity"); throw 0; }
 virtual void EntitySetLightAsOn(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetLightAsOn"); throw 0; }
 virtual void EntityFadeOut(const CScriptThing&, float) const { assert(!"unexpected interface call: EntityFadeOut"); throw 0; }
 virtual void EntityFadeIn(const CScriptThing&, float) const { assert(!"unexpected interface call: EntityFadeIn"); throw 0; }
 virtual void EntityBeginLoadingAnimation(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntityBeginLoadingAnimation"); throw 0; }
 virtual void EntityBeginLoadingBasicAnimations(const CScriptThing&) const { assert(!"unexpected interface call: EntityBeginLoadingBasicAnimations"); throw 0; }
 virtual bool EntityCastForcePush(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityCastForcePush"); throw 0; }
 virtual void EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityCastLightningAtTarget"); throw 0; }
 virtual void BeginLoadingMesh(const CCharString&) const { assert(!"unexpected interface call: BeginLoadingMesh"); throw 0; }
 virtual bool EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const { assert(!"unexpected interface call: EntityWillTeleportToArea"); throw 0; }
 virtual void EntityStartScreamerSuperAttackThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityStartScreamerSuperAttackThing"); throw 0; }
 virtual void EntityEndScreamerSuperAttackThing(const CScriptThing&) const { assert(!"unexpected interface call: EntityEndScreamerSuperAttackThing"); throw 0; }
 virtual void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool) { assert(!"unexpected interface call: SetHeroGuideToShowQuestCardsWhenSpokenTo"); throw 0; }
 virtual void SetLightColour(const CScriptThing&, const CRGBColour&) const { assert(!"unexpected interface call: SetLightColour"); throw 0; }
 virtual void CreatureGeneratorSetFamily(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: CreatureGeneratorSetFamily"); throw 0; }
 virtual void CreatureGeneratorTrigger(const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorTrigger"); throw 0; }
 virtual void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const CScriptThing&, bool) const { assert(!"unexpected interface call: CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger"); throw 0; }
 virtual bool CreatureGeneratorIsDepleted(const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorIsDepleted"); throw 0; }
 virtual bool CreatureGeneratorIsDestroyed(const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorIsDestroyed"); throw 0; }
 virtual void CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: CreatureGeneratorSetGeneratedCreatureScriptName"); throw 0; }
 virtual void CreatureGeneratorSetNumTriggers(const CScriptThing&, long) const { assert(!"unexpected interface call: CreatureGeneratorSetNumTriggers"); throw 0; }
 virtual long CreatureGeneratorGetNumGeneratedCreatures(const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorGetNumGeneratedCreatures"); throw 0; }
 virtual bool CreatureGeneratorAreAllCreaturesAlive(const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorAreAllCreaturesAlive"); throw 0; }
 virtual void CreatureGeneratorAddTriggerer(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorAddTriggerer"); throw 0; }
 virtual void CreatureGeneratorRemoveTriggerer(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CreatureGeneratorRemoveTriggerer"); throw 0; }
 virtual void SetCreatureGeneratorEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetCreatureGeneratorEnabled"); throw 0; }
 virtual void SetCreatureGeneratorsEnabled(const CCharString&, bool) const { assert(!"unexpected interface call: SetCreatureGeneratorsEnabled"); throw 0; }
 virtual void SetCreatureGeneratorsEnabledDuringScript(const CCharString&, bool) const { assert(!"unexpected interface call: SetCreatureGeneratorsEnabledDuringScript"); throw 0; }
 virtual void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const { assert(!"unexpected interface call: SetCreatureGeneratorsCreatureGroupAsEnabled"); throw 0; }
 virtual bool IsCreatureGenerationEnabledForRegion(const CCharString&) const { assert(!"unexpected interface call: IsCreatureGenerationEnabledForRegion"); throw 0; }
 virtual bool IsCreatureFlying(const CScriptThing&) const { assert(!"unexpected interface call: IsCreatureFlying"); throw 0; }
 virtual void SetTeleporterAsActive(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetTeleporterAsActive"); throw 0; }
 virtual bool IsTeleporterActive(const CScriptThing&) const { assert(!"unexpected interface call: IsTeleporterActive"); throw 0; }
 virtual void SetTeleportingAsActive(bool) const { assert(!"unexpected interface call: SetTeleportingAsActive"); throw 0; }
 virtual bool IsTeleportingActive() const { assert(!"unexpected interface call: IsTeleportingActive"); throw 0; }
 virtual void SetRegionExitAsActive(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetRegionExitAsActive"); throw 0; }
 virtual void SetRegionEntranceAsActive(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetRegionEntranceAsActive"); throw 0; }
 virtual void SetRegionTextDisplayAsActive(bool) const { assert(!"unexpected interface call: SetRegionTextDisplayAsActive"); throw 0; }
 virtual void SetHeroSleepingAsEnabled(bool) const { assert(!"unexpected interface call: SetHeroSleepingAsEnabled"); throw 0; }
 virtual bool IsHeroSleepingEnabled() const { assert(!"unexpected interface call: IsHeroSleepingEnabled"); throw 0; }
 virtual void SetExperienceSpendingAsEnabled(bool) const { assert(!"unexpected interface call: SetExperienceSpendingAsEnabled"); throw 0; }
 virtual void SetMoralityChangingAsEnabled(bool) const { assert(!"unexpected interface call: SetMoralityChangingAsEnabled"); throw 0; }
 virtual void SetSummonerDeathExplosionAffectsHero(bool) const { assert(!"unexpected interface call: SetSummonerDeathExplosionAffectsHero"); throw 0; }
 virtual CScriptThing GetNearestEnabledDiggingSpot(const CScriptThing&) const { assert(!"unexpected interface call: GetNearestEnabledDiggingSpot"); throw 0; }
 virtual bool IsDiggingSpotEnabled(const CScriptThing&) const { assert(!"unexpected interface call: IsDiggingSpotEnabled"); throw 0; }
 virtual bool IsDiggingSpotHidden(const CScriptThing&) const { assert(!"unexpected interface call: IsDiggingSpotHidden"); throw 0; }
 virtual void SetDiggingSpotAsHidden(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetDiggingSpotAsHidden"); throw 0; }
 virtual bool CheckForCameraMessage(const CCharString&) const { assert(!"unexpected interface call: CheckForCameraMessage"); throw 0; }
 virtual void WaitForCameraMessage(const CCharString&) const { assert(!"unexpected interface call: WaitForCameraMessage"); throw 0; }
 virtual void SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const { assert(!"unexpected interface call: SetThingAsConscious"); throw 0; }
 virtual void SetFireToThing(const CScriptThing&) const { assert(!"unexpected interface call: SetFireToThing"); throw 0; }
 virtual void ExtinguishFiresOnThing(const CScriptThing&) const { assert(!"unexpected interface call: ExtinguishFiresOnThing"); throw 0; }
 virtual bool IsThingOnFire(const CScriptThing&) const { assert(!"unexpected interface call: IsThingOnFire"); throw 0; }
 virtual void AddItemToContainer(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: AddItemToContainer"); throw 0; }
 virtual void RemoveItemFromContainer(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: RemoveItemFromContainer"); throw 0; }
 virtual void EntitySetDeathContainerAsEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetDeathContainerAsEnabled"); throw 0; }
 virtual void GetItemDefNamesFromContainer(const CScriptThing&, std::vector<CCharString,std::allocator<CCharString> >&) const { assert(!"unexpected interface call: GetItemDefNamesFromContainer"); throw 0; }
 virtual void SetCreatureBrain(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: SetCreatureBrain"); throw 0; }
 virtual void EntitySetStategroupEnabled(const CScriptThing&, const CCharString&, bool) const { assert(!"unexpected interface call: EntitySetStategroupEnabled"); throw 0; }
 virtual void EntitySetAllStategroupsEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetAllStategroupsEnabled"); throw 0; }
 virtual void EntitySetCombatEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetCombatEnabled"); throw 0; }
 virtual void EntitySetSleepEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetSleepEnabled"); throw 0; }
 virtual void EntitySetOpinionReactionsEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetOpinionReactionsEnabled"); throw 0; }
 virtual void EntitySetDeedReactionsEnabled(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntitySetDeedReactionsEnabled"); throw 0; }
 virtual void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned long,std::less<unsigned long>,std::allocator<unsigned long> >&) const { assert(!"unexpected interface call: DebugGetAllTextEntriesForTargetedThing"); throw 0; }
 virtual void EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntitySetThingAsEnemyOfThing"); throw 0; }
 virtual void EntityUnsetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityUnsetThingAsEnemyOfThing"); throw 0; }
 virtual void EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntitySetThingAsAllyOfThing"); throw 0; }
 virtual void EntityUnsetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntityUnsetThingAsAllyOfThing"); throw 0; }
 virtual void EntitySetInFaction(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetInFaction"); throw 0; }
 virtual void SetFactionAsAlliedToFaction(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: SetFactionAsAlliedToFaction"); throw 0; }
 virtual void SetFactionAsNeutralToFaction(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: SetFactionAsNeutralToFaction"); throw 0; }
 virtual void SetFactionAsEnemyToFaction(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: SetFactionAsEnemyToFaction"); throw 0; }
 virtual bool AreEntitiesEnemies(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: AreEntitiesEnemies"); throw 0; }
 virtual EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const { assert(!"unexpected interface call: GetNextInOpinionAttitudeGraph"); throw 0; }
 virtual void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString&) const { assert(!"unexpected interface call: GetOpinionAttitudeAsString"); throw 0; }
 virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetOpinionAttitudeToPlayer"); throw 0; }
 virtual void EntityGetOpinionAttitudeToPlayerAsString(const CScriptThing&, CCharString&) const { assert(!"unexpected interface call: EntityGetOpinionAttitudeToPlayerAsString"); throw 0; }
 virtual float EntityGetOpinionOfPlayer(const CScriptThing&, EOpinion) const { assert(!"unexpected interface call: EntityGetOpinionOfPlayer"); throw 0; }
 virtual void EntitySetOpinionReactionMask(const CScriptThing&, long) const { assert(!"unexpected interface call: EntitySetOpinionReactionMask"); throw 0; }
 virtual void EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetOpinionReactionMask"); throw 0; }
 virtual void EntitySetOpinionDeedMask(const CScriptThing&, long) const { assert(!"unexpected interface call: EntitySetOpinionDeedMask"); throw 0; }
 virtual void EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetOpinionDeedMask"); throw 0; }
 virtual void EntitySetOpinionDeedTypeEnabled(const CScriptThing&, EOpinionDeedType, bool) const { assert(!"unexpected interface call: EntitySetOpinionDeedTypeEnabled"); throw 0; }
 virtual void EntitySetOpinionAttitudeEnabled(const CScriptThing&, EOpinionAttitudeType, bool) const { assert(!"unexpected interface call: EntitySetOpinionAttitudeEnabled"); throw 0; }
 virtual void EntitySetOpinionReactionEnabled(const CScriptThing&, EOpinionReactionType, bool) const { assert(!"unexpected interface call: EntitySetOpinionReactionEnabled"); throw 0; }
 virtual void EntitySetPersonalityOverride(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetPersonalityOverride"); throw 0; }
 virtual void EntitySetPersonalityOverride(const CScriptThing&, long) const { assert(!"unexpected interface call: EntitySetPersonalityOverride"); throw 0; }
 virtual void EntityClearPersonalityOverride(const CScriptThing&) const { assert(!"unexpected interface call: EntityClearPersonalityOverride"); throw 0; }
 virtual void EntitySetAsOpinionSource(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: EntitySetAsOpinionSource"); throw 0; }
 virtual void EntitySetAsOpinionSource(const CScriptThing&, long) const { assert(!"unexpected interface call: EntitySetAsOpinionSource"); throw 0; }
 virtual void EntityUnsetAsOpinionSource(const CScriptThing&, bool) const { assert(!"unexpected interface call: EntityUnsetAsOpinionSource"); throw 0; }
 virtual void OpinionSourceSetAsExclusive(const CScriptThing&, bool) const { assert(!"unexpected interface call: OpinionSourceSetAsExclusive"); throw 0; }
 virtual void OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool) const { assert(!"unexpected interface call: OpinionSourceSetAsAttentionGrabbing"); throw 0; }
 virtual void EntityPostOpinionDeedToAll(const CScriptThing&, EOpinionDeedType) const { assert(!"unexpected interface call: EntityPostOpinionDeedToAll"); throw 0; }
 virtual void EntityPostOpinionDeedToRecipient(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { assert(!"unexpected interface call: EntityPostOpinionDeedToRecipient"); throw 0; }
 virtual void EntityPostOpinionDeedToRecipientVillage(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { assert(!"unexpected interface call: EntityPostOpinionDeedToRecipientVillage"); throw 0; }
 virtual long EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { assert(!"unexpected interface call: EntityPostOpinionDeedKeepSearchingForWitnesses"); throw 0; }
 virtual void RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing&, long) const { assert(!"unexpected interface call: RemoveOpinionDeedStillSearchingForWitnesses"); throw 0; }
 virtual bool IsDeedWitnessed(long) const { assert(!"unexpected interface call: IsDeedWitnessed"); throw 0; }
 virtual bool CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CanThingBe_Seen_ByOtherThing"); throw 0; }
 virtual bool CanThingBe_NearlySeen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CanThingBe_NearlySeen_ByOtherThing"); throw 0; }
 virtual bool CanThingBe_Smelled_ByOtherThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CanThingBe_Smelled_ByOtherThing"); throw 0; }
 virtual bool CanThingBe_Heard_ByOtherThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: CanThingBe_Heard_ByOtherThing"); throw 0; }
 virtual bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: IsThingAwareOfOtherThingInAnyWay"); throw 0; }
 virtual void EntitySetAsAwareOfThing(const CScriptThing&, const CScriptThing&) const { assert(!"unexpected interface call: EntitySetAsAwareOfThing"); throw 0; }
 virtual void EntitySetSoundRadius(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetSoundRadius"); throw 0; }
 virtual void EntitySetSmellRadius(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetSmellRadius"); throw 0; }
 virtual void EntitySetSightRadius(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetSightRadius"); throw 0; }
 virtual void EntitySetExtendedSightRadius(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetExtendedSightRadius"); throw 0; }
 virtual void EntitySetGiveUpChaseRadius(const CScriptThing&, float) const { assert(!"unexpected interface call: EntitySetGiveUpChaseRadius"); throw 0; }
 virtual float EntityGetHearingRadius(const CScriptThing&) const { assert(!"unexpected interface call: EntityGetHearingRadius"); throw 0; }
 virtual bool ManuallyTriggerTrap(const CScriptThing&) const { assert(!"unexpected interface call: ManuallyTriggerTrap"); throw 0; }
 virtual bool ManuallyResetTrap(const CScriptThing&) const { assert(!"unexpected interface call: ManuallyResetTrap"); throw 0; }
 virtual void SetTimeOfDay(float) const { assert(!"unexpected interface call: SetTimeOfDay"); throw 0; }
 virtual long GetTimeOfDay() const { assert(!"unexpected interface call: GetTimeOfDay"); throw 0; }
 virtual void SetTimeAsStopped(bool, long&) const { assert(!"unexpected interface call: SetTimeAsStopped"); throw 0; }
 virtual void FastForwardTimeTo(float, float) const { assert(!"unexpected interface call: FastForwardTimeTo"); throw 0; }
 virtual bool IsTimeOfDayBetween(long, long) const { assert(!"unexpected interface call: IsTimeOfDayBetween"); throw 0; }
 virtual long GetDayOfWeek() const { assert(!"unexpected interface call: GetDayOfWeek"); throw 0; }
 virtual long GetDayCount() const { assert(!"unexpected interface call: GetDayCount"); throw 0; }
 virtual long GetWorldFrame() const { assert(!"unexpected interface call: GetWorldFrame"); throw 0; }
 virtual long GetConstantFPS() const { assert(!"unexpected interface call: GetConstantFPS"); throw 0; }
 virtual CCharString GetActiveQuestName() const { assert(!"unexpected interface call: GetActiveQuestName"); throw 0; }
 virtual void TransitionToTheme(const CCharString&, float) const { assert(!"unexpected interface call: TransitionToTheme"); throw 0; }
 virtual void ResetToDefaultTheme(float) const { assert(!"unexpected interface call: ResetToDefaultTheme"); throw 0; }
 virtual void TransitionToThemeAllInternals(const CCharString&, float) const { assert(!"unexpected interface call: TransitionToThemeAllInternals"); throw 0; }
 virtual void ResetToDefaultThemeAllInternals(float) const { assert(!"unexpected interface call: ResetToDefaultThemeAllInternals"); throw 0; }
 virtual void TransitionToThemeExternals(const CCharString&, float) const { assert(!"unexpected interface call: TransitionToThemeExternals"); throw 0; }
 virtual void ResetToDefaultThemeExternals(float) const { assert(!"unexpected interface call: ResetToDefaultThemeExternals"); throw 0; }
 virtual void SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const { assert(!"unexpected interface call: SetEnvironmentThemeWeightAllChannels"); throw 0; }
 virtual void SetEnvironmentThemeWeightAllInternals(const CCharString&, float) const { assert(!"unexpected interface call: SetEnvironmentThemeWeightAllInternals"); throw 0; }
 virtual void SetEnvironmentThemeWeightExternals(const CCharString&, float) const { assert(!"unexpected interface call: SetEnvironmentThemeWeightExternals"); throw 0; }
 virtual void SetSoundThemesAsEnabledForRegion(const CCharString&, bool) const { assert(!"unexpected interface call: SetSoundThemesAsEnabledForRegion"); throw 0; }
 virtual void SetAllSoundsAsMuted(bool) const { assert(!"unexpected interface call: SetAllSoundsAsMuted"); throw 0; }
 virtual void* RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const { assert(!"unexpected interface call: RadialBlurFadeTo"); throw 0; }
 virtual void* RadialBlurFadeTo(float, float, float, float, float, float, float) const { assert(!"unexpected interface call: RadialBlurFadeTo"); throw 0; }
 virtual void RadialBlurFadeOut(float, void*) const { assert(!"unexpected interface call: RadialBlurFadeOut"); throw 0; }
 virtual bool IsRadialBlurFadeActive() const { assert(!"unexpected interface call: IsRadialBlurFadeActive"); throw 0; }
 virtual void CancelRadialBlurFade() const { assert(!"unexpected interface call: CancelRadialBlurFade"); throw 0; }
 virtual void RadialBlurSetCenterWorldPos(void*, const C3DVector&) const { assert(!"unexpected interface call: RadialBlurSetCenterWorldPos"); throw 0; }
 virtual void* DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const { assert(!"unexpected interface call: DisplacementMonochromeEffectColourFadeTo"); throw 0; }
 virtual void DisplacementMonochromeEffectColourFadeOut(float, void*) const { assert(!"unexpected interface call: DisplacementMonochromeEffectColourFadeOut"); throw 0; }
 virtual void* ScreenFilterFadeTo(float, float, float, float, float, const CRGBFloatColour&, const std::vector<CScreenFilterSThingByPass,std::allocator<CScreenFilterSThingByPass> >&) const { assert(!"unexpected interface call: ScreenFilterFadeTo"); throw 0; }
 virtual void ScreenFilterFadeOut(float, void*) const { assert(!"unexpected interface call: ScreenFilterFadeOut"); throw 0; }
 virtual void SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&, void*) const { assert(!"unexpected interface call: SetThingAndCarriedItemsNotAffectedByScreenFilter"); throw 0; }
 virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const { assert(!"unexpected interface call: UnSetThingAndCarriedItemsNotAffectedByScreenFilter"); throw 0; }
 virtual bool IsGiftRomantic(const CCharString&) const { assert(!"unexpected interface call: IsGiftRomantic"); throw 0; }
 virtual bool IsGiftFriendly(const CCharString&) const { assert(!"unexpected interface call: IsGiftFriendly"); throw 0; }
 virtual bool IsGiftOffensive(const CCharString&) const { assert(!"unexpected interface call: IsGiftOffensive"); throw 0; }
 virtual bool IsThingABed(const CScriptThing&) const { assert(!"unexpected interface call: IsThingABed"); throw 0; }
 virtual bool IsThingAChest(const CScriptThing&) const { assert(!"unexpected interface call: IsThingAChest"); throw 0; }
 virtual bool IsThingADoor(const CScriptThing&) const { assert(!"unexpected interface call: IsThingADoor"); throw 0; }
 virtual bool IsThingSmashable(const CScriptThing&) const { assert(!"unexpected interface call: IsThingSmashable"); throw 0; }
 virtual bool IsThingSearchable(const CScriptThing&) const { assert(!"unexpected interface call: IsThingSearchable"); throw 0; }
 virtual void ApplyScriptBrush(const CCharString&) const { assert(!"unexpected interface call: ApplyScriptBrush"); throw 0; }
 virtual void EnableDecals(bool) const { assert(!"unexpected interface call: EnableDecals"); throw 0; }
 virtual unsigned long PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: PlayCriteriaSoundOnThing"); throw 0; }
 virtual unsigned long PlaySoundOnThing(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: PlaySoundOnThing"); throw 0; }
 virtual unsigned long PlaySoundAtPos(const C3DVector&, const CCharString&) const { assert(!"unexpected interface call: PlaySoundAtPos"); throw 0; }
 virtual unsigned long Play2DSound(const CCharString&) const { assert(!"unexpected interface call: Play2DSound"); throw 0; }
 virtual bool IsSoundPlaying(unsigned long) const { assert(!"unexpected interface call: IsSoundPlaying"); throw 0; }
 virtual void EnableSounds(bool) const { assert(!"unexpected interface call: EnableSounds"); throw 0; }
 virtual void StopSound(unsigned long) const { assert(!"unexpected interface call: StopSound"); throw 0; }
 virtual void OverrideMusic(EMusicSetType, bool, bool) const { assert(!"unexpected interface call: OverrideMusic"); throw 0; }
 virtual void StopOverrideMusic(bool) const { assert(!"unexpected interface call: StopOverrideMusic"); throw 0; }
 virtual void CacheMusicSet(EMusicSetType) const { assert(!"unexpected interface call: CacheMusicSet"); throw 0; }
 virtual void EnableDangerMusic(bool) const { assert(!"unexpected interface call: EnableDangerMusic"); throw 0; }
 virtual bool IsDangerMusicEnabled() const { assert(!"unexpected interface call: IsDangerMusicEnabled"); throw 0; }
 virtual void StartCountdownTimer(float) const { assert(!"unexpected interface call: StartCountdownTimer"); throw 0; }
 virtual float GetCountdownTimer() const { assert(!"unexpected interface call: GetCountdownTimer"); throw 0; }
 virtual void AutoSaveCheckPoint() const { assert(!"unexpected interface call: AutoSaveCheckPoint"); throw 0; }
 virtual void AutoSaveQuestStart() const { assert(!"unexpected interface call: AutoSaveQuestStart"); throw 0; }
 virtual void AutoSave() const { assert(!"unexpected interface call: AutoSave"); throw 0; }
 virtual void SetSavingAsEnabled(bool) const { assert(!"unexpected interface call: SetSavingAsEnabled"); throw 0; }
 virtual bool IsSavingEnabled() const { assert(!"unexpected interface call: IsSavingEnabled"); throw 0; }
 virtual void SetSaveGameMarkerPos(const C3DVector&) const { assert(!"unexpected interface call: SetSaveGameMarkerPos"); throw 0; }
 virtual void ResetToFrontEnd() const { assert(!"unexpected interface call: ResetToFrontEnd"); throw 0; }
 virtual void SetGuildSealRecallLocation(const C3DVector&, float) const { assert(!"unexpected interface call: SetGuildSealRecallLocation"); throw 0; }
 virtual C3DVector GetGuildSealRecallPos() const { assert(!"unexpected interface call: GetGuildSealRecallPos"); throw 0; }
 virtual float GetGuildSealRecallAngleXY() const { assert(!"unexpected interface call: GetGuildSealRecallAngleXY"); throw 0; }
 virtual void SetReadableObjectText(const CScriptThing&, const CWideString&) const { assert(!"unexpected interface call: SetReadableObjectText"); throw 0; }
 virtual void SetReadableObjectTextTag(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: SetReadableObjectTextTag"); throw 0; }
 virtual CWideString GetFormattedString(const CCharString&, const std::vector<CWideString,std::allocator<CWideString> >&) const { assert(!"unexpected interface call: GetFormattedString"); throw 0; }
 virtual CWideString GetTextString(const CCharString&) const { assert(!"unexpected interface call: GetTextString"); throw 0; }
 virtual void AddRumourCategory(const CCharString&) const { assert(!"unexpected interface call: AddRumourCategory"); throw 0; }
 virtual void AddNewRumourToCategory(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: AddNewRumourToCategory"); throw 0; }
 virtual void RemoveRumourCategory(const CCharString&) const { assert(!"unexpected interface call: RemoveRumourCategory"); throw 0; }
 virtual void SetCategoryActivity(const CCharString&, bool) const { assert(!"unexpected interface call: SetCategoryActivity"); throw 0; }
 virtual void AddGossipVillage(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: AddGossipVillage"); throw 0; }
 virtual void AddGossipFactionToCategory(const CCharString&, const CCharString&) const { assert(!"unexpected interface call: AddGossipFactionToCategory"); throw 0; }
 virtual void SetIsGossipForPlayer(const CCharString&, bool) const { assert(!"unexpected interface call: SetIsGossipForPlayer"); throw 0; }
 virtual void SetIsGossipForPlayer(CCharString, bool) { assert(!"unexpected interface call: SetIsGossipForPlayer"); throw 0; }
 virtual void UpdateOnlineScore_Archery(float) const { assert(!"unexpected interface call: UpdateOnlineScore_Archery"); throw 0; }
 virtual void UpdateOnlineScore_ChickenKick(float) const { assert(!"unexpected interface call: UpdateOnlineScore_ChickenKick"); throw 0; }
 virtual void UpdateOnlineScore_ChapelOrTemple(float) const { assert(!"unexpected interface call: UpdateOnlineScore_ChapelOrTemple"); throw 0; }
 virtual void UpdateOnlineScore_FishingCompo(float) const { assert(!"unexpected interface call: UpdateOnlineScore_FishingCompo"); throw 0; }
 virtual void UpdateScore_FishingCompetition(float) const { assert(!"unexpected interface call: UpdateScore_FishingCompetition"); throw 0; }
 virtual float GetBestTimePairs() const { assert(!"unexpected interface call: GetBestTimePairs"); throw 0; }
 virtual float GetBestTimeSorting() const { assert(!"unexpected interface call: GetBestTimeSorting"); throw 0; }
 virtual long GetBestScoreBlackjack() const { assert(!"unexpected interface call: GetBestScoreBlackjack"); throw 0; }
 virtual long GetBestScoreCoinGolfOakVale() const { assert(!"unexpected interface call: GetBestScoreCoinGolfOakVale"); throw 0; }
 virtual long GetBestScoreCoinGolfSnowSpire() const { assert(!"unexpected interface call: GetBestScoreCoinGolfSnowSpire"); throw 0; }
 virtual long GetBestScoreShoveHaPenny() const { assert(!"unexpected interface call: GetBestScoreShoveHaPenny"); throw 0; }
 virtual float GetBestTimeGuessTheAddition() const { assert(!"unexpected interface call: GetBestTimeGuessTheAddition"); throw 0; }
 virtual bool IsHeroInTavernGame() const { assert(!"unexpected interface call: IsHeroInTavernGame"); throw 0; }
 virtual long GetNumHousesOwned() const { assert(!"unexpected interface call: GetNumHousesOwned"); throw 0; }
 virtual void StartSneaking() const { assert(!"unexpected interface call: StartSneaking"); throw 0; }
 virtual long GetStealDuration(const CScriptThing&) const { assert(!"unexpected interface call: GetStealDuration"); throw 0; }
 virtual void SetUseableByHero(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetUseableByHero"); throw 0; }
 virtual void SetOwnedByHero(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetOwnedByHero"); throw 0; }
 virtual void SetTavernTableAvailableForUse(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetTavernTableAvailableForUse"); throw 0; }
 virtual void SetIsThingTurncoatable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetIsThingTurncoatable"); throw 0; }
 virtual void SetIsThingForcePushable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetIsThingForcePushable"); throw 0; }
 virtual void SetIsThingLightningable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetIsThingLightningable"); throw 0; }
 virtual void SetIsThingEpicSpellable(const CScriptThing&, bool) const { assert(!"unexpected interface call: SetIsThingEpicSpellable"); throw 0; }
 virtual bool IsThingTurncoated(const CScriptThing&) const { assert(!"unexpected interface call: IsThingTurncoated"); throw 0; }
 virtual void AddCreatureScriptedMode(const CScriptThing&, const CCharString&) const { assert(!"unexpected interface call: AddCreatureScriptedMode"); throw 0; }
 virtual void RemoveCreatureScriptedMode(const CScriptThing&) const { assert(!"unexpected interface call: RemoveCreatureScriptedMode"); throw 0; }
 virtual void ForceShipsVisible() const { assert(!"unexpected interface call: ForceShipsVisible"); throw 0; }
 virtual bool GetSleepingPositionAndOrientationFromBed(const CScriptThing&, const CScriptThing&, C3DVector&, C3DVector&) { assert(!"unexpected interface call: GetSleepingPositionAndOrientationFromBed"); throw 0; }
 virtual void SetBedAvailability(const CScriptThing&, bool) { assert(!"unexpected interface call: SetBedAvailability"); throw 0; }
 virtual void RepopulateVillage(const CScriptThing&) const { assert(!"unexpected interface call: RepopulateVillage"); throw 0; }
 virtual void SmashAllWindowsWithinRadiusOfPoint(const C3DVector&, float) { assert(!"unexpected interface call: SmashAllWindowsWithinRadiusOfPoint"); throw 0; }
 virtual CScriptThing SetResidency(const CScriptThing&, bool) { assert(!"unexpected interface call: SetResidency"); throw 0; }
 virtual void SetThankingPhrase(CScriptThing, unsigned long) { assert(!"unexpected interface call: SetThankingPhrase"); throw 0; }
 virtual unsigned long GetThankingPhrase(CScriptThing) { assert(!"unexpected interface call: GetThankingPhrase"); throw 0; }
 virtual void ResetThankingPhrase(CScriptThing) { assert(!"unexpected interface call: ResetThankingPhrase"); throw 0; }
 virtual void SetIgnoringPhrase(CScriptThing, unsigned long) { assert(!"unexpected interface call: SetIgnoringPhrase"); throw 0; }
 virtual unsigned long GetIgnoringPhrase(CScriptThing) { assert(!"unexpected interface call: GetIgnoringPhrase"); throw 0; }
 virtual void ResetIgnoringPhrase(CScriptThing) { assert(!"unexpected interface call: ResetIgnoringPhrase"); throw 0; }
 virtual void SetWanderCentrePoint(CScriptThing, C3DVector) { assert(!"unexpected interface call: SetWanderCentrePoint"); throw 0; }
 virtual C3DVector GetWanderCentrePoint(CScriptThing) { assert(!"unexpected interface call: GetWanderCentrePoint"); throw 0; }
 virtual void ResetWanderCentrePoint(CScriptThing) { assert(!"unexpected interface call: ResetWanderCentrePoint"); throw 0; }
 virtual void SetWanderMinDistance(CScriptThing, float) { assert(!"unexpected interface call: SetWanderMinDistance"); throw 0; }
 virtual float GetWanderMinDistance(CScriptThing) { assert(!"unexpected interface call: GetWanderMinDistance"); throw 0; }
 virtual void ResetWanderMinDistance(CScriptThing) { assert(!"unexpected interface call: ResetWanderMinDistance"); throw 0; }
 virtual void SetWanderMaxDistance(CScriptThing, float) { assert(!"unexpected interface call: SetWanderMaxDistance"); throw 0; }
 virtual float GetWanderMaxDistance(CScriptThing) { assert(!"unexpected interface call: GetWanderMaxDistance"); throw 0; }
 virtual void ResetWanderMaxDistance(CScriptThing) { assert(!"unexpected interface call: ResetWanderMaxDistance"); throw 0; }
 virtual void SetGossipCounter(CScriptThing, long) { assert(!"unexpected interface call: SetGossipCounter"); throw 0; }
 virtual long GetGossipCounter(CScriptThing) { assert(!"unexpected interface call: GetGossipCounter"); throw 0; }
 virtual void ResetGossipCounter(CScriptThing) { assert(!"unexpected interface call: ResetGossipCounter"); throw 0; }
 virtual void SetMaxGossipPhrase(CScriptThing, long) { assert(!"unexpected interface call: SetMaxGossipPhrase"); throw 0; }
 virtual long GetMaxGossipPhrase(CScriptThing) { assert(!"unexpected interface call: GetMaxGossipPhrase"); throw 0; }
 virtual void ResetMaxGossipPhrase(CScriptThing) { assert(!"unexpected interface call: ResetMaxGossipPhrase"); throw 0; }
 virtual void SetWarningPhrase(CScriptThing, unsigned long) { assert(!"unexpected interface call: SetWarningPhrase"); throw 0; }
 virtual unsigned long GetWarningPhrase(CScriptThing) { assert(!"unexpected interface call: GetWarningPhrase"); throw 0; }
 virtual void ResetWarningPhrase(CScriptThing) { assert(!"unexpected interface call: ResetWarningPhrase"); throw 0; }
 virtual void SetBeerRequestPhrase(CScriptThing, unsigned long) { assert(!"unexpected interface call: SetBeerRequestPhrase"); throw 0; }
 virtual unsigned long GetBeerRequestPhrase(CScriptThing) { assert(!"unexpected interface call: GetBeerRequestPhrase"); throw 0; }
 virtual void ResetBeerRequestPhrase(CScriptThing) { assert(!"unexpected interface call: ResetBeerRequestPhrase"); throw 0; }
 virtual void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups) { assert(!"unexpected interface call: SetScriptingStateGroup"); throw 0; }
 virtual EScriptingStateGroups GetScriptingStateGroup(CScriptThing) { assert(!"unexpected interface call: GetScriptingStateGroup"); throw 0; }
 virtual void ResetScriptingStateGroup(CScriptThing) { assert(!"unexpected interface call: ResetScriptingStateGroup"); throw 0; }
 virtual void SetMaxHeroReactionDistance(CScriptThing, float) { assert(!"unexpected interface call: SetMaxHeroReactionDistance"); throw 0; }
 virtual float GetMaxHeroReactionDistance(CScriptThing) { assert(!"unexpected interface call: GetMaxHeroReactionDistance"); throw 0; }
 virtual void ResetMaxHeroReactionDistance(CScriptThing) { assert(!"unexpected interface call: ResetMaxHeroReactionDistance"); throw 0; }
 virtual void SetActionFrequency(CScriptThing, long) { assert(!"unexpected interface call: SetActionFrequency"); throw 0; }
 virtual long GetActionFrequency(CScriptThing) { assert(!"unexpected interface call: GetActionFrequency"); throw 0; }
 virtual void ResetActionFrequency(CScriptThing) { assert(!"unexpected interface call: ResetActionFrequency"); throw 0; }
 virtual void SetActionFrequencyVariation(CScriptThing, float) { assert(!"unexpected interface call: SetActionFrequencyVariation"); throw 0; }
 virtual float GetActionFrequencyVariation(CScriptThing) { assert(!"unexpected interface call: GetActionFrequencyVariation"); throw 0; }
 virtual void ResetActionFrequencyVariation(CScriptThing) { assert(!"unexpected interface call: ResetActionFrequencyVariation"); throw 0; }
 virtual void SetAction(CScriptThing, CCharString) { assert(!"unexpected interface call: SetAction"); throw 0; }
 virtual CCharString GetAction(CScriptThing) { assert(!"unexpected interface call: GetAction"); throw 0; }
 virtual void ResetAction(CScriptThing) { assert(!"unexpected interface call: ResetAction"); throw 0; }
 virtual void SetFaceHeroForAction(CScriptThing, bool) { assert(!"unexpected interface call: SetFaceHeroForAction"); throw 0; }
 virtual bool GetFaceHeroForAction(CScriptThing) { assert(!"unexpected interface call: GetFaceHeroForAction"); throw 0; }
 virtual void ResetFaceHeroForAction(CScriptThing) { assert(!"unexpected interface call: ResetFaceHeroForAction"); throw 0; }
 virtual void SetTargetName(CScriptThing, CCharString) { assert(!"unexpected interface call: SetTargetName"); throw 0; }
 virtual CCharString GetTargetName(CScriptThing) { assert(!"unexpected interface call: GetTargetName"); throw 0; }
 virtual void ResetTargetName(CScriptThing) { assert(!"unexpected interface call: ResetTargetName"); throw 0; }
 virtual void SetFollowDistance(CScriptThing, float) { assert(!"unexpected interface call: SetFollowDistance"); throw 0; }
 virtual float GetFollowDistance(CScriptThing) { assert(!"unexpected interface call: GetFollowDistance"); throw 0; }
 virtual void ResetFollowDistance(CScriptThing) { assert(!"unexpected interface call: ResetFollowDistance"); throw 0; }
 virtual void SetAttackHeroOnSight(CScriptThing, bool) { assert(!"unexpected interface call: SetAttackHeroOnSight"); throw 0; }
 virtual bool GetAttackHeroOnSight(CScriptThing) { assert(!"unexpected interface call: GetAttackHeroOnSight"); throw 0; }
 virtual void ResetAttackHeroOnSight(CScriptThing) { assert(!"unexpected interface call: ResetAttackHeroOnSight"); throw 0; }
 virtual void SetTimeToSpendHarassingHero(CScriptThing, long) { assert(!"unexpected interface call: SetTimeToSpendHarassingHero"); throw 0; }
 virtual long GetTimeToSpendHarassingHero(CScriptThing) { assert(!"unexpected interface call: GetTimeToSpendHarassingHero"); throw 0; }
 virtual void ResetTimeToSpendHarassingHero(CScriptThing) { assert(!"unexpected interface call: ResetTimeToSpendHarassingHero"); throw 0; }
 virtual void SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float) { assert(!"unexpected interface call: SetCombatNearbyEnemyFleeingBreakOffRange"); throw 0; }
 virtual float GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) { assert(!"unexpected interface call: GetCombatNearbyEnemyFleeingBreakOffRange"); throw 0; }
 virtual void ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) { assert(!"unexpected interface call: ResetCombatNearbyEnemyFleeingBreakOffRange"); throw 0; }
 virtual void SetCombatNearbyBreakOffRange(CScriptThing, float) { assert(!"unexpected interface call: SetCombatNearbyBreakOffRange"); throw 0; }
 virtual float GetCombatNearbyBreakOffRange(CScriptThing) { assert(!"unexpected interface call: GetCombatNearbyBreakOffRange"); throw 0; }
 virtual void ResetCombatNearbyBreakOffRange(CScriptThing) { assert(!"unexpected interface call: ResetCombatNearbyBreakOffRange"); throw 0; }
 virtual void SetStealStealableItems(CScriptThing, bool) { assert(!"unexpected interface call: SetStealStealableItems"); throw 0; }
 virtual bool GetStealStealableItems(CScriptThing) { assert(!"unexpected interface call: GetStealStealableItems"); throw 0; }
 virtual void ResetStealStealableItems(CScriptThing) { assert(!"unexpected interface call: ResetStealStealableItems"); throw 0; }
 virtual void SetRecoverStealableItems(CScriptThing, bool) { assert(!"unexpected interface call: SetRecoverStealableItems"); throw 0; }
 virtual bool GetRecoverStealableItems(CScriptThing) { assert(!"unexpected interface call: GetRecoverStealableItems"); throw 0; }
 virtual void ResetRecoverStealableItems(CScriptThing) { assert(!"unexpected interface call: ResetRecoverStealableItems"); throw 0; }
 virtual void SetTakeStealableItemToRandomDestination(CScriptThing, bool) { assert(!"unexpected interface call: SetTakeStealableItemToRandomDestination"); throw 0; }
 virtual bool GetTakeStealableItemToRandomDestination(CScriptThing) { assert(!"unexpected interface call: GetTakeStealableItemToRandomDestination"); throw 0; }
 virtual void ResetTakeStealableItemToRandomDestination(CScriptThing) { assert(!"unexpected interface call: ResetTakeStealableItemToRandomDestination"); throw 0; }
 virtual void SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool) { assert(!"unexpected interface call: SetKillSelfAndStealableItemAfterReachingDestination"); throw 0; }
 virtual bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) { assert(!"unexpected interface call: GetKillSelfAndStealableItemAfterReachingDestination"); throw 0; }
 virtual void ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) { assert(!"unexpected interface call: ResetKillSelfAndStealableItemAfterReachingDestination"); throw 0; }
 virtual void SetAllowedToFollow(CScriptThing, bool) { assert(!"unexpected interface call: SetAllowedToFollow"); throw 0; }
 virtual bool GetAllowedToFollow(CScriptThing) { assert(!"unexpected interface call: GetAllowedToFollow"); throw 0; }
 virtual void ResetAllowedToFollow(CScriptThing) { assert(!"unexpected interface call: ResetAllowedToFollow"); throw 0; }
 virtual void SetTableName(CScriptThing, CCharString) { assert(!"unexpected interface call: SetTableName"); throw 0; }
 virtual CCharString GetTableName(CScriptThing) { assert(!"unexpected interface call: GetTableName"); throw 0; }
 virtual void ResetTableName(CScriptThing) { assert(!"unexpected interface call: ResetTableName"); throw 0; }
 virtual void SetSeatName(CScriptThing, CCharString) { assert(!"unexpected interface call: SetSeatName"); throw 0; }
 virtual CCharString GetSeatName(CScriptThing) { assert(!"unexpected interface call: GetSeatName"); throw 0; }
 virtual void ResetSeatName(CScriptThing) { assert(!"unexpected interface call: ResetSeatName"); throw 0; }
 virtual void SetDisableHeadLooking(CScriptThing, bool) { assert(!"unexpected interface call: SetDisableHeadLooking"); throw 0; }
 virtual bool GetDisableHeadLooking(CScriptThing) { assert(!"unexpected interface call: GetDisableHeadLooking"); throw 0; }
 virtual void ResetDisableHeadLooking(CScriptThing) { assert(!"unexpected interface call: ResetDisableHeadLooking"); throw 0; }
 virtual void SetIsPushableByHero(CScriptThing, bool) { assert(!"unexpected interface call: SetIsPushableByHero"); throw 0; }
 virtual bool GetIsPushableByHero(CScriptThing) { assert(!"unexpected interface call: GetIsPushableByHero"); throw 0; }
 virtual void ResetIsPushableByHero(CScriptThing) { assert(!"unexpected interface call: ResetIsPushableByHero"); throw 0; }
 virtual void SetLookForFiniteTime(CScriptThing, bool) { assert(!"unexpected interface call: SetLookForFiniteTime"); throw 0; }
 virtual bool GetLookForFiniteTime(CScriptThing) { assert(!"unexpected interface call: GetLookForFiniteTime"); throw 0; }
 virtual void ResetLookForFiniteTime(CScriptThing) { assert(!"unexpected interface call: ResetLookForFiniteTime"); throw 0; }
 virtual void SetAvoidRegionExits(CScriptThing, bool) { assert(!"unexpected interface call: SetAvoidRegionExits"); throw 0; }
 virtual bool GetAvoidRegionExits(CScriptThing) { assert(!"unexpected interface call: GetAvoidRegionExits"); throw 0; }
 virtual void ResetAvoidRegionExits(CScriptThing) { assert(!"unexpected interface call: ResetAvoidRegionExits"); throw 0; }
 virtual void SetTargetingDistanceOffset(CScriptThing, float) { assert(!"unexpected interface call: SetTargetingDistanceOffset"); throw 0; }
 virtual float GetTargetingDistanceOffset(CScriptThing) { assert(!"unexpected interface call: GetTargetingDistanceOffset"); throw 0; }
 virtual void ResetTargetingDistanceOffset(CScriptThing) { assert(!"unexpected interface call: ResetTargetingDistanceOffset"); throw 0; }
 virtual void SetPlayerUsingMeleeDummies(bool) { assert(!"unexpected interface call: SetPlayerUsingMeleeDummies"); throw 0; }
 virtual bool GetPlayerUsingMeleeDummies() { assert(!"unexpected interface call: GetPlayerUsingMeleeDummies"); throw 0; }
 virtual void SetPlayerUsingRangedDummies(bool) { assert(!"unexpected interface call: SetPlayerUsingRangedDummies"); throw 0; }
 virtual bool GetPlayerUsingRangedDummies() { assert(!"unexpected interface call: GetPlayerUsingRangedDummies"); throw 0; }
 virtual void SetPlayerUsingWillDummies(bool) { assert(!"unexpected interface call: SetPlayerUsingWillDummies"); throw 0; }
 virtual bool GetPlayerUsingWillDummies() { assert(!"unexpected interface call: GetPlayerUsingWillDummies"); throw 0; }
 virtual void SetCheapHeadLooking(bool) { assert(!"unexpected interface call: SetCheapHeadLooking"); throw 0; }
 virtual bool GetCheapHeadLooking() { assert(!"unexpected interface call: GetCheapHeadLooking"); throw 0; }
 virtual void SetQuitTavernGame(bool) { assert(!"unexpected interface call: SetQuitTavernGame"); throw 0; }
 virtual bool GetQuitTavernGame() { assert(!"unexpected interface call: GetQuitTavernGame"); throw 0; }
 virtual void SetPrizeTavernTable(bool) { assert(!"unexpected interface call: SetPrizeTavernTable"); throw 0; }
 virtual bool GetPrizeTavernTable() { assert(!"unexpected interface call: GetPrizeTavernTable"); throw 0; }
 virtual void SetBettingActive(bool) { assert(!"unexpected interface call: SetBettingActive"); throw 0; }
 virtual bool GetBettingActive() { assert(!"unexpected interface call: GetBettingActive"); throw 0; }
 virtual void SetBettingAccept(bool) { assert(!"unexpected interface call: SetBettingAccept"); throw 0; }
 virtual bool GetBettingAccept() { assert(!"unexpected interface call: GetBettingAccept"); throw 0; }
 virtual void SetBettingAmount(long) { assert(!"unexpected interface call: SetBettingAmount"); throw 0; }
 virtual long GetBettingAmount() { assert(!"unexpected interface call: GetBettingAmount"); throw 0; }
 virtual void SetCountBetMoneyDown(bool) { assert(!"unexpected interface call: SetCountBetMoneyDown"); throw 0; }
 virtual bool GetCountBetMoneyDown() { assert(!"unexpected interface call: GetCountBetMoneyDown"); throw 0; }
 virtual void SetSpotTheAdditionBeaten(bool) { assert(!"unexpected interface call: SetSpotTheAdditionBeaten"); throw 0; }
 virtual bool GetSpotTheAdditionBeaten() { assert(!"unexpected interface call: GetSpotTheAdditionBeaten"); throw 0; }
 virtual void SetGlobalTargetingDistanceOffset(float) { assert(!"unexpected interface call: SetGlobalTargetingDistanceOffset"); throw 0; }
 virtual float GetGlobalTargetingDistanceOffset() { assert(!"unexpected interface call: GetGlobalTargetingDistanceOffset"); throw 0; }
 virtual void SetTradingPriceMult(float) { assert(!"unexpected interface call: SetTradingPriceMult"); throw 0; }
 virtual float GetTradingPriceMult() { assert(!"unexpected interface call: GetTradingPriceMult"); throw 0; }
 virtual void SetBoastingEnabled(bool) { assert(!"unexpected interface call: SetBoastingEnabled"); throw 0; }
 virtual bool GetBoastingEnabled() { assert(!"unexpected interface call: GetBoastingEnabled"); throw 0; }
 virtual void SetActiveGossipCategories(CCharString, bool) { assert(!"unexpected interface call: SetActiveGossipCategories"); throw 0; }
 virtual bool* GetActiveGossipCategories(CCharString) { assert(!"unexpected interface call: GetActiveGossipCategories"); throw 0; }
 virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetActiveGossipCategories() { assert(!"unexpected interface call: GetActiveGossipCategories"); throw 0; }
 virtual int GetActiveGossipCategoriesSize() { assert(!"unexpected interface call: GetActiveGossipCategoriesSize"); throw 0; }
 virtual void ClearActiveGossipCategories() { assert(!"unexpected interface call: ClearActiveGossipCategories"); throw 0; }
 virtual bool* GetIsGossipForPlayer(CCharString) { assert(!"unexpected interface call: GetIsGossipForPlayer"); throw 0; }
 virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetIsGossipForPlayer() { assert(!"unexpected interface call: GetIsGossipForPlayer"); throw 0; }
 virtual int GetIsGossipForPlayerSize() { assert(!"unexpected interface call: GetIsGossipForPlayerSize"); throw 0; }
 virtual void ClearIsGossipForPlayer() { assert(!"unexpected interface call: ClearIsGossipForPlayer"); throw 0; }
 virtual void SetGossip(CCharString, CCharString, long) { assert(!"unexpected interface call: SetGossip"); throw 0; }
 virtual CCharString GetGossip(CCharString, long) { assert(!"unexpected interface call: GetGossip"); throw 0; }
 virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossip(CCharString) { assert(!"unexpected interface call: GetGossip"); throw 0; }
 virtual int GetGossipSize(CCharString) { assert(!"unexpected interface call: GetGossipSize"); throw 0; }
 virtual void ClearGossip(CCharString) { assert(!"unexpected interface call: ClearGossip"); throw 0; }
 virtual void RemoveGossip(CCharString) { assert(!"unexpected interface call: RemoveGossip"); throw 0; }
 virtual void AddGossip(CCharString, CCharString) { assert(!"unexpected interface call: AddGossip"); throw 0; }
 virtual void AddGossip(CCharString) { assert(!"unexpected interface call: AddGossip"); throw 0; }
 virtual void SetGossipVillages(CCharString, CCharString, long) { assert(!"unexpected interface call: SetGossipVillages"); throw 0; }
 virtual CCharString GetGossipVillages(CCharString, long) { assert(!"unexpected interface call: GetGossipVillages"); throw 0; }
 virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipVillages(CCharString) { assert(!"unexpected interface call: GetGossipVillages"); throw 0; }
 virtual int GetGossipVillagesSize(CCharString) { assert(!"unexpected interface call: GetGossipVillagesSize"); throw 0; }
 virtual void ClearGossipVillages(CCharString) { assert(!"unexpected interface call: ClearGossipVillages"); throw 0; }
 virtual void RemoveGossipVillages(CCharString) { assert(!"unexpected interface call: RemoveGossipVillages"); throw 0; }
 virtual void AddGossipVillages(CCharString, CCharString) { assert(!"unexpected interface call: AddGossipVillages"); throw 0; }
 virtual void AddGossipVillages(CCharString) { assert(!"unexpected interface call: AddGossipVillages"); throw 0; }
 virtual void SetGossipFactions(CCharString, CCharString, long) { assert(!"unexpected interface call: SetGossipFactions"); throw 0; }
 virtual CCharString GetGossipFactions(CCharString, long) { assert(!"unexpected interface call: GetGossipFactions"); throw 0; }
 virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipFactions(CCharString) { assert(!"unexpected interface call: GetGossipFactions"); throw 0; }
 virtual int GetGossipFactionsSize(CCharString) { assert(!"unexpected interface call: GetGossipFactionsSize"); throw 0; }
 virtual void ClearGossipFactions(CCharString) { assert(!"unexpected interface call: ClearGossipFactions"); throw 0; }
 virtual void RemoveGossipFactions(CCharString) { assert(!"unexpected interface call: RemoveGossipFactions"); throw 0; }
 virtual void AddGossipFactions(CCharString, CCharString) { assert(!"unexpected interface call: AddGossipFactions"); throw 0; }
 virtual void AddGossipFactions(CCharString) { assert(!"unexpected interface call: AddGossipFactions"); throw 0; }
};

class CommentThingFixture:public CScriptThing {
public:
 int identity;bool alive;const char* name;
 CommentThingFixture():identity(0),alive(true),name("_GOOD1"){}
 bool IsAlive()const{return alive;}
 bool IsEqualTo(const CScriptThing& other)const{return this==&other;}
 CCharString GetDataString()const{CCharString result(name);return result;}
};
static void __fastcall keep_fixture(void*){assert(!"fixture count destroyed early");}
class CommentGameFixture:public FixtureScriptInterface {
public:
 CommentThingFixture impl[5];CScriptThing items[5];CCPPointerInfo* infos[5];
 bool active;mutable int checks;int timerCalls;mutable int conversations;mutable int persons;mutable int lines;
 mutable int personIds[2],speakerIds[2],listenerIds[2];mutable char lineText[2][256];
 CommentGameFixture():active(false),checks(0),timerCalls(0),conversations(0),persons(0),lines(0){
  const char* names[]={"_GOOD1","_EVIL2","_GOOD2","_EVIL1","HERO"};
  for(int i=0;i<5;++i){impl[i].identity=i;impl[i].name=names[i];infos[i]=new CCPPointerInfo(1,keep_fixture,&impl[i]);items[i].PImp.ShareData(&impl[i],infos[i]);}
 }
 ~CommentGameFixture(){for(int i=0;i<5;++i){assert(infos[i]->RefCount==2);items[i].PImp.Reset(0);delete infos[i];}}
 int id(const CScriptThing& item)const{for(int i=0;i<5;++i)if(item.PImp.Get()==&impl[i])return i;assert(false);return -1;}
 CScriptThing& GetHero(){return items[4];}
 bool IsConversationActive(long n)const{assert(n==17);++checks;return active;}
 void SetTimer(long n,long time){assert(n==0&&time==5);++timerCalls;}
 long AddNewConversation(const CScriptThing& first,bool a,bool b)const{assert(!a&&!b);assert(id(first)>=0);++conversations;return 99;}
 void AddPersonToConversation(long n,const CScriptThing& item)const{assert(n==99&&persons<2);personIds[persons++]=id(item);}
 void AddLineToConversation(long n,const CCharString& line,bool a,const CScriptThing& speaker,const CScriptThing& listener)const{assert(n==99&&!a&&lines<2);speakerIds[lines]=id(speaker);listenerIds[lines]=id(listener);strcpy(lineText[lines],text(line));++lines;}
};
int main(){unsigned int cases=0;
 for(int count=0;count<=3;++count)for(int mode=0;mode<4;++mode)for(int choice=0;choice<3;++choice)for(int response=0;response<3;++response)for(int specific=0;specific<3;++specific)for(int guard=0;guard<3;++guard)for(int living=0;living<3;++living){
  {CommentGameFixture game;pDSTGame=&game;NScript::CQ_SunnyvaleMasterData data;data.HenchmanCommentaryState=guard==2?11:0;
   NScript::CV_BodyGuardScript script(&data,game);game.active=guard==1;for(int i=0;i<count;++i)script.HenchmanList.push_back(game.items[i]);
   CScriptThing empty;const CScriptThing& specified=specific==0?empty:game.items[specific==1?(count?choice%count:0):3];
   if(living==1&&count)game.impl[choice%count].alive=false;if(living==2)game.impl[3].alive=false;
   randomCalls=0;randomValues[0]=choice;randomValues[1]=response;
   bool result=script.MakeHenchmanComment(CCharString("HELLO"),7,(NScript::CV_BodyGuardScript::EHENCHMAN_COMMENT)mode,specified);
   bool expected=count>0&&guard==0;assert(result==expected);
   assert(game.timerCalls==(expected?1:0)&&game.conversations==(expected?1:0));
   if(!expected){assert(randomCalls==0&&game.lines==0&&script.ConversationIndex==17&&data.HenchmanCommentaryState==(guard==2?11:0));}
   else {int selected=choice%count;int buddy=(selected+1)%count;if(specific==2&&living!=2){buddy=selected;selected=3;}
    bool responseAllowed=mode==3?response==0:mode!=2;bool pair=count>1&&responseAllowed;
    assert(randomCalls==(mode==3?2:1));assert(script.ConversationIndex==99&&data.HenchmanCommentaryState==7);
    assert(game.lines==(pair?2:1)&&game.persons==(pair?2:1));assert(game.personIds[pair?1:0]==4);if(pair)assert(game.personIds[0]==buddy);
    char expectedText[256];sprintf(expectedText,"TEXT_QST_016_BODYGUARD%s_HELLO",game.impl[selected].name);
    assert(!strcmp(game.lineText[0],expectedText)&&game.speakerIds[0]==selected&&game.listenerIds[0]==(pair?buddy:4));
    if(pair){char morality[32];strcpy(morality,game.impl[selected].name);if(game.impl[selected].alive)morality[strlen(morality)-1]=0;else morality[0]=0;sprintf(expectedText,"TEXT_QST_016_BODYGUARD%s_HELLO_RESPONSE%s",game.impl[buddy].name,morality);assert(!strcmp(game.lineText[1],expectedText)&&game.speakerIds[1]==buddy&&game.listenerIds[1]==selected);}
   }
  }assert(strings==0);++cases;
 }
 assert(cases==3888);puts("00e5ebd0_TEST PASS");return 0;
}