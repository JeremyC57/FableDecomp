// Behavioral fixture for the exact constructor body below; dependency models expose observable state.
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdlib.h>
static void Check(bool ok) { if(!ok) { puts("FAIL"); exit(1); } }
enum EHeroAbility {
    HERO_ABILITY_NULL=0,
    HERO_ABILITY_FORCE_PUSH=1,
    HERO_ABILITY_TIME_SPELL=2,
    HERO_ABILITY_ENFLAME_SPELL=3,
    HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4,
    HERO_ABILITY_TURNCOAT_SPELL=5,
    HERO_ABILITY_DRAIN_LIFE_SPELL=6,
    HERO_ABILITY_RAISE_DEAD_SPELL=7,
    HERO_ABILITY_BERSERK=8,
    HERO_ABILITY_DOUBLE_STRIKE=9,
    HERO_ABILITY_SUMMON_SPELL=10,
    HERO_ABILITY_LIGHTNING_SPELL=11,
    HERO_ABILITY_BATTLE_CHARGE=12,
    HERO_ABILITY_ASSASSIN_RUSH=13,
    HERO_ABILITY_HEAL_LIFE_SPELL=14,
    HERO_ABILITY_GHOST_SWORD_SPELL=15,
    HERO_ABILITY_FIREBALL_SPELL=16,
    HERO_ABILITY_MULTI_ARROW=17,
    HERO_ABILITY_DIVINE_WRATH_SPELL=18,
    HERO_ABILITY_UNHOLY_POWER_SPELL=19,
    MAX_NUMBER_OF_HERO_ABILITIES=20
};
class CCharString { public: const char* Text; CCharString(const char* s,long=-1):Text(s){} };
// The complete original CDefPointer class below matches the production opt-in.
// The pointee dependency is a fixture refcount observer, not an engine-layout model.
class CDefPointeeBase {
public:
    mutable long IVRefCount;
    CDefPointeeBase():IVRefCount(0){}
    void AddRef()const{++IVRefCount;}
    unsigned long Release()const{Check(IVRefCount>0);return --IVRefCount;}
    unsigned long GetRefCount()const{return IVRefCount;}
};
class CDefClassBase : public CDefPointeeBase { };
enum ENullDef { NULL_DEF };

template <class T> class CDefPointer {
public:
    inline CDefPointer(ENullDef = NULL_DEF) : Object(0) {}
    inline CDefPointer(const CDefPointer& other) : Object(other.Object) {
        if (Object) Object->AddRef();
    }
    CDefPointer(T*);
    template <class U> inline CDefPointer(const CDefPointer<U>& other)
        : Object(other.InternalGetObject()) {
        if (Object) Object->AddRef();
    }
    inline ~CDefPointer() { Release(); }

    inline CDefPointer& operator=(const CDefPointer& other) {
        CDefPointeeBase* object = other.Object;
        if (Object != object) {
            Release(); Object = object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
    template <class U> inline CDefPointer& operator=(const CDefPointer<U>& other) {
        CDefPointeeBase* object = other.InternalGetObject();
        if (Object != object) {
            Release(); Object = object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
    template <class U> inline void SetDynamicCast(const CDefPointer<U>& other) {
        CDefPointeeBase* object = other.InternalGetObject();
        if (Object != object) {
            Release(); Object = object;
            if (Object) Object->AddRef();
        }
    }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);
    void Swap(CDefPointer&);
    inline void Release() {
        if (Object) { Object->Release(); Object = 0; }
    }

private:
    CDefPointeeBase* Object;
};
// Fixture definition for the original declared-only T* constructor.
template<class T>CDefPointer<T>::CDefPointer(T* p):Object(const_cast<CDefPointeeBase*>(static_cast<const CDefPointeeBase*>(p))){if(Object)Object->AddRef();}
class CSpecialAbilitiesForcePushDef : public CDefClassBase {public:float LevelUpTimings[3];};
class CSpecialAbilitiesBulletTimeDef : public CDefClassBase {public:float LevelUpTimings[3];};
class CSpecialAbilitiesBerserkDef : public CDefClassBase {public:std::vector<float> LevelUpTimings;};
class CSpecialAbilitiesEnflameDef : public CDefClassBase {public:float LevelUpTimings[3];};
static CSpecialAbilitiesForcePushDef forceDef;
static CSpecialAbilitiesBulletTimeDef timeDef;
static CSpecialAbilitiesBerserkDef berserkDef;
static CSpecialAbilitiesEnflameDef enflameDef;
static int lookupCount=0,definitionCount=0;
class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& name,long index)const {
        Check(index==1);++lookupCount;
        if(!strcmp(name.Text,"SPECIAL_ABILITIES_FORCE_PUSH_DEF"))return 1;
        if(!strcmp(name.Text,"SPECIAL_ABILITIES_BULLET_TIME_DEF"))return 2;
        if(!strcmp(name.Text,"SPECIAL_ABILITIES_BERSERK_DEF"))return 3;
        if(!strcmp(name.Text,"SPECIAL_ABILITIES_ENFLAME_DEF"))return 4;
        Check(false);return 0;
    }
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index)const {
        ++definitionCount;
        if(index==1)return CDefPointer<const CDefClassBase>(&forceDef);
        if(index==2)return CDefPointer<const CDefClassBase>(&timeDef);
        if(index==3)return CDefPointer<const CDefClassBase>(&berserkDef);
        if(index==4)return CDefPointer<const CDefClassBase>(&enflameDef);
        return CDefPointer<const CDefClassBase>();
    }
    template<class T>bool GetDef(long index,CDefPointer<const T>* out)const {
        if(index<=0)return false;
        CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
        if(!def.IsNull()){out->SetDynamicCast(def);return true;}
        return false;
    }
};
class CGameDefinitionManager : public CDefinitionManager {
public:static CGameDefinitionManager& Get(){static CGameDefinitionManager manager;return manager;}
};
class CMainGameComponent {public:static long ConstantFPS;static long GetConstantFPS(){return ConstantFPS;}};
long CMainGameComponent::ConstantFPS=20;
class CTCInventoryAbilities {
public:long Maximum,Current;mutable unsigned MaximumCalls,CurrentCalls;mutable EHeroAbility LastMaximum,LastCurrent;
    CTCInventoryAbilities():Maximum(3),Current(2),MaximumCalls(0),CurrentCalls(0),LastMaximum(HERO_ABILITY_NULL),LastCurrent(HERO_ABILITY_NULL){}
    long GetAbilityMaximumLevel(EHeroAbility ability)const {++MaximumCalls;LastMaximum=ability;return Maximum;}
    long GetAbilityLevel(EHeroAbility ability)const {++CurrentCalls;LastCurrent=ability;return Current;}
};
class CThing {
public:const CTCInventoryAbilities* Inventory;
    explicit CThing(const CTCInventoryAbilities* p):Inventory(p){}
    bool PeekTC(const CTCInventoryAbilities** out)const {if(Inventory){*out=Inventory;return true;}return false;}
};
template<class T>class CIntelligentPointer {T* P;public:explicit CIntelligentPointer(T* p):P(p){}T* operator->()const{return P;}};
class CWillButtonHelperTimer {
public:
    enum EButtonHeldState { BEING_HELD_DOWN,JUST_RELEASED_DELAYED,JUST_RELEASED,RELEASED };
    CWillButtonHelperTimer(EHeroAbility,CThing&);
    long ButtonHeldDownTimer,WillAccumulatedLevel;
    CIntelligentPointer<CThing> PCreature;
    EHeroAbility MyAbility;
    EButtonHeldState CurrentState;
    std::vector<float> WillPowerUpAccumulatedTimings;
    float MaxNumberLevels,CurMaxLevels;
    bool WillIncrementedFlag,UpdateAccLevel,IsAICasting,IsAIHoldingDown;
    long LevelToCast;
};
CWillButtonHelperTimer::CWillButtonHelperTimer(EHeroAbility ability,CThing& creature)
    : ButtonHeldDownTimer(0), WillAccumulatedLevel(0), PCreature(&creature),
      MyAbility(ability), CurrentState(BEING_HELD_DOWN), MaxNumberLevels(0),
      CurMaxLevels(0), WillIncrementedFlag(false), UpdateAccLevel(true),
      IsAICasting(false), IsAIHoldingDown(false), LevelToCast(0)
{
    const CTCInventoryAbilities* ptc_inventory_abilities=0;
    if(PCreature->PeekTC(&ptc_inventory_abilities)) {
        MaxNumberLevels=(float)ptc_inventory_abilities->GetAbilityMaximumLevel(MyAbility);
        CurMaxLevels=(float)ptc_inventory_abilities->GetAbilityLevel(MyAbility);
    } else {
        MaxNumberLevels=3;
        CurMaxLevels=3;
    }
    switch(ability) {
    case HERO_ABILITY_FORCE_PUSH:
        {
            long def_index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_FORCE_PUSH_DEF",1);
            CDefPointer<const CSpecialAbilitiesForcePushDef> def;
            CGameDefinitionManager::Get().GetDef(def_index,&def);
            long index=(long)MaxNumberLevels;
            float accum=0;
            for(long c1=0;c1<index;++c1) {
                accum+=def->LevelUpTimings[c1];
                WillPowerUpAccumulatedTimings.push_back(accum*CMainGameComponent::GetConstantFPS());
            }
        }
        break;
    case HERO_ABILITY_TIME_SPELL:
        {
            long def_index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_BULLET_TIME_DEF",1);
            CDefPointer<const CSpecialAbilitiesBulletTimeDef> def;
            CGameDefinitionManager::Get().GetDef(def_index,&def);
            long index=(long)MaxNumberLevels;
            float accum=0;
            for(long c1=0;c1<index;++c1) {
                accum+=def->LevelUpTimings[c1];
                WillPowerUpAccumulatedTimings.push_back(accum*CMainGameComponent::GetConstantFPS());
            }
        }
        break;
    case HERO_ABILITY_BERSERK:
        {
            long def_index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_BERSERK_DEF",1);
            CDefPointer<const CSpecialAbilitiesBerserkDef> def;
            CGameDefinitionManager::Get().GetDef(def_index,&def);
            long index=(long)MaxNumberLevels;
            float accum=0;
            for(long c1=0;c1<index;++c1) {
                accum+=def->LevelUpTimings[c1];
                WillPowerUpAccumulatedTimings.push_back(accum*CMainGameComponent::GetConstantFPS());
            }
        }
        break;
    case HERO_ABILITY_BATTLE_CHARGE:
    case HERO_ABILITY_ASSASSIN_RUSH:
        MaxNumberLevels=0;
        break;
    case HERO_ABILITY_ENFLAME_SPELL:
        {
            long def_index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_ENFLAME_DEF",1);
            CDefPointer<const CSpecialAbilitiesEnflameDef> def;
            CGameDefinitionManager::Get().GetDef(def_index,&def);
            long index=(long)MaxNumberLevels;
            float accum=0;
            for(long c1=0;c1<index;++c1) {
                accum+=def->LevelUpTimings[c1];
                WillPowerUpAccumulatedTimings.push_back(accum*CMainGameComponent::GetConstantFPS());
            }
        }
        break;
    default:
        long index=(long)MaxNumberLevels;
            for(long c1=0;c1<index;++c1)
            WillPowerUpAccumulatedTimings.push_back((float)((c1+1)*CMainGameComponent::GetConstantFPS()));
    }
}

static unsigned tested=0;
static void Exercise(EHeroAbility ability,long maximum,long current,bool hasInventory) {
    CTCInventoryAbilities inventory;inventory.Maximum=maximum;inventory.Current=current;
    CThing creature(hasInventory?&inventory:0);
    lookupCount=definitionCount=0;
    CWillButtonHelperTimer timer(ability,creature);
    Check(timer.ButtonHeldDownTimer==0 && timer.WillAccumulatedLevel==0 && timer.LevelToCast==0);
    Check(timer.MyAbility==ability && timer.CurrentState==CWillButtonHelperTimer::BEING_HELD_DOWN);
    Check(!timer.WillIncrementedFlag && timer.UpdateAccLevel && !timer.IsAICasting && !timer.IsAIHoldingDown);
    bool zero=ability==HERO_ABILITY_BATTLE_CHARGE || ability==HERO_ABILITY_ASSASSIN_RUSH;
    long bound=hasInventory?maximum:3;
    Check(timer.MaxNumberLevels==float(zero?0:bound));
    Check(timer.CurMaxLevels==float(hasInventory?current:3));
    Check(timer.WillPowerUpAccumulatedTimings.size()==(zero||bound<0?0u:unsigned(bound)));
    bool special=ability==HERO_ABILITY_FORCE_PUSH||ability==HERO_ABILITY_TIME_SPELL||ability==HERO_ABILITY_BERSERK||ability==HERO_ABILITY_ENFLAME_SPELL;
    Check(lookupCount==(special?1:0) && definitionCount==(special?1:0));
    const float forceExpected[3]={5,15,30},timeExpected[3]={10,20,40},berserkExpected[3]={20,25,30},enflameExpected[3]={15,20,30};
    for(unsigned i=0;i<timer.WillPowerUpAccumulatedTimings.size();++i){
        float expected=float((i+1)*20);
        if(ability==HERO_ABILITY_FORCE_PUSH)expected=forceExpected[i];
        if(ability==HERO_ABILITY_TIME_SPELL)expected=timeExpected[i];
        if(ability==HERO_ABILITY_BERSERK)expected=berserkExpected[i];
        if(ability==HERO_ABILITY_ENFLAME_SPELL)expected=enflameExpected[i];
        Check(timer.WillPowerUpAccumulatedTimings[i]==expected);
    }
    Check(inventory.MaximumCalls==(hasInventory?1u:0u)&&inventory.CurrentCalls==(hasInventory?1u:0u));
    if(hasInventory)Check(inventory.LastMaximum==ability&&inventory.LastCurrent==ability);
    Check(forceDef.IVRefCount==0&&timeDef.IVRefCount==0&&berserkDef.IVRefCount==0&&enflameDef.IVRefCount==0);
    ++tested;
}
int main(){
    forceDef.LevelUpTimings[0]=0.25f;forceDef.LevelUpTimings[1]=0.5f;forceDef.LevelUpTimings[2]=0.75f;
    timeDef.LevelUpTimings[0]=0.5f;timeDef.LevelUpTimings[1]=0.5f;timeDef.LevelUpTimings[2]=1.0f;
    berserkDef.LevelUpTimings.push_back(1.0f);berserkDef.LevelUpTimings.push_back(0.25f);berserkDef.LevelUpTimings.push_back(0.25f);
    enflameDef.LevelUpTimings[0]=0.75f;enflameDef.LevelUpTimings[1]=0.25f;enflameDef.LevelUpTimings[2]=0.5f;
    for(int ability=0;ability<=20;++ability){
        Exercise((EHeroAbility)ability,3,2,false);
        for(long maximum=-1;maximum<=3;++maximum)
            for(long current=0;current<=3;++current)
                Exercise((EHeroAbility)ability,maximum,current,true);
    }
    Check(tested==441);puts("008edff0_TEST PASS");return 0;
}
