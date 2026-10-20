#include <cstdio>
#include <cstring>
template <class T> struct vector {               // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return _M_start[n]; }
};
struct CSpecialAbilitiesLightningSpellDef {
    unsigned char _pad_0x00[0x3c];               // CDefClassBase head (retail 0x3C bytes)
    vector<float> m_faAreaEffect;                // +0x3c
    vector<float> m_faDamagePerHit;              // +0x48
    vector<float> m_faSubBeamDamagePerHit;       // +0x54
    vector<float> m_faActivationTime;            // +0x60
    vector<float> m_faDeActivationTime;          // +0x6c
    vector<long>  m_naMinMainBeams;              // +0x78
    vector<long>  m_naMaxMainBeams;              // +0x84
    vector<long>  m_naMinSubBeams;               // +0x90
    vector<long>  m_naMaxSubBeams;               // +0x9c
    vector<float> m_faTemporaryBeamLife;         // +0xa8
    vector<float> m_faTargettingFOV;             // +0xb4
    vector<float> m_faStaminaPerSecond;          // +0xc0
    float m_fRandomBeamMinAngleX;                // +0xcc
    float m_fRandomBeamMaxAngleX;                // +0xd0
    float m_fRandomBeamMinAngleZ;                // +0xd4
    float m_fRandomBeamMaxAngleZ;                // +0xd8
    vector<float> m_faMainBeamDecapitationTime;  // +0xdc
    vector<float> m_faSubBeamDecapitationTime;   // +0xe8
    vector<float> m_faSubBeamDelay;              // +0xf4
    vector<float> OnhitEveryNSecs;               // +0x100
};
extern long g_FramesPerSecond;                   // 0x01375550
struct CTCWillLightning {
    void* __vftable;                             // +0x00
    unsigned char _pad_0x04[0x28 - 0x04];
    CSpecialAbilitiesLightningSpellDef* m_lspDef_Object;   // +0x28 CDefPointer<...>::Object
    unsigned char _pad_0x2c[0x9c - 0x2c];
    float m_fAreaEffect;                         // +0x9c
    float m_fDamagePerHit;                       // +0xa0
    float m_fSubBeamDamagePerHit;                // +0xa4
    float m_fActivationTime;                     // +0xa8
    float m_fDeActivationTime;                   // +0xac
    long  m_nMinMainBeams;                       // +0xb0
    long  m_nMaxMainBeams;                       // +0xb4
    long  m_nMinSubBeams;                        // +0xb8
    long  m_nMaxSubBeams;                        // +0xbc
    float m_fTemporaryBeamLife;                  // +0xc0
    float m_fTargettingFOV;                      // +0xc4
    float m_fStaminaPerSecond;                   // +0xc8
    float m_fMainBeamDecapitationTime;           // +0xcc
    float m_fSubBeamDecapitationTime;            // +0xd0
    float m_fSubBeamDelay;                       // +0xd4
    bool  m_bPlayedSound;                        // +0xd8
    long  m_OnhitEveryFrame;                     // +0xdc
    void SetLevel(long level);
};
void CTCWillLightning::SetLevel(long level)
{
    CSpecialAbilitiesLightningSpellDef* def = m_lspDef_Object;
    m_fAreaEffect = def->m_faAreaEffect[level];
    m_fDamagePerHit = def->m_faDamagePerHit[level];
    m_fSubBeamDamagePerHit = def->m_faSubBeamDamagePerHit[level];
    m_fActivationTime = def->m_faActivationTime[level];
    m_fDeActivationTime = def->m_faDeActivationTime[level];
    m_nMinMainBeams = def->m_naMinMainBeams[level];
    m_nMaxMainBeams = def->m_naMaxMainBeams[level];
    m_nMinSubBeams = def->m_naMinSubBeams[level];
    m_nMaxSubBeams = def->m_naMaxSubBeams[level];
    m_fTemporaryBeamLife = def->m_faTemporaryBeamLife[level];
    m_fTargettingFOV = def->m_faTargettingFOV[level];
    m_fStaminaPerSecond = def->m_faStaminaPerSecond[level];
    m_fMainBeamDecapitationTime = def->m_faMainBeamDecapitationTime[level];
    m_fSubBeamDecapitationTime = def->m_faSubBeamDecapitationTime[level];
    m_fSubBeamDelay = def->m_faSubBeamDelay[level];
    m_OnhitEveryFrame = (long)(g_FramesPerSecond * def->OnhitEveryNSecs[level]);
}
long g_FramesPerSecond = 30;
int main()
{
    float f[19][4]; long l[4][4];
    for (int i = 0; i < 19; ++i) for (int k = 0; k < 4; ++k) f[i][k] = (float)(i * 10 + k) + 0.5f;
    for (int i = 0; i < 4; ++i) for (int k = 0; k < 4; ++k) l[i][k] = 100 * i + k;
    CSpecialAbilitiesLightningSpellDef d; std::memset(&d, 0, sizeof d);
    vector<float>* fv[] = { &d.m_faAreaEffect, &d.m_faDamagePerHit, &d.m_faSubBeamDamagePerHit, &d.m_faActivationTime,
        &d.m_faDeActivationTime, &d.m_faTemporaryBeamLife, &d.m_faTargettingFOV, &d.m_faStaminaPerSecond,
        &d.m_faMainBeamDecapitationTime, &d.m_faSubBeamDecapitationTime, &d.m_faSubBeamDelay, &d.OnhitEveryNSecs };
    for (int i = 0; i < 12; ++i) fv[i]->_M_start = f[i];
    d.m_naMinMainBeams._M_start = l[0]; d.m_naMaxMainBeams._M_start = l[1];
    d.m_naMinSubBeams._M_start = l[2]; d.m_naMaxSubBeams._M_start = l[3];
    f[11][2] = 1.5f;                              // OnhitEveryNSecs[2] = 1.5 s -> 45 frames
    CTCWillLightning w; std::memset(&w, 0, sizeof w);
    w.m_lspDef_Object = &d;
    w.m_bPlayedSound = true;
    w.SetLevel(2);
    bool ok = w.m_fAreaEffect == f[0][2] && w.m_fDamagePerHit == f[1][2] && w.m_fSubBeamDamagePerHit == f[2][2]
        && w.m_fActivationTime == f[3][2] && w.m_fDeActivationTime == f[4][2]
        && w.m_nMinMainBeams == 2 && w.m_nMaxMainBeams == 102 && w.m_nMinSubBeams == 202
        && w.m_nMaxSubBeams == 302 && w.m_fTemporaryBeamLife == f[5][2] && w.m_fTargettingFOV == f[6][2]
        && w.m_fStaminaPerSecond == f[7][2] && w.m_fMainBeamDecapitationTime == f[8][2]
        && w.m_fSubBeamDecapitationTime == f[9][2] && w.m_fSubBeamDelay == f[10][2]
        && w.m_OnhitEveryFrame == 45 && w.m_bPlayedSound;
    if (!ok) { std::printf("0079f810_TEST FAIL\n"); return 1; }
    std::printf("0079f810_TEST PASS\n");
    return 0;
}