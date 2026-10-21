#include <cstdio>
#include <cstdlib>
// behaviour test: CTCHeroExperience::FrameUpdate decays the multiplier param (clamped at 0) or counts the attenuation timer down,
// fires OnComboMultiplierDecrease when the pickup multiplier drops, ages out pickups, copies the param, and wipes a stale attack history
static long g_FramesPerSecond = 10;
static long g_frame = 0;
static bool g_paused = false;
struct CNode { CNode* next; CNode* prev; long a; long b; };
struct CList {
    CNode* head;
    CList() { head = (CNode*)std::malloc(sizeof(CNode)); head->next = head->prev = head; }
    void push_back(long a, long b) { CNode* n = (CNode*)std::malloc(sizeof(CNode)); n->a = a; n->b = b; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    int size() const { int c = 0; for (CNode* n = head->next; n != head; n = n->next) ++c; return c; }
    CNode* erase(CNode* pos) { CNode* next = pos->next; CNode* prev = pos->prev; prev->next = next; next->prev = prev; std::free(pos); return next; }
};
inline void ClampToZero(float& value) { if (value < 0.0f) value = 0.0f; }
struct CTCHeroExperience {
    float ExperienceMultiplierParam, LastExperienceMultiplierParam; long ExperienceAttenuationTimer;
    CList Pickups, AttackHistory; float PerUnit, Decay; int decreases;
    long GetExperiencePickUpMultiplier() const { long m = (long)(ExperienceMultiplierParam / PerUnit + 1.0); if (m < 1) m = 1; return m; }
    void OnComboMultiplierDecrease() { ++decreases; }
    void FrameUpdate()
    {
        if (!g_paused) {
            long oldMultiplier = GetExperiencePickUpMultiplier();
            if (ExperienceAttenuationTimer > 0) {
                --ExperienceAttenuationTimer;
            } else {
                ExperienceMultiplierParam -= Decay / g_FramesPerSecond;
                ClampToZero(ExperienceMultiplierParam);
            }
            if (GetExperiencePickUpMultiplier() < oldMultiplier)
                OnComboMultiplierDecrease();
        }
        for (CNode* node = Pickups.head->next; node != Pickups.head; ) {
            if (--node->a == 0) node = Pickups.erase(node); else node = node->next;
        }
        LastExperienceMultiplierParam = ExperienceMultiplierParam;
        long frame = g_frame;
        if (AttackHistory.head->next != AttackHistory.head) {
            CNode* last = AttackHistory.head->prev;
            if (last->a != -1 && frame > last->b - (long)(g_FramesPerSecond * -3.0f))
                for (CNode* node = AttackHistory.head->next; node != AttackHistory.head; node = node->next) { node->a = -1; node->b = 0; }
        }
    }
};
int main()
{
    CTCHeroExperience h; h.ExperienceMultiplierParam = 2.5f; h.LastExperienceMultiplierParam = 0; h.ExperienceAttenuationTimer = 1;
    h.PerUnit = 1.0f; h.Decay = 10.0f; h.decreases = 0;
    h.Pickups.push_back(1, 5); h.Pickups.push_back(3, 7);
    h.AttackHistory.push_back(2, 100); h.AttackHistory.push_back(4, 110);
    g_frame = 120;
    h.FrameUpdate();    // timer 1 -> 0, param kept; pickup 1 expires; history (110 + 30 = 140) still fresh
    bool ok = h.ExperienceAttenuationTimer == 0 && h.ExperienceMultiplierParam == 2.5f && h.decreases == 0 && h.Pickups.size() == 1
        && h.LastExperienceMultiplierParam == 2.5f && h.AttackHistory.head->prev->a == 4;
    g_frame = 141;
    h.FrameUpdate();    // param 2.5 -> 1.5: multiplier 3 -> 2 => decrease; history stale -> wiped
    ok = ok && h.ExperienceMultiplierParam == 1.5f && h.decreases == 1 && h.AttackHistory.head->next->a == -1
        && h.AttackHistory.head->prev->b == 0 && h.AttackHistory.size() == 2;
    h.FrameUpdate(); h.FrameUpdate();   // 1.5 -> 0.5 -> clamped 0
    ok = ok && h.ExperienceMultiplierParam == 0.0f && h.decreases == 2 && h.Pickups.size() == 0;
    g_paused = true; h.ExperienceMultiplierParam = 5.0f; h.FrameUpdate();
    ok = ok && h.ExperienceMultiplierParam == 5.0f && h.LastExperienceMultiplierParam == 5.0f;
    if (!ok) { std::printf("006d8580_TEST FAIL\n"); return 1; }
    std::printf("006d8580_TEST PASS\n");
    return 0;
}
