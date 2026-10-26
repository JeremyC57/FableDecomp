// CTCPhysicsBase::GetPhysicsStateFlags(SPhysicsStateFlags* out) -- when the global physics-state switch is on, fill a flag record from the Moving bit and five boolean virtual queries and return true; otherwise return false (vtable slot 88, catalog "vfunc_88"; name descriptive)
// Leaf: GetPhysicsStateFlags
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFBF0, 100 bytes. CTCPhysicsBase vtable slot 88 (0x006AFBF0 in the RTTI table).
// Callees: CTCPhysicsBase virtual slots 52 (+0xd0), 55 (+0xdc), 56 (+0xe0), 57 (+0xe4), 60 (+0xf0); the base bodies of 54..60 are
//   `xor al,al ; ret`; the CTCPhysicsStandard overrides read the PDB bits (52 Immoveable +0x85 bit 3, 55 OnMesh +0x84 bit 5,
//   56 OnGround bit 4, 57 CollidesWithThings bit 7, 60 InteractsWithThings bit 6), which name them. Global byte 0x013B83C5.
// Layout: CTCPhysicsBase +0x3c bit 0 = Moving (engine/CTCPhysicsBase.h bitfield group). The out record holds bools at
//   +0, +1, +8, +9, +0xa, +0xb (descriptive model).
// Shape: stores in source order (Moving first, then slot 56, 55, 57, 60, 52); the virtual calls reuse esi = this.
extern bool g_bPhysicsStateFlagsEnabled;        // 0x013B83C5
struct SPhysicsStateFlags {
    bool IsOnMesh;                               // +0x00
    bool IsOnGround;                               // +0x01
    unsigned char _pad02[6];
    bool Moving;                                // +0x08
    bool GetCollidesWithThings;                               // +0x09
    bool GetInteractsWithThings;                               // +0x0a
    bool IsImmoveable;                               // +0x0b
};
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual bool IsImmoveable();   // slot 52 (+0xd0)
    virtual void v053(); virtual void v054();
    virtual bool IsOnMesh();   // slot 55 (+0xdc)
    virtual bool IsOnGround();   // slot 56 (+0xe0)
    virtual bool GetCollidesWithThings();   // slot 57 (+0xe4)
    virtual void v058(); virtual void v059();
    virtual bool GetInteractsWithThings();   // slot 60 (+0xf0)
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066();
    virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087();
    virtual bool GetPhysicsStateFlags(SPhysicsStateFlags* out);   // slot 88 (+0x160)
    void*         ParentThing;                  // +0x04
    unsigned char _pad08[0x34];
    unsigned char Moving : 1;                   // +0x3c bit 0
    unsigned char OldMoving : 1;                // +0x3c bit 1
};

bool CTCPhysicsBase::GetPhysicsStateFlags(SPhysicsStateFlags* out)
{
    if (g_bPhysicsStateFlagsEnabled) {
        out->Moving = Moving;
        out->IsOnGround = IsOnGround();
        out->IsOnMesh = IsOnMesh();
        out->GetCollidesWithThings = GetCollidesWithThings();
        out->GetInteractsWithThings = GetInteractsWithThings();
        out->IsImmoveable = IsImmoveable();
        return true;
    }
    return false;
}