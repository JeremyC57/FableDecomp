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
#include "fable_compiler.h"
namespace NCreatureMode {
struct CTCBase;
struct CThing;
} // (hoisted to global scope: naming wave 22, the PDB CCharString)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
// CVectorMap, ETCInterfaceType and CTCBase are global (Ego_r.pdb)
class CTCBase;
enum ETCInterfaceType { };
// agentNF15: the stand-in CInterfaceMap struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x0040F020 is its LowerBound (NF14 / established spelling).
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
namespace NCreatureMode {
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || interfaceType < it->first)
                it = map->_M_finish;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    FABLE_FORCEINLINE bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->_M_finish || interfaceType < it->first) ? map->_M_finish : it)->second;
    }
    template<class T> FABLE_FORCEINLINE T* GetTCIfPresent(long interfaceType)
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
}   // namespace NCreatureMode -- CTCCreatureModeManager is a global class (Ego_r.pdb ?RemoveMode@CTCCreatureModeManager@@QAEXK@Z)
class CTCCreatureModeManager { public:          // interface type 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
namespace NCreatureMode {
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
class CModeScorpionKingVulnerable : public CModeBase { public:
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