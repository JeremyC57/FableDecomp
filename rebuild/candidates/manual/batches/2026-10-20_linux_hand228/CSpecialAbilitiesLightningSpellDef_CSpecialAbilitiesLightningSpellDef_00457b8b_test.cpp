#include <cstdio>
#include <cstring>
#include <new>
static int g_baseCalls = 0;
struct CParentDefClassBase {
    virtual void v00() {}
    unsigned char _base[0x38];
    CParentDefClassBase() { ++g_baseCalls; }
};
struct CDefString {                             // def-table string index, -1 = none
    long TablePos;
    CDefString() : TablePos(-1) { }
};

template <class T>
struct CAllocProxy {                            // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    T* p;
    CAllocProxy(T* v) : p(v) { }
};
template <class T>
struct CRetailVector {                          // STLport _Vector_base: _M_start / _M_finish / _M_end_of_storage
    T* first;
    T* last;
    CAllocProxy<T> end;
    CRetailVector() : first(0), last(0), end(0) { }
};

struct CSpecialAbilitiesLightningSpellDef : CParentDefClassBase {
    CRetailVector<float> m_faAreaEffect;                // +0x3c
    CRetailVector<float> m_faDamagePerHit;              // +0x48
    CRetailVector<float> m_faSubBeamDamagePerHit;       // +0x54
    CRetailVector<float> m_faActivationTime;            // +0x60
    CRetailVector<float> m_faDeActivationTime;          // +0x6c
    CRetailVector<long>  m_naMinMainBeams;              // +0x78
    CRetailVector<long>  m_naMaxMainBeams;              // +0x84
    CRetailVector<long>  m_naMinSubBeams;               // +0x90
    CRetailVector<long>  m_naMaxSubBeams;               // +0x9c
    CRetailVector<float> m_faTemporaryBeamLife;         // +0xa8
    CRetailVector<float> m_faTargettingFOV;             // +0xb4
    CRetailVector<float> m_faStaminaPerSecond;          // +0xc0
    float m_fRandomBeamMinAngleX;                       // +0xcc
    float m_fRandomBeamMaxAngleX;                       // +0xd0
    float m_fRandomBeamMinAngleZ;                       // +0xd4
    float m_fRandomBeamMaxAngleZ;                       // +0xd8
    CRetailVector<float> m_faMainBeamDecapitationTime;  // +0xdc
    CRetailVector<float> m_faSubBeamDecapitationTime;   // +0xe8
    CRetailVector<float> m_faSubBeamDelay;              // +0xf4
    CRetailVector<float> OnhitEveryNSecs;               // +0x100
    float CombatMultiplierIncreaseTime;                 // +0x10c
    CDefString InitSound;                               // +0x110
    CDefString LoopingSound;                            // +0x114
    CDefString EndHeroSound;                            // +0x118
    CDefString EndAttackeeSound;                        // +0x11c
    float LightningThemeTransitionInSecs;               // +0x120
    float LightningThemeTransitionOutSecs;              // +0x124
    virtual void v00() {}
    CSpecialAbilitiesLightningSpellDef();
};

CSpecialAbilitiesLightningSpellDef::CSpecialAbilitiesLightningSpellDef()
    : CParentDefClassBase()
{
}
template <class T> static int Empty(const CRetailVector<T>& v) { return v.first == 0 && v.last == 0 && v.end.p == 0; }
int main()
{
    static unsigned char buf[sizeof(CSpecialAbilitiesLightningSpellDef) + 16];
    std::memset(buf, 0xcd, sizeof(buf));
    CSpecialAbilitiesLightningSpellDef* d = new (buf) CSpecialAbilitiesLightningSpellDef();
    int ok = g_baseCalls == 1;
    ok = ok && Empty(d->m_faAreaEffect);
    ok = ok && Empty(d->m_faDamagePerHit);
    ok = ok && Empty(d->m_faSubBeamDamagePerHit);
    ok = ok && Empty(d->m_faActivationTime);
    ok = ok && Empty(d->m_faDeActivationTime);
    ok = ok && Empty(d->m_naMinMainBeams);
    ok = ok && Empty(d->m_naMaxMainBeams);
    ok = ok && Empty(d->m_naMinSubBeams);
    ok = ok && Empty(d->m_naMaxSubBeams);
    ok = ok && Empty(d->m_faTemporaryBeamLife);
    ok = ok && Empty(d->m_faTargettingFOV);
    ok = ok && Empty(d->m_faStaminaPerSecond);
    ok = ok && Empty(d->m_faMainBeamDecapitationTime);
    ok = ok && Empty(d->m_faSubBeamDecapitationTime);
    ok = ok && Empty(d->m_faSubBeamDelay);
    ok = ok && Empty(d->OnhitEveryNSecs);
    ok = ok && d->InitSound.TablePos == -1 && d->LoopingSound.TablePos == -1 && d->EndHeroSound.TablePos == -1 && d->EndAttackeeSound.TablePos == -1;
    unsigned char raw[4]; std::memcpy(raw, &d->CombatMultiplierIncreaseTime, 4);
    ok = ok && raw[0] == 0xcd;
    if (!ok) { std::printf("00457b8b_TEST FAIL\n"); return 1; }
    std::printf("00457b8b_TEST PASS\n");
    return 0;
}
