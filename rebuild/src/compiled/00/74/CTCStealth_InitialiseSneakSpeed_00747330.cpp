// CTCStealth::InitialiseSneakSpeed() -- derive the maximum sneaking speed from the creature's crouching walk animation: its scaled movement length over its duration in frames (0.1 when either is not positive)
// Leaf: InitialiseSneakSpeed
// Retail: Fable.exe (TLC, VC7.1) @ 0x00747330, 279 bytes (plain `ret`).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("CROUCH" @ 0x01267B28, "ST_WALK" @ 0x0125A990), CCharString()
//   @ 0x0099E4B0, CCharString::operator= @ 0x0099EFB0, CRC Calc @ 0x004014A0 (`__fastcall`, "" @ 0x0129AAF4 for an empty
//   name), CThingCreatureBase::GetAnimFromName(const CCharString&, const CAnimFilterBase*, long) @ 0x00662B10, ~CCharString
//   @ 0x0099EAE0 (x3), the creature's animation movement length @ 0x006627A0 and duration @ 0x006627F0 (both `(long anim)`,
//   float), the appearance component's scale getter @ 0x004BBFF0 (manifest "CMap::DrawGetSeaLevel", unverified).
//   g_framesPerSecond @ 0x01375550; 0.0f @ 0x0122DEDC; 0.1f inline.
// Layout: Ego_r.pdb CTCStealth MaxSneakingSpeed +0x18; retail CThing graphic-appearance component +0x64; CAnimFilterOfGroup
//   (vtable 0x01267568; base CAnimFilterBase 0x0122FFCC) {Group +4, GroupCRC +8} as the landed 0x008C7C00 model; the anim
//   entry's id at +0.
// Shape: the names and the group filter live in a block that closes after the anim lookup (the filter's inline
//   constructor assigns the group and its CRC from the argument; the base's inline virtual destructor gives the
//   implicit filter destructor its 0x0122FFCC vtable restore); the scaled length and the duration are float locals
//   compared with `> 0.0f`.
class CCharStringData { public:
    const char*   m_pData;                              // +0
    unsigned long m_length;                             // +4
};
class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC
class CAnimFilterBase { public:                                // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() {}
};
class CAnimFilterOfGroup : public CAnimFilterBase { public:           // vtable 0x01267568
    CCharString   Group;                                // +4
    unsigned long GroupCRC;                             // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.GetPString(), group.GetLength());
    }
};
class CAnimationEntry { public:
    long AnimID;                                        // +0
};
struct CTCGraphicAppearance {
    float GetScale();                                   // 0x004BBFF0
};
class CThingCreatureBase {
public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    CAnimationEntry* GetAnimFromName(const CCharString& name, const CAnimFilterBase* filter, long flags);   // 0x00662B10
    float GetAnimMovementLength(long anim);             // 0x006627A0
    float GetAnimDuration(long anim);                   // 0x006627F0
};
extern long g_framesPerSecond;                          // 0x01375550
class CTCStealth { public:
    virtual void v00();
    CThingCreatureBase* ParentThing;                    // +0x04 (CTCBase)
    unsigned char _pad08[0x18 - 0x08];
    float MaxSneakingSpeed;                             // +0x18
    void InitialiseSneakSpeed();
};

void CTCStealth::InitialiseSneakSpeed()
{
    CThingCreatureBase* creature = ParentThing;
    long anim;
    {
        CCharString group("CROUCH", -1);
        CCharString name("ST_WALK", -1);
        CAnimFilterOfGroup filter(group);
        anim = creature->GetAnimFromName(name, &filter, 0)->AnimID;
    }
    CTCGraphicAppearance* appearance = creature->PTCGraphicAppearance;
    float length = creature->GetAnimMovementLength(anim) * appearance->GetScale();
    float duration = creature->GetAnimDuration(anim);
    if (length > 0.0f && duration > 0.0f)
        MaxSneakingSpeed = length / (duration * g_framesPerSecond);
    else
        MaxSneakingSpeed = 0.1f;
}