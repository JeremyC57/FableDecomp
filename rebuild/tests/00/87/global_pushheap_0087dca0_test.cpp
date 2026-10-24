// Behaviour test for 0087dca0: __push_heap over const CAnimationEntry* with CTurningAnimCompare -- retail's functor reads
// the LEFT operand twice, so the comparison is always false: the value is stored at holeIndex, nothing moves, and only
// the parent's rotation is ever queried (twice per call).
#include <cstdio>
#include <cmath>

static int g_queries[8];
class CAnimationEntry {
public:
    int   Id;
    float Rotation;
    void GetRotationInAnimation(float* rotation) const { ++g_queries[Id]; *rotation = -Rotation; }
};

struct CTurningAnimCompare {
    float RotationAngle;
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const
    {
        float rotation_a;
        float rotation_b;
        a->GetRotationInAnimation(&rotation_a);
        a->GetRotationInAnimation(&rotation_b);
        float delta_a = (float)std::fabs(RotationAngle - rotation_a);
        float delta_b = (float)std::fabs(RotationAngle - rotation_b);
        return delta_a < delta_b;
    }
};

void __push_heap(const CAnimationEntry** first, int holeIndex, int topIndex, const CAnimationEntry* val,
                 CTurningAnimCompare comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}

int main()
{
    CAnimationEntry anims[8];
    for (int i = 0; i < 8; ++i) {
        anims[i].Id = i;
        anims[i].Rotation = 10.0f * i;
    }
    const CAnimationEntry* heap[8];
    for (int i = 0; i < 7; ++i)
        heap[i] = &anims[i];
    CTurningAnimCompare comp;
    comp.RotationAngle = 0.0f;                      // anims[7] would be "farthest", yet nothing sifts
    __push_heap(heap, 7, 0, &anims[7], comp);
    for (int i = 0; i < 7; ++i)
        if (heap[i] != &anims[i]) { std::printf("0087dca0_TEST FAIL\n"); return 1; }
    if (heap[7] != &anims[7] || g_queries[3] != 2 || g_queries[7] != 0) { std::printf("0087dca0_TEST FAIL\n"); return 1; }
    __push_heap(heap, 2, 2, &anims[5], comp);       // holeIndex == topIndex: plain store, no query
    if (heap[2] != &anims[5] || g_queries[0] != 0) { std::printf("0087dca0_TEST FAIL\n"); return 1; }
    std::printf("0087dca0_TEST PASS\n");
    return 0;
}