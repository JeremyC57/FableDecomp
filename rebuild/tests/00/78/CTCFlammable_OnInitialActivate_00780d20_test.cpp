#include <cstdio>
static int g_destroyed = 0, g_firePointInits = 0, g_forceUpdates = 0;
struct CDefPointee { long RefCount; virtual void Destroy() { ++g_destroyed; } void Release() { if (--RefCount == 0) Destroy(); } };
struct CFlammableDef : CDefPointee { long FireResistance; };
struct CFlammableDefPointer {
    CFlammableDef* Object;
    CFlammableDef* operator->() const { return Object; }
    CFlammableDefPointer& operator=(int) { if (Object) { Object->Release(); Object = 0; } Object = 0; return *this; }
};
struct CDefClassBase {
    CFlammableDef* m_def; bool m_found;
    bool GetSubDef(CFlammableDefPointer* out) { if (m_found) { out->Object = m_def; ++m_def->RefCount; } return m_found; }
};
struct CThing { CDefClassBase* m_pDefClass; void SetForceFrameUpdate(bool u) { if (u) ++g_forceUpdates; } };
struct CTCFlammable {
    void* vptrSlot; CThing* ParentThing;
    CFlammableDefPointer PDef; bool Burning; long FireDamageAfterFrame; long FireResistance; long FireStrength;
    float DamageCaused; bool DamagePerPoint; bool HasFirePoint;
    void InitialiseFirePoints() { ++g_firePointInits; }
    void OnInitialActivate()
    {
        if (!ParentThing->m_pDefClass->GetSubDef(&PDef))
            PDef = 0;
        Burning = false; FireDamageAfterFrame = 0; FireResistance = PDef->FireResistance; FireStrength = 1;
        DamageCaused = 0.0f; DamagePerPoint = false;
        InitialiseFirePoints();
        ParentThing->SetForceFrameUpdate(true);
    }
};
int main()
{
    CFlammableDef def; def.RefCount = 1; def.FireResistance = 25;
    CDefClassBase dc; dc.m_def = &def; dc.m_found = true; CThing t; t.m_pDefClass = &dc;
    CTCFlammable f; f.ParentThing = &t; f.PDef.Object = 0; f.Burning = true; f.FireDamageAfterFrame = 9; f.FireStrength = 0;
    f.DamageCaused = 3.0f; f.DamagePerPoint = true; f.HasFirePoint = true;
    f.OnInitialActivate();
    if (f.PDef.Object != &def || def.RefCount != 2 || f.Burning || f.FireDamageAfterFrame != 0 || f.FireResistance != 25 || f.FireStrength != 1
        || f.DamageCaused != 0.0f || f.DamagePerPoint || !f.HasFirePoint || g_firePointInits != 1 || g_forceUpdates != 1 || g_destroyed != 0) { std::printf("00780d20_TEST FAIL\n"); return 1; }
    std::printf("00780d20_TEST PASS\n");
    return 0;
}