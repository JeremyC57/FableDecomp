// CSpecialAbilitiesPhysicalShieldDef::operator=(const CSpecialAbilitiesPhysicalShieldDef&) -- memberwise copy of the physical-shield def; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045FEE3, length 458 bytes, `ret 4`.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (ebp frame, `push 4 ; pop ecx`).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D.
// Identity: the catalog label "operator=" belongs to the def's slot-19 Copy function; the copied offsets are
// the Ego_r.pdb CSpecialAbilitiesPhysicalShieldDef layout shifted by the retail 4-byte-smaller
// CParentDefClassBase (PDB +0x40 -> retail +0x3c) and the 12-byte retail vector<float> (PDB 16).
// Shape: base part out of line, then every member in declaration order. The CDefString arrays copy as an
// integer element loop, the float arrays as an x87 element loop (`fld [edx+eax] ; fstp [edx]`), the
// scalars (floats included) as dword moves, and each vector<float> through its out-of-line operator=.
// Layout: engine/CSpecialAbilitiesPhysicalShieldDef.h is the PDB view (0x16c, 16-byte vectors, base 0x40);
// the retail object is 0x148 bytes, so the class is modelled locally on the landed CParentDefClassBase base.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CDefString {                         // a def-table string reference (TablePos)
    long TablePos;
};

struct CFloatVector {                       // retail std::vector<float> (12 bytes)
    float* m_first;
    float* m_last;
    float* m_end;
    CFloatVector& operator=(const CFloatVector& other);   // 0x0045EF2D
};

struct CSpecialAbilitiesPhysicalShieldDef : CParentDefClassBase {
    CDefString   InitSound;                     // +0x3c
    CDefString   PowerUpSound[3];               // +0x40
    CDefString   CastSound;                     // +0x4c
    CDefString   LoopingSound[4];               // +0x50
    CDefString   EndSound;                      // +0x60
    float        Health[4];                     // +0x64
    float        LevelUpTimings[3];             // +0x74
    float        CamDist[4];                    // +0x80
    float        CamHeight[4];                  // +0x90
    float        CamFOV[4];                     // +0xa0
    float        CamSwitchTime[4];              // +0xb0
    float        CastCamDist;                   // +0xc0
    float        CastCamHeight;                 // +0xc4
    float        CastCamFOV;                    // +0xc8
    float        CastCamSwitchTime;             // +0xcc
    float        MinWidth;                      // +0xd0
    CFloatVector MaxWidth;                      // +0xd4
    float        BeenHitExtraWidth;             // +0xe0
    float        BeenHitFadeOffTimeSecs;        // +0xe4
    float        WobbleRate;                    // +0xe8
    CFloatVector ColorStartLevel1;              // +0xec
    CFloatVector ColorStartLevel2;              // +0xf8
    CFloatVector ColorStartLevel3;              // +0x104
    CFloatVector ColorStartLevel4;              // +0x110
    CFloatVector ColorEnd;                      // +0x11c
    CFloatVector ColorHit;                      // +0x128
    float        FadeOutPhysicalShieldTimeSecs; // +0x134
    CFloatVector ManaDamage;                    // +0x138
    bool         ShieldCoversCarriedItems;      // +0x144
    CSpecialAbilitiesPhysicalShieldDef& operator=(const CSpecialAbilitiesPhysicalShieldDef& other);
};

CSpecialAbilitiesPhysicalShieldDef& CSpecialAbilitiesPhysicalShieldDef::operator=(const CSpecialAbilitiesPhysicalShieldDef& other)
{
    CParentDefClassBase::operator=(other);
    InitSound = other.InitSound;
    for (int i = 0; i < 3; ++i)
        PowerUpSound[i] = other.PowerUpSound[i];
    CastSound = other.CastSound;
    for (int i = 0; i < 4; ++i)
        LoopingSound[i] = other.LoopingSound[i];
    EndSound = other.EndSound;
    for (int i = 0; i < 4; ++i)
        Health[i] = other.Health[i];
    for (int i = 0; i < 3; ++i)
        LevelUpTimings[i] = other.LevelUpTimings[i];
    for (int i = 0; i < 4; ++i)
        CamDist[i] = other.CamDist[i];
    for (int i = 0; i < 4; ++i)
        CamHeight[i] = other.CamHeight[i];
    for (int i = 0; i < 4; ++i)
        CamFOV[i] = other.CamFOV[i];
    for (int i = 0; i < 4; ++i)
        CamSwitchTime[i] = other.CamSwitchTime[i];
    CastCamDist = other.CastCamDist;
    CastCamHeight = other.CastCamHeight;
    CastCamFOV = other.CastCamFOV;
    CastCamSwitchTime = other.CastCamSwitchTime;
    MinWidth = other.MinWidth;
    MaxWidth = other.MaxWidth;
    BeenHitExtraWidth = other.BeenHitExtraWidth;
    BeenHitFadeOffTimeSecs = other.BeenHitFadeOffTimeSecs;
    WobbleRate = other.WobbleRate;
    ColorStartLevel1 = other.ColorStartLevel1;
    ColorStartLevel2 = other.ColorStartLevel2;
    ColorStartLevel3 = other.ColorStartLevel3;
    ColorStartLevel4 = other.ColorStartLevel4;
    ColorEnd = other.ColorEnd;
    ColorHit = other.ColorHit;
    FadeOutPhysicalShieldTimeSecs = other.FadeOutPhysicalShieldTimeSecs;
    ManaDamage = other.ManaDamage;
    ShieldCoversCarriedItems = other.ShieldCoversCarriedItems;
    return *this;
}