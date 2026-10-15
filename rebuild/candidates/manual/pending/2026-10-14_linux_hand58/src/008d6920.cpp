// NCreatureMode::CModeSummonerDie::OnActivate() -- the CModeStandBase activation (0x008621A0), the animation suspended on a player creature (type byte +0x10 == 1: SetSuspendAnimation(true, 0) @ 0x00833A10), then the frame counter at +0x74 set to twice the global frame rate (0x01375550, a long converted through the FPU and back with __ftol2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6920, length 44 bytes.
// Callees: CModeStandBase::OnActivate @ 0x008621A0, CThingAnimatable::SetSuspendAnimation @ 0x00833A10, __ftol2 @ 0x00BFEA70. Vtable slot 5 of CModeSummonerDie (rtti-vtables.tsv). Shape: `fild ; fadd st,st` (2.0f * x strength-reduced to x + x), `ret`.
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
}

extern long g_framesPerSecond;                     // 0x01375550

namespace NCreatureMode {
struct CModeSummonerDie : CModeStandBase {
    unsigned char _pad68[0xc];                     // +0x68 .. +0x74
    long          m_framesToDie;                   // +0x74
    virtual void OnActivate();
};

void CModeSummonerDie::OnActivate()
{
    CModeStandBase::OnActivate();
    if (m_pThing->m_type == 1)
        m_pThing->SetSuspendAnimation(true, 0);
    m_framesToDie = (long)(g_framesPerSecond * 2.0f);
}
}
