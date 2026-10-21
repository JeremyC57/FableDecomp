// NCreatureMode::CModeScorpionKingVulnerable::Recover() -- the scorpion king tries a tail plunge at its target; on success the owner's queued animations are cleared and mode 0x37 dropped, otherwise the belly hit location is closed and the body one reopened
// Leaf: ?Recover@CModeScorpionKingVulnerable@NCreatureMode@@AAEXXZ
// Label: the catalog row "CCreatureAction_BriarRose::Execute" (forge-autoname4) is wrong: the only caller is 0x008E0D08 inside
//   NCreatureMode::CModeScorpionKingVulnerable's FrameUpdate, `this` matches Ego_r.pdb CModeScorpionKingVulnerable exactly
//   (PCreature +0x78, bRecovering +0x7c, PTarget +0x7d packed), and the class's private Recover (AAEXXZ) is the direct-call member.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DF3A0, 359 bytes (368 with padding), `ret`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CCreatureAction_ScorpionTailPlunge(CThingCreatureBase*, CThing*)
//   0x008E4640 (a 0x120-byte temporary), CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900,
//   CVectorMap::LowerBound 0x0040F020 (inlined interface lookups 0x5A / 0x31 / 0x7F), CTCAnimationComplex::GetAnimation
//   0x0070B460, CAnimateThing 0x0070D510 (clears the queues at +0x58/+0x64; catalog label _Dest_val<...> is a folded name),
//   CTCCreatureModeManager::RemoveMode 0x0073ABA0, CTCHitLocations::GetPHitLocationFromName 0x00815820
//   ("HIT_LOCATION_SCORPION_KING_BELLY" 0x01285A28, "HIT_LOCATION_SCORPION_KING" 0x01285A0C; implicit temporaries),
//   CHitLocation::SetEnabled 0x00816390.
// Shape: the landed CModeScorpionKingExhausted::Recover model (creature re-read from +0x78, explicit action temporary to
//   SetCurrentAction); each component comes from an inline accessor `T* c = 0; if (HasTC) c = GetTC(ternary)` (the landed
//   CTCBed accessor) -- the out-parameter GetInterface or a plain if/else puts the zero in eax instead of ecx.
// Layout: local model -- no generated header for CModeScorpionKingVulnerable; offsets are Ego_r.pdb's (CModeBase ParentThing +4).
namespace NCreatureMode {
struct CTCBase;
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
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
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
};
struct CCreatureActionBase { virtual void v00(); };
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
struct CCombatActionBase : CCreatureActionBase {                // retail sizeof 0x114
    unsigned char _body[0x110];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_ScorpionTailPlunge : CCombatActionBase {   // retail sizeof 0x120
    unsigned char _own[0xc];
    CCreatureAction_ScorpionTailPlunge(CThingCreatureBase* owner, CThing* target);   // 0x008E4640
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CAnimateThing {
    void ClearQueuedAnims();                    // 0x0070D510 (clears the two queues at +0x58/+0x64)
};
struct CTCAnimationComplex {                    // interface type 0x5A
    CAnimateThing& GetAnimation();              // 0x0070B460
};
struct CTCCreatureModeManager {                 // interface type 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CHitLocation {
    void SetEnabled(bool enabled);              // 0x00816390
};
struct CTCHitLocations {                        // interface type 0x7F
    CHitLocation* GetPHitLocationFromName(const CCharString& name);   // 0x00815820
};

struct CModeBase {
    virtual ~CModeBase();
    CThing* ParentThing;                        // +0x04
};
#pragma pack(push, 1)
struct CModeScorpionKingVulnerable : CModeBase {
    unsigned char _pad08[0x78 - 0x08];
    CThingCreatureBase* PCreature;              // +0x78
    bool bRecovering;                           // +0x7c
    CBaseIntelligentPointer PTarget;            // +0x7d CIntelligentPointer<CThingCreatureBase>
    void Recover();
};
#pragma pack(pop)

void CModeScorpionKingVulnerable::Recover()
{
    if (PCreature->SetCurrentAction(CCreatureAction_ScorpionTailPlunge(PCreature, PTarget.GetPItem()))) {
        ParentThing->GetTCIfPresent<CTCAnimationComplex>(0x5a)->GetAnimation().ClearQueuedAnims();
        PCreature->GetTCIfPresent<CTCCreatureModeManager>(0x31)->RemoveMode(0x37);
    }
    else {
        CTCHitLocations* hitLocations = PCreature->GetTCIfPresent<CTCHitLocations>(0x7f);
        CHitLocation* belly = hitLocations->GetPHitLocationFromName("HIT_LOCATION_SCORPION_KING_BELLY");
        CHitLocation* body = hitLocations->GetPHitLocationFromName("HIT_LOCATION_SCORPION_KING");
        belly->SetEnabled(false);
        body->SetEnabled(true);
    }
}
}