// CTCSpecialAbilities::SetupBerserk() -- start the strength-berserk cast on the creature; when its primary-slot weapon is the item it holds in hand, first sheathe it with the berserk cast queued as the follow-up action; false when the creature is barred (flag +0x30 bit 4)
// Leaf: SetupBerserk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072CBA0, 352 bytes (`ret`).
// Callees: CVectorMap::LowerBound const 0x004365B0 (CTCCarrying 0x46, interface 0x13); CTCCarrying::GetThingInPrimarySlot
//          0x006A1440; CBaseIntelligentPointer::GetPItem 0x00A01B50; operator new 0x00BFEA1A; CCreatureAction_CastStrengthBerserk
//          ctor 0x008E6880 (vtable 0x01287384; catalog "CCreatureAction_BattleChargeTurnLeft", a twin) and dtor 0x0062E900;
//          CIntelligentPointer<const CThing>(CThing*) 0x004A96F0; the berserk action's item setter 0x0072D360 (by-value
//          intelligent pointer; catalog "CTCInventoryBase::GetItemDescription", a twin; descriptive name);
//          CCreatureAction_SheatheItemToInventory ctor 0x0069EF80 / dtor 0x0048C950 (catalog twin label); global CCharString
//          0x013BAD30 (descriptive name); CThingCreatureBase::SetCurrentAction 0x006644F0.
// Layout: retail CTCSpecialAbilities::Creature +0x18 (PDB +0x20); CThing interface bitset +0x20, flags +0x30, map +0x44; the
//          interface-0x13 component's held-item intelligent pointer at +0x141 (descriptive); the weapon's flag byte +0x91.
// Shape: the barred test wraps the body (the `xor al,al` arm sits last); the default cast is a temporary of the
//          SetCurrentAction call, the sheathe action a named local; both lookups are the ternary GetInterface (this in ebp).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[4];                 // +0x20
    unsigned char m_flags30;                        // +0x30
    unsigned char _pad31[0x44 - 0x31];
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
template <class T> struct CIntelligentPointer {
    unsigned char _d[8];
    CIntelligentPointer(T* item);                   // 0x004A96F0 (const CThing instantiation)
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();                // 0x006A1440
};
#pragma pack(push, 1)
struct CTCHeldItem {                                // interface 0x13 (descriptive)
    unsigned char           _pad00[0x141];
    CBaseIntelligentPointer HeldItem;               // +0x141
};
#pragma pack(pop)
class CThingCreatureBase;
class CCreatureActionBase {
public:
    unsigned char _d[0xbc];
};
class CCreatureAction_CastStrengthBerserk : public CCreatureActionBase {
public:
    unsigned char _more[0x114 - 0xbc];
    CCreatureAction_CastStrengthBerserk(CThingCreatureBase* creature);   // 0x008E6880
    ~CCreatureAction_CastStrengthBerserk();         // 0x0062E900
    void SetItem(CIntelligentPointer<const CThing> item);   // 0x0072D360
};
struct CCharString { void* m_pData; };
extern const CCharString g_sheatheForAbilityName;   // 0x013BAD30
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase {
public:
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* creature, const CCharString& name, CCreatureActionBase* followUp, long flags);   // 0x0069EF80
    ~CCreatureAction_SheatheItemToInventory();      // 0x0048C950
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CTCSpecialAbilities {
public:
    virtual void v00();
    unsigned char       _pad04[0x18 - 0x04];
    CThingCreatureBase* Creature;                   // +0x18
    bool SetupBerserk();
};

bool CTCSpecialAbilities::SetupBerserk()
{
    if (!(Creature->m_flags30 & 0x10)) {
        CTCCarrying* carrying;
        if (Creature->GetInterface(&carrying, 0x46)) {
            CThing* weapon = carrying->GetThingInPrimarySlot();
            if (weapon && !(weapon->m_flags91 & 1)) {
                CTCHeldItem* held;
                Creature->GetInterface(&held, 0x13);
                CThing* item = (CThing*)held->HeldItem.GetPItem();
                if (weapon == item) {
                    CCreatureAction_CastStrengthBerserk* berserk = new CCreatureAction_CastStrengthBerserk(Creature);
                    berserk->SetItem(item);
                    CCreatureAction_SheatheItemToInventory action(Creature, g_sheatheForAbilityName, berserk, 0);
                    return Creature->SetCurrentAction(action);
                }
            }
        }
        return Creature->SetCurrentAction(CCreatureAction_CastStrengthBerserk(Creature));
    }
    return false;
}