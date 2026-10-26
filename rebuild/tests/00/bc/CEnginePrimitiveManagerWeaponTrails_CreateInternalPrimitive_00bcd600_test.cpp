#include <cstdio>
static int g_baseType = -1; static unsigned long g_baseFlags = 0;
class CEngineInternalPrimitiveBase { public: virtual void v00() { } unsigned char _pad04[0x44]; CEngineInternalPrimitiveBase(long type, unsigned long flags) { g_baseType = type; g_baseFlags = flags; } };
class CEngineInternalPrimitiveWeaponTrail : public CEngineInternalPrimitiveBase { public: unsigned char _pad48[0x14]; long m_w[6]; unsigned char _rest[8]; CEngineInternalPrimitiveWeaponTrail() : CEngineInternalPrimitiveBase(0x13, 0x200000) { for (int i = 0; i < 6; ++i) m_w[i] = 0; } };
class CEnginePrimitiveManagerWeaponTrails { public: CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type) { if (type != 0x13) return 0; return new CEngineInternalPrimitiveWeaponTrail; } };

int main() {
    CEnginePrimitiveManagerWeaponTrails m; if (m.CreateInternalPrimitive(0x13 + 1) || g_baseType != -1) { std::printf("00bcd600_TEST FAIL\n"); return 1; }
    CEngineInternalPrimitiveWeaponTrail* p = (CEngineInternalPrimitiveWeaponTrail*)m.CreateInternalPrimitive(0x13); if (!p || g_baseType != 0x13 || g_baseFlags != 0x200000 || p->m_w[0] || p->m_w[5]) { std::printf("00bcd600_TEST FAIL\n"); return 1; }
    delete p;
    std::printf("00bcd600_TEST PASS\n"); return 0;
}