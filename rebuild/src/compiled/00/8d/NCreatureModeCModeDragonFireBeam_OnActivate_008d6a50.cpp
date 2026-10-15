// NCreatureMode::CModeDragonFireBeam::OnActivate() -- the CModeStandBase activation (0x008621A0), the animation suspended on a player creature (type byte +0x10 == 1: SetSuspendAnimation(true, this) @ 0x00833A10), then the beam state (+0x8c) and flag (+0x90) cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6A50, length 41 bytes.
// Callees: CModeStandBase::OnActivate @ 0x008621A0, CThingAnimatable::SetSuspendAnimation @ 0x00833A10 (thiscall on the thing, (bool, CModeBase*), `ret 8`). Vtable slot 5 of CModeDragonFireBeam (rtti-vtables.tsv; the chunk label CModeDragonRearUp is propagated). Shape: `this` in esi, one zero register for the dword and the byte, `ret`.
namespace NCreatureMode {
struct CModeBase;
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                          // +0x10 (1 = the player creature)
    void SetSuspendAnimation(bool suspend, CModeBase* mode);   // 0x00833A10
};

struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    CThing* m_pThing;                              // +0x04
};

struct CModeStandBase : CModeBase {
    unsigned char _pad08[0x60];                    // +0x08 .. +0x68
    void OnActivate();                             // 0x008621A0
};

struct CModeDragonFireBeam : CModeStandBase {
    unsigned char _pad68[0x24];                    // +0x68 .. +0x8c
    long          m_beamState;                     // +0x8c
    bool          m_beamFlag;                      // +0x90
    virtual void OnActivate();
};

void CModeDragonFireBeam::OnActivate()
{
    CModeStandBase::OnActivate();
    if (m_pThing->m_type == 1)
        m_pThing->SetSuspendAnimation(true, this);
    m_beamState = 0;
    m_beamFlag = false;
}
}