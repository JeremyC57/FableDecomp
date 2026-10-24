// NCreatureMode::CModeGuildmasterPowerUpHeal::FrameUpdate() -- base update; when the heal timer runs out or the boss has no living helper creatures left, set the "heal followers release" combat action
// Leaf: ?FrameUpdate@CModeGuildmasterPowerUpHeal
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DFFD0, 140 bytes (+ int3 padding), `ret`; vtable 0x012685BC slot 3.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50 (qualified), CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter
//   GetInterface, type 0xad = bit 13 of the +0x34 dword = CTCBoss), CTCBoss::CountLivingHelperCreatures @ 0x007AEEC0 (landed;
//   manifest "CountEffects" is propagated), CCombatAction_Guildmaster_HealFollowersRelease(CThingCreatureBase*) @ 0x008E4860
//   (the manifest label "...HealFollowersInto" is propagated: the ctor stores vtable 0x012869BC =
//   CCombatAction_Guildmaster_HealFollowersRelease), CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase
//   @ 0x0062E900.
// Name: FableWin.pdb `?FrameUpdate@CModeGuildmasterPowerUpHeal@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: Ego_r.pdb CModeGuildmasterPowerUpHeal: CModeStandBase 0x68, Filter +0x68, Timer +0x74.
// Shape: the decremented timer is a named local (kept in ebx across the lookup call); the CTCBoss lookup result is not
//   tested (the absent path joins with the never-written out variable); `||` short-circuits the helper count.
struct CCreatureActionBase;
struct CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CThingCreatureBase {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
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
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CTCBoss {                           // interface type 0xad
    long CountLivingHelperCreatures() const;   // 0x007AEEC0
};

struct CCreatureActionBase {               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    unsigned char _combat[0x6c];
    ~CCombatActionBase();                  // 0x0062E900
};
struct CCombatAction_Guildmaster_HealFollowersRelease : CCombatActionBase {
    CCombatAction_Guildmaster_HealFollowersRelease(CThingCreatureBase* owner);   // 0x008E4860
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
    void FrameUpdate();                    // 0x0073AC50
};
struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {
struct CModeGuildmasterPowerUpHeal : CModeStandBase {   // vtable 0x012685BC
    unsigned char Filter[0x0c];            // +0x68 CAnimFilterOfGroup
    long          Timer;                   // +0x74
    void FrameUpdate();
};
}

void NCreatureMode::CModeGuildmasterPowerUpHeal::FrameUpdate()
{
    CModeBase::FrameUpdate();
    long remaining = --Timer;
    CTCBoss* boss;
    ParentThing->GetInterface(&boss, 0xad);
    if (remaining <= 0 || boss->CountLivingHelperCreatures() == 0) {
        CThingCreatureBase* creature = ParentThing;
        creature->SetCurrentAction(CCombatAction_Guildmaster_HealFollowersRelease(creature));
    }
}