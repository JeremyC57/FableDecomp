// Behaviour test for CSpecialAbilitiesPhysicalShieldDef::CSpecialAbilitiesPhysicalShieldDef() (0x00457776).
#include <cstdio>
#include <new>
static int g_baseCalls = 0;
class CParentDefClassBase {                        // 0x3C bytes in retail
public:
    virtual void v000() {}
    unsigned long _body[0x0E];
    CParentDefClassBase() { g_baseCalls++; }
};
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) {}                 // out-of-line copy 0x0042D9D0 for the arrays
};
template <class T> struct CRetailVector {          // retail std::vector<T> (12 bytes)
    T* m_first; T* m_last; T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) {}
};
class CSpecialAbilitiesPhysicalShieldDef : public CParentDefClassBase {
public:
    virtual void v000() {}
    CDefString           InitSound;                     // +0x3c
    CDefString           PowerUpSound[3];               // +0x40
    CDefString           CastSound;                     // +0x4c
    CDefString           LoopingSound[4];               // +0x50
    CDefString           EndSound;                      // +0x60
    float                Health[4];                     // +0x64
    float                LevelUpTimings[3];             // +0x74
    float                CamDist[4];                    // +0x80
    float                CamHeight[4];                  // +0x90
    float                CamFOV[4];                     // +0xa0
    float                CamSwitchTime[4];              // +0xb0
    float                CastCamDist;                   // +0xc0
    float                CastCamHeight;                 // +0xc4
    float                CastCamFOV;                    // +0xc8
    float                CastCamSwitchTime;             // +0xcc
    float                MinWidth;                      // +0xd0
    CRetailVector<float> MaxWidth;                      // +0xd4
    float                BeenHitExtraWidth;             // +0xe0
    float                BeenHitFadeOffTimeSecs;        // +0xe4
    float                WobbleRate;                    // +0xe8
    CRetailVector<float> ColorStartLevel1;              // +0xec
    CRetailVector<float> ColorStartLevel2;              // +0xf8
    CRetailVector<float> ColorStartLevel3;              // +0x104
    CRetailVector<float> ColorStartLevel4;              // +0x110
    CRetailVector<float> ColorEnd;                      // +0x11c
    CRetailVector<float> ColorHit;                      // +0x128
    float                FadeOutPhysicalShieldTimeSecs; // +0x134
    CRetailVector<float> ManaDamage;                    // +0x138
    bool                 ShieldCoversCarriedItems;      // +0x144
    CSpecialAbilitiesPhysicalShieldDef();
};
CSpecialAbilitiesPhysicalShieldDef::CSpecialAbilitiesPhysicalShieldDef()
{
}

static bool AllByte(const void* p, unsigned n, unsigned char v) { const unsigned char* b = (const unsigned char*)p; for (unsigned i = 0; i < n; ++i) if (b[i] != v) return false; return true; }
template <class T> static bool Empty(const CRetailVector<T>& v) { return v.m_first == 0 && v.m_last == 0 && v.m_end == 0; }
int main()
{
    static unsigned char raw[sizeof(CSpecialAbilitiesPhysicalShieldDef)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xcd;
    CSpecialAbilitiesPhysicalShieldDef* d = new (raw) CSpecialAbilitiesPhysicalShieldDef();
    if (g_baseCalls != 1 || d->InitSound.TablePos != -1 || d->CastSound.TablePos != -1 || d->EndSound.TablePos != -1) { std::printf("00457776_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (d->PowerUpSound[i].TablePos != -1) { std::printf("00457776_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) if (d->LoopingSound[i].TablePos != -1) { std::printf("00457776_TEST FAIL\n"); return 1; }
    if (!Empty(d->MaxWidth) || !Empty(d->ColorStartLevel1) || !Empty(d->ColorStartLevel2) || !Empty(d->ColorStartLevel3) || !Empty(d->ColorStartLevel4)
        || !Empty(d->ColorEnd) || !Empty(d->ColorHit) || !Empty(d->ManaDamage)) { std::printf("00457776_TEST FAIL\n"); return 1; }
    /* the float tables and the flag are left for the def loader */
    if (!AllByte(d->Health, sizeof(d->Health), 0xcd) || !AllByte(&d->MinWidth, 4, 0xcd) || !AllByte(&d->WobbleRate, 4, 0xcd)
        || !AllByte(&d->FadeOutPhysicalShieldTimeSecs, 4, 0xcd) || !AllByte(&d->ShieldCoversCarriedItems, 1, 0xcd)) { std::printf("00457776_TEST FAIL\n"); return 1; }
    std::printf("00457776_TEST PASS\n");
    return 0;
}