// CTCCombat::IsMeleeFlourishValid() const -- a flourish is possible when the creature lacks interface 0x84, its interface-8 component has a live target that is vulnerable to the creature's melee strikes, an ability is selected and the usable-ability list is not empty
//
// Retail 0x00733EE0, 129 bytes (plain `ret`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0, the interface-8 component's virtual slot 33 (+0x84, returns the target CThing*),
// IsThingVulnerableToMeleeStrikesFromThing(CThing*, CThing*) @ 0x007F59F0 (`__fastcall`).
// Layout: retail CTCCombat with 12-byte STLport vectors (engine/CTCCombat.h is the PDB's 16-byte
// layout, so a local model is used): AbilityList +0x2c, UsableAbilityList {first +0x38, last +0x3c},
// CurrentAbilityIndex +0x44 (PDB +0x4c); ParentThing +0x04.
// Shape: one `if (... && ...) return true; return false;` chain (separate `mov al,1` exit, shared
// `xor al,al`); interface 0x84 is a byte test of bitset word 4 (`test byte [eax+30h],10h`), the
// interface-8 component comes from the out-parameter GetInterface with an uninitialised out pointer
// (whole-word bit test `mov ecx,[eax+20h] ; test ch,1`); the target passes the inline IsThingAlive.
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;                   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                             // +0x91 (bit 0 = dead)
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
};

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(CThing* target, CThing* attacker);   // 0x007F59F0

struct CTCTargeting {                                       // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTarget();                            // slot 33 (+0x84)
};

class CTCCombat { public:
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x2c - 0x08];
    void*         AbilityList[3];                           // +0x2c vector<CDefPointer<const CMeleeCombatAbilityDef>>
    void**        UsableAbilityList_first;                  // +0x38 vector<CDefPointer<const CMeleeCombatAbilityDef>>
    void**        UsableAbilityList_last;                   // +0x3c
    void**        UsableAbilityList_end;                    // +0x40
    long          CurrentAbilityIndex;                      // +0x44
    bool IsMeleeFlourishValid() const;
};

bool CTCCombat::IsMeleeFlourishValid() const
{
    if (!ParentThing->HasInterface(0x84))
    {
        CTCTargeting* targeting;
        ParentThing->GetInterface(&targeting, 8);
        CThing* target = targeting->GetTarget();
        if (IsThingAlive(target) && IsThingVulnerableToMeleeStrikesFromThing(target, ParentThing)
            && CurrentAbilityIndex >= 0 && UsableAbilityList_first != UsableAbilityList_last)
            return true;
    }
    return false;
}