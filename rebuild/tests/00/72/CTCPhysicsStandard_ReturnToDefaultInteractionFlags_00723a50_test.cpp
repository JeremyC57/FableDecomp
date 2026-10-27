// Behaviour test for CTCPhysicsStandard::ReturnToDefaultInteractionFlags (0x00723A50): virtual SetInteractionFlags(def's InteractionFlags).
#include <cstdio>
class CPhysicsDef { public: float Mass; int InteractionFlags; };
class CTCPhysicsBase { public:
    const CPhysicsDef* PhysicsDef;
    int Flags;
    virtual ~CTCPhysicsBase() {}
    virtual void SetInteractionFlags(int) {}
    virtual void ReturnToDefaultInteractionFlags() {}
};
class CTCPhysicsStandard : public CTCPhysicsBase { public:
    void SetInteractionFlags(int f) { Flags = f; }
    void ReturnToDefaultInteractionFlags() { SetInteractionFlags(PhysicsDef->InteractionFlags); }
};
int main()
{
    CPhysicsDef def; def.InteractionFlags = 0x15;
    CTCPhysicsStandard s; s.PhysicsDef = &def; s.Flags = 0;
    CTCPhysicsBase& b = s;
    b.ReturnToDefaultInteractionFlags();
    if (s.Flags != 0x15) { std::printf("00723a50_TEST FAIL\n"); return 1; }
    std::printf("00723a50_TEST PASS\n");
    return 0;
}