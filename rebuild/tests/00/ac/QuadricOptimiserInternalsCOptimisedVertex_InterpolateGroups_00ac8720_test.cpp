// Behaviour test for 0x00AC8720 COptimisedVertex::InterpolateGroups (bone-group weight blend)
#include <cstdio>

struct Groups {                       // sorted (bone, weight) pairs standing in for map<unsigned long, float>
    unsigned int key[16]; float val[16]; int n;
    float& insert0(unsigned int k)    // insert(value_type(k, 0)).first->second
    {
        int i = 0;
        while (i < n && key[i] < k) ++i;
        if (i < n && key[i] == k) return val[i];
        for (int j = n; j > i; --j) { key[j] = key[j - 1]; val[j] = val[j - 1]; }
        key[i] = k; val[i] = 0.0f; ++n;
        return val[i];
    }
};

static void InterpolateGroups(Groups& self, const Groups& other, float t)
{
    for (int i = 0; i < self.n; ++i)
        self.val[i] *= 1.0f - t;
    for (int j = 0; j < other.n; ++j) {
        float weight = other.val[j] * t;
        self.insert0(other.key[j]) += weight;
    }
}

static bool near(float a, float b) { return a - b < 1e-5f && b - a < 1e-5f; }

int main()
{
    Groups a; a.n = 0; Groups b; b.n = 0;
    a.insert0(3) = 1.0f; a.insert0(7) = 0.5f;
    b.insert0(7) = 1.0f; b.insert0(9) = 0.25f;
    InterpolateGroups(a, b, 0.25f);
    if (a.n != 3 || a.key[0] != 3 || a.key[1] != 7 || a.key[2] != 9) { std::printf("00ac8720_TEST FAIL\n"); return 1; }
    if (!near(a.val[0], 0.75f) || !near(a.val[1], 0.375f + 0.25f) || !near(a.val[2], 0.0625f)) { std::printf("00ac8720_TEST FAIL\n"); return 1; }
    std::printf("00ac8720_TEST PASS\n");
    return 0;
}