// behaviour test for 0x004DF050 CKrakenTentacleDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Context {
    unsigned char buf[1024]; unsigned int pos; int load; const void* order[64]; unsigned int count;
    Context(int l) : pos(0), load(l), count(0) { }
    template <class T> void Transfer(T& v)
    {
        order[count++] = &v;
        if (load) std::memcpy(&v, buf + pos, sizeof(T)); else std::memcpy(buf + pos, &v, sizeof(T));
        pos += (unsigned int)sizeof(T);
    }
};
struct Def {
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
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.TentacleSmackDownTime);
    c.Transfer(d.TentacleTurnSpeed);
    c.Transfer(d.TentacleMoveSpeed);
    c.Transfer(d.TentacleStrikeAngleTolerence);
    c.Transfer(d.TentacleStrikeDistTolerence);
    c.Transfer(d.TentacleHeightOffset);
    c.Transfer(d.TentacleTimeBeforeExplosion);
    c.Transfer(d.TentacleTimeBetweenAttacks);
    c.Transfer(d.TentacleTimeBeforeObstructionRemoval);
    c.Transfer(d.TentacleObstructionRadius);
    c.Transfer(d.TentacleExplosionBodgeAngle);
    c.Transfer(d.TentacleExplosionRadius);
    c.Transfer(d.TentacleObstructionRadiusMin);
    c.Transfer(d.TentacleObstructionRadiusMax);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    Context save(0); Persist(d, save);
    const void* want[14] = { &d.TentacleSmackDownTime, &d.TentacleTurnSpeed, &d.TentacleMoveSpeed, &d.TentacleStrikeAngleTolerence, &d.TentacleStrikeDistTolerence, &d.TentacleHeightOffset, &d.TentacleTimeBeforeExplosion, &d.TentacleTimeBetweenAttacks, &d.TentacleTimeBeforeObstructionRemoval, &d.TentacleObstructionRadius, &d.TentacleExplosionBodgeAngle, &d.TentacleExplosionRadius, &d.TentacleObstructionRadiusMin, &d.TentacleObstructionRadiusMax };
    if (save.count != 14u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 14u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.TentacleSmackDownTime, 0, sizeof(e.TentacleSmackDownTime));
    std::memset(&e.TentacleTurnSpeed, 0, sizeof(e.TentacleTurnSpeed));
    std::memset(&e.TentacleMoveSpeed, 0, sizeof(e.TentacleMoveSpeed));
    std::memset(&e.TentacleStrikeAngleTolerence, 0, sizeof(e.TentacleStrikeAngleTolerence));
    std::memset(&e.TentacleStrikeDistTolerence, 0, sizeof(e.TentacleStrikeDistTolerence));
    std::memset(&e.TentacleHeightOffset, 0, sizeof(e.TentacleHeightOffset));
    std::memset(&e.TentacleTimeBeforeExplosion, 0, sizeof(e.TentacleTimeBeforeExplosion));
    std::memset(&e.TentacleTimeBetweenAttacks, 0, sizeof(e.TentacleTimeBetweenAttacks));
    std::memset(&e.TentacleTimeBeforeObstructionRemoval, 0, sizeof(e.TentacleTimeBeforeObstructionRemoval));
    std::memset(&e.TentacleObstructionRadius, 0, sizeof(e.TentacleObstructionRadius));
    std::memset(&e.TentacleExplosionBodgeAngle, 0, sizeof(e.TentacleExplosionBodgeAngle));
    std::memset(&e.TentacleExplosionRadius, 0, sizeof(e.TentacleExplosionRadius));
    std::memset(&e.TentacleObstructionRadiusMin, 0, sizeof(e.TentacleObstructionRadiusMin));
    std::memset(&e.TentacleObstructionRadiusMax, 0, sizeof(e.TentacleObstructionRadiusMax));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004df050_TEST FAIL %d\n", fail); return 1; }
    std::printf("004df050_TEST PASS\n");
    return 0;
}