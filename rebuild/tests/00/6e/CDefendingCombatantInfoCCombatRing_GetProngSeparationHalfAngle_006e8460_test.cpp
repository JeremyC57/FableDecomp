// Behaviour test (0x006e8460): CCombatRing::GetProngSeparationHalfAngle is half of 1/prong-count
#include <cstdio>
#include <cmath>
class C3DVector { public: float X, Y, Z; };
class CProng { public: bool Blocked; C3DVector Prong; long ProngBlockageTimer; unsigned char POccupier[8]; };
class CProngVector {
public:
    CProng* _M_start; CProng* _M_finish; CProng* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
class CCombatRing {
public:
    float        RotationAngle;
    float        DeltaRotation;
    long         Index;
    float        RingThickness;
    CProngVector Prongs;
    float GetProngSeparationHalfAngle() const
    {
        long count = Prongs.size();
        return (1.0f / count) * 0.5f;
    }
};
int main()
{
    CProng prongs[8];
    CCombatRing ring; ring.Prongs._M_start = prongs; ring.Prongs._M_end_of_storage = prongs + 8;
    ring.Prongs._M_finish = prongs + 4;
    if (std::fabs(ring.GetProngSeparationHalfAngle() - 0.125f) > 1e-6f) { std::printf("006e8460_TEST FAIL\n"); return 1; }
    ring.Prongs._M_finish = prongs + 1;
    if (std::fabs(ring.GetProngSeparationHalfAngle() - 0.5f) > 1e-6f) { std::printf("006e8460_TEST FAIL\n"); return 1; }
    ring.Prongs._M_finish = prongs + 8;
    if (std::fabs(ring.GetProngSeparationHalfAngle() - 0.0625f) > 1e-6f) { std::printf("006e8460_TEST FAIL\n"); return 1; }
    std::printf("006e8460_TEST PASS\n");
    return 0;
}