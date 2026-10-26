// CTCObjectAugmentations::OnSetInLimbo(bool inLimbo) -- re-attach the augmentation particles and set the augmentation sounds up again (the limbo flag itself is not read)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00768310, length 19 bytes.
// Leaf: ?OnSetInLimbo@CTCObjectAugmentations@@UAEX_N@Z
// Label correction: the catalog "_Dest_val<..., std::pair<long, CMusicManager::CRegisteredMusicEntry>>" is propagated.
// The only reference is CTCObjectAugmentations vtable 0x0126A3A4 slot 16 (rtti-vtables, at 0x0126A3E4), the CTCBase
// OnSetInLimbo(bool) slot (as the landed CTCThingOwner::OnSetInLimbo 0x0071B2A0 and CTCSwitchableNavigation /
// CTCDParticleEmitter slot-16 labels); `ret 4` = the one bool argument, which the body ignores. Ego_r/FableWin:
// ?OnSetInLimbo@CTCObjectAugmentations@@UAEX_N@Z (virtual).
// Callees (Ego_r/FableWin, both private): CTCObjectAugmentations::AttachAugmentationParticles @ 0x007677E0,
//          CTCObjectAugmentations::SetupAugmentationSounds @ 0x00766C00 (landed under that name; the manifest's
//          "CTCObjectAugmentations_UpdateParticleMorphs" is a forge autoname).
// Layout: no engine header for CTCObjectAugmentations; `this` is only forwarded, so the model carries no members.
// Shape: `this` kept in esi across the first call and copied back into ecx for the second.
class CTCObjectAugmentations {
public:
    virtual void OnSetInLimbo(bool inLimbo);        // vtable 0x0126A3A4 slot 16
private:
    void AttachAugmentationParticles();     // 0x007677E0
    void SetupAugmentationSounds();         // 0x00766C00
};

void CTCObjectAugmentations::OnSetInLimbo(bool inLimbo)
{
    (void)inLimbo;
    AttachAugmentationParticles();
    SetupAugmentationSounds();
}
