// NCreatureMode::CModeRockTrollVulnerable::Recover() -- give the troll its recover action, resume a player-type creature's animation (plus the +0x1a8 follow-up), then take the creature's mode manager out of mode 0x3a
// Grade: DIFFER(160v160) -- retail reads PCreature into eax for the GetInterface test (`mov eax,[esi+2Ch] ; test [eax+24h] ; lea esi,[eax+44h]`), the lane reuses esi (`mov esi,[esi+2Ch] ; ... ; add esi,44h`)
// Leaf: Recover
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DE700, 160 bytes (`ret`, both GetInterface arms duplicated).
// Callees: the 0x114-byte recover action's ctor(CThingCreatureBase*) @ 0x008E4490 (catalog "CCreatureAction_BattleChargeTurnLeft"
// is propagated; name descriptive) and its out-of-line dtor @ 0x0062E900 (catalog "~CTCLightningOrb", propagated),
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, CThingAnimatable::SetSuspendAnimation
// @ 0x00833A10, the creature's +0x1a8 forwarder @ 0x006D5AB0 (`mov ecx,[ecx+1A8h] ; jmp`, catalog label propagated),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x31 = CTCCreatureModeManager), RemoveMode @ 0x0073ABA0.
// Layout: Ego_r.pdb CModeRockTrollVulnerable: RecoveryCount +0x1c, Filter +0x20, PCreature +0x2c, bRecovering +0x30.
// Shape: the action is a temporary handed by address to SetCurrentAction (the ctor's returned `this` is pushed) and
// destroyed right after; the frame word below it is the lookup key / out pointer; the GetInterface result is ignored,
// so VC duplicates the RemoveMode call into both arms (the absent arm reads the unset out pointer).
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CCreatureActionBase;
struct CThingCreatureBase {
    unsigned char _pad00[0x10];
    unsigned char m_type;                   // +0x10 (1 = player creature)
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
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
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
    void SetSuspendAnimation(bool suspend, long reason);  // 0x00833A10
    void ResumeAnimFollowUp();                            // 0x006D5AB0 (forwards to the +0x1a8 member)
};
struct CTCCreatureModeManager {             // interface type 0x31
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

struct CRockTrollRecoverAction {            // retail 0x114 bytes (a CCombatActionBase-sized action)
    unsigned char _body[0x114];
    CRockTrollRecoverAction(CThingCreatureBase* creature);   // 0x008E4490
    ~CRockTrollRecoverAction();                              // 0x0062E900
    operator CCreatureActionBase*() { return (CCreatureActionBase*)this; }
};

namespace NCreatureMode {

struct CModeBase {
    virtual void v00();
    unsigned char _body[0x18];
};

struct CModeRockTrollVulnerable : CModeBase {
    long                RecoveryCount;      // +0x1c
    unsigned char       _filter[0xc];       // +0x20 CAnimFilterOfGroup Filter
    CThingCreatureBase* PCreature;          // +0x2c
    bool                bRecovering;        // +0x30
    void Recover();
};

void CModeRockTrollVulnerable::Recover()
{
    PCreature->SetCurrentAction(CRockTrollRecoverAction(PCreature));
    CThingCreatureBase* creature = PCreature;
    if (creature->m_type == 1) {
        creature->SetSuspendAnimation(false, 0);
        creature->ResumeAnimFollowUp();
    }
    CTCCreatureModeManager* modes;
    PCreature->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x3a);
}

}
