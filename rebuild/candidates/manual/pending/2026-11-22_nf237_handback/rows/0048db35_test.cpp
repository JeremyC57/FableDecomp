// Behaviour test for 0x0048DB35 CUserProfileManager::IsSubtitlingEnabled (NF237): the row's model and body (no inline asm). Cases: the
// new-profile flag selects +0xCD, a clear flag selects +0x0D, and no other byte is read.
#include <cstdio>
#include <cstring>
class CUserProfileManager { public:
    char pad0[0xc];
    bool UseNewProfileValues;           // +0x0c
    bool SubtitlingEnabled;             // +0x0d
    char pad1[0xcd - 0x0e];
    bool SubtitlingEnabledNewProfile;   // +0xcd
    bool IsSubtitlingEnabled() const;
};
bool CUserProfileManager::IsSubtitlingEnabled() const
{
    if (UseNewProfileValues)
        return SubtitlingEnabledNewProfile;
    return SubtitlingEnabled;
}
int main()
{
    CUserProfileManager o;
    std::memset(&o, 0, sizeof(o));
    bool ok = true;
    o.UseNewProfileValues = true; o.SubtitlingEnabled = false; o.SubtitlingEnabledNewProfile = true;
    ok = ok && o.IsSubtitlingEnabled() == true;
    o.SubtitlingEnabledNewProfile = false; o.SubtitlingEnabled = true;
    ok = ok && o.IsSubtitlingEnabled() == false;
    o.UseNewProfileValues = false;
    ok = ok && o.IsSubtitlingEnabled() == true;
    o.SubtitlingEnabled = false; o.SubtitlingEnabledNewProfile = true;
    ok = ok && o.IsSubtitlingEnabled() == false;
    std::printf(ok ? "OK_0x0048db35\n" : "FAIL_0x0048db35\n");
    return ok ? 0 : 1;
}
