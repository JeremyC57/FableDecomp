#include <cstdio>
// Behaviour test for 0x005DC1D0 CAnimationEntry::GetTransitionTime: links against the source object; GetComponent<CAnimComponentTransitionInTime> is
// stubbed (the entry's own component, else a default one) so the returned field is observable.
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    unsigned long IVRefCount;
};
class CAnimComponentBase : public CIVCountedPointeeBase { };
class CAnimComponentTransitionInTime : public CAnimComponentBase {
public:
    long TransitionInTime;
};
class CAnimationEntry {
public:
    template <class T> void GetComponent(const T** component) const;
    long GetTransitionTime() const;
    const CAnimComponentTransitionInTime* Own;
};
static CAnimComponentTransitionInTime g_default;
static int g_calls = 0;
template <> void CAnimationEntry::GetComponent<CAnimComponentTransitionInTime>(const CAnimComponentTransitionInTime** component) const
{
    ++g_calls;
    *component = Own ? Own : &g_default;
}
int main()
{
    g_default.TransitionInTime = 11;
    CAnimComponentTransitionInTime own; own.TransitionInTime = 250;
    CAnimationEntry entry; entry.Own = &own;
    if (entry.GetTransitionTime() != 250 || g_calls != 1) { std::printf("005dc1d0_TEST FAIL own\n"); return 1; }
    entry.Own = 0;
    if (entry.GetTransitionTime() != 11 || g_calls != 2) { std::printf("005dc1d0_TEST FAIL default\n"); return 1; }
    own.TransitionInTime = -3; entry.Own = &own;
    if (entry.GetTransitionTime() != -3) { std::printf("005dc1d0_TEST FAIL negative\n"); return 1; }
    std::printf("005dc1d0_TEST PASS\n");
    return 0;
}
