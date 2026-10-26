// behaviour test for CKrakenTentacleDef::operator= (0x004E1494): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
class CDefClassBase { public: unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };

class CKrakenTentacleDef : public CDefClassBase { public:
    float TentacleSmackDownTime;
    float TentacleTurnSpeed;
    float TentacleMoveSpeed;
    float TentacleStrikeAngleTolerence;
    float TentacleStrikeDistTolerence;
    float TentacleHeightOffset;
    float TentacleTimeBeforeExplosion;
    float TentacleTimeBetweenAttacks;
    float TentacleTimeBeforeObstructionRemoval;
    float TentacleObstructionRadius;
    float TentacleExplosionBodgeAngle;
    float TentacleExplosionRadius;
    float TentacleObstructionRadiusMin;
    float TentacleObstructionRadiusMax;
    CKrakenTentacleDef& operator=(const CKrakenTentacleDef& other);
};
CKrakenTentacleDef& CKrakenTentacleDef::operator=(const CKrakenTentacleDef& other)
{
    CDefClassBase::operator=(other);
    TentacleSmackDownTime = other.TentacleSmackDownTime;
    TentacleTurnSpeed = other.TentacleTurnSpeed;
    TentacleMoveSpeed = other.TentacleMoveSpeed;
    TentacleStrikeAngleTolerence = other.TentacleStrikeAngleTolerence;
    TentacleStrikeDistTolerence = other.TentacleStrikeDistTolerence;
    TentacleHeightOffset = other.TentacleHeightOffset;
    TentacleTimeBeforeExplosion = other.TentacleTimeBeforeExplosion;
    TentacleTimeBetweenAttacks = other.TentacleTimeBetweenAttacks;
    TentacleTimeBeforeObstructionRemoval = other.TentacleTimeBeforeObstructionRemoval;
    TentacleObstructionRadius = other.TentacleObstructionRadius;
    TentacleExplosionBodgeAngle = other.TentacleExplosionBodgeAngle;
    TentacleExplosionRadius = other.TentacleExplosionRadius;
    TentacleObstructionRadiusMin = other.TentacleObstructionRadiusMin;
    TentacleObstructionRadiusMax = other.TentacleObstructionRadiusMax;
    return *this;
}
int main()
{
    CKrakenTentacleDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    CKrakenTentacleDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1;
    { const unsigned char* x = (const unsigned char*)&a.TentacleSmackDownTime; const unsigned char* y = (const unsigned char*)&b.TentacleSmackDownTime; for (unsigned int i = 0; i < sizeof(a.TentacleSmackDownTime); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleTurnSpeed; const unsigned char* y = (const unsigned char*)&b.TentacleTurnSpeed; for (unsigned int i = 0; i < sizeof(a.TentacleTurnSpeed); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleMoveSpeed; const unsigned char* y = (const unsigned char*)&b.TentacleMoveSpeed; for (unsigned int i = 0; i < sizeof(a.TentacleMoveSpeed); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleStrikeAngleTolerence; const unsigned char* y = (const unsigned char*)&b.TentacleStrikeAngleTolerence; for (unsigned int i = 0; i < sizeof(a.TentacleStrikeAngleTolerence); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleStrikeDistTolerence; const unsigned char* y = (const unsigned char*)&b.TentacleStrikeDistTolerence; for (unsigned int i = 0; i < sizeof(a.TentacleStrikeDistTolerence); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleHeightOffset; const unsigned char* y = (const unsigned char*)&b.TentacleHeightOffset; for (unsigned int i = 0; i < sizeof(a.TentacleHeightOffset); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleTimeBeforeExplosion; const unsigned char* y = (const unsigned char*)&b.TentacleTimeBeforeExplosion; for (unsigned int i = 0; i < sizeof(a.TentacleTimeBeforeExplosion); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleTimeBetweenAttacks; const unsigned char* y = (const unsigned char*)&b.TentacleTimeBetweenAttacks; for (unsigned int i = 0; i < sizeof(a.TentacleTimeBetweenAttacks); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleTimeBeforeObstructionRemoval; const unsigned char* y = (const unsigned char*)&b.TentacleTimeBeforeObstructionRemoval; for (unsigned int i = 0; i < sizeof(a.TentacleTimeBeforeObstructionRemoval); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleObstructionRadius; const unsigned char* y = (const unsigned char*)&b.TentacleObstructionRadius; for (unsigned int i = 0; i < sizeof(a.TentacleObstructionRadius); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleExplosionBodgeAngle; const unsigned char* y = (const unsigned char*)&b.TentacleExplosionBodgeAngle; for (unsigned int i = 0; i < sizeof(a.TentacleExplosionBodgeAngle); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleExplosionRadius; const unsigned char* y = (const unsigned char*)&b.TentacleExplosionRadius; for (unsigned int i = 0; i < sizeof(a.TentacleExplosionRadius); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleObstructionRadiusMin; const unsigned char* y = (const unsigned char*)&b.TentacleObstructionRadiusMin; for (unsigned int i = 0; i < sizeof(a.TentacleObstructionRadiusMin); ++i) if (x[i] != y[i]) bad = 1; }
    { const unsigned char* x = (const unsigned char*)&a.TentacleObstructionRadiusMax; const unsigned char* y = (const unsigned char*)&b.TentacleObstructionRadiusMax; for (unsigned int i = 0; i < sizeof(a.TentacleObstructionRadiusMax); ++i) if (x[i] != y[i]) bad = 1; }
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004e1494_TEST FAIL\n"); return 1; }
    std::printf("004e1494_TEST PASS\n");
    return 0;
}