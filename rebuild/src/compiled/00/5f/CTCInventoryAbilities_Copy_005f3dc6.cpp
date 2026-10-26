// CTCInventoryAbilities::Copy(const CTCInventoryBase*) -- copies another abilities inventory (base state unless suppressed, then the spent experience, abilities, buttons and the initialised flag)
// Leaf: ?Copy@CTCInventoryAbilities
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F3DC6, 118 bytes (+2 int3); size TU without a frame pointer here
// (`#pragma optimize("s", on)`), `ret 4`. Vtable 0x01256AD4 slot 12 (catalog "vfunc_12"; CTCInventoryClothing's
// slot 12 is the base NInventory::CTCInventoryBase::Copy @ 0x005BFB96).
// Callees: NInventory::CTCInventoryBase::Copy(const CTCInventoryBase*) @ 0x005BFB96 and the +0x130 screen base's
//          Copy @ 0x0066E7A6 (an empty `ret 4`), both as qualified non-virtual calls and skipped while the global
//          flag @ 0x013B8A88 is set; std::vector<long>::operator= @ 0x00454886, the abilities vector's operator=
//          @ 0x005FA1BA, CVectorMap<long, EHeroAbility>::operator= @ 0x005F9BB2.
// Shape: the screen base gets `other` converted to that base (null-safe `neg ; sbb ; and`); the member copies are
// plain assignments in declaration order, the bool last.
// Layout: retail CTCInventoryAbilities (0x18 below the PDB here): screen base +0x130, Initialised +0x141,
// ExperienceSpentOnAbilities +0x148, Abilities +0x160, AbilityButtons +0x16C.
#pragma optimize("s", on)

extern bool g_SuppressInventoryBaseCopy;             // 0x013B8A88

struct CLongVector {                                 // std::vector<long>
    long* _M_start; long* _M_finish; long* _M_end_of_storage;
    CLongVector& operator=(const CLongVector& other);    // 0x00454886
};

struct CAbilityEntryVector {                         // vector<pair<long, CDefPointer<const CHeroAbilityDef>>>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CAbilityEntryVector& operator=(const CAbilityEntryVector& other);   // 0x005FA1BA
};

struct CAbilityButtonMap {                           // CVectorMap<long, EHeroAbility> (retail 16 bytes)
    void* _M_start; void* _M_finish; void* _M_end_of_storage; bool Dirty;
    CAbilityButtonMap& operator=(const CAbilityButtonMap& other);       // 0x005F9BB2
};

struct CTCInventoryBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Copy(const CTCInventoryBase* other);    // slot 12 (+0x30), base body 0x005BFB96
    unsigned char _pad_0x04[0x12C];
};

struct CInventoryScreenBase {                        // the screen interface base at +0x130
    virtual void Copy(const CInventoryScreenBase* other);    // 0x0066E7A6 (empty)
};

class CTCInventoryAbilities : public CTCInventoryBase, public CInventoryScreenBase { public:
    unsigned char       _pad_0x134[0xC];
    bool                WaitingToGiveTutorial;       // +0x140
    bool                Initialised;                 // +0x141
    unsigned char       _pad_0x142[0x2];
    long                CurrentAssignedAbilityLevel; // +0x144
    CLongVector         ExperienceSpentOnAbilities;  // +0x148
    CLongVector         ReceivedAbilityOrder;        // +0x154
    CAbilityEntryVector Abilities;                   // +0x160
    CAbilityButtonMap   AbilityButtons;              // +0x16C

    virtual void Copy(const CTCInventoryBase* other);
};

void CTCInventoryAbilities::Copy(const CTCInventoryBase* other)
{
    const CTCInventoryAbilities* source = static_cast<const CTCInventoryAbilities*>(other);
    if (!g_SuppressInventoryBaseCopy) {
        CTCInventoryBase::Copy(source);
        CInventoryScreenBase::Copy(source);
    }
    ExperienceSpentOnAbilities = source->ExperienceSpentOnAbilities;
    Abilities = source->Abilities;
    AbilityButtons = source->AbilityButtons;
    Initialised = source->Initialised;
}