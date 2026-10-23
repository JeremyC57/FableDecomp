// Grade: DIFFER(5v1784) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CModeBriarRoseConcentrate::OnActivate() -- Briar Rose's concentrate mode: base stand activation, suspend animation,
//   create the DRAIN_CHARGE_SWORD / DRAIN_CHARGE_BODY / WEAPON_FOCUS_01 effects, add CTCAbsorbHits and spawn the
//   CREATURE_RIVAL_HERO_BRIAR_ROSE_IMPOSTER copies at the TeleportMarker things
// Leaf: OnActivate
// Stub: x87-heavy 0x190 aligned frame; first-pass skeleton only, not reconstructed yet.
class CModeStandBase {
public:
    virtual void OnActivate();                         // 0x008621A0
};
class CModeBriarRoseConcentrate : public CModeStandBase {
public:
    virtual void OnActivate();
};
void CModeBriarRoseConcentrate::OnActivate()
{
    CModeStandBase::OnActivate();
}
