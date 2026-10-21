// CAIStateGroup_HoverHornetQueenTakeDamage::Update() -- advance the boss phase when ready; on a range attack hit within 90 frames of the last one, count it, and on the third register the target with the combat manager, mark the queen ready to attack and perform the dragon roar; terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095B280, length 352 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, 0xad = CTCBoss, bit 13 of +0x34),
//          CTCBoss::IsReadyForPhaseChange @ 0x007AEE90 / IncrementBossPhase @ 0x007AED20, CAIStateGroupBase::GetPWorld
//          @ 0x008FCFC0, CWorld::GetWorldFrame @ 0x0049D870, CThingCreatureBase::GetCombatManager @ 0x00661ED0 /
//          IsCombatant @ 0x00662410, CCombatManager::RegisterCombatant @ 0x006EDC50 / SetAttackerAsReadyToAttack
//          @ 0x006EA450, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CCreatureAction_DragonRoar ctor @ 0x0095CAC0
//          (creature, target; ret 8), CAIStateGroupBase::AIAction_CombatPerformActionAndWait @ 0x0097DA40,
//          ~CCombatActionBase @ 0x0062E900, CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: the boss lookup is the out-parameter GetInterface (miss path reads the never-written slot, which shares the
//          key's home); the 0x114-byte action temporary fills the rest of the 0x118 frame.
// Layout: engine/CAIStateGroup_HoverHornetQueenTakeDamage.h (bIsRangeAttack +0x1d, RangeAttackCount +0x20,
//          FrameOfLastHit +0x24); PTarget (intelligent pointer) at +0x14 as in the landed IsRunnable 0x0095AA60.
#include "engine/CAIStateGroup_HoverHornetQueenTakeDamage.h"

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CCombatManager;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    CCombatManager* GetCombatManager();         // 0x00661ED0
    bool IsCombatant();                         // 0x00662410
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
struct CCombatManager {
    void RegisterCombatant(CThing* creature, CThing* target, bool flag);   // 0x006EDC50
    void SetAttackerAsReadyToAttack(CThing* creature, bool ready);         // 0x006EA450
};
struct CTCBoss {                                // interface 0xad
    bool IsReadyForPhaseChange();               // 0x007AEE90
    void IncrementBossPhase();                  // 0x007AED20
};
struct CWorld {
    unsigned char _pad[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
    static long GetWorldFrame();                // 0x0049D870 (static: the world pointer is evaluated and dropped)
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CCreatureActionBase {
    void* __vftable;
    unsigned char _body[0xa4];                  // +0x04..+0xa7
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x6c];                // +0xa8..+0x113
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_DragonRoar : CCombatActionBase {
    CCreatureAction_DragonRoar(CThing* creature, CThing* target);   // 0x0095CAC0
};

struct CAIStateGroupHoverHornetQueenTakeDamageMethods : CAIStateGroup_HoverHornetQueenTakeDamage {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& PTarget() { return *(CBaseIntelligentPointer*)_pad_0x14; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DA40
    void Terminate();                           // 0x008FCF40
    void Update();
};

void CAIStateGroupHoverHornetQueenTakeDamageMethods::Update()
{
    CTCBoss* boss;
    GetCreature()->GetInterface(&boss, 0xad);
    if (boss->IsReadyForPhaseChange())
        boss->IncrementBossPhase();
    if (bIsRangeAttack) {
        long elapsed = GetPWorld()->GetWorldFrame() - FrameOfLastHit;
        FrameOfLastHit = GetPWorld()->GetWorldFrame();
        if (elapsed < 90) {
            if (++RangeAttackCount >= 3) {
                CCombatManager* combat = GetCreature()->GetCombatManager();
                if (!GetCreature()->IsCombatant()) {
                    CThing* creature = GetCreature();
                    combat->RegisterCombatant(creature, PTarget().GetPItem(), true);
                }
                if (GetCreature()->IsCombatant()) {
                    CThing* creature = GetCreature();
                    CWorld* world = GetPWorld();
                    world->PCombatManager->SetAttackerAsReadyToAttack(creature, true);
                }
                CThing* creature = GetCreature();
                AIAction_CombatPerformActionAndWait(&CCreatureAction_DragonRoar(creature, PTarget().GetPItem()), false);
                Terminate();
                return;
            }
        } else {
            RangeAttackCount = 1;
        }
    }
    Terminate();
}
