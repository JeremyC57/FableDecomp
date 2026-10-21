// CAIStateGroup_BanditKingThrowWeapons::Update() -- wait for the bandit king to finish, throw weapons at the player, alert, and advance the boss phase (rescaling health)
// Grade: DIFFER(323v323) -- evaluation order only: VC7.1 loads the brain/creature for the throw action before the target GetPItem call (into edi) where retail loads it after (ecx), and the rotated idle-loop test uses eax/ecx instead of ecx/ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00961640, length 323 bytes.
// Callees: CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0, AIAction_DoNothing @ 0x0097E200, CBaseIntelligentPointer ctor
//          @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (the by-value CIntelligentPointer<const
//          CThing> argument, vtable 0x01237F1C), CCreatureAction_BanditKingThrowWeapons(creature, target) @ 0x008C7FD0,
//          AIAction_PerformActionTargetFaceAndWait @ 0x0097DCB0, ~CCreatureActionBase @ 0x00693EF0, CCreatureAction_AlertToPlayer
//          @ 0x008C7F60, AIAction_PerformActionAndWait @ 0x0097DB70, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface
//          0xAD = CTCBoss, bit 13 of +0x34), CTCBoss::GetBossPhase @ 0x007AED10 (manifest "CWorld::DrawGetEnvironment" is
//          propagated), CTCBoss::SetNextBossPhase(long, bool) @ 0x007AEDA0, Terminate @ 0x008FCF40.
// Constants: 0.01f @ 0x01230A5C.
// Shape: the idle loop leaves through the shared Terminate tail on any result other than 4 / 2; the target face
//          argument is the inline converting copy of Player built in its stack slot before the action temporary; the
//          boss phase's health percentage rescales the creature's health from its maximum (`fild ; fmul ; fmul`).
// Layout: engine/CAIStateGroup_BanditKingThrowWeapons.h (Player +0x14) behind engine/CAIStateGroupBase.h; creature max
//          health +0xB0, health +0xB4; CTCBoss PDef +0xC with the per-phase health percentages at def +0x28.
#include "engine/CAIStateGroup_BanditKingThrowWeapons.h"

struct CTCBase;
struct CThing;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointerConstThing(const CIntelligentPointerConstThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CTCBossDef {
    unsigned char _00[0x28];
    long* PhaseHealthPercent;                   // +0x28
};
struct CTCBoss {                                // interface 0xad
    unsigned char _00[0x0c];
    CTCBossDef* PDef;                           // +0x0c
    long GetBossPhase();                        // 0x007AED10
    void SetNextBossPhase(long phase, bool immediate);   // 0x007AEDA0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0xb0 - 0x4c];
    float MaxHealth;                            // +0xb0
    float Health;                               // +0xb4
    bool IsPerformingAnyAction();               // 0x006622A0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_BanditKingThrowWeapons : CCreatureActionBase {
    CBaseIntelligentPointer PTarget;            // +0xa8
    CCreatureAction_BanditKingThrowWeapons(CThing* creature, CThing* target);   // 0x008C7FD0
};
struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThing* creature);   // 0x008C7F60
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBanditKingThrowWeaponsMethods : CAIStateGroup_BanditKingThrowWeapons {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& PlayerPointer() { return *(CBaseIntelligentPointer*)Player; }
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_PerformActionTargetFaceAndWait(CCreatureActionBase* action, CIntelligentPointerConstThing target, bool flag);   // 0x0097DCB0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
    void Update();
};

void CAIStateGroupBanditKingThrowWeaponsMethods::Update()
{
    while (GetCreature()->IsPerformingAnyAction()) {
        long result = AIAction_DoNothing();
        if (result != 4 && result != 2) {
            Terminate();
            return;
        }
    }
    AIAction_PerformActionTargetFaceAndWait(&CCreatureAction_BanditKingThrowWeapons(GetCreature(), PlayerPointer().GetPItem()),
        PlayerPointer(), false);
    AIAction_PerformActionAndWait(&CCreatureAction_AlertToPlayer(GetCreature()), false);
    CTCBoss* boss;
    if (GetCreature()->GetInterface(&boss, 0xad)) {
        long phase = boss->GetBossPhase();
        if (boss->PDef)
            GetCreature()->Health = (float)boss->PDef->PhaseHealthPercent[phase] * GetCreature()->MaxHealth * 0.01f;
        boss->SetNextBossPhase(phase + 1, false);
    }
    Terminate();
}
