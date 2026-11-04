// CCreatureAction_TentacleStrikeLoop::FinishAction() -- base FinishAction; keep looping (a new TentacleStrikeLoop with the same start tick) until interrupted or the tentacle def's smack-down time has elapsed, then switch to TentacleStrikeOutOf
// Leaf: ?FinishAction@CCreatureAction_TentacleStrikeLoop@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CF7D0, 287 bytes, virtual FinishAction (slot 4), plain `ret`.
// Callees: CCombatActionBase::FinishAction 0x00859A20 (direct, first), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CVectorMap::LowerBound 0x0040F020 (the unchecked GetInterface 0xE3 = CTCKrakenTentacle, bit 3 of +0x3c), CWorld::GetFrame
//   0x0049D870 (static __fastcall), _ftol2 0x00BFEA70, CCreatureAction_TentacleStrikeLoop(CThingCreatureBase*, long) 0x008CF990,
//   CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030 (the inline TentacleStrikeOutOf ctor, action 250,
//   vtable 0x0128503C), CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900.
//   Global: CMainGameComponent::ConstantFPS 0x01375550 (protected static long, read through the inline GetConstantFPS()).
// Layout: Ego_r.pdb CTCKrakenTentacle::PTentacleDef +0x0c (CDefPointer<const CKrakenTentacleDef>), CKrakenTentacleDef::
//   TentacleSmackDownTime +0x28; StartTick / Interrupt / StartHealth retail +0x114 / +0x118 / +0x11c (PDB +0x12c / +0x130 / +0x134).
// Shape: the tentacle comes from the unchecked out-parameter lookup in an inline accessor (as the landed FrameUpdate 0x008CF460);
//   the limit is written `(long)(def->TentacleSmackDownTime * GetConstantFPS())`: the fps-first spelling keeps the int-to-float
//   spill in its own slot and costs the frame 4 bytes (the lookup key no longer packs with the CSoundPair temporary).
class CTCBase;
enum ETCInterfaceType { };
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
};
class CKrakenTentacleDef { public:
    unsigned char _pad00[0x28];
    float TentacleSmackDownTime;                // +0x28
};
template <class T> class CDefPointer { public:
    T* PDef;
    T* operator->() const { return PDef; }
};
class CTCKrakenTentacle { public:
    unsigned char _pad00[0x0c];
    CDefPointer<const CKrakenTentacleDef> PTentacleDef;   // +0x0c
};
class CMainGameComponent { public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }
protected:
    static long ConstantFPS;                    // 0x01375550
};
class CWorld { public:
    static long __fastcall GetFrame();          // 0x0049D870
};
class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0x114 - 0x10];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    virtual ~CCombatActionBase();               // 0x0062E900
    virtual void FinishAction();                // 0x00859A20 (virtual slot 4)
};
class CCreatureAction_TentacleStrikeOutOf : public CCombatActionBase { public:   // vtable 0x0128503C
    virtual void v00();
    CCreatureAction_TentacleStrikeOutOf(CThingCreatureBase* owner) : CCombatActionBase(owner, 250, CSoundPair()) { }
};
class CCreatureAction_TentacleStrikeLoop : public CCombatActionBase { public:   // vtable 0x01284EF4
    long  StartTick;                            // +0x114
    bool  Interrupt;                            // +0x118
    float StartHealth;                          // +0x11c
    virtual void v00();
    CCreatureAction_TentacleStrikeLoop(CThingCreatureBase* owner, long startTick);   // 0x008CF990
    virtual void FinishAction();
    __forceinline CTCKrakenTentacle* GetTentacle() const
    {
        CTCKrakenTentacle* tentacle;
        PCreature.GetPItem()->GetInterface(&tentacle, 0xe3);
        return tentacle;
    }
};

void CCreatureAction_TentacleStrikeLoop::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCKrakenTentacle* tentacle = GetTentacle();
    if (!Interrupt && CWorld::GetFrame() - StartTick <= (long)(tentacle->PTentacleDef->TentacleSmackDownTime * CMainGameComponent::GetConstantFPS()))
        PCreature.GetPItem()->SetCurrentAction(CCreatureAction_TentacleStrikeLoop(PCreature.GetPItem(), StartTick));
    else
        PCreature.GetPItem()->SetCurrentAction(CCreatureAction_TentacleStrikeOutOf(PCreature.GetPItem()));
}