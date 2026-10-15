// NCreatureMode::CModeKnockedDown::OnDeactivate() -- resume the animation of a player creature (type byte +0x10 == 1: SetSuspendAnimation(false, 0) @ 0x00833A10) and clear the knocked-down flag byte at +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6740, length 27 bytes.
// Callees: CThingAnimatable::SetSuspendAnimation @ 0x00833A10 (thiscall on the thing, (bool, CModeBase*), `ret 8`). Vtable slot 6 of CModeKnockedDown (rtti-vtables.tsv). Shape: `this` in esi, no base call, `ret`.
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

struct CModeKnockedDown : CModeBase {
    unsigned char _pad08[0x10];                    // +0x08 .. +0x18
    bool          m_knockedDown;                   // +0x18
    virtual void OnDeactivate();
};

void CModeKnockedDown::OnDeactivate()
{
    if (m_pThing->m_type == 1)
        m_pThing->SetSuspendAnimation(false, 0);
    m_knockedDown = false;
}
}