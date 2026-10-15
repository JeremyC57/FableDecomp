// CTCObjectAugmentations::GetOnHitExplodingEffect() const -- the def's on-hit effect for fire+lightning, fire or lightning augmentations, else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00766480, length 81 bytes.
// Callees: CTCObjectAugmentations::HasCombinationOfAugmentations(type) @ 0x00766050 (0xc = fire|lightning),
// HasAugmentationOfType(type) @ 0x007660C0 (4 = fire, 8 = lightning); both bool.
// CObjectAugmentationsDef (retail offsets, PDB minus 0x14 for its five 4-byte-narrower containers):
// FireProjectileOnHitEffect +0x7c, LightningProjectileOnHitEffect +0x80, FireAndLightningProjectileOnHitEffect +0x84.
// Shape: a zero-initialised result local (edi = 0 up front) assigned in an if/else-if chain; each arm returns through its
// own epilogue, the fall-through returns the local.
struct CObjectAugmentationsDef {
    unsigned char _pad00[0x7c];
    long          FireProjectileOnHitEffect;        // +0x7c
    long          LightningProjectileOnHitEffect;   // +0x80
    long          FireAndLightningProjectileOnHitEffect;   // +0x84
};

struct CTCBase_00766480 {
    virtual void v00();
    unsigned char _pad04[8];
};

struct CTCObjectAugmentations : CTCBase_00766480 {
    const CObjectAugmentationsDef* PDef;            // +0x0c CDefPointer
    bool HasCombinationOfAugmentations(long types) const;   // 0x00766050
    bool HasAugmentationOfType(long type) const;    // 0x007660C0
    long GetOnHitExplodingEffect() const;
};

long CTCObjectAugmentations::GetOnHitExplodingEffect() const
{
    long effect = 0;
    if (HasCombinationOfAugmentations(0xc))
        effect = PDef->FireAndLightningProjectileOnHitEffect;
    else if (HasAugmentationOfType(4))
        effect = PDef->FireProjectileOnHitEffect;
    else if (HasAugmentationOfType(8))
        effect = PDef->LightningProjectileOnHitEffect;
    return effect;
}
