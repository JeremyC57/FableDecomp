// CMeleeCombatAbilityDef::operator=(const CMeleeCombatAbilityDef&) -- memberwise copy: CParentDefClassBase part, flourish anim names, damage multiplier, flags, valid weapon classes, knockdown/evade flags, required stats and combo bounds; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00462451, length 117 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<EWeaponClass>::operator= @ 0x004624C6,
//          CHeroAbilityStatPackage::operator= @ 0x004621E9 (landed as a CRetailTree map assignment: the
//          stat package is a 12-byte retail map).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CMeleeCombatAbilityDef's vtable 0x01234D5C is the 5-byte `jmp 0x00462451` right before this body; PDB
// members +0x40..+0x84 sit at retail +0x3c..+0x78 (retail base 0x39 bytes, containers 12 bytes).
// Shape: memberwise assignment in declaration order; CDefString is a 4-byte handle copied as a dword,
// bools as byte moves, containers through their out-of-line operator=.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CDefString {                     // 4-byte def-string handle (trivially copyable)
    long m_index;
};

struct CWeaponClassVector {             // retail std::vector<EWeaponClass> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CWeaponClassVector& operator=(const CWeaponClassVector& other);   // 0x004624C6
};

struct CHeroAbilityStatPackage {        // retail 12 bytes (a map of stat requirements)
    void*         m_allocator;
    void*         m_head;
    unsigned long m_size;
    CHeroAbilityStatPackage& operator=(const CHeroAbilityStatPackage& other);   // 0x004621E9
};

struct CMeleeCombatAbilityDef : CParentDefClassBase {  // vtable 0x01234D5C
    CDefString FlourishAnimName;                // +0x3c
    CDefString FlourishResponseAnimName;        // +0x40
    float      DamageMultiplier;                // +0x44
    bool       Flourish;                        // +0x48
    bool       BlockCounter;                    // +0x49
    long       AbilityButtonGraphicOverride;    // +0x4c
    CWeaponClassVector ValidWeaponClasses;      // +0x50
    bool       Decapitate;                      // +0x5c
    long       KnockDownEffects;                // +0x60
    bool       EvadeAllHits;                    // +0x64
    bool       CauseRecoil;                     // +0x65
    CHeroAbilityStatPackage StatsNeeded;        // +0x68
    long       ComboMultiplierLowerBound;       // +0x74
    long       ComboMultiplierUpperBound;       // +0x78
    CMeleeCombatAbilityDef& operator=(const CMeleeCombatAbilityDef& other);
};

CMeleeCombatAbilityDef& CMeleeCombatAbilityDef::operator=(const CMeleeCombatAbilityDef& other)
{
    CParentDefClassBase::operator=(other);
    FlourishAnimName = other.FlourishAnimName;
    FlourishResponseAnimName = other.FlourishResponseAnimName;
    DamageMultiplier = other.DamageMultiplier;
    Flourish = other.Flourish;
    BlockCounter = other.BlockCounter;
    AbilityButtonGraphicOverride = other.AbilityButtonGraphicOverride;
    ValidWeaponClasses = other.ValidWeaponClasses;
    Decapitate = other.Decapitate;
    KnockDownEffects = other.KnockDownEffects;
    EvadeAllHits = other.EvadeAllHits;
    CauseRecoil = other.CauseRecoil;
    StatsNeeded = other.StatsNeeded;
    ComboMultiplierLowerBound = other.ComboMultiplierLowerBound;
    ComboMultiplierUpperBound = other.ComboMultiplierUpperBound;
    return *this;
}