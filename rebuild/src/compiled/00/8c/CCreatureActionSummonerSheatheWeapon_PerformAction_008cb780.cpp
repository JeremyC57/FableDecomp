// CCreatureAction_SummonerSheatheWeapon::PerformAction() -- move the live weapon out of the creature's primary carry slot into the "CARRY_SLOT_PRIMARY_SHEATHE" slot, run CCreatureAction_SheatheWeapons::PerformAction, then give the creature's combat component the attack-style def named by its creature def (+0x124)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB780, length 360 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface:
// 0x46 CTCCarrying, bit 6 of +0x28, tested; 0x30 CTCCombat, bit 16 of +0x24, untested), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440,
// RemoveThing(CThing*) @ 0x006A19B0 and PutThingInCarrySlot(CThing*, long, bool) @ 0x006A1F80, CCharString ctor/dtor @ 0x0099EBF0 /
// 0x0099EAE0, CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
// CCreatureAction_SheatheWeapons::PerformAction @ 0x008B7B10 (direct base call), CCreatureActionBase::GetDefinitionManager
// @ 0x00692910, CDefinitionManager::GetDef<CAttackStyleDef>(long, CDefPointer*) @ 0x00669640, CTCCombat::SetAttackStyleDef
// (const CDefPointer<CAttackStyleDef>*) @ 0x007355F0.
// Layout: retail CCombatActionBase is 0x114 bytes (PCreature +8); CThing +0x70 the def pointer; CCreatureDef +0x124 the attack-style
// def index.
// Shape: `this` spills to the frame across the carrying block (ebx holds the slot index); the creature def is taken as a by-value
// CDefPointer temporary converted to a raw pointer (add-ref then immediate release, the index read afterwards); the attack-style
// def is a CDefPointer local released by the inline dtor through the pointee `Release() { DecRef(); if (GetRefCount() == 0) Destroy(); }` (retail's long release form).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
class CCreatureDef : public CDefClassBase { public:
    unsigned char _pad08[0x124 - 0x08];
    long AttackStyleDefIndex;                   // +0x124
};
struct CAttackStyleDef : CDefClassBase { };
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    operator T*() const { return Object; }
};
class CDefinitionManager { public:
    void GetDef(long index, CDefPointer<CAttackStyleDef>* out);   // 0x00669640
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CCreatureDef> PDef;             // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    CDefinitionManager* GetDefinitionManager(); // 0x004C7990
    CDefPointer<CCreatureDef> GetDef() const { return PDef; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCCarrying { public:
    void RemoveThing(CThing* thing);            // 0x006A19B0
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);   // 0x006A1F80
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
class CTCCombat { public:
    void SetAttackStyleDef(const CDefPointer<CAttackStyleDef>* def);   // 0x007355F0
};
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    CDefinitionManager* GetDefinitionManager(); // 0x00692910
};
class CCreatureAction_SheatheWeapons : public CCombatActionBase { public:
    void PerformAction();                       // 0x008B7B10
};
class CCreatureAction_SummonerSheatheWeapon : public CCreatureAction_SheatheWeapons { public:
    void PerformAction();
};

void CCreatureAction_SummonerSheatheWeapon::PerformAction()
{
    CTCCarrying* carrying;
    if (PCreature.GetPItem()->GetInterface(&carrying, 0x46)) {
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon && !(weapon->m_flags91 & 1)) {
            carrying->RemoveThing(weapon);
            long slot = PCreature.GetPItem()->GetDefinitionManager()->GetDefGlobalIndexFromName("CARRY_SLOT_PRIMARY_SHEATHE");
            carrying->PutThingInCarrySlot(weapon, slot, true);
        }
    }
    CCreatureAction_SheatheWeapons::PerformAction();
    CTCCombat* combat;
    PCreature.GetPItem()->GetInterface(&combat, 0x30);
    const CCreatureDef* creatureDef = PCreature.GetPItem()->GetDef();
    CDefPointer<CAttackStyleDef> attackStyle;
    GetDefinitionManager()->GetDef(creatureDef->AttackStyleDefIndex, &attackStyle);
    combat->SetAttackStyleDef(&attackStyle);
}