// CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath::FinishAction() -- when the recovering creature is the hero, give it tutorial category 0x31, then finish as the knockdown-recover base
// Leaf: ?FinishAction@CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00866FB0, 83 bytes, thiscall `ret`.
// Identity (proven): the body sits in the FromDeath vtable 0x01271A74 at the FinishAction slot; FableWin.pdb
//   ?FinishAction@CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath@@UAEXXZ, and it calls the base's
//   ?FinishAction@CCombatAction_GenericStrikeResponseKnockdownRecoverBase@@UAEXXZ 0x00866E80 non-virtually.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (Ego_r.pdb CCreatureActionBase::PCreature
//   CIntelligentPointer<CThingCreatureBase> +8), CInterfaceMap::LowerBound 0x0040F020 (GetTC, interface 0x29 = CTCHero),
//   CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) 0x006C9FA0 (FableWin.pdb), the base FinishAction 0x00866E80.
// Shape: GetTC out-pointer form (the old park's direct-pointer helper loaded the hero straight into ecx).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        CInterfaceMap* map = &Components;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || T::INTERFACE_TYPE < it->first)
            it = map->end();
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || T::INTERFACE_TYPE < it->first)
            it = map->end();
        *out = (const T*)it->second;
        return true;
    }
};
enum ETutorialCategory { ETUTORIAL_CATEGORY_PLACEHOLDER = 0 };
class CTCHero {
public:
    enum { INTERFACE_TYPE = 0x29 };
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x006C9FA0
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject { };
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* PItem;                                  // +0x04
    CBaseObject* GetPItem() const;                       // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    virtual void FinishAction();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;  // +0x08
};
class CCombatActionBase : public CCreatureActionBase { };
class CCombatAction_GenericStrikeResponseKnockdownRecoverBase : public CCombatActionBase {
public:
    virtual void FinishAction();                         // 0x00866E80
};
class CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath : public CCombatAction_GenericStrikeResponseKnockdownRecoverBase {
public:
    virtual void FinishAction();
};
void CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath::FinishAction()
{
    CTCHero* hero;
    if (PCreature.GetPItem()->GetTC(&hero))
        hero->GiveTutorialForCategory((ETutorialCategory)0x31, false);
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase::FinishAction();
}