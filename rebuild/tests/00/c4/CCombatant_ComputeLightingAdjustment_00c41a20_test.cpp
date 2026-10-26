#include <cstdio>

// Model of CCombatant::ComputeLightingAdjustment @ 0x00C41A20
class C3DVector { public: float X, Y, Z; };
static float g_combatLightingScale = 0.5f;
struct CCombatant {
    unsigned char _pad[0x108]; C3DVector m_lightDirection; unsigned char _pad114[0x120 - 0x114]; C3DVector m_facing; unsigned char _pad12c[0x18c - 0x12c]; float m_lightingIntensity;
    float ComputeLightingAdjustment() const {
        float dot = m_lightDirection.Z * m_facing.Z + m_lightDirection.Y * m_facing.Y + m_lightDirection.X * m_facing.X;
        float adjustment = 1.0f - dot * m_lightingIntensity * g_combatLightingScale;
        if (adjustment < 0.0f) adjustment = 0.0f;
        return adjustment;
    }
};
int main()
{
    CCombatant c; c.m_lightDirection.X = 1.0f; c.m_lightDirection.Y = 0.0f; c.m_lightDirection.Z = 0.0f; c.m_facing.X = 1.0f; c.m_facing.Y = 2.0f; c.m_facing.Z = 3.0f; c.m_lightingIntensity = 1.0f;
    if (c.ComputeLightingAdjustment() != 0.5f) { std::printf("00c41a20_TEST FAIL\n"); return 1; }           // 1 - 1*1*0.5
    c.m_lightingIntensity = 8.0f; if (c.ComputeLightingAdjustment() != 0.0f) { std::printf("00c41a20_TEST FAIL\n"); return 1; }   // clamped
    c.m_lightDirection.X = 0.0f; c.m_lightDirection.Z = 1.0f; c.m_lightingIntensity = 0.5f; if (c.ComputeLightingAdjustment() != 0.25f) { std::printf("00c41a20_TEST FAIL\n"); return 1; }   // 1 - 3*0.5*0.5
    if ((sizeof(void*) == 4 && (char*)&c.m_lightingIntensity - (char*)&c != 0x18c)) { std::printf("00c41a20_TEST FAIL\n"); return 1; }
    std::printf("00c41a20_TEST PASS\n"); return 0;
}