// CCreatureAction_UnsheatheWeapons::Initialise() -- ask whether the creature should play the unsheathe action for its weapon type (and whether it is really a sheathe), name the animation STANDARD_SHEATHE or STANDARD_UNSHEATHE, tell the creature's combat component which weapon type is being drawn, then the combat-action Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7ED0, length 176 bytes (plain `ret`, virtual; also called directly by
// CCreatureAction_SummonerUnsheatheStrike::Initialise @ 0x008CB620).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, twice), __fastcall ShouldPlayUnsheatheWeaponAction(
// CThingCreatureBase*, EUnsheatheWeaponType, bool*, bool*) 0x008B7CF0 (result dropped), CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetAnimationName(const CCharString&) 0x008574D0,
// CVectorMap::LowerBound 0x0040F020 (inline GetInterface 0x30: bit 16 of the present-bits dword +0x24),
// CCombatActionBase::Initialise 0x00857520 (direct base call, last).
// Layout: retail WeaponType +0x114 (the PDB header puts it at +0x12C: the debug base is 0x18 larger), so a local model; the
// combat component keeps the weapon type being drawn at +0x6C.
// Shape: the creature goes through the intelligent pointer's inline conversion operator, so GetPItem runs before the
// plain arguments are pushed; only the sheathe flag is initialised (false); the literal is a ternary selected into eax before the string is
// built; the string is block-scoped, so the interface lookup key reuses its frame word; the absent-interface path skips
// the store.
struct CTCBase;
struct CThingCreatureBase;
enum EUnsheatheWeaponType { UNSHEATHE_WEAPON_TYPE_DEFAULT };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
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
};

bool __fastcall ShouldPlayUnsheatheWeaponAction(CThing* creature, EUnsheatheWeaponType type, bool* pCanUnsheathe,
                                                bool* pSheathe);   // 0x008B7CF0

struct CTCCombat {
    unsigned char        _pad0[0x6c];
    EUnsheatheWeaponType m_unsheatheWeaponType; // +0x6c
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};

struct CCombatActionBase {
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_UnsheatheWeapons : CCombatActionBase {
    EUnsheatheWeaponType WeaponType;            // +0x114
    virtual void Initialise();
};

void CCreatureAction_UnsheatheWeapons::Initialise()
{
    bool sheathe = false;
    bool canUnsheathe;
    ShouldPlayUnsheatheWeaponAction(PCreature, WeaponType, &canUnsheathe, &sheathe);
    {
        CCharString name(sheathe ? "STANDARD_SHEATHE" : "STANDARD_UNSHEATHE", -1);
        SetAnimationName(name);
    }
    CTCCombat* combat;
    if (PCreature.GetPItem()->GetInterface(&combat, 0x30))
        combat->m_unsheatheWeaponType = WeaponType;
    CCombatActionBase::Initialise();
}
