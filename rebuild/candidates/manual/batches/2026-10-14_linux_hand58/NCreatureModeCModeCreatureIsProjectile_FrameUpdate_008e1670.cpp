// NCreatureMode::CModeCreatureIsProjectile::FrameUpdate() -- the CModeBase frame update (0x0073AC50), then Recover (0x008E08C0) unless the creature's projectile state (+0x84) has bit 0x800000 of its flags (+0x2c) set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E1670, length 33 bytes.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50, CModeCreatureIsProjectile::Recover @ 0x008E08C0 (a sibling jump). Vtable slot 3 (rtti-vtables.tsv). Shape: `this` in esi, the state pointer and flag word tested in place, `ret`.
namespace NCreatureMode {
struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    virtual void v01();
    virtual void v02();
    virtual void FrameUpdate();                    // 0x0073AC50
};

struct SProjectileState_008e1670 {
    unsigned char _pad00[0x2c];
    unsigned long m_flags;                         // +0x2c
};

struct CModeCreatureIsProjectile : CModeBase {
    unsigned char             _pad04[0x80];
    SProjectileState_008e1670* m_pState;           // +0x84
    virtual void FrameUpdate();
    void Recover();                                // 0x008E08C0
};

void CModeCreatureIsProjectile::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (!(m_pState->m_flags & 0x800000))
        Recover();
}
}
