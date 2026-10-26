#include <cstdio>
class C3DVector { public: float x, y, z; };
struct CExplosionDriver { float m_innerRadius, m_outerRadius, m_maxDamage; long m_fireDamage; };
static int g_kills = 0;
class CThing { public:
    unsigned char m_driverFlags; CExplosionDriver* m_pDriver; C3DVector m_pos; bool m_dead;
    const C3DVector& GetPosition() { return m_pos; }
    void Kill(bool immediately) { if (immediately) { m_dead = true; ++g_kills; } }
    bool GetTCDriver(CExplosionDriver** out) { if (m_driverFlags & 2) { *out = m_pDriver; return true; } return false; }
};
static CExplosionDriver g_driver;
static CThing g_explosion;
static long g_createdDef = -1; static C3DVector g_createdAt;
CThing* CreateThing(long defIndex, const C3DVector& position)
{
    g_createdDef = defIndex; g_createdAt = position;
    g_explosion.m_driverFlags = 2; g_explosion.m_pDriver = &g_driver;
    return &g_explosion;
}
class CExplodingObjectDef { public: long ExplosionType; };
class CTCExplodingObject { public:
    CThing* ParentThing; float MaxDamage; float Radius; long FireDamage; CExplodingObjectDef* PDef;
    void Explode()
    {
        CExplodingObjectDef* def = PDef;
        CThing* explosion = CreateThing(def->ExplosionType, ParentThing->GetPosition());
        CExplosionDriver* driver;
        explosion->GetTCDriver(&driver);
        driver->m_innerRadius = Radius; driver->m_outerRadius = Radius;
        driver->m_fireDamage = FireDamage; driver->m_maxDamage = MaxDamage;
        ParentThing->Kill(true);
    }
};
int main()
{
    CThing barrel; barrel.m_dead = false; barrel.m_pos.x = 1.0f; barrel.m_pos.y = 2.0f; barrel.m_pos.z = 3.0f;
    CExplodingObjectDef def; def.ExplosionType = 77;
    CTCExplodingObject o; o.ParentThing = &barrel; o.MaxDamage = 40.0f; o.Radius = 6.5f; o.FireDamage = 12; o.PDef = &def;
    o.Explode();
    if (g_createdDef != 77 || g_createdAt.y != 2.0f || g_driver.m_innerRadius != 6.5f || g_driver.m_outerRadius != 6.5f
        || g_driver.m_maxDamage != 40.0f || g_driver.m_fireDamage != 12 || !barrel.m_dead || g_kills != 1) { std::printf("00750dd0_TEST FAIL\n"); return 1; }
    std::printf("00750dd0_TEST PASS\n");
    return 0;
}