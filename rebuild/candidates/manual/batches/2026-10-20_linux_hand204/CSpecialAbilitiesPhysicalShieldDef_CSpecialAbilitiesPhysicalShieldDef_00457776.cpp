// CSpecialAbilitiesPhysicalShieldDef::CSpecialAbilitiesPhysicalShieldDef() -- default ctor: def base, own vtable, the five sound CDefStrings (two of them arrays) unset, and eight empty float vectors
// Leaf: ??0CSpecialAbilitiesPhysicalShieldDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00457776, 179 bytes (reached by a `jmp` from 0x004600BE). Vtable 0x0123439C.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370, `vector constructor iterator' @ 0x004038C0 (PowerUpSound
//   3 x 4 bytes, LoopingSound 4 x 4 bytes) with CDefString::CDefString @ 0x0042D9D0.
// Layout: Ego_r.pdb CSpecialAbilitiesPhysicalShieldDef (0x16c) minus 4 (0x3C-byte retail def base) with 12-byte retail
//   vectors, so engine/CSpecialAbilitiesPhysicalShieldDef.h (PDB offsets, 16-byte vectors) does not fit and the class is
//   modelled locally: InitSound +0x3c, PowerUpSound +0x40, CastSound +0x4c, LoopingSound +0x50, EndSound +0x60, the
//   float tables +0x64..+0xd3 (left unset), MaxWidth +0xd4, ColorStartLevel1..4 +0xec/+0xf8/+0x104/+0x110, ColorEnd
//   +0x11c, ColorHit +0x128, ManaDamage +0x138, ShieldCoversCarriedItems +0x144.
// Shape: size-optimised def module (`or [m],-1`, `lea ecx` vector zeroing through one shared zero register): the lone
//   CDefStrings use the inline `TablePos(-1)` ctor, the arrays go through the iterator with the out-of-line copy (held
//   in edi for both calls); an empty ctor body, as the landed CSpecialAbilitiesMultiStrikeDef ctor 0x00457178.
#pragma optimize("s", on)
class CParentDefClassBase {                        // 0x3C bytes in retail
public:
    virtual void v000();
    unsigned long _body[0x0E];
    CParentDefClassBase();                         // 0x00430370
};
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) {}                 // out-of-line copy 0x0042D9D0 for the arrays
};
template <class T> struct CRetailVector {          // retail std::vector<T> (12 bytes)
    T* m_first; T* m_last; T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) {}
};
class CSpecialAbilitiesPhysicalShieldDef : public CParentDefClassBase {   // vtable 0x0123439C
public:
    virtual void v000();
    CDefString           InitSound;                     // +0x3c
    CDefString           PowerUpSound[3];               // +0x40
    CDefString           CastSound;                     // +0x4c
    CDefString           LoopingSound[4];               // +0x50
    CDefString           EndSound;                      // +0x60
    float                Health[4];                     // +0x64
    float                LevelUpTimings[3];             // +0x74
    float                CamDist[4];                    // +0x80
    float                CamHeight[4];                  // +0x90
    float                CamFOV[4];                     // +0xa0
    float                CamSwitchTime[4];              // +0xb0
    float                CastCamDist;                   // +0xc0
    float                CastCamHeight;                 // +0xc4
    float                CastCamFOV;                    // +0xc8
    float                CastCamSwitchTime;             // +0xcc
    float                MinWidth;                      // +0xd0
    CRetailVector<float> MaxWidth;                      // +0xd4
    float                BeenHitExtraWidth;             // +0xe0
    float                BeenHitFadeOffTimeSecs;        // +0xe4
    float                WobbleRate;                    // +0xe8
    CRetailVector<float> ColorStartLevel1;              // +0xec
    CRetailVector<float> ColorStartLevel2;              // +0xf8
    CRetailVector<float> ColorStartLevel3;              // +0x104
    CRetailVector<float> ColorStartLevel4;              // +0x110
    CRetailVector<float> ColorEnd;                      // +0x11c
    CRetailVector<float> ColorHit;                      // +0x128
    float                FadeOutPhysicalShieldTimeSecs; // +0x134
    CRetailVector<float> ManaDamage;                    // +0x138
    bool                 ShieldCoversCarriedItems;      // +0x144
    CSpecialAbilitiesPhysicalShieldDef();
};
CSpecialAbilitiesPhysicalShieldDef::CSpecialAbilitiesPhysicalShieldDef()
{
}
