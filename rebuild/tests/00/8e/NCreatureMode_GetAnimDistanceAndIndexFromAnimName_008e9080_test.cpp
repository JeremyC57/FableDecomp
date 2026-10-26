// Behaviour test for 0x008E9080 NCreatureMode::GetAnimDistanceAndIndexFromAnimName: index out, distance per frame out.
#include <cstdio>
#include <cstring>

static long g_FramesPerSecond = 30;

class C3DAnimationInfo { public: float Duration; float GetDuration() const { return Duration; } };
class CMeshDataBank { public:
    C3DAnimationInfo anims[4];
    const C3DAnimationInfo* PeekAnimation(unsigned int index) const { return &anims[index]; }
};
static CMeshDataBank g_bank;

class CAnimationEntry { public: long BankIndex; };

class CThingCreatureBase { public:
    CAnimationEntry entries[2];
    float           distances[4];
    const CAnimationEntry* GetAnimFromName(const char* name, long) const { return std::strcmp(name, "WALK") ? &entries[1] : &entries[0]; }
    float GetAnimMovementDistance(long animIndex) const { return distances[animIndex]; }
};

static bool GetAnimDistanceAndIndexFromAnimName(const CThingCreatureBase& creature, const char* animName, long& index,
                                                float& distance)
{
    CMeshDataBank* bank = &g_bank;
    index = creature.GetAnimFromName(animName, 0)->BankIndex;
    float duration = bank->PeekAnimation(index)->GetDuration();
    float speed = creature.GetAnimMovementDistance(index) / duration;
    distance = speed / (float)g_FramesPerSecond;
    return true;
}

int main()
{
    g_bank.anims[2].Duration = 2.0f;
    g_bank.anims[3].Duration = 0.5f;
    CThingCreatureBase c;
    c.entries[0].BankIndex = 2; c.entries[1].BankIndex = 3;
    c.distances[2] = 6.0f; c.distances[3] = 1.5f;
    long index = -1;
    float distance = 0.0f;
    int fail = 0;
    if (!GetAnimDistanceAndIndexFromAnimName(c, "WALK", index, distance) || index != 2) fail = 1;
    float d = distance - 0.1f;                       // 6 / 2 / 30
    if (d > 0.0001f || d < -0.0001f) fail = 1;
    if (!GetAnimDistanceAndIndexFromAnimName(c, "RUN", index, distance) || index != 3) fail = 1;
    d = distance - 0.1f;                             // 1.5 / 0.5 / 30
    if (d > 0.0001f || d < -0.0001f) fail = 1;
    if (fail) { std::printf("008e9080_TEST FAIL\n"); return 1; }
    std::printf("008e9080_TEST PASS\n");
    return 0;
}