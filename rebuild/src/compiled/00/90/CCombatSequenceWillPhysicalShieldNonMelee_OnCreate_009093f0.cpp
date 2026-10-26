// CCombatSequence_WillPhysicalShieldNonMelee::OnCreate -- enqueue the buildup animation and physical shield spell.
#define CCombatSequence_WillPhysicalShieldNonMelee CCombatSequence_WillPhysicalShieldNonMelee_PDB
#include "engine/CCombatSequence_WillPhysicalShieldNonMelee.h"
#undef CCombatSequence_WillPhysicalShieldNonMelee
#define CCreatureAction_PlayCombatAnimation CCreatureAction_PlayCombatAnimation_PDB
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#undef CCreatureAction_PlayCombatAnimation
#define CCreatureAction_CastPhysicalShieldSpell CCreatureAction_CastPhysicalShieldSpell_PDB
#include "engine/CCreatureAction_CastPhysicalShieldSpell.h"
#undef CCreatureAction_CastPhysicalShieldSpell
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#include "fable_string.h"
// PDB8F0D10 PlayCombatAnimation takes two Creature references;8F3C30 shield takes Creature reference.
// Field-free actual string API adapter restores the genuine implicit literal conversion and default length.
struct CRetailString:CCharString {CRetailString(const char*,long=-1);}; //99EBF0, implicit actual base dtor99EAE0
class CThingCreatureBase;
class CThing;
class CCreatureActionBase { public: unsigned char nativeStorage[0xa8]; };
class CCombatActionBase : public CCreatureActionBase { public: unsigned char nativeTail[0x6c]; };
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:
    bool FaceTarget;
    bool UsePhysics;
    bool AlwaysUseMovementInAnim;
    unsigned char padding117;
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase&, CThingCreatureBase&, const CRetailString&, bool); // 0x008B7F80
};
class CCreatureAction_CastPhysicalShieldSpell : public CCombatActionBase { public:
    bool ImmediateCast;                 // native +0x114
    bool OneAnimCast;                    // native +0x115
    unsigned char padding116[2];
    long LevelToCast;                    // native +0x118
    long TimeLimit;                      // native +0x11c
    float ShieldHealth;                  // native +0x120
    CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase&, long, bool, const CRetailString&, long, float); // 0x008BB6F0
};
struct SCountBlock { long refs; void (FABLE_FASTCALL* destroy)(void*); void* object; };
template<class T> class CCountedPointer { public:
    T* object;
    SCountBlock* count;
    CCountedPointer() : object(0), count(0) {}
    CCountedPointer& operator=(T* p){Reset(p);return *this;}
    void Reset(T*); // 0x00905FD0
    ~CCountedPointer() {
        if (count) {
            --count->refs;
            if (!count->refs) { count->destroy(count->object); delete count; }
        }
        object = 0;
        count = 0;
    }
};
extern long g_01375550; // native frames-per-second global at 0x01375550
struct CCombatSequence_WillPhysicalShieldNonMelee {
    unsigned char nativePrefix[0x1c];
    CThingCreatureBase* ownerAt1c;
    CThingCreatureBase* targetAt20;
    CThingCreatureBase& GetCreature()const{return *ownerAt1c;}
    CThingCreatureBase& GetTarget()const{return *targetAt20;}
    void AddAction(const CCountedPointer<CCreatureActionBase>&); // 0x008B0B10
    void OnCreate();
    void OnCreateBase(); // 0x008B0650
};
void CCombatSequence_WillPhysicalShieldNonMelee::OnCreate() {
    long timeLimit = g_01375550 * 0xf;
    CCountedPointer<CCreatureActionBase> action;
    action=new CCreatureAction_PlayCombatAnimation(GetCreature(),GetTarget(),"ST_BUILDUP_PHYSICAL_SHIELD",true);
    AddAction(action);
    action=new CCreatureAction_CastPhysicalShieldSpell(GetCreature(),2,true,"",timeLimit,0.0f);
    AddAction(action);
    OnCreateBase();
}