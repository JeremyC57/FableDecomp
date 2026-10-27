// Behaviour test for CInputProcessGameBase::GetMouseWheelMovement (0x00687D40): forwards to the player interface.
#include <cstdio>
class CInputEvent { public: float Wheel; };
class CGamePlayerInterface { public:
    int Calls;
    float GetMouseWheelMovement(const CInputEvent& e) const { ++((CGamePlayerInterface*)this)->Calls; return e.Wheel * 2.0f; }
};
class CInputProcessGameBase { public:
    CGamePlayerInterface* PGamePlayerInterface;
    float GetMouseWheelMovement(const CInputEvent& event) const
    {
        float movement = PGamePlayerInterface->GetMouseWheelMovement(event);
        return movement;
    }
};
int main()
{
    CGamePlayerInterface gpi; gpi.Calls = 0;
    CInputProcessGameBase p; p.PGamePlayerInterface = &gpi;
    CInputEvent e; e.Wheel = -1.5f;
    if (p.GetMouseWheelMovement(e) != -3.0f || gpi.Calls != 1) { std::printf("00687d40_TEST FAIL\n"); return 1; }
    std::printf("00687d40_TEST PASS\n");
    return 0;
}