#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CDataOutputStream { public: void WriteCBYTE(long v) { std::sprintf(g_seq + std::strlen(g_seq), "B%ld ", v); } void WriteFloat(float v) { std::sprintf(g_seq + std::strlen(g_seq), "F%g ", v); } };
class CExplosionRing { public: float m_radius; long m_count; float m_height; float m_speed; float GetRadius() const { return m_radius; } float GetHeight() const { return m_height; } float GetSpeed() const { return m_speed; }
    void TransferBinaryOut(CDataOutputStream* s) { s->WriteCBYTE(m_count); s->WriteFloat(GetRadius()); s->WriteFloat(GetHeight()); s->WriteFloat(GetSpeed()); } };
int main() {
    CDataOutputStream s; CExplosionRing r; r.m_radius = 1.5f; r.m_count = 3; r.m_height = 2.5f; r.m_speed = 4.0f; r.TransferBinaryOut(&s);
    if (std::strcmp(g_seq, "B3 F1.5 F2.5 F4 ")) { std::printf("004eb1d9_TEST FAIL\n"); return 1; }
    std::printf("004eb1d9_TEST PASS\n"); return 0;
}