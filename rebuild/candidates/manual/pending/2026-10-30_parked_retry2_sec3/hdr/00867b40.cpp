// CCombatAction_GenericStrikeResponseKnockdown::FinishAction() -- base FinishAction; keep FlipFacing only if the
// Grade: DIFFER(465v465) -- ternary GetInterface store (size now equal) + engine/CCountedPointer.h for the animation stats (byte-neutral); left: retail keeps the 0x31 mode manager in its key-shared slot and the animation index in edi
// animation's stats do not veto it and the creature is not in mode 0x1D; drop mode 0x1D; then, with (almost) no hit
// points left, either be stabbed to death (when the def allows it) or die outright, otherwise be stunned (when the
// knockdown turns into a stun) or get up
// Leaf: ?FinishAction@CCombatAction_GenericStrikeResponseKnockdown
#include "engine/CCountedPointer.h"
// Tried: chained stats lookup (a), named info local (b), modes declared first (c), temporaries sized to the 0x118 slot (d, this).
// Layout: retail CCombatActionBase is 0x114 (PDB 0x12c): FlipFacing +0x13a (PDB +0x152); creature hit points +0xb4;
//   CThing interface bitset +0x20, map +0x44, def +0x70 (def flag +0x148 bit 0).
void __cdecl operator delete(void* block);            // 0x00BFE9BC
extern const float g_fEpsilon;                        // 0x0129BA3C (0.0001)
struct CTCBase;
struct C3DAnimationStats {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool IsVetoingFlip() const;               // slot 4
};
struct C3DAnimationInfo {
    CCountedPointer<const C3DAnimationStats> GetAnimationStats();   // 0x00A269C0
};
struct CAnimationBank {
    C3DAnimationInfo* GetAnimation(long animation);   // 0x00A242C0
};
struct CEngineView {
    unsigned char _pad[0x3c];
    CAnimationBank* AnimationBank;                    // +0x3c
};
struct CTCCreatureModeManager {                       // interface 0x31
    bool HasMode(unsigned long mode);                 // 0x0073A8A0
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CCreatureDefView {
    unsigned char _pad[0x148];
    bool CanBeStabbedToDeath;                         // +0x148 bit 0
};
struct CCreatureActionBase;
struct CThingCreatureBase {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CCreatureDefView* m_pDef;                         // +0x70
    unsigned char _pad74[0xb4 - 0x74];
    float HitPoints;                                  // +0xb4
    void Kill(bool b);                                // 0x004C9B80
    bool IsKnockdownToStun() const;                   // 0x00865AC0
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThingCreatureBase* GetPItem() const;             // 0x00A01B50
};
struct CCreatureActionBase {
    virtual ~CCreatureActionBase();                   // 0x00693EF0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual long GetAnimationIndex() const;           // slot 7
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CThingCreatureBase* GetCreature() const { return PCreature.GetPItem(); }
    CEngineView* GetPEngine() const;                  // 0x00692950
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _pada8[0x114 - 0xa8];
    ~CCombatActionBase();                             // 0x0062E900
    void FinishAction();                              // 0x00859A20
};
struct CCreatureAction_StabbedToDeath : CCreatureActionBase {
    unsigned char _padx[0x18];
    CCreatureAction_StabbedToDeath(CThingCreatureBase* creature, bool flip);   // 0x008ED650
};
struct CCreatureAction_BeStunned : CCreatureActionBase {
    unsigned char _padx[0x18];
    CCreatureAction_BeStunned(CThingCreatureBase* creature, bool flip);        // 0x00865940
};
struct CCombatAction_KnockdownGetUp : CCombatActionBase {
    unsigned char _padx[4];
    CCombatAction_KnockdownGetUp(CThingCreatureBase* creature, bool flip);     // 0x007F05C0 (manifest: CDrawAutoPickups)
};
struct CCombatAction_GenericStrikeResponseKnockdown : CCombatActionBase {
    unsigned char _pad114[0x13a - 0x114];
    bool FlipFacing;                                  // +0x13a
    void FinishAction();
};

void CCombatAction_GenericStrikeResponseKnockdown::FinishAction()
{
    CCombatActionBase::FinishAction();
    long animation = GetAnimationIndex();
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    bool flip = FlipFacing;
    if (flip) {
        if (animation) {
            C3DAnimationInfo* info = GetPEngine()->AnimationBank->GetAnimation(animation);
            CCountedPointer<const C3DAnimationStats> stats = info->GetAnimationStats();
            if (stats->IsVetoingFlip())
                flip = false;
        }
        if (flip && modes->HasMode(0x1d))
            flip = false;
    }
    modes->RemoveMode(0x1d);
    if (PCreature.GetPItem()->HitPoints < g_fEpsilon) {
        if (PCreature.GetPItem()->m_pDef->CanBeStabbedToDeath)
            GetCreature()->SetCurrentAction(CCreatureAction_StabbedToDeath(PCreature.GetPItem(), flip));
        else
            PCreature.GetPItem()->Kill(true);
    } else if (PCreature.GetPItem()->IsKnockdownToStun())
        GetCreature()->SetCurrentAction(CCreatureAction_BeStunned(PCreature.GetPItem(), flip));
    else
        GetCreature()->SetCurrentAction(CCombatAction_KnockdownGetUp(PCreature.GetPItem(), flip));
}
