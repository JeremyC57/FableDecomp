// NCreatureMode::CModeSummonerDie::OnActivate() -- the CModeStandBase activation (0x008621A0), the animation suspended on a player creature (type byte +0x10 == 1: SuspendAI(true, 0) @ 0x00833A10), then the frame counter at +0x74 set to twice the global frame rate (0x01375550, a long converted through the FPU and back with __ftol2)
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6920, length 44 bytes.
// Callees: CModeStandBase::OnActivate @ 0x008621A0, CThingAICreature::SuspendAI @ 0x00833A10, __ftol2 @ 0x00BFEA70. Vtable slot 5 of CModeSummonerDie (rtti-vtables.tsv). Shape: `fild ; fadd st,st` (2.0f * x strength-reduced to x + x), `ret`.
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
namespace NCreatureMode {
struct CModeBase;
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                          // +0x10 (1 = the player creature)
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

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

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
        ((CThingAICreature*)m_pThing)->SuspendAI(true, 0);
    m_framesToDie = (long)(CMainGameComponent::GetConstantFPS() * 2.0f);
}
}