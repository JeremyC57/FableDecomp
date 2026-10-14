// NUISystem::CListArrow::ChangeState(long state) -- only while the arrow is armed (byte +0x18c): the CChangingStateComponent base changes state (0x0052CF40) and the arrow is disarmed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054C270, length 48 bytes.
// Leaf: ChangeState
// Callees: CChangingStateComponent::ChangeState @ 0x0052CF40. Shape: `this` in esi (ecx still holds it for the base call), `ret 4`.
struct CChangingStateComponent_0054c270 {
    unsigned char _pad00[0x18c];
    void ChangeState(long state);                  // 0x0052CF40
};

namespace NUISystem {
struct CListArrow : CChangingStateComponent_0054c270 {
    bool m_armed;                                  // +0x18c
    void ChangeState(long state);
};
}

void NUISystem::CListArrow::ChangeState(long state)
{
    if (m_armed) {
        CChangingStateComponent_0054c270::ChangeState(state);
        m_armed = false;
    }
}
