// CTCSpecialAbilities::SetupTurncoatSpell() -- cast turncoat on the hero's current target: a live creature enemy that has a turncoat component which can be, and is not yet, turncoated gets a CTCTurncoatSpell component aimed at it; always returns false
// Leaf: SetupTurncoatSpell
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072B160, 247 bytes (plain `ret`).
// Callees: the const interface LowerBound @ 0x004365B0 (interface 8), that component's virtual slot 33 (+0x84, the target
//   thing), AreThingsEnemies(const CThing*, const CThing*) @ 0x0076D2F0 (`__fastcall`), CVectorMap::LowerBound @ 0x0040F020
//   (interface 0x9F = CTCTurncoat), CTCTurncoat::IsTurncoatable @ 0x007C7870, CTCTurncoat::IsTurncoated @ 0x007C7890,
//   CCharString(const char*, long) @ 0x0099EBF0 ("CTCTurncoatSpell" @ 0x0123D1B8), CThing::AddTC(const CCharString&, bool,
//   CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0, CTCTurncoatSpell::SetPTarget(CThingCreatureBase*) @ 0x007C6940.
// Layout: Ego_r.pdb CTCSpecialAbilities with the retail 4-byte list: Creature +0x18 (PDB 0x20); retail CThing interface bitset
//   +0x20 (8 = word +0x20 bit 8, 0x9F = word +0x30 bit 31: `test eax,eax ; jns`), map +0x44, ThingTypeFlags +0x6C (8 =
//   creature), dead bit +0x91.
// Shape: nested tests with every exit on the shared `xor al,al`; the lookup keys and the implicit component-name temporary
//   share one frame word.
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCParamsBase;
class CThing;
struct CTCTargeter {                                    // interface 8 (named descriptively)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTarget();                       // slot 33 (+0x84)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                       // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (T*)((it == map->last || type < it->first) ? map->last : it)->second;
    }
    template <class T> __forceinline T* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (T*)((it == map->last || type < it->first) ? map->last : it)->second;
    }
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};
class CThingCreatureBase : public CThing {};
bool __fastcall AreThingsEnemies(const CThing* a, const CThing* b);   // 0x0076D2F0
class CTCTurncoat { public:
    bool IsTurncoatable();                              // 0x007C7870
    bool IsTurncoated();                                // 0x007C7890
};
class CTCTurncoatSpell { public:
    void SetPTarget(CThingCreatureBase* target);        // 0x007C6940
};
class CTCSpecialAbilities { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                              // +0x0c
    bool UsingBed;                                      // +0x0d
    long CurrentAbility;                                // +0x10
    void* UndeadFollowers;                              // +0x14 list (retail 4 bytes)
    CThingCreatureBase* Creature;                       // +0x18
    bool SetupTurncoatSpell();
};

bool CTCSpecialAbilities::SetupTurncoatSpell()
{
    const CThing* parent = ParentThing;
    if (parent->HasTC(8)) {
        CThing* target = parent->GetTC<CTCTargeter>(8)->GetPTarget();
        if (target && !(target->m_flags91 & 1) && (target->ThingTypeFlags & 8)
            && AreThingsEnemies(ParentThing, target) && target->HasTC(0x9f)) {
            CTCTurncoat* turncoat = target->GetTC<CTCTurncoat>(0x9f);
            if (turncoat->IsTurncoatable() && !turncoat->IsTurncoated()) {
                CTCTurncoatSpell* spell = (CTCTurncoatSpell*)Creature->AddTC("CTCTurncoatSpell", true, 0);
                spell->SetPTarget((CThingCreatureBase*)target);
            }
        }
    }
    return false;
}