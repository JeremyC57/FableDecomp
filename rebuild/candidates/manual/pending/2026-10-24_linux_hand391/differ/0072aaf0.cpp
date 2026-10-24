// CTCSpecialAbilities::SetupMultiStrike() -- start the multi-strike ability: false when the owning thing already carries the multi-strike component (interface bit 0x87), else add a fresh CTCMultiStrike component and return true
// Grade: DIFFER(66v66) -- retail loads ParentThing into ecx right after the name ctor, before pushing 0/1/&name (`mov ecx,[esi+4] ; push 0 ; push 1 ; lea eax ; push eax`); ours pushes first and loads ecx last (`lea ecx ; push ecx ; mov ecx,[esi+4]`). Same family as 0x0072A970/0x0072A9B0 (unlanded); the landed named-local twin 0x0072B0D0 has the late load.
// Leaf: SetupMultiStrike
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072AAF0, 66 bytes (80 with padding), `ret`.
// Label correction: catalog "UseSpecialAbility_CTCMultiStrike" -- it is case 0x0C.. of UseSpecialAbility's switch
//   (call site 0x0072D04E) and the Ego_r.pdb name set has ?SetupMultiStrike@CTCSpecialAbilities@@AAE_NXZ beside the
//   landed SetupPhysicalShieldSpell 0x0072B0D0 / SetupEnflameSpell 0x0072C790 twins.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("CTCMultiStrike" @ 0x0123D374),
//   CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, CCharString::~CCharString @ 0x0099EAE0.
// Layout: this is CTCBase-derived, ParentThing at +0x04 (unlike the shield/berserk setups, which use the creature
//   pointer at +0x18); CThing interface-presence bitset at +0x20, so interface 0x87 is bit 7 of the byte at +0x30.
// Shape: VC7.1 tests a top-bit byte mask as `mov cl,[eax+30h] ; test cl,cl ; js`; the present test wraps the body
//   (`if (!HasTC) { ...; return true; } return false;` puts the `xor al,al` arm last -- the early-return form puts it
//   first); the name is an IMPLICIT conversion temporary of the AddTC call (as in the landed AddOwnedEntityTC
//   0x0072DD60): VC7.1 builds it in the `push ecx` frame word, then loads ParentThing into ecx BEFORE pushing the
//   other arguments -- a named CCharString local loads ecx after the pushes (`lea ecx ; push ecx ; mov ecx,[esi+4]`).
class CTCBase;
struct CTCParamsBase;
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
struct CTCSpecialAbilities {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool SetupMultiStrike();
};

bool CTCSpecialAbilities::SetupMultiStrike()
{
    if (!ParentThing->HasTC(0x87)) {
        ParentThing->AddTC("CTCMultiStrike", true, 0);
        return true;
    }
    return false;
}
