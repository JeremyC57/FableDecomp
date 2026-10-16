// CTCSpecialAbilities::SetupFireballSpell() -- if the creature is free to cast a will fireball, make sure it carries a CTCFireballSpell component (adding one and setting its weapon state); false when it is busy
// Grade: DIFFER(143v143) -- register choice: ours tests the creature's interface bit through ecx and loads AddTC's this before `lea edx`; retail tests through eax and does `lea ecx ; push ecx ; mov ecx,[esi+18h]`
// Leaf: SetupFireballSpell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072C930, length 143 bytes (padded row 144).
// Callees: CCreatureAction_CastWillFireballSpell::CCreatureAction_CastWillFireballSpell(CThingCreatureBase*) @ 0x008BCD10 (vtable
// 0x0127DF4C; manifest label "CCreatureAction_CastBattleCharge" is a byte-twin propagation), CThingCreatureBase::IsFreeToPerformAction
// @ 0x00662210, the action's destructor @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 ("CTCFireballSpell" @ 0x0123D1EC), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60,
// CCharString::~CCharString @ 0x0099EAE0, CalculateWeaponState(CThingCreatureBase*) __fastcall @ 0x0072B940,
// CTCFireballSpell::SetWeaponState(EWEAPON_STATE) @ 0x007C8660.
// Layout: retail CTCSpecialAbilities keeps Creature at +0x18 (PDB +0x20; the STLport list is one pointer). The action is 0x11c bytes
// (retail CCombatActionBase 0x114 + two bools + a long), which with the CCharString gives retail's 0x120 frame. CThing interface
// bitset +0x20 (interface 0x9d = bit 29 of +0x30).
// Shape: the action is a temporary passed by reference (retail pushes the ctor's eax) and destroyed before the test (result kept in bl);
// the CCharString is a named local in its own block, the AddTC result kept in edi across its destructor.
class CTCBase;
struct CTCParamsBase;
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CCreatureActionBase;
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 (CThing)
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
struct CThingCreatureBase : CThing {
    bool IsFreeToPerformAction(const CCreatureActionBase& action);                   // 0x00662210
};
struct CCreatureActionBase {
    unsigned char _pad00[0x11c];
};
struct CCreatureAction_CastWillFireballSpell : CCreatureActionBase {
    CCreatureAction_CastWillFireballSpell(CThingCreatureBase* creature);   // 0x008BCD10
    ~CCreatureAction_CastWillFireballSpell();                             // 0x0062E900
};
enum EWEAPON_STATE { WEAPON_STATE_NONE };
EWEAPON_STATE __fastcall CalculateWeaponState(CThingCreatureBase* creature);   // 0x0072B940
struct CTCFireballSpell {
    void SetWeaponState(EWEAPON_STATE state);   // 0x007C8660
};
struct CTCSpecialAbilities {
    virtual void v00();
    void* ParentThing;                          // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    void* UndeadFollowers;                      // +0x14 list sentinel
    CThingCreatureBase* Creature;               // +0x18 (PDB +0x20)
    bool SetupFireballSpell();
};

bool CTCSpecialAbilities::SetupFireballSpell()
{
    if (Creature->IsFreeToPerformAction(CCreatureAction_CastWillFireballSpell(Creature))) {
        if (!Creature->HasTC(0x9d)) {
            CTCFireballSpell* spell;
            {
                CCharString name("CTCFireballSpell");
                spell = (CTCFireballSpell*)Creature->AddTC(name, true, 0);
            }
            spell->SetWeaponState(CalculateWeaponState(Creature));
        }
        return true;
    }
    return false;
}
