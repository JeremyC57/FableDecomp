#include <cstdio>
// Behaviour test for 0x005DC210 CAnimationEntry::GetFlags: links against the source object; GetComponent<CAnimComponentFlags> is
// stubbed (the entry's own component, else a default one) so the returned field is observable.
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    unsigned long IVRefCount;
};
class CAnimComponentBase : public CIVCountedPointeeBase { };
class CAnimComponentFlags : public CAnimComponentBase {
public:
    long Flags;
};
class CAnimationEntry {
public:
    template <class T> void GetComponent(const T** component) const;
    long GetFlags() const;
    const CAnimComponentFlags* Own;
};
static CAnimComponentFlags g_default;
static int g_calls = 0;
template <> void CAnimationEntry::GetComponent<CAnimComponentFlags>(const CAnimComponentFlags** component) const
{
    ++g_calls;
    *component = Own ? Own : &g_default;
}
int main()
{
    g_default.Flags = 11;
    CAnimComponentFlags own; own.Flags = 250;
    CAnimationEntry entry; entry.Own = &own;
    if (entry.GetFlags() != 250 || g_calls != 1) { std::printf("005dc210_TEST FAIL own\n"); return 1; }
    entry.Own = 0;
    if (entry.GetFlags() != 11 || g_calls != 2) { std::printf("005dc210_TEST FAIL default\n"); return 1; }
    own.Flags = -3; entry.Own = &own;
    if (entry.GetFlags() != -3) { std::printf("005dc210_TEST FAIL negative\n"); return 1; }
    std::printf("005dc210_TEST PASS\n");
    return 0;
}
