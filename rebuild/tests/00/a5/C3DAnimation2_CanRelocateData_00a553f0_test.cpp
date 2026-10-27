// Behaviour test for 0x00A553F0 C3DAnimation2::CanRelocateData(long) const (CMovableResource override)
#include <cstdio>
class CMovableResource {
public:
    virtual ~CMovableResource() { }
    virtual bool CanRelocateData(long delta) const = 0;
};
class C3DAnimation2 : public CMovableResource {
public:
    bool LockedFromMemMemoryRelocation;
    virtual bool CanRelocateData(long) const { return !LockedFromMemMemoryRelocation; }
};
int main()
{
    C3DAnimation2 anim;
    CMovableResource* res = &anim;
    anim.LockedFromMemMemoryRelocation = false;
    if (!res->CanRelocateData(16)) { std::printf("00a553f0_TEST FAIL\n"); return 1; }
    anim.LockedFromMemMemoryRelocation = true;
    if (res->CanRelocateData(16)) { std::printf("00a553f0_TEST FAIL\n"); return 1; }
    std::printf("00a553f0_TEST PASS\n");
    return 0;
}