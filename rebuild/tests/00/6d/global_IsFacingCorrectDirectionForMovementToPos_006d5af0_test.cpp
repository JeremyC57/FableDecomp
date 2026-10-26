// Behaviour test for 0x006D5AF0 IsFacingCorrectDirectionForMovementToPos: true unless the creature's interface-90
// component reports a state of kind 7; the brain output, look component and target position do not matter.
#include <cstdio>

struct CTurnState { unsigned char m_kind; };
struct CTCTurnSource { CTurnState* m_pState; CTurnState* GetState() { return m_pState; } };
class CThingAICreature { public:
    CTCTurnSource* m_source;
    bool GetInterface(long, CTCTurnSource** out) const { if (!m_source) return false; *out = m_source; return true; }
};
class CBrainOutput { public: int x; };
class CTCLook { public: int y; };
class C3DVector { public: float X, Y, Z; };

static bool IsFacingCorrectDirectionForMovementToPos(const CThingAICreature& creature, const CBrainOutput&, const CTCLook&, const C3DVector&)
{
    CTCTurnSource* source = 0;
    creature.GetInterface(90, &source);
    return source->GetState()->m_kind != 7 ? true : false;
}

int main()
{
    CTurnState st; CTCTurnSource src; src.m_pState = &st;
    CThingAICreature c; c.m_source = &src;
    CBrainOutput o; CTCLook l; C3DVector p = { 1.0f, 2.0f, 3.0f };
    st.m_kind = 7; if (IsFacingCorrectDirectionForMovementToPos(c, o, l, p)) { std::printf("006d5af0_TEST FAIL\n"); return 1; }
    st.m_kind = 3; if (!IsFacingCorrectDirectionForMovementToPos(c, o, l, p)) { std::printf("006d5af0_TEST FAIL\n"); return 1; }
    st.m_kind = 0; if (!IsFacingCorrectDirectionForMovementToPos(c, o, l, p)) { std::printf("006d5af0_TEST FAIL\n"); return 1; }
    std::printf("006d5af0_TEST PASS\n");
    return 0;
}