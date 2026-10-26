// CTCSpecialAbilities::SetupEnflameSpell() -- start the creature's enflame-spell cast: target the creature's current combat target (with the player's lock flag when both are player-flagged), and when a primary-slot weapon is alive wrap a heap cast action holding the weapon in a generic strike response; returns whether the action was accepted
// Retail: Fable.exe @ 0x0072C790, 416 bytes (plain `ret`, no EH frame).
// Callees: LowerBound 0x0040F020 / 0x004365B0 (const), CTCInventory::GetThingInPrimarySlot 0x006A1440, combat TC virtual
//   slot 33 (+0x84, current target), CThing::PeekPlayer 0x004C7A10, operator new 0x00BFEA1A,
//   CCreatureAction_CastWillEnflameSpell ctor 0x008BB500 / ~CCombatActionBase 0x0062E900, CBaseIntelligentPointer ctor
//   0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10, CCombatAction_GenericStrikeResponse ctor
//   0x0069F060 / dtor 0x0072D390, CThingCreatureBase::SetCurrentAction 0x006644F0.
// Layout (local model): CTCSpecialAbilities creature +0x18; CThing TC bitset +0x20 (8 = +0x21 bit 0, 0x46 = +0x28 bit 6),
//   interface map +0x44, flags +0x6c (8 / 0x20), Dead +0x91 bit 0; CPlayer lock flag +0x20a; the cast action's weapon
//   pointer +0x2c; g_13bad30 the strike-response name.
#include <stddef.h>
void* operator new(size_t size);                       // 0x00BFEA1A
class CThing;
class CPlayer { public: unsigned char _pad[0x20a]; bool LockFlag; };
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last; SEntry* end;
    SEntry* LowerBound(const long& key);                // 0x0040F020
    const SEntry* LowerBound(const long& key) const;    // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long TCBits[9];                            // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad50[0x1c];
    unsigned char Flags6c;                              // +0x6c
    unsigned char _pad6d[0x24];
    unsigned char Dead : 1;                             // +0x91 bit 0
public:
    const CPlayer& PeekPlayer() const;   // 0x004C7A10
};
class CTCInventory { public: CThing* GetThingInPrimarySlot(); };   // 0x006A1440
class CTCCombat { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual CThing* GetCurrentTarget();                // slot 33 (+0x84)
};
class CCombatActionBase { public:
    virtual ~CCombatActionBase();
    unsigned char _pad04[0x28];
};
class CCreatureAction_CastWillEnflameSpell : public CCombatActionBase { public:
    CIntelligentPointer<CThing> Weapon;                    // +0x2c
    unsigned char _pad34[0x11c - 0x34];
    CCreatureAction_CastWillEnflameSpell(CThing* creature, CThing* target, bool lock, long flags);   // 0x008BB500
    ~CCreatureAction_CastWillEnflameSpell();            // 0x0062E900
};
class CCharString { public: void* m_data; };
extern const CCharString g_StrikeResponseName;         // 0x013BAD30
class CCombatAction_GenericStrikeResponse { public:
    unsigned char _pad[0x110];
    CCombatAction_GenericStrikeResponse(CThing* creature, const CCharString& name, CCombatActionBase* action, long flags);   // 0x0069F060
    ~CCombatAction_GenericStrikeResponse();             // 0x0072D390
};
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
class CTCSpecialAbilities { public:
    void* __vftable;
    unsigned char _pad04[0x14];
    CThingCreatureBase* Creature;                       // +0x18
    bool SetupEnflameSpell();
};

bool CTCSpecialAbilities::SetupEnflameSpell()
{
    CTCInventory* inventory;
    if (Creature->TCBits[0x46 >> 5] & (1UL << (0x46 & 31))) {
        long key = 0x46;
        CInterfaceMap* map = &Creature->Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        inventory = (CTCInventory*)(it == map->last || 0x46 < it->type ? map->last : it)->component;
    }
    CThing* weapon = inventory->GetThingInPrimarySlot();
    CThing* target = 0;
    bool lock = false;
    if (Creature->TCBits[8 >> 5] & (1UL << (8 & 31))) {
        long key = 8;
        const CInterfaceMap* map = &Creature->Interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        CTCCombat* combat = (CTCCombat*)(it == map->last || 8 < it->type ? map->last : it)->component;
        target = combat->GetCurrentTarget();
        if (target && !target->Dead && (target->Flags6c & 8) && (Creature->Flags6c & 0x20))
            lock = Creature->PeekPlayer().LockFlag;
    }
    if (weapon && !weapon->Dead)
    {
        CCreatureAction_CastWillEnflameSpell* action = new CCreatureAction_CastWillEnflameSpell(Creature, target, lock, 0);
        action->Weapon.SetPItem(CIntelligentPointer<CThing>(weapon).GetPItem());
        CCombatAction_GenericStrikeResponse response(Creature, g_StrikeResponseName, action, 0);
        return Creature->SetCurrentAction(*(CCombatActionBase*)&response);
    }
    return Creature->SetCurrentAction(CCreatureAction_CastWillEnflameSpell(Creature, target, lock, 0));
}