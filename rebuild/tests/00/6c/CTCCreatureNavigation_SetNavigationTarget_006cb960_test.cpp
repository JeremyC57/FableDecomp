// Behaviour test for 0x006CB960 (CTCCreatureNavigation::SetNavigationTarget): the whole 24-byte
// navigation input is copied into NavigationTarget.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
enum EMovementType { MOVEMENT_TYPE_DEFAULT, MOVEMENT_TYPE_1, MOVEMENT_TYPE_2, MOVEMENT_TYPE_3 };
class CNavigationInput { public:
    C3DVector     Destination;
    float         Proximity;
    EMovementType MovementType;
    float         Speed;
};

class CTCCreatureNavigation { public:
    long             m_before;
    CNavigationInput NavigationTarget;
    long             m_after;
    void SetNavigationTarget(const CNavigationInput& target);
};

void CTCCreatureNavigation::SetNavigationTarget(const CNavigationInput& target)
{
    NavigationTarget = target;
}

int main()
{
    CTCCreatureNavigation nav;
    nav.m_before = 5; nav.m_after = 6;
    CNavigationInput in = { { 1.0f, 2.0f, 3.0f }, 0.5f, MOVEMENT_TYPE_3, 4.0f };
    nav.SetNavigationTarget(in);
    if (nav.NavigationTarget.Destination.Y != 2.0f || nav.NavigationTarget.Proximity != 0.5f ||
        nav.NavigationTarget.MovementType != MOVEMENT_TYPE_3 || nav.NavigationTarget.Speed != 4.0f || nav.m_before != 5 || nav.m_after != 6) {
        std::printf("006cb960_TEST FAIL\n");
        return 1;
    }
    std::printf("006cb960_TEST PASS\n");
    return 0;
}