#include <cstdio>
// CBossDef::operator=(const CBossDef&) -- def copy: CDefClassBase part, three phase/shot vectors, two twin-blade scalars
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2C08, 69 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::vector<long>::operator= @ 0x00454886.
// Identification: PDB CBossDef = CDefClassBase + PhaseHealthPercent, Shots, TwinBladeMissesBeforeLunge (std::vector<long>)
//   +0x28/+0x34/+0x40, TwinBladeStuckLoops +0x4c, TwinBladeTimeBeforeLunge (float) +0x50 (retail offsets).
// Shape: memberwise; each vector goes through the out-of-line vector<long>::operator= 0x00454886.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CLongVector {                    // std::vector<long> (retail STLport, 12 bytes)
    long* m_pFirst;
    long* m_pLast;
    long* m_pEndOfStorage;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CBossDef : CDefClassBase {
    CLongVector PhaseHealthPercent;     // +0x28
    CLongVector Shots;                  // +0x34
    CLongVector TwinBladeMissesBeforeLunge;// +0x40
    long TwinBladeStuckLoops;           // +0x4c
    float TwinBladeTimeBeforeLunge;     // +0x50
    CBossDef& operator=(const CBossDef& other);
};

CBossDef& CBossDef::operator=(const CBossDef& other)
{
    CDefClassBase::operator=(other);
    PhaseHealthPercent = other.PhaseHealthPercent;
    Shots = other.Shots;
    TwinBladeMissesBeforeLunge = other.TwinBladeMissesBeforeLunge;
    TwinBladeStuckLoops = other.TwinBladeStuckLoops;
    TwinBladeTimeBeforeLunge = other.TwinBladeTimeBeforeLunge;
    return *this;
}


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
CLongVector& CLongVector::operator=(const CLongVector& other) { m_pFirst = other.m_pFirst; m_pLast = other.m_pLast; m_pEndOfStorage = other.m_pEndOfStorage; return *this; }

int main()
{
    static CBossDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.PhaseHealthPercent.m_pFirst = (long*)&b;
    b.Shots.m_pLast = (long*)&b + 1;
    b.TwinBladeMissesBeforeLunge.m_pEndOfStorage = (long*)&b + 2;
    b.TwinBladeStuckLoops = 3;
    b.TwinBladeTimeBeforeLunge = 4.5f;
    CBossDef* r = &(a = b);
    if (r != &a) { std::printf("004e2c08_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "004e2c08"); return 1; }
    if (!(a.PhaseHealthPercent.m_pFirst == b.PhaseHealthPercent.m_pFirst)) { std::printf("004e2c08_TEST FAIL\n"); return 1; }
    if (!(a.Shots.m_pLast == b.Shots.m_pLast)) { std::printf("004e2c08_TEST FAIL\n"); return 1; }
    if (!(a.TwinBladeMissesBeforeLunge.m_pEndOfStorage == b.TwinBladeMissesBeforeLunge.m_pEndOfStorage)) { std::printf("004e2c08_TEST FAIL\n"); return 1; }
    if (!(a.TwinBladeStuckLoops == b.TwinBladeStuckLoops)) { std::printf("004e2c08_TEST FAIL\n"); return 1; }
    if (!(a.TwinBladeTimeBeforeLunge == b.TwinBladeTimeBeforeLunge)) { std::printf("004e2c08_TEST FAIL\n"); return 1; }
    std::printf("004e2c08_TEST PASS\n");
    return 0;
}
