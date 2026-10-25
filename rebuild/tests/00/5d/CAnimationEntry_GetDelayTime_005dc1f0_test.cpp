#include <cstdio>
// Behaviour test for 0x005DC1F0 CAnimationEntry::GetDelayTime: links against the source object; GetComponent<CAnimComponentDelayTime> is
// stubbed (the entry's own component, else a default one) so the returned field is observable.
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    unsigned long IVRefCount;
};
class CAnimComponentBase : public CIVCountedPointeeBase { };
class CAnimComponentDelayTime : public CAnimComponentBase {
public:
    long Delay;
};
class CAnimationEntry {
public:
    template <class T> void GetComponent(const T** component) const;
    long GetDelayTime() const;
    const CAnimComponentDelayTime* Own;
};
static CAnimComponentDelayTime g_default;
static int g_calls = 0;
template <> void CAnimationEntry::GetComponent<CAnimComponentDelayTime>(const CAnimComponentDelayTime** component) const
{
    ++g_calls;
    *component = Own ? Own : &g_default;
}
int main()
{
    g_default.Delay = 11;
    CAnimComponentDelayTime own; own.Delay = 250;
    CAnimationEntry entry; entry.Own = &own;
    if (entry.GetDelayTime() != 250 || g_calls != 1) { std::printf("005dc1f0_TEST FAIL own\n"); return 1; }
    entry.Own = 0;
    if (entry.GetDelayTime() != 11 || g_calls != 2) { std::printf("005dc1f0_TEST FAIL default\n"); return 1; }
    own.Delay = -3; entry.Own = &own;
    if (entry.GetDelayTime() != -3) { std::printf("005dc1f0_TEST FAIL negative\n"); return 1; }
    std::printf("005dc1f0_TEST PASS\n");
    return 0;
}
