// Behaviour test for 0x008570D0 CCombatActionBase::GetStrikeEventHelperName.
#include <cstdio>
#include <cstring>
class CCharString { public: char text[32]; CCharString(const char* t, long) { std::strcpy(text, t); } };
struct CStrikeEvent { bool Legacy; CCharString HelperPointName; CStrikeEvent(bool l, const char* n) : Legacy(l), HelperPointName(n, -1) { } };
class CCombatActionBase { public:
    CStrikeEvent* active;
    CStrikeEvent* GetPActiveStrikeEvent() const { return active; }
    CCharString GetStrikeEventHelperName() const
    {
        CStrikeEvent* pEvent = GetPActiveStrikeEvent();
        if (pEvent != 0) { if (!pEvent->Legacy) return pEvent->HelperPointName; }
        return CCharString("", -1);
    }
};
int main()
{
    CStrikeEvent modern(false, "HAND_R"), legacy(true, "OLD");
    CCombatActionBase a; a.active = &modern;
    if (std::strcmp(a.GetStrikeEventHelperName().text, "HAND_R") != 0) { std::printf("008570d0_TEST FAIL\n"); return 1; }
    a.active = &legacy;
    if (std::strcmp(a.GetStrikeEventHelperName().text, "") != 0) { std::printf("008570d0_TEST FAIL\n"); return 1; }
    a.active = 0;
    if (std::strcmp(a.GetStrikeEventHelperName().text, "") != 0) { std::printf("008570d0_TEST FAIL\n"); return 1; }
    std::printf("008570d0_TEST PASS\n");
    return 0;
}