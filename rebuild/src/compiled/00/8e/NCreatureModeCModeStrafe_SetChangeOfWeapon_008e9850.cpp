// NCreatureMode::CModeStrafe::SetChangeOfWeapon(CThing* weapon) -- take the new weapon's id and animation group into the strafe filter ("STAND" and -1 with no weapon thing), rebuild the combination anims, recompute the aiming frame count / centre from the projectile-aim animation's length, then clear the strafing combination anims
// Leaf: SetChangeOfWeapon
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9850, 251 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0 (const instantiation; inlined HasTC + GetTC for
//   CTCWeapon 0x48), CTCWeapon::GetAnimationGroupName 0x005DABE0, CAnimFilterOfGroup::SetGroup(const CCharString&)
//   0x0073DFF0 (out of line at the first site, inlined at the second), CCharString(const char*, long) 0x0099EBF0 /
//   operator= 0x0099EFB0 / ~CCharString 0x0099EAE0, CCRC::Calc 0x004014A0 (`__fastcall`), CModeStrafe::
//   SetUpCombinationAnims 0x008E9190, the mode's global-component getter 0x0073A680 (reads [0x013B8A1C]+0x24; unused
//   `this`), the animation bank lookup 0x00A242C0 (index -> record, length in seconds at +0x10), __ftol2 0x00BFEA70,
//   CModeStrafe::ClearStrafingCombinationAnims(bool) 0x008E9710.
// Globals: the frames-per-second long 0x01375550.
// Layout: Ego_r.pdb CModeStrafe (0x70, matches retail; as the landed sibling UpdateWeaponState 0x008E9950): Filter +0x2c
//   (Group +0x30, GroupCRC +0x34), ActiveWeapon +0x38, AimingNumFrames +0x3c, AimingCenterFrame +0x40,
//   PAnimProjectileAim +0x50 (CCountedPointer<CAnimEntryLite>, the entry's first word is its animation index).
// Shape: HasTC + one-ternary GetTC (key homed in the dead `weapon` slot); the aim entry is a named local held across
//   the getter call (edi), the components a named local and the bank an inline accessor (`mov eax,[eax+3Ch] ; push ;
//   mov ecx,eax`); the centre re-reads AimingNumFrames.
struct CTCBase;

struct CCharStringData {
    const char*   m_pData;                     // +0
    unsigned long m_length;                    // +4
};
struct CCharString {
    CCharStringData* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0 CCRC::Calc

struct CAnimFilterOfGroup {                    // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                       // +4
    unsigned long GroupCRC;                    // +8
    void SetGroup(const CCharString& group)    // 0x0073DFF0
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.GetPString(), group.GetLength());
    }
};

struct CInterfaceMap {                         // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0 (the const instantiation)
};

struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];           // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;               // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short m_weaponId;                 // +0x8c (DefGlobalIndex)
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        const CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (const T*)sel->component;
    }
};

struct CTCWeapon {                             // interface type 0x48
    const CCharString& GetAnimationGroupName() const;   // 0x005DABE0
};

struct CAnimBankRecord {
    unsigned char _pad00[0x10];
    float         Seconds;                     // +0x10
};
struct CAnimBank {
    CAnimBankRecord* GetRecord(long index);    // 0x00A242C0
};
struct CGlobalComponents {
    unsigned char _pad00[0x3c];
    CAnimBank*    AnimBank;                    // +0x3c
    CAnimBank* GetAnimBank() const { return AnimBank; }
};

namespace NAnimationEntryBodge {
struct CAnimEntryLite {
    long AnimIndex;                            // +0
};
}
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
};

extern long g_framesPerSecond;                 // 0x01375550

struct CModeBase {                             // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;                 // +0x04
    unsigned char _body[0x14];
    CGlobalComponents* GetGlobalComponents() const;   // 0x0073A680
};

namespace NCreatureMode {

struct CModeStrafe : CModeBase {               // vtable 0x01268588
    float              AnimMovementDistance;   // +0x1c
    void*              StrafeAnimName;         // +0x20 (CCharString)
    long               AnimIndex;              // +0x24
    bool               LastUsable;             // +0x28
    CAnimFilterOfGroup Filter;                 // +0x2c
    long               ActiveWeapon;           // +0x38
    long               AimingNumFrames;        // +0x3c
    long               AimingCenterFrame;      // +0x40
    float              RumbleParam;            // +0x44
    float              PullBackAmount;         // +0x48
    bool               ForceAnimNoBlendIn;     // +0x4c
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileAim;   // +0x50
    void SetUpCombinationAnims();              // 0x008E9190
    void ClearStrafingCombinationAnims(bool force);   // 0x008E9710
    void SetChangeOfWeapon(CThing* weapon);
};

void CModeStrafe::SetChangeOfWeapon(CThing* weapon)
{
    if (weapon && weapon->HasTC(0x48)) {
        const CTCWeapon* tc = weapon->GetTC<CTCWeapon>(0x48);
        ActiveWeapon = weapon->m_weaponId;
        Filter.SetGroup(tc->GetAnimationGroupName());
    } else {
        ActiveWeapon = -1;
        Filter.SetGroup("STAND");
    }
    SetUpCombinationAnims();
    NAnimationEntryBodge::CAnimEntryLite* aim = PAnimProjectileAim.Data;
    AimingNumFrames = 0;
    if (aim) {
        CGlobalComponents* components = GetGlobalComponents();
        AimingNumFrames = (long)(g_framesPerSecond * components->GetAnimBank()->GetRecord(aim->AnimIndex)->Seconds);
    }
    AimingCenterFrame = AimingNumFrames / 2;
    ClearStrafingCombinationAnims(false);
}

}