// Behaviour test for 0x00754050 CTCLook::UpdateTurningAnims: model of the turning-anim table rebuild.
#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
#include <utility>
static const float DELTA_NOTIONAL_ZERO = 1e-4f;
struct Anim { const char* name; float rotation; int group; };
struct Look {
    std::vector<std::pair<float, const Anim*> > TurningAnims;
    float TurningViaAnimMinAngle;
};
static void Collect(const Anim* set, int n, std::vector<const Anim*>& out, int group)
{
    for (int i = 0; i < n; ++i)
        if (std::strncmp(set[i].name, "TURNING_", 8) == 0 && (group < 0 || set[i].group == group)) out.push_back(&set[i]);
}
static bool Update(Look& l, bool hasModeManager, const Anim* set, int n, int modeGroup, int defaultGroup)
{
    if (!hasModeManager || !set) return false;
    l.TurningAnims.clear();
    l.TurningViaAnimMinAngle = 0.5f;
    std::vector<const Anim*> anims;
    Collect(set, n, anims, modeGroup);
    Collect(set, n, anims, defaultGroup);
    if (anims.empty()) Collect(set, n, anims, -1);
    for (std::vector<const Anim*>::iterator it = anims.begin(); it != anims.end(); ++it) {
        const Anim* a = *it;
        float rotation = a->rotation;
        if (std::fabs(rotation) > DELTA_NOTIONAL_ZERO) {
            l.TurningAnims.push_back(std::pair<float, const Anim*>(rotation, a));
            float size = std::fabs(rotation);
            float limit = 0.5f - DELTA_NOTIONAL_ZERO;
            if (size > limit) size = limit;
            long sign = -1;
            if (!(rotation < 0.0f)) sign = 1;
            rotation = sign * size;
            if (std::fabs(rotation) < l.TurningViaAnimMinAngle) l.TurningViaAnimMinAngle = std::fabs(rotation);
        }
    }
    return true;
}
int main()
{
    Anim set[] = { {"TURNING_L", -0.25f, 1}, {"TURNING_R", 0.9f, 2}, {"WALK", 0.1f, 1}, {"TURNING_Z", 0.0f, 1}, {"TURNING_X", 0.3f, 3} };
    Look l; l.TurningViaAnimMinAngle = 7.0f;
    bool ok = true;
    if (Update(l, false, set, 5, 1, 2)) ok = false;
    if (l.TurningViaAnimMinAngle != 7.0f) ok = false;
    Update(l, true, set, 5, 1, 2);
    if (l.TurningAnims.size() != 2 || l.TurningAnims[0].second != &set[0] || l.TurningAnims[1].first != 0.9f) ok = false;
    if (std::fabs(l.TurningViaAnimMinAngle - 0.25f) > 1e-6f) ok = false;
    Update(l, true, set, 5, 7, 8);               // no group match: all TURNING_ anims, zero-rotation one skipped
    if (l.TurningAnims.size() != 3) ok = false;
    if (std::fabs(l.TurningViaAnimMinAngle - 0.25f) > 1e-6f) ok = false;
    Anim big[] = { {"TURNING_B", -2.0f, 1} };
    Update(l, true, big, 1, 1, 1);               // clamp to 0.5 - delta (twice collected)
    if (l.TurningAnims.size() != 2 || std::fabs(l.TurningViaAnimMinAngle - (0.5f - DELTA_NOTIONAL_ZERO)) > 1e-6f) ok = false;
    if (!ok) { std::printf("00754050_TEST FAIL\n"); return 1; }
    std::printf("00754050_TEST PASS\n");
    return 0;
}