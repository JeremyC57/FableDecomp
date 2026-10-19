// NCreatureMode::CModeStrafe::CModeStrafe(CThing*, bool) -- CModeBase(thing), empty strafe anim name, an empty-group filter, no active weapon, one aiming frame, four empty projectile-anim pointers
// Leaf: ??0CModeStrafe
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741FE0, 185 bytes (`ret 8`). Vtable 0x01268588 (NCreatureMode::CModeStrafe).
// Callees: CModeBase::CModeBase(CThing*) 0x0073EAC0; CCharString(const char*, long) 0x0099EBF0 (twice, literal ""),
// CCharString() 0x0099E4B0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0.
// Layout (PDB 0x70, matches retail): CModeBase 0x1c, AnimMovementDistance +0x1c, StrafeAnimName +0x20, AnimIndex +0x24,
// LastUsable +0x28, Filter +0x2c, ActiveWeapon +0x38, AimingNumFrames +0x3c, AimingCenterFrame +0x40, RumbleParam +0x44,
// PullBackAmount +0x48, ForceAnimNoBlendIn +0x4c, PAnimProjectileAim +0x50, PAnimProjectileBuildUp +0x58,
// PAnimProjectilePullBack +0x60, PAnimStrafeUpperBody +0x68 (CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>).
// Shape: init list in declaration order; the zero words (distance, counted pointers) share the edi the CRC test uses.
struct CThing;
struct CThingCreatureBase;
struct CBaseObject;

struct CCharStringData {
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

struct CCharString {
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

struct CAnimFilterOfGroup {                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.GetPString(), group.GetLength());
    }
};

struct CModeBase {                         // retail 0x1c: vptr, ParentThing, WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    virtual void v00();
    unsigned char _body[0x18];
    CModeBase(CThing* thing);              // 0x0073EAC0
};

template <class T> struct CCountedPointer {   // 8 bytes: object + shared count block
    T*    Object;
    void* Count;
    CCountedPointer() : Object(0), Count(0) { }
};

namespace NAnimationEntryBodge { struct CAnimEntryLite; }

namespace NCreatureMode {

struct CModeStrafe : CModeBase {           // vtable 0x01268588
    float              AnimMovementDistance;   // +0x1c
    CCharString        StrafeAnimName;         // +0x20
    long               AnimIndex;              // +0x24
    bool               LastUsable;             // +0x28
    CAnimFilterOfGroup Filter;                 // +0x2c
    long               ActiveWeapon;           // +0x38
    long               AimingNumFrames;        // +0x3c
    long               AimingCenterFrame;      // +0x40
    float              RumbleParam;            // +0x44
    float              PullBackAmount;         // +0x48
    bool               ForceAnimNoBlendIn;     // +0x4c
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileAim;       // +0x50
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileBuildUp;   // +0x58
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectilePullBack;  // +0x60
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimStrafeUpperBody;     // +0x68
    virtual void v00();
    CModeStrafe(CThing* thing, bool forceAnimNoBlendIn);
};

CModeStrafe::CModeStrafe(CThing* thing, bool forceAnimNoBlendIn)
    : CModeBase(thing)
    , AnimMovementDistance(0.0f)
    , StrafeAnimName("")
    , LastUsable(true)
    , Filter("")
    , ActiveWeapon(-1)
    , AimingNumFrames(1)
    , ForceAnimNoBlendIn(forceAnimNoBlendIn)
{
}

}