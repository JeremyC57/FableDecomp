// Behaviour test for 0x00860300 CVillageTaskSecuritySweep::IsValid() const: valid until HasBecomeInvalid is set.
#include <cstdio>

class SecuritySweep {
public:
    bool HasBecomeInvalid;
    bool IsValid() const { return !HasBecomeInvalid; }
};

int main()
{
    SecuritySweep s;
    s.HasBecomeInvalid = false;
    if (!s.IsValid()) { std::printf("00860300_TEST FAIL\n"); return 1; }
    s.HasBecomeInvalid = true;
    if (s.IsValid()) { std::printf("00860300_TEST FAIL\n"); return 1; }
    std::printf("00860300_TEST PASS\n");
    return 0;
}