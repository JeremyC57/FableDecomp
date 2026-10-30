// CCreatureAction_KrakenHighAttackLoop::FinishAction() -- creature-action FinishAction; while the target is alive and loops remain, run the next high attack loop (an early finish jumps to the last two loops and marks it interrupted; entering them ends the beam and stops head tracking), otherwise the high attack "out of" action
// Leaf: ?FinishAction@CCreatureAction_KrakenHighAttackLoop
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): IsTargetAlive's dead test through CThing::IsDead() const (Ego_r.pdb
//   ?IsDead@CThing@@QBE_NXZ, the bool keeps retail's mov cl,1 ; test cl,dl) plus a same-TU noinline
//   CInterfaceMap::LowerBound stand-in (GOTCHAS chunk 240) that lets the kraken share the 0xE2 key word (sub esp,120h).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924930, 344 bytes (+ int3 padding), vtable slot 4 of 0x0129107C (RTTI
//   `?A0x29922355::CCreatureAction_KrakenHighAttackLoop`; FableWin.pdb
//   ?FinishAction@CCreatureAction_KrakenHighAttackLoop@?A0xe2035276@@UAEXXZ). Catalog "vfunc_4".
// Callees: CCreatureActionBase::FinishAction @ 0x00694430 (first: the combat base's override is skipped), GetPItem
//   @ 0x00A01B50, LowerBound @ 0x0040F020 (the inline unchecked GetKraken(), interface 0xe2; the kraken is homed in the key
//   word), CTCKraken::EndAttackBeam @ 0x007FC800, CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase*, CThing*, long,
//   bool) @ 0x00925780, CCreatureAction_KrakenHighAttackOutOf(CThingCreatureBase*) @ 0x00925820 (landed),
//   CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Layout: Ego_r.pdb CCreatureActionBase TotalTime +0x10 / FramesRunning +0x1C; the landed FrameUpdate 0x009242D0
//   (Interrupt +0x114, LoopNum +0x118); CKrakenDef HighStrikeAttackLoops +0x44; CTCKraken HeadTrackTarget +0x51 (PDB +0x61).
// Shape: the loop limit is read into a local before the alive test; the target is fetched into a local before the
//   SetCurrentAction call (retail's fetch order), as the twin 0x00924720.
class CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
// Same-TU stand-in for CVectorMap::LowerBound (not the retail body; GOTCHAS chunk 240 / 008328d0 idiom).
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) 
{
    CInterfaceMap::SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    unsigned char _pad92[0xb4 - 0x92];
    float         m_health;                     // +0xb4
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
class CThingCreatureBase;
class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CKrakenDef { public:
    unsigned char _pad00[0x44];
    long          HighStrikeAttackLoops;        // +0x44 (Ego_r.pdb)
};
class CTCKraken { public:                              // interface type 0xe2 component
    unsigned char     _pad00[0x0c];
    const CKrakenDef* Def;                      // +0x0C
    unsigned char     _pad10[0x51 - 0x10];
    bool              HeadTrackTarget;          // +0x51 (PDB +0x61)
    void EndAttackBeam();                       // 0x007FC800
};

class CCreatureActionBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void FinishAction();                // 0x00694430 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    long                    TotalTime;          // +0x10
    long                    ActionTime;         // +0x14
    long                    AnimDelayTime;      // +0x18
    long                    FramesRunning;      // +0x1C
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    virtual void FinishAction();                // 0x00859A20
    unsigned char           _pad20[0xa8 - 0x20];
    CBaseIntelligentPointer PTarget;            // +0xa8 CIntelligentPointer<CThing>
    unsigned char           _padb0[0x114 - 0xb0];
    ~CCombatActionBase();                       // 0x0062E900
    __forceinline CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.GetPItem()); }
    __forceinline bool IsTargetAlive() const    // two fetches: present, then not dead
    {
        return PTarget.GetPItem() && !PTarget.GetPItem()->IsDead();
    }
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetInterface(&kraken, 0xe2);
        return kraken;
    }
};
struct CCreatureAction_KrakenHighAttackOutOf : CCombatActionBase {   // vtable 0x01291124
    CCreatureAction_KrakenHighAttackOutOf(CThingCreatureBase* owner);   // 0x00925820
};
struct CCreatureAction_KrakenHighAttackLoop : CCombatActionBase {   // vtable 0x0129107C
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase* owner, CThing* thing, long loopNum, bool interrupt);   // 0x00925780
    virtual void FinishAction();
};

void CCreatureAction_KrakenHighAttackLoop::FinishAction()
{
    CCreatureActionBase::FinishAction();
    CTCKraken* kraken = GetKraken();
    long loops = kraken->Def->HighStrikeAttackLoops;
    if (IsTargetAlive() && ++LoopNum < loops) {
        if (FramesRunning < TotalTime) {
            LoopNum = loops - 2;
            Interrupt = true;
        }
        if (LoopNum == loops - 2) {
            kraken->EndAttackBeam();
            kraken->HeadTrackTarget = false;
        }
        CThing* target = PTarget.GetPItem();
        Creature().SetCurrentAction(CCreatureAction_KrakenHighAttackLoop(&Creature(), target, LoopNum, Interrupt));
    } else
        Creature().SetCurrentAction(CCreatureAction_KrakenHighAttackOutOf(&Creature()));
}