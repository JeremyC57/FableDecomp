// CCreatureAction_UnsheatheWeapons::Initialise() -- ask whether the weapon type wants a sheathe or unsheathe, play STANDARD_SHEATHE / STANDARD_UNSHEATHE, store the weapon type in the owner's combat component (interface 0x30), then the base Initialise
// Grade: DIFFER(174v174) -- scheduling only: retail stores sheathe=false before the GetPItem call and uses ecx/edx (ours eax/ecx) for the flag/key addresses; the string/lookup sequence orders mov ecx,edi and the key lea differently
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7ED0, length 174 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, ShouldPlayUnsheatheWeaponAction(CThingCreatureBase*, EUnsheatheWeaponType,
// bool*, bool*) @ 0x008B7CF0 (`__fastcall`, `ret 8`), CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// CCombatActionBase::SetAnimationName @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0, CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface 0x30, bit 16 of +0x24), CCombatActionBase::Initialise @ 0x00857520.
// Shape: two bool out flags in the frame's high bytes (the sheathe flag zero-initialised, the other left unset); the literal is
// chosen by a ternary into eax before the block-scoped string; the CHECKED out-parameter lookup stores WeaponType at +0x6c of
// the combat component (the string's frame word is reused for the lookup key).
// Layout: owner CIntelligentPointer +8; Ego_r.pdb WeaponType +0x12c, retail +0x114 (CCombatActionBase is 0x114 in retail).
enum EUnsheatheWeaponType { UNSHEATHE_WEAPON_TYPE_0 };
struct CTCBase;
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
struct CThingCreatureBase : CThing { };

bool __fastcall ShouldPlayUnsheatheWeaponAction(CThingCreatureBase* creature, EUnsheatheWeaponType type, bool* play, bool* sheathe);   // 0x008B7CF0

struct CTCCombat {
    unsigned char        _pad[0x6c];
    EUnsheatheWeaponType m_weaponType;          // +0x6c
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CBaseIntelligentPointer {
    void*               __vftable;
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const;       // 0x00A01B50
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_UnsheatheWeapons : public CCombatActionBase {
public:
    EUnsheatheWeaponType WeaponType;            // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_UnsheatheWeapons::Initialise()
{
    bool sheathe = false;
    bool play;
    ShouldPlayUnsheatheWeaponAction(PCreature.GetPItem(), WeaponType, &play, &sheathe);
    {
        CCharString name(sheathe ? "STANDARD_SHEATHE" : "STANDARD_UNSHEATHE", -1);
        SetAnimationName(name);
    }
    CTCCombat* combat;
    if (PCreature.GetPItem()->GetInterface(&combat, 0x30))
        combat->m_weaponType = WeaponType;
    CCombatActionBase::Initialise();
}
