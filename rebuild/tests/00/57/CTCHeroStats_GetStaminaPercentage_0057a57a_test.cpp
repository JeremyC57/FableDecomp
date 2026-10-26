// Behaviour test for 0x0057A57A CTCHeroStats::GetStaminaPercentage: Stamina / MaxStamina while the maximum is
// positive, 0 for a zero or negative maximum.
#include <cstdio>

class CTCHeroStats { public:
    int Stamina; int MaxStamina;
    float GetStaminaPercentage() const
    {
        float max = (float)MaxStamina;
        if (max > 0.0f) {
            float fraction = (float)Stamina;
            fraction /= max;
            return fraction;
        }
        return 0.0f;
    }
};

int main()
{
    CTCHeroStats s;
    s.Stamina = 50; s.MaxStamina = 200;
    bool ok = s.GetStaminaPercentage() == 0.25f;
    s.Stamina = 200; ok = ok && s.GetStaminaPercentage() == 1.0f;
    s.MaxStamina = 0; ok = ok && s.GetStaminaPercentage() == 0.0f;
    s.MaxStamina = -5; ok = ok && s.GetStaminaPercentage() == 0.0f;
    std::printf(ok ? "0057a57a_TEST PASS\n" : "0057a57a_TEST FAIL\n");
    return ok ? 0 : 1;
}