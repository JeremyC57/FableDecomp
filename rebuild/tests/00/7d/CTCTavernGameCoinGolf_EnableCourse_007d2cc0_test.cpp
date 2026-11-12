// Behaviour test for CTCTavernGameCoinGolf::EnableCourse (0x007D2CC0): course pieces shown / hidden by name flag, angle wrap.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>

struct Thing { std::string ScriptName; int drawable; int frameUpdated; int rendered; };
class Coin { public: Thing* PThing; };
struct Golf {
    long Courses;
    std::vector<Coin> Coins;
    float ZAngle, XAngle, StartX, StartY;
    void EnableCourse(long course)
    {
        if (course >= Courses)
            return;
        for (unsigned long i = 0; i < Coins.size(); ++i) {
            std::string name = Coins[i].PThing->ScriptName;
            if (name == "" || name == "NULL")
                continue;
            if (course + 1 < (long)name.size()) {
                if (name.c_str()[course + 1] == '1') {
                    Coins[i].PThing->drawable = 1;
                    Coins[i].PThing->frameUpdated++;
                    Coins[i].PThing->rendered = 4;
                } else {
                    Coins[i].PThing->drawable = 0;
                }
            }
        }
        float angle;
        XAngle = 0.45f;
        angle = (float)atan2(StartX, StartY) * 0.15915494f;
        if (angle < 0.0f || angle >= 1.0f) {
            angle = (float)fmod((double)angle, 1.0);
            if (angle < 0.0f)
                angle += 1.0f;
        }
        ZAngle = angle;
    }
};

int main()
{
    Thing a = { "P101", -1, 0, 0 }, b = { "P010", -1, 0, 0 }, c = { "NULL", -1, 0, 0 }, d = { "P1", -1, 0, 0 }, e = { "", -1, 0, 0 };
    Golf g; g.Courses = 3; g.ZAngle = 9.0f; g.XAngle = 0.0f; g.StartX = -1.0f; g.StartY = 0.0f;
    Coin cs[5] = { { &a }, { &b }, { &c }, { &d }, { &e } };
    for (int k = 0; k < 5; ++k) g.Coins.push_back(cs[k]);
    g.EnableCourse(0);
    int ok = a.drawable == 1 && a.frameUpdated == 1 && a.rendered == 4 && b.drawable == 0 && c.drawable == -1
          && d.drawable == 1 && e.drawable == -1 && g.XAngle == 0.45f && std::fabs(g.ZAngle - 0.75f) < 1e-5f;
    g.EnableCourse(2);                       // 'P101'[3] = '1'; 'P1' too short
    ok = ok && a.frameUpdated == 2 && d.drawable == 1 && d.frameUpdated == 1 && b.drawable == 0;
    g.ZAngle = 5.0f; g.XAngle = 0.0f; g.EnableCourse(3);   // course out of range: nothing changes
    ok = ok && g.ZAngle == 5.0f && g.XAngle == 0.0f;
    if (!ok) { std::printf("007d2cc0_TEST FAIL\n"); return 1; }
    std::printf("007d2cc0_TEST PASS\n");
    return 0;
}