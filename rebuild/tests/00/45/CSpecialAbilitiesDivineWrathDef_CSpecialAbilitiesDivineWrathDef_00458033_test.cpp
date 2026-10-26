#include <cstdio>
#include <cstring>
#include <new>
// Behaviour test for the retail-model constructor (12-byte vectors, scalars left unset).
class CParentDefClassBase {                        // 0x3C bytes in retail
public:
    virtual void v000();
    unsigned long _body[0x0E];
    CParentDefClassBase();                         // 0x00430370
};
class CDefString { public:
    long TablePos;
};
template <class T> struct CRetailVector {          // retail std::vector<T> (12 bytes)
    T* m_first; T* m_last; T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) {}
};
class CSpecialAbilitiesDivineWrathDef : public CParentDefClassBase {   // vtable 0x0123487C
public:
    virtual void v000();
    CRetailVector<CDefString> StartPowerupSound;             // +0x3c
    CRetailVector<CDefString> LoopingPowerupSound;           // +0x48
    CRetailVector<CDefString> ReleaseSound;                  // +0x54
    CRetailVector<CDefString> ReleaseLoopSound;              // +0x60
    CRetailVector<CDefString> FinishSound;                   // +0x6c
    CRetailVector<CDefString> CancelSound;                   // +0x78
    float                     RandomFactorForNumSecsBetweenDamageHit;   // +0x84
    CRetailVector<float>      NumSecsBetweenDamageHit;       // +0x88
    CRetailVector<float>      DamagePerHit;                  // +0x94
    CRetailVector<float>      DurationOfSpellSecs;           // +0xa0
    CRetailVector<float>      RadiusOfEffect;                // +0xac
    CRetailVector<float>      EnvChangeSecs;                 // +0xb8
    CRetailVector<float>      EnvReturnSecs;                 // +0xc4
    bool                      DoLifting;                     // +0xd0
    float                     VelocityOfLift;                // +0xd4
    float                     DistanceToLift;                // +0xd8
    CRetailVector<float>      StaminaCostPerSec;             // +0xdc
    CRetailVector<float>      DamageTakenMultiplier;         // +0xe8
    float                     SmallRingRadius;               // +0xf4
    float                     LargeRingRadius;               // +0xf8
    CSpecialAbilitiesDivineWrathDef();
};

CSpecialAbilitiesDivineWrathDef::CSpecialAbilitiesDivineWrathDef()
{
}

CParentDefClassBase::CParentDefClassBase() { for (int i = 0; i < 0x0E; ++i) _body[i] = 0x5a5a5a5aUL; }
void CParentDefClassBase::v000() { }
void CSpecialAbilitiesDivineWrathDef::v000() { }

int main() {
    union Storage { double align; unsigned char bytes[sizeof(CSpecialAbilitiesDivineWrathDef)]; } storage;
    std::memset(storage.bytes, 0xa5, sizeof(storage.bytes));
    CSpecialAbilitiesDivineWrathDef* d = new (storage.bytes) CSpecialAbilitiesDivineWrathDef();
    for (int i = 0; i < 0x0E; ++i) if (d->_body[i] != 0x5a5a5a5aUL) { std::printf("00458033_TEST FAIL base\n"); return 1; }
    const void* vectors[][3] = {
        { d->StartPowerupSound.m_first, d->StartPowerupSound.m_last, d->StartPowerupSound.m_end },
        { d->LoopingPowerupSound.m_first, d->LoopingPowerupSound.m_last, d->LoopingPowerupSound.m_end },
        { d->ReleaseSound.m_first, d->ReleaseSound.m_last, d->ReleaseSound.m_end },
        { d->ReleaseLoopSound.m_first, d->ReleaseLoopSound.m_last, d->ReleaseLoopSound.m_end },
        { d->FinishSound.m_first, d->FinishSound.m_last, d->FinishSound.m_end },
        { d->CancelSound.m_first, d->CancelSound.m_last, d->CancelSound.m_end },
        { d->NumSecsBetweenDamageHit.m_first, d->NumSecsBetweenDamageHit.m_last, d->NumSecsBetweenDamageHit.m_end },
        { d->DamagePerHit.m_first, d->DamagePerHit.m_last, d->DamagePerHit.m_end },
        { d->DurationOfSpellSecs.m_first, d->DurationOfSpellSecs.m_last, d->DurationOfSpellSecs.m_end },
        { d->RadiusOfEffect.m_first, d->RadiusOfEffect.m_last, d->RadiusOfEffect.m_end },
        { d->EnvChangeSecs.m_first, d->EnvChangeSecs.m_last, d->EnvChangeSecs.m_end },
        { d->EnvReturnSecs.m_first, d->EnvReturnSecs.m_last, d->EnvReturnSecs.m_end },
        { d->StaminaCostPerSec.m_first, d->StaminaCostPerSec.m_last, d->StaminaCostPerSec.m_end },
        { d->DamageTakenMultiplier.m_first, d->DamageTakenMultiplier.m_last, d->DamageTakenMultiplier.m_end },
    };
    for (int i = 0; i < 14; ++i)
        for (int j = 0; j < 3; ++j)
            if (vectors[i][j] != 0) { std::printf("00458033_TEST FAIL vector\n"); return 1; }
    const unsigned char* scalars[] = {
        (const unsigned char*)&d->RandomFactorForNumSecsBetweenDamageHit, (const unsigned char*)&d->DoLifting,
        (const unsigned char*)&d->VelocityOfLift, (const unsigned char*)&d->DistanceToLift,
        (const unsigned char*)&d->SmallRingRadius, (const unsigned char*)&d->LargeRingRadius,
    };
    for (int i = 0; i < 6; ++i) if (scalars[i][0] != 0xa5) { std::printf("00458033_TEST FAIL scalar\n"); return 1; }
    std::printf("00458033_TEST PASS\n");
    return 0;
}
