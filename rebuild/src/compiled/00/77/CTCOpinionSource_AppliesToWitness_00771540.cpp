// CTCOpinionSource::AppliesToWitness(const CThingAICreature* witness) const -- does this opinion source affect the witness: no witness never, no faction restriction always, otherwise only witnesses with an enemy component whose faction is allied with the restricting faction (inverted by RestrictToFactionIsReversed)
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00771540, 221 bytes (`ret 4`; PDB ?AppliesToWitness@CTCOpinionSource@@ABE_NPBVCThingAICreature@@@Z).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (interface 0x49 = CTCEnemy, bitset word 2 bit 9),
//   virtual CDefClassBase::GetInstantiationName() const slot 10 (+0x28, CDefString by hidden pointer), CDefStringTable::GetString
//   @ 0x009D49B0 on GDefStringTable @ 0x013CA828, CFactionManager::GetPFaction(const CCharString&) const @ 0x006B93D0 (manifest
//   "Faction::FindFactionByHash"; PDB ?GetPFaction@CFactionManager@@QBEPAVCFaction@@ABVCCharString@@@Z) through
//   CMainGameComponent::Get() @ 0x013B86A0 ->PWorld (+0x24) ->PFactionManager (+0x54), ~CCharString @ 0x0099EAE0,
//   CTCEnemy::IsAlliedWithFaction(const CFaction*) const @ 0x0076C7F0 (PDB QBE_NPBVCFaction@@@Z; the one landed caller declares
//   it with a `long` parameter).
// Layout: Ego_r.pdb CTCOpinionSource (12-byte DefStack): RestrictToFactionAndAllies +0x20, RestrictToFactionIsReversed +0x24;
//   CMainGameComponent PWorld +0x24, CWorld PFactionManager +0x54 (PDB = retail).
// Shape: the witness test is the outer `if` (its false exit at the very end); HasTC is the raw bitset test, GetTC the
//   `it == last || key < it->type` selection with the component kept in ebx; the faction name is a CCharString in its own
//   block (homed in the dead witness slot) destroyed before the null test; PWorld/PFactionManager are scoped_ptrs read through
//   their inline operator-> (the plain pointer form loads the global into ecx, retail into eax).
class CCharString { public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CDefString { public: long Index; };
class CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)                // 0x013CA828
class CFaction;
template <class T> struct scoped_ptr {                  // boost-style scoped_ptr, one pointer
    T* ptr;
    T* operator->() const { return ptr; }
};
class CFactionManager { public:
    CFaction* GetPFaction(const CCharString& name) const;   // 0x006B93D0
};
class CWorld { public:
    unsigned char    _pad00[0x54];
    scoped_ptr<CFactionManager> PFactionManager;        // +0x54
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x24];
    scoped_ptr<CWorld> PWorld;                          // +0x24
};
class CDefClassBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetInstantiationName() const;    // slot 10 (+0x28)
};
class CFactionDef : public CDefClassBase { public: };
class CTCBase;
class CTCEnemy { public:
    bool IsAlliedWithFaction(const CFaction* faction) const;   // 0x0076C7F0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        if (it == map->end() || type < it->first)
            it = map->end();
        return it->second;
    }
};
class CThingAICreature : public CThing { public: };
template <class T> class CDefPointer { public: T* Object; };
class CTCOpinionSource { public:
    virtual void v00();
    void*         ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x20 - 0x08];
    CDefPointer<const CFactionDef> RestrictToFactionAndAllies;   // +0x20
    bool          RestrictToFactionIsReversed;          // +0x24
    bool AppliesToWitness(const CThingAICreature* witness) const;
};

bool CTCOpinionSource::AppliesToWitness(const CThingAICreature* witness) const
{
    if (witness) {
        const CFactionDef* faction = RestrictToFactionAndAllies.Object;
        if (!faction)
            return true;
        if (witness->HasTC(0x49)) {
            const CTCEnemy* enemy = (const CTCEnemy*)witness->GetTC(0x49);
            const CFaction* factionObject;
            {
                CCharString name = GDefStringTable.GetString(faction->GetInstantiationName().Index);
                factionObject = CMainGameComponent::Get()->PWorld->PFactionManager->GetPFaction(name);
            }
            if (factionObject) {
                if (enemy->IsAlliedWithFaction(factionObject))
                    return !RestrictToFactionIsReversed;
                return RestrictToFactionIsReversed;
            }
            return false;
        }
        return false;
    }
    return false;
}