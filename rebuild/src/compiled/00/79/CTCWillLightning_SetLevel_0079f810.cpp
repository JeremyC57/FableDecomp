// CTCWillLightning::SetLevel(long) -- copy the per-level lightning-spell tunables out of the def's level vectors
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079F810, 247 bytes (+9 int3). __fastcall, `ret 4`.
// Callees: _ftol2 0x00BFEA70 (the (long) truncation); global frame-rate tick count 0x01375550 (fild).
// Shape: the def object (CDefPointer<CSpecialAbilitiesLightningSpellDef const>::Object, +0x28) is loaded once into eax and
//        the level once into ecx; each of the 15 tunables is `member = def->vector[level]` (STLport vector: _M_start is the
//        first dword, so `mov edx,[eax+N] ; mov edx,[edx+ecx*4]`). The last one is an int: OnhitEveryNSecs[level] seconds
//        scaled by the tick rate (`fild g ; fmul [v]`) and truncated with _ftol2 into m_OnhitEveryFrame.
// Layout: local model -- retail CTCWillLightning sits 0xC below the PDB from m_fTime on (the landed Deactivate 0x007A0310
//         shows m_fTime +0x60 v PDB +0x64 and 4-byte STLport list m_lbBeams), so m_fAreaEffect is +0x9C (PDB +0xA8) and
//         m_OnhitEveryFrame +0xDC (PDB +0xE8). The def has a 0x3C-byte CDefClassBase head (PDB -4) and 12-byte STLport
//         vectors (PDB 16): m_faAreaEffect +0x3C ... m_faStaminaPerSecond +0xC0, four random-beam angle floats +0xCC..+0xD8,
//         m_faMainBeamDecapitationTime +0xDC, m_faSubBeamDecapitationTime +0xE8, m_faSubBeamDelay +0xF4,
//         OnhitEveryNSecs +0x100. The engine header has the PDB layout, so a local model is used.
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