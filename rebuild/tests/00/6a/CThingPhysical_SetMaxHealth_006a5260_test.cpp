#include <cstdio>
template <class T> inline void ClampMax(T& value, const T& maximum)
{
    if (value > maximum)
        value = maximum;
}
struct CThingPhysical {
    float MaxHealth;
    float Health;
    void SetMaxHealth(float maxHealth, bool adjustHealth);
};
void CThingPhysical::SetMaxHealth(float maxHealth, bool adjustHealth)
{
    float oldMaxHealth = MaxHealth;
    MaxHealth = maxHealth;
    if (adjustHealth && maxHealth > oldMaxHealth)
        Health += maxHealth - oldMaxHealth;
    ClampMax(Health, MaxHealth);
}
int main()
{
    int bad = 0;
    CThingPhysical t; t.MaxHealth = 100.0f; t.Health = 50.0f;
    t.SetMaxHealth(150.0f, true);
    if (t.MaxHealth != 150.0f || t.Health != 100.0f) bad = 1;
    t.SetMaxHealth(200.0f, false);
    if (t.MaxHealth != 200.0f || t.Health != 100.0f) bad = 2;
    t.SetMaxHealth(80.0f, true);
    if (t.MaxHealth != 80.0f || t.Health != 80.0f) bad = 3;
    t.Health = 60.0f; t.SetMaxHealth(70.0f, true);
    if (t.Health != 60.0f) bad = 4;
    if (bad) { std::printf("006a5260_TEST FAIL\n"); return 1; }
    std::printf("006a5260_TEST PASS\n");
    return 0;
}