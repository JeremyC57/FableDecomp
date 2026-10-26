// CSpecialAbilitiesPhysicalShieldDef::~CSpecialAbilitiesPhysicalShieldDef() -- free the eight float vectors (ManaDamage, the six colour vectors, MaxWidth), then the CParentDefClassBase part
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045784B, length 147 bytes.
// Leaf: ??1CSpecialAbilitiesPhysicalShieldDef
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (`pop ecx` after each free).
// Label correction: the catalog "~CPatchTesselationEdgeStrip" is propagated over byte-similar def destructors. The only
// caller is 0x00457832 inside the deleting destructor 0x0045782F, which is slot 0 of CSpecialAbilitiesPhysicalShieldDef's
// vtable 0x0123439C.
// Callees: free @ 0x00BFEA14 (x8, the inline vector<float> destructors), the CParentDefClassBase destructor @ 0x00430300
// (manifest label "CTCInventoryItem::OnDie" is propagated; tail jump).
// Layout: Ego_r.pdb CSpecialAbilitiesPhysicalShieldDef with the retail 12-byte vectors: MaxWidth +0xd4, ColorStartLevel1..4
// +0xec..+0x110, ColorEnd +0x11c, ColorHit +0x128, ManaDamage +0x138 (each PDB vector is 16 bytes, so the later members
// drift by 4 per vector).
// Shape: as 0x004575FC -- empty destructor, `__forceinline` vector<float> destructors in reverse order, base tail jump.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* memory);        // 0x00BFEA14
class CParentDefClassBase { public:                       // retail 0x3c bytes (with the CDefClassBase part)
    unsigned char m_baseData[0x3c];
    ~CParentDefClassBase();                        // 0x00430300
};
struct CFloatVector {                              // retail std::vector<float> (12 bytes)
    float* m_first;
    float* m_last;
    float* m_end;
    __forceinline ~CFloatVector() { if (m_first) free(m_first); }
};
typedef long CDefString;                           // table position word
class C3DVector { public: float X, Y, Z; };
class CSpecialAbilitiesPhysicalShieldDef : public CParentDefClassBase { public:   // vtable 0x0123439C, retail 0x148 bytes (slot 20)
    CDefString   InitSound;                        // +0x3c
    CDefString   PowerUpSound[3];                  // +0x40
    CDefString   CastSound;                        // +0x4c
    CDefString   LoopingSound[4];                  // +0x50
    CDefString   EndSound;                         // +0x60
    float        Health[4];                        // +0x64
    float        LevelUpTimings[3];                // +0x74
    float        CamDist[4];                       // +0x80
    float        CamHeight[4];                     // +0x90
    float        CamFOV[4];                        // +0xa0
    float        CamSwitchTime[4];                 // +0xb0
    float        CastCamDist;                      // +0xc0
    float        CastCamHeight;                    // +0xc4
    float        CastCamFOV;                       // +0xc8
    float        CastCamSwitchTime;                // +0xcc
    float        MinWidth;                         // +0xd0
    CFloatVector MaxWidth;                         // +0xd4
    float        BeenHitExtraWidth;                // +0xe0
    float        BeenHitFadeOffTimeSecs;           // +0xe4
    float        WobbleRate;                       // +0xe8
    CFloatVector ColorStartLevel1;                 // +0xec
    CFloatVector ColorStartLevel2;                 // +0xf8
    CFloatVector ColorStartLevel3;                 // +0x104
    CFloatVector ColorStartLevel4;                 // +0x110
    CFloatVector ColorEnd;                         // +0x11c
    CFloatVector ColorHit;                         // +0x128
    float        FadeOutPhysicalShieldTimeSecs;    // +0x134
    CFloatVector ManaDamage;                       // +0x138
    bool         ShieldCoversCarriedItems;         // +0x144
    ~CSpecialAbilitiesPhysicalShieldDef();
};
CSpecialAbilitiesPhysicalShieldDef::~CSpecialAbilitiesPhysicalShieldDef()
{
}