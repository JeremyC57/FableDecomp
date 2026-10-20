// Behaviour test for 00c6adb0: body copied from src/00c6adb0.cpp (CReverbSettings00c6adb0::SetDefaults), the zero-vector global supplied.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C3DVector { float X, Y, Z; };
const C3DVector C3DVector_Zero = { 0.0f, 0.0f, 0.0f };
struct CReverbSettings00c6adb0 {
    unsigned long m_mask;
    long  m_room;
    long  m_roomHF;
    long  m_roomLF;
    float m_scale10;
    float m_scale14;
    float m_ratio18;
    float m_level1c;
    float m_hfReference;
    float m_delay24;
    float m_reverbDelay;
    float m_value2c;
    float m_value30;
    float m_scale34;
    float m_scale38;
    float m_value3c;
    float m_value40;
    float m_value44;
    float m_value48;
    C3DVector m_reflectionsPan;
    C3DVector m_reverbPan;
    float m_environmentSize;
    void SetDefaults();
};
void CReverbSettings00c6adb0::SetDefaults()
{
    m_room = -10000;
    m_roomHF = 0;
    m_roomLF = 0;
    m_scale10 = 1.0f;
    m_scale14 = 1.0f;
    m_ratio18 = 0.5f;
    m_level1c = 200.0f;
    m_hfReference = 5000.0f;
    m_delay24 = 0.0498f;
    m_reverbDelay = 0.011f;
    m_value2c = 1.259f;
    m_value30 = 0.04f;
    m_scale34 = 1.0f;
    m_scale38 = 1.0f;
    m_value3c = 0.075f;
    m_value40 = 0;
    m_value44 = 0.04f;
    m_value48 = 0;
    m_reflectionsPan = C3DVector_Zero;
    m_reverbPan = C3DVector_Zero;
    m_environmentSize = 7.5f;
    m_mask = 0x1fffff;
}
int main()
{
    CReverbSettings00c6adb0 r; memset(&r, 0xAB, sizeof(r));
    CHECK(sizeof(r) == 0x68);
    r.SetDefaults();
    CHECK(r.m_mask == 0x1fffff); CHECK(r.m_room == -10000); CHECK(r.m_roomHF == 0); CHECK(r.m_roomLF == 0);
    CHECK(r.m_scale10 == 1.0f); CHECK(r.m_scale14 == 1.0f); CHECK(r.m_ratio18 == 0.5f); CHECK(r.m_level1c == 200.0f);
    CHECK(r.m_hfReference == 5000.0f); CHECK(r.m_delay24 == 0.0498f); CHECK(r.m_reverbDelay == 0.011f); CHECK(r.m_value2c == 1.259f);
    CHECK(r.m_value30 == 0.04f); CHECK(r.m_scale34 == 1.0f); CHECK(r.m_scale38 == 1.0f); CHECK(r.m_value3c == 0.075f);
    CHECK(r.m_value40 == 0.0f); CHECK(r.m_value44 == 0.04f); CHECK(r.m_value48 == 0.0f);
    CHECK(r.m_reflectionsPan.X == 0.0f && r.m_reflectionsPan.Y == 0.0f && r.m_reflectionsPan.Z == 0.0f);
    CHECK(r.m_reverbPan.X == 0.0f && r.m_reverbPan.Y == 0.0f && r.m_reverbPan.Z == 0.0f);
    CHECK(r.m_environmentSize == 7.5f);
    // every byte of the block is written (no 0xAB survives)
    const unsigned char* p = (const unsigned char*)&r; int untouched = 0;
    for (unsigned i = 0; i < sizeof(r); ++i) if (p[i] == 0xAB) ++untouched;
    CHECK(untouched == 0);
    if (fails == 0) printf("00c6adb0_TEST PASS\n");
    else printf("00c6adb0_TEST FAIL (%d)\n", fails);
    return fails;
}