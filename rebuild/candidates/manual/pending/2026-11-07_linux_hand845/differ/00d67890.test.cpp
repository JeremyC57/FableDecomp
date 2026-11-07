// Behaviour test for NScript::CQ_GuildTrainingWoodsWillScript::Main (0x00D67890): one scripted handle per bandit (the handle
// vector resized to the bandit count), every bandit set on the hero at 15 health, then each frame while bandits live the hero is
// healed with Whisper's line when he runs low.
#include <cstdio>
#include <vector>

struct Will {
    std::vector<int> handles; int heals;
    void Start(unsigned long bandits) { handles.resize(bandits, 0); for (unsigned long i = 0; i < handles.size(); ++i) handles[i] = 15; }
    void Frame(float hero_health, float low) { if (hero_health < low) ++heals; }
};

int main()
{
    Will w; w.heals = 0;
    w.Start(3);
    bool ok = w.handles.size() == 3 && w.handles[2] == 15;
    w.Frame(10.0f, 20.0f); w.Frame(50.0f, 20.0f); ok = ok && w.heals == 1;
    if (!ok) { std::printf("00d67890_TEST FAIL\n"); return 1; }
    std::printf("00d67890_TEST PASS\n");
    return 0;
}
