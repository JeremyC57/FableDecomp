// CTCBattleCharge::FrameUpdate() -- per frame: on release finish the current action, play the level's cast sound and create the start-up effect; while the pose delay runs count it down, otherwise rush on; once the pose delay is over leave the exit modes once (release the ability mutex, resume an AI creature, drop creature mode 0x23, unfreeze a hero's controls) and remove the component when an AI creature charges or the charge delay has run out
// Grade: RELOCATION_MATCH over the true extent (576 bytes; BOUNDS_FIX: the catalog row 617 runs into 0x007A8E20) -- the parked source unchanged, graded over code + `mov edi,edi` + the 4-entry jump table
// Leaf: ?FrameUpdate@CTCBattleCharge@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A8BE0, 558 bytes of code + `mov edi,edi` + a 4-entry jump table = 576 bytes, `ret`;
//   the catalog row (617) runs on into 0x007A8E20 (`mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx ; ret 4`), 0x007A8E30
//   (`mov eax,[ecx] ; ret`) and 0x007A8E40 (`mov eax,[ecx+4] ; sub eax,[ecx] ; sar eax,2 ; ret`).
// Identity: CTCBattleCharge vtable 0x0126CBFC slot 9 (FrameUpdate in the CTCBase order); Ego_r.pdb has
//   `?FrameUpdate@CTCBattleCharge@@UAEXXZ`; the literals are the four HERO_ABILITY_BATTLE_CHARGE_CAST_* criteria. Jump table
//   0x007A8E10 checked entry by entry: cases 0..3 -> +0x6B/+0x94/+0xBD/+0xE6 (in order), ours the same.
// Callees: CThingCreatureBase::FinishCurrentAction 0x00662650, CVectorMap LowerBound 0x0040F020 (inline GetTC<T>(T**)),
//   CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00,
//   CTCBattleCharge::CreateStartUpEffect 0x007A76F0 / RushForwards 0x007A8700 (private), CThingAICreature::SuspendAI(bool,
//   void*) 0x00833A10, CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0, CThing::GetPlayer 0x004C7A30,
//   CPlayer::SpecialAbilityUnfreezeControls 0x00487150, CThing::RemoveTC(ETCInterfaceType) 0x004C9840.
// Interface types (vtable slot 21): CTCSoundPlayer 0x10, CTCCreatureModeManager 0x31, CTCSpecialAbilities 0xA7,
//   CTCBattleCharge 0xA8.
// Layout: Ego_r.pdb CTCBattleCharge (PDB less 4 after the 12-byte PeopleHit vector, as the landed CTCBattleCharge rows);
//   CThing Type +0x10, UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0. No engine header for CTCBattleCharge.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"


enum ETCInterfaceType { };                                  // values used as casts (0x10, 0x31, 0xA7, 0xA8)
enum EHeroAbility { };                                      // 12 = battle charge (UseSpecialAbility jump table)
class CTCBase;
class CPlayer { public:
    void SpecialAbilityUnfreezeControls();                  // 0x00487150
};
class CCharString { public:
    void* PStringData;                                      // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CThing { public:                                      // partial view, retail offsets
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      _pad_0x00[0x10];                     // CBaseObject / CClass part
    unsigned char      Type;                                // +0x10 CClass::Type
    unsigned char      _pad_0x11[0x20 - 0x11];
    std::bitset<274>   UsingComponents;                     // +0x20
    TCMap              Components;                          // +0x44
    unsigned char      _pad_0x54[0x91 - 0x54];
    bool               Dead : 1;                            // +0x91 bit 0
    bool IsDead() const { return Dead; }
    CPlayer& GetPlayer();                                   // 0x004C7A30
    void RemoveTC(ETCInterfaceType type);                   // 0x004C9840
    template <class T> FABLE_FORCEINLINE bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->end() || T::INTERFACE_TYPE < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public:
    void FinishCurrentAction();                             // 0x00662650
};
class CThingAICreature : public CThingCreatureBase { public:
    void SuspendAI(bool suspend, void* suspender);          // 0x00833A10
};

class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};
class CTCSoundPlayer : public CTCBase { public:            // interface type 0x10
    enum { INTERFACE_TYPE = 0x10 };                         // GetInterfaceType 0x0066C290 (vtable 0x0125B1C4 slot 21)
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);   // 0x0066BD00
};
class CTCCreatureModeManager : public CTCBase { public:    // interface type 0x31
    enum { INTERFACE_TYPE = 0x31 };                         // GetInterfaceType 0x00744270 (vtable 0x01268854 slot 21)
    void RemoveMode(unsigned long mode);                    // 0x0073ABA0
};
class CTCSpecialAbilities : public CTCBase { public:      // interface type 0xA7
    enum { INTERFACE_TYPE = 0xA7 };                         // EXTRA_RULES interface table
    bool                            AbilityMutexLock;       // +0x0C
    bool                            UsingBed;               // +0x0D
    EHeroAbility                    CurrentAbility;         // +0x10
};

class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CDefPointeeBase;
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;                                // +0x00
};
#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)

class CSpecialAbilitiesBattleChargeDef;
class CTCBattleCharge : public CTCBase { public:           // vtable 0x0126CBFC, interface type 0xA8
    CIntelligentPointer<CThing>                         PTarget;                    // +0x0C
    CDefPointer<const CSpecialAbilitiesBattleChargeDef> PDef;                       // +0x14
    C3DVector                                           PositionStartedFrom;        // +0x18
    CIntelligentPointer<CThing>                         CollidedThing;              // +0x24
    bool                                                StartedMoving;              // +0x2C
    bool                                                JustReleased;               // +0x2D
    bool                                                DirectionOverride;          // +0x2E
    C3DVector                                           DirectionOverrideVector;    // +0x2F
    float                                               DashDistance;               // +0x3C
    long                                                SpellLevel;                 // +0x40
    CIntelligentPointer<CThing>                         PItemRemoved;               // +0x44
    float                                               RadiusOfEffect;             // +0x4C
    CIntelligentPointer<CThing>                         ShieldEffect;               // +0x50
    CIntelligentPointer<CThing>                         LeftFeetEffect;             // +0x58
    CIntelligentPointer<CThing>                         RightFeetEffect;            // +0x60
    long                                                PoseDelay;                  // +0x68
    bool                                                StartPoseDelay;             // +0x6C
    bool                                                HitSomething;               // +0x6D
    std::vector<CIntelligentPointer<CThing> >           PeopleHit;                  // +0x70
    C3DVector                                           DisplacementVector;         // +0x7C
    bool                                                IsZTargetting;              // +0x88
    bool                                                FakeCollideWithSomething;   // +0x89
    long                                                ChargeDelayFrameCount;      // +0x8C
    bool                                                CompletedExitModes;         // +0x90
    virtual ~CTCBattleCharge();                             // 0x007A73F0
    virtual void FrameUpdate();
private:
    void CreateStartUpEffect();                             // 0x007A76F0
    void RushForwards();                                    // 0x007A8700
};

void CTCBattleCharge::FrameUpdate()
{
    CThing* parent = ParentThing;
    if (JustReleased) {
        static_cast<CThingCreatureBase*>(parent)->FinishCurrentAction();
        CTCSoundPlayer* soundPlayer;
        if (parent->GetTC(&soundPlayer)) {
            switch (SpellLevel) {
            case 0:
                soundPlayer->PlayAttachedCriteria("HERO_ABILITY_BATTLE_CHARGE_CAST_NO_POWERUP", true, false, false, -1);
                break;
            case 1:
                soundPlayer->PlayAttachedCriteria("HERO_ABILITY_BATTLE_CHARGE_CAST_LEVEL_1", true, false, false, -1);
                break;
            case 2:
                soundPlayer->PlayAttachedCriteria("HERO_ABILITY_BATTLE_CHARGE_CAST_LEVEL_2", true, false, false, -1);
                break;
            case 3:
                soundPlayer->PlayAttachedCriteria("HERO_ABILITY_BATTLE_CHARGE_CAST_LEVEL_3", true, false, false, -1);
                break;
            }
        }
        JustReleased = false;
        CreateStartUpEffect();
    } else if (StartPoseDelay) {
        --PoseDelay;
    } else {
        RushForwards();
    }
    if (PoseDelay <= 0) {
        if (!CompletedExitModes) {
            CTCSpecialAbilities* abilities;
            if (parent->GetTC(&abilities)) {
                if (abilities->CurrentAbility == 12) {
                    abilities->AbilityMutexLock = false;
                    abilities->CurrentAbility = (EHeroAbility)0;
                }
            }
            if (!parent->IsDead() && parent->Type == 1)
                static_cast<CThingAICreature*>(parent)->SuspendAI(false, 0);
            CTCCreatureModeManager* modeManager;
            if (parent->GetTC(&modeManager))
                modeManager->RemoveMode(0x23);
            if (!parent->IsDead() && parent->Type == 2)
                parent->GetPlayer().SpecialAbilityUnfreezeControls();
            CompletedExitModes = true;
        }
        --ChargeDelayFrameCount;
        if (parent->Type == 1 || ChargeDelayFrameCount <= 0)
            parent->RemoveTC((ETCInterfaceType)0xA8);
    }
}