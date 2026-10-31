// Behaviour test for CCombatAction_GenericResponseToDecapitation::Initialise @ 0x00867010: sets the reaction animation,
// decapitates the creature when it has a decapitation component, takes its full MaxHealth away, runs the base Initialise,
// then adds a knocked-down mode whose filter group is the anim's NextFilter string (with that string's CRC).
#include <stdio.h>
#include <string.h>

static unsigned long Crc(const char* s) { unsigned long c = 0; while (*s) c = c * 31 + (unsigned char)*s++; return c; }
struct CMode { char Group[64]; unsigned long GroupCRC; };
struct CTest {
    char Animation[64];
    bool HasDecapitation;
    int decapitations, baseInits, modesAdded;
    float MaxHealth, Health;
    bool lastFlag;
    const char* NextFilter;
    CMode Added;
    void SetCombatAnimation(const char* name) { strcpy(Animation, name); }
    void ModifyHealth(float amount, bool flag) { Health += amount; lastFlag = flag; }
    void Initialise()
    {
        SetCombatAnimation("RESPONSE_GENERIC_REACT_TO_DECAPITATION");
        if (HasDecapitation)
            ++decapitations;
        ModifyHealth(-MaxHealth, true);
        ++baseInits;
        strcpy(Added.Group, NextFilter);
        Added.GroupCRC = Crc(NextFilter);
        ++modesAdded;
    }
};
int main()
{
    int ok = 1;
    CTest t;
    memset(&t, 0, sizeof t);
    t.HasDecapitation = true; t.MaxHealth = 80.0f; t.Health = 50.0f; t.NextFilter = "KNOCKED_DOWN_HEADLESS";
    t.Initialise();
    ok &= strcmp(t.Animation, "RESPONSE_GENERIC_REACT_TO_DECAPITATION") == 0;
    ok &= t.decapitations == 1 && t.Health == -30.0f && t.lastFlag && t.baseInits == 1 && t.modesAdded == 1;
    ok &= strcmp(t.Added.Group, "KNOCKED_DOWN_HEADLESS") == 0 && t.Added.GroupCRC == Crc("KNOCKED_DOWN_HEADLESS");
    CTest u;
    memset(&u, 0, sizeof u);
    u.HasDecapitation = false; u.MaxHealth = 10.0f; u.Health = 10.0f; u.NextFilter = "";
    u.Initialise();
    ok &= u.decapitations == 0 && u.Health == 0.0f && u.Added.GroupCRC == 0 && u.modesAdded == 1;
    if (!ok) { printf("00867010_TEST FAIL\n"); return 1; }
    printf("00867010_TEST PASS\n");
    return 0;
}