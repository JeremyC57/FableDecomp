#include <cstdio>
// Behaviour copy of C3DAnimationManager::CombineBlendedBaseAnimations (0x00A9F2F0): pairwise state trees, interpolated time/progress.
struct Key { float v; };
struct Mesh { int id; };
struct Anim { unsigned long AnimType; float lastTime; int calls;
    void Rel(Key* out, unsigned long n, float t, const Mesh& m) { ++calls; lastTime = t; for (unsigned long i = 0; i < n; ++i) out[i].v = t + (float)m.id; }
    void Abs(Key* out, unsigned long n, float t) { ++calls; lastTime = t; for (unsigned long i = 0; i < n; ++i) out[i].v = t; }
    bool IsRelative() const { return (AnimType & 4) != 0; } };
struct State { int type; long index; float time; State* start; State* end; float progress; long GetStateType() const { return type; } };
inline float InterpolateFloat(float from, float to, float t) { return from + (to - from) * t; }
struct Manager {
    Anim* anims; float lastWeight;
    void Blend(Key* a, Key* b, unsigned long n, float w) { lastWeight = w; for (unsigned long i = 0; i < n; ++i) a[i].v = a[i].v * (1 - w) + b[i].v * w; }
    void Combine(const State& a, const State& b, float blend, Key* out, unsigned long count, unsigned long depth, const Mesh& mesh) {
        long type = a.GetStateType();
        if (type == 0) {
            long index = a.index; float time = InterpolateFloat(a.time, b.time, blend); Anim* animation = &anims[index];
            if (animation->IsRelative()) animation->Rel(out, count, time, mesh); else animation->Abs(out, count, time);
        } else if (type == 2) {
            unsigned long childDepth = depth + 1;
            Combine(*a.start, *b.start, blend, out, count, childDepth, mesh);
            if (childDepth < 5) { Key* scratch = out + count; Combine(*a.end, *b.end, blend, scratch, count, childDepth, mesh);
                Blend(out, scratch, count, InterpolateFloat(a.progress, b.progress, blend)); }
        }
    }
};
int main() {
    Anim anims[2] = { {0, 0, 0}, {4, 0, 0} };
    State a0 = {0, 0, 1.0f, 0, 0, 0}, b0 = {0, 0, 3.0f, 0, 0, 0}, a1 = {0, 1, 0.0f, 0, 0, 0}, b1 = {0, 1, 4.0f, 0, 0, 0};
    State ta = {2, 0, 0, &a0, &a1, 0.0f}, tb = {2, 0, 0, &b0, &b1, 1.0f};
    Manager m = { anims, -1 }; Mesh mesh = { 10 }; Key keys[2];
    m.Combine(ta, tb, 0.5f, keys, 1, 0, mesh);
    // start: abs at t=2 -> 2 ; end: rel at t=2 -> 12 ; weight 0.5 -> 7
    if (anims[0].lastTime != 2.0f || anims[1].lastTime != 2.0f || m.lastWeight != 0.5f || keys[0].v != 7.0f) { std::printf("00a9f2f0_TEST FAIL\n"); return 1; }
    std::printf("00a9f2f0_TEST PASS\n"); return 0;
}