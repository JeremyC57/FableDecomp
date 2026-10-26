// Behaviour test for 0x008025D0 CTCTargetingPlayer::GetFOVToBeInToBeTargetable: aiming-mode arc vs normal arc, in turns.
#include <cstdio>
class CPlayer { public: bool PCProjectileWeaponThirdPersonAimingMode; };
class CThing { public: CPlayer* player; CPlayer* GetPlayer() { return player; } };
class CTargetingDef { public: float TargetArc; float TargetArcPCThirdPersonAimingMode; };
class CTCTargetingPlayer { public:
    CThing* ParentThing;
    CTargetingDef* PSubDef;
    float GetFOVToBeInToBeTargetable() const
    {
        if (ParentThing->GetPlayer()->PCProjectileWeaponThirdPersonAimingMode)
            return PSubDef->TargetArcPCThirdPersonAimingMode * (1.0f / 360.0f);
        return PSubDef->TargetArc * (1.0f / 360.0f);
    }
};
static bool near(float a, float b) { float d = a - b; return d < 1e-6f && d > -1e-6f; }
int main()
{
    CPlayer player; player.PCProjectileWeaponThirdPersonAimingMode = false;
    CThing thing; thing.player = &player;
    CTargetingDef def; def.TargetArc = 90.0f; def.TargetArcPCThirdPersonAimingMode = 180.0f;
    CTCTargetingPlayer tc; tc.ParentThing = &thing; tc.PSubDef = &def;
    if (!near(tc.GetFOVToBeInToBeTargetable(), 0.25f)) { std::printf("008025d0_TEST FAIL\n"); return 1; }
    player.PCProjectileWeaponThirdPersonAimingMode = true;
    if (!near(tc.GetFOVToBeInToBeTargetable(), 0.5f)) { std::printf("008025d0_TEST FAIL\n"); return 1; }
    std::printf("008025d0_TEST PASS\n");
    return 0;
}