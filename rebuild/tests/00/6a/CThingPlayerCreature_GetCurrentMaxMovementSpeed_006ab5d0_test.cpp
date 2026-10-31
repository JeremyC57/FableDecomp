// CThingPlayerCreature::GetCurrentMaxMovementSpeed() const -- behaviour test: 0 while blocking, sneaking speed under the sneak
// option when the stealth component sneaks, else the band speed, overridden by the sprint speed while sprinting.
#include <stdio.h>

struct Stealth { bool Sneaking; float SneakSpeed; };
struct Special { bool Sprinting; };
struct Player {
    bool Blocking, SneakOption; Stealth* PStealth; Special* PSpecial; int Band;
    float GetMaxWalkingSpeed() const { return 2.0f; }
    float GetMaxJoggingSpeed() const { return 4.0f; }
    float GetMaxRunningSpeed() const { return 6.0f; }
    float GetBandSpeed() const
    {
        switch (Band) {
        case 0: return 0.0f;
        case 1: case 2: if (PStealth && PStealth->Sneaking) return PStealth->SneakSpeed; return GetMaxWalkingSpeed();
        case 3: return GetMaxRunningSpeed();
        case 4: return GetMaxJoggingSpeed();
        default: return GetMaxRunningSpeed();
        }
    }
    float GetMaxSprintingSpeed() const { return 9.0f; }
    float GetCurrentMaxMovementSpeed() const
    {
        if (Blocking) return 0.0f;
        if (SneakOption) {
            const Stealth* sneak = PStealth;
            if (sneak && sneak->Sneaking) return sneak->SneakSpeed;
        }
        float speed = GetBandSpeed();
        const Special* special = PSpecial;
        if (special && special->Sprinting) { float sprint = GetMaxSprintingSpeed(); speed = sprint; }
        return speed;
    }
};

int main()
{
    Stealth st = { true, 0.5f }; Special sp = { false };
    Player p; p.Blocking = true; p.SneakOption = true; p.PStealth = &st; p.PSpecial = &sp; p.Band = 2;
    bool ok = p.GetCurrentMaxMovementSpeed() == 0.0f;
    p.Blocking = false; ok = ok && p.GetCurrentMaxMovementSpeed() == 0.5f;
    st.Sneaking = false; ok = ok && p.GetCurrentMaxMovementSpeed() == 2.0f;
    p.Band = 3; ok = ok && p.GetCurrentMaxMovementSpeed() == 6.0f;
    p.Band = 4; ok = ok && p.GetCurrentMaxMovementSpeed() == 4.0f;
    p.Band = 7; ok = ok && p.GetCurrentMaxMovementSpeed() == 6.0f;
    p.Band = 0; ok = ok && p.GetCurrentMaxMovementSpeed() == 0.0f;
    p.Band = 2;
    p.SneakOption = false; st.Sneaking = true; ok = ok && p.GetCurrentMaxMovementSpeed() == 0.5f;
    p.Band = 3; ok = ok && p.GetCurrentMaxMovementSpeed() == 6.0f;
    p.Band = 2;
    st.Sneaking = false; sp.Sprinting = true; ok = ok && p.GetCurrentMaxMovementSpeed() == 9.0f;
    p.PSpecial = 0; p.PStealth = 0; p.SneakOption = true; ok = ok && p.GetCurrentMaxMovementSpeed() == 2.0f;
    printf(ok ? "006ab5d0_TEST PASS\n" : "006ab5d0_TEST FAIL\n");
    return ok ? 0 : 1;
}