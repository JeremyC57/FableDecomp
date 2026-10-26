// Test for 006cb6c0 CTCCreatureNavigation::FindPosZAt: forwards to the navigator's interface with the flag set.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CNavigatorExternalInterface { public:
    bool LastFlag; const C3DVector* LastPos;
    virtual ~CNavigatorExternalInterface() {}
    virtual float FindPosZAt(const C3DVector& pos, bool flag) { LastFlag = flag; LastPos = &pos; return pos.X + pos.Y + 0.5f; }
};
class CNavigatorBase { public:
    CNavigatorExternalInterface* PInterface;
    float FindPosZAt(const C3DVector& pos) const { return PInterface->FindPosZAt(pos, true); }
};
class CTCCreatureNavigation { public:
    CNavigatorBase* PNavigator;
    float FindPosZAt(const C3DVector& pos) const { return PNavigator->FindPosZAt(pos); }
};
int main()
{
    CNavigatorExternalInterface i; i.LastFlag = false; i.LastPos = 0;
    CNavigatorBase n = { &i };
    CTCCreatureNavigation c = { &n };
    C3DVector p = { 1.0f, 2.0f, 9.0f };
    float z = c.FindPosZAt(p);
    if (z != 3.5f || !i.LastFlag || i.LastPos != &p) { std::printf("006cb6c0_TEST FAIL\n"); return 1; }
    std::printf("006cb6c0_TEST PASS\n");
    return 0;
}