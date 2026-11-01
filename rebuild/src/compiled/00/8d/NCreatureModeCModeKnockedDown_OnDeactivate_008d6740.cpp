// NCreatureMode::CModeKnockedDown::OnDeactivate() -- resume the animation of a player creature (type byte +0x10 == 1: SuspendAI(false, 0) @ 0x00833A10) and clear the knocked-down flag byte at +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6740, length 27 bytes.
// Callees: CThingAICreature::SuspendAI @ 0x00833A10 (thiscall on the thing, (bool, CModeBase*), `ret 8`). Vtable slot 6 of CModeKnockedDown (rtti-vtables.tsv). Shape: `this` in esi, no base call, `ret`.
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

struct CModeKnockedDown : CModeBase {
    unsigned char _pad08[0x10];                    // +0x08 .. +0x18
    bool          m_knockedDown;                   // +0x18
    virtual void OnDeactivate();
};

void CModeKnockedDown::OnDeactivate()
{
    if (m_pThing->m_type == 1)
        ((CThingAICreature*)m_pThing)->SuspendAI(false, 0);
    m_knockedDown = false;
}
}