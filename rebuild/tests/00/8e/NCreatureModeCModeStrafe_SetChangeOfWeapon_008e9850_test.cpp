// Behaviour test for 0x008E9850 CModeStrafe::SetChangeOfWeapon: weapon id + group from the weapon TC, or -1 + "STAND";
// combination anims rebuilt; aiming frames = fps * aim-anim seconds (0 without an aim entry), centre = half; clear called.
#include <cstdio>
#include <cstring>

static unsigned long Crc(const char* s, unsigned long n) { unsigned long h = 7; for (unsigned long i = 0; i < n; ++i) h = h * 31 + (unsigned char)s[i]; return h; }
class CCharString { public:
    const char* text;
    CCharString(const char* t) : text(t) {}
    const char* GetPString() const { return text ? text : ""; }
    unsigned long GetLength() const { return text ? (unsigned long)std::strlen(text) : 0; }
};
class CAnimFilterOfGroup { public:
    const char* Group; unsigned long GroupCRC;
    void SetGroup(const CCharString& g) { Group = g.GetPString(); GroupCRC = Crc(g.GetPString(), g.GetLength()); }
};
class CTCWeapon { public: CCharString group; const CCharString& GetAnimationGroupName() const { return group; } };
class CThing { public:
    unsigned long bits[9]; CTCWeapon* weapon; unsigned short m_weaponId;
    bool HasTC(long t) const { if (bits[t >> 5] & (1UL << (t & 31))) return true; return false; }
    const CTCWeapon* GetTCWeapon() const { return weapon; }
};
struct CAnimBankRecord { float Seconds; };
static CAnimBankRecord g_records[4] = { {0.5f}, {1.0f}, {2.0f}, {0.25f} };
static long g_framesPerSecond = 30;
struct CAnimEntryLite { long AnimIndex; };
static int g_setups, g_clears;
struct CModeStrafe {
    CAnimFilterOfGroup Filter; long ActiveWeapon, AimingNumFrames, AimingCenterFrame; CAnimEntryLite* aimData;
    void SetUpCombinationAnims() { ++g_setups; }
    void ClearStrafingCombinationAnims(bool) { ++g_clears; }
    void SetChangeOfWeapon(CThing* weapon)
    {
        if (weapon && weapon->HasTC(0x48)) {
            const CTCWeapon* tc = weapon->GetTCWeapon();
            ActiveWeapon = weapon->m_weaponId;
            Filter.SetGroup(tc->GetAnimationGroupName());
        } else {
            ActiveWeapon = -1;
            Filter.SetGroup("STAND");
        }
        SetUpCombinationAnims();
        CAnimEntryLite* aim = aimData;
        AimingNumFrames = 0;
        if (aim)
            AimingNumFrames = (long)(g_framesPerSecond * g_records[aim->AnimIndex].Seconds);
        AimingCenterFrame = AimingNumFrames / 2;
        ClearStrafingCombinationAnims(false);
    }
};
static int Fail(int n) { std::printf("008e9850_TEST FAIL %d\n", n); return 1; }
int main()
{
    CTCWeapon tcw = { CCharString("SWORD") };
    CThing sword; std::memset(sword.bits, 0, sizeof(sword.bits)); sword.bits[2] = 1UL << 8; sword.weapon = &tcw; sword.m_weaponId = 17;
    CAnimEntryLite aim = { 2 };
    CModeStrafe m; m.aimData = &aim;
    m.SetChangeOfWeapon(&sword);
    if (m.ActiveWeapon != 17 || std::strcmp(m.Filter.Group, "SWORD") || m.Filter.GroupCRC != Crc("SWORD", 5)) return Fail(1);
    if (m.AimingNumFrames != 60 || m.AimingCenterFrame != 30) return Fail(2);
    m.aimData = 0; m.SetChangeOfWeapon(0);
    if (m.ActiveWeapon != -1 || std::strcmp(m.Filter.Group, "STAND") || m.AimingNumFrames != 0 || m.AimingCenterFrame != 0) return Fail(3);
    sword.bits[2] = 0; aim.AnimIndex = 3; m.aimData = &aim; m.SetChangeOfWeapon(&sword);   // no weapon TC: treated as none
    if (m.ActiveWeapon != -1 || m.AimingNumFrames != 7 || m.AimingCenterFrame != 3) return Fail(4);
    if (g_setups != 3 || g_clears != 3) return Fail(5);
    std::printf("008e9850_TEST PASS\n");
    return 0;
}