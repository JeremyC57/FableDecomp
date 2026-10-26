// Behaviour test for 0x00854810 CCreatureAction_SpiritHitResponse::Initialise.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
static void Log(int v) { if (g_n < 8) g_log[g_n++] = v; }
class C3DVector { public:
    float X, Y, Z;
    C3DVector& operator*=(float scale) { X *= scale; Y *= scale; Z *= scale; return *this; }
};
class CTCPhysicsBase { public: C3DVector m_applied; void ApplyAcceleration(const C3DVector& a) { m_applied = a; Log(2); } };
class CTCCoopSpirit { public: void OnHit() { Log(3); } };
class CThing { public:
    unsigned int m_presentBits[9];
    CTCCoopSpirit* m_spirit;
    CTCPhysicsBase* m_pPhysics;
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_spirit; return true; }
        return false;
    }
};
static int g_SpiritHitResponseFrames = 15;
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAction {
    CBaseIntelligentPointer PCreature; int TotalTime; C3DVector Knockback;
    void Initialise()
    {
        Log(1);   // CCreatureActionBase::Initialise
        TotalTime = (int)(float)g_SpiritHitResponseFrames;
        Knockback *= 2.0f;
        CThing* creature = PCreature.GetPItem();
        creature->m_pPhysics->ApplyAcceleration(Knockback);
        CTCCoopSpirit* pSpirit = 0;
        PCreature.GetPItem()->GetInterface(&pSpirit, 0xc7);
        pSpirit->OnHit();
    }
};
int main()
{
    CTCPhysicsBase physics; CTCCoopSpirit spirit;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_presentBits[0xc7 >> 5] |= 1u << (0xc7 & 31); me.m_spirit = &spirit; me.m_pPhysics = &physics;
    CAction a; a.PCreature.m_pItem = &me; a.TotalTime = 0; a.Knockback.X = 1.0f; a.Knockback.Y = -0.5f; a.Knockback.Z = 3.0f;
    a.Initialise();
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || a.TotalTime != 15
        || physics.m_applied.X != 2.0f || physics.m_applied.Y != -1.0f || physics.m_applied.Z != 6.0f || a.Knockback.Z != 6.0f) { std::printf("00854810_TEST FAIL\n"); return 1; }
    std::printf("00854810_TEST PASS\n");
    return 0;
}