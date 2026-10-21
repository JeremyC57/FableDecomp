#include <cstdio>
// Behaviour copy of C3DAnimationManager::CombineBaseAnimationsBoneList (0x00AA0C50): leaf sampling and depth-limited blending.
struct Key { float v; };
struct Mesh { int id; };
struct Anim { unsigned long AnimType; int relCalls, absCalls;
    void Rel(Key* out, unsigned long bone, float t, const Mesh& m) { ++relCalls; out->v = (float)bone + t + (float)m.id; }
    void Abs(Key* out, unsigned long bone, float t) { ++absCalls; out->v = (float)bone + t; }
    bool IsRelative() const { return (AnimType & 4) != 0; } };
struct State { int type; long index; float time; State* a; State* b; float progress; long GetStateType() const { return type; } };
struct Manager {
    Anim* anims; int blends;
    void Blend(Key* a, Key* b, unsigned long n, float w) { ++blends; for (unsigned long i = 0; i < n; ++i) a[i].v = a[i].v * (1 - w) + b[i].v * w; }
    void Combine(const State& s, Key* out, unsigned long count, unsigned long depth, const Mesh& mesh, const unsigned long* bones) {
        long type = s.GetStateType();
        if (type == 0) {
            Anim* animation = &anims[s.index]; float time = s.time;
            if (animation->IsRelative()) { for (unsigned long i = 0; i < count; ++i) animation->Rel(&out[i], bones[i], time, mesh); }
            else { for (unsigned long i = 0; i < count; ++i) animation->Abs(&out[i], bones[i], time); }
        } else if (type == 2) {
            unsigned long childDepth = depth + 1;
            Combine(*s.a, out, count, childDepth, mesh, bones);
            if (childDepth < 5) { Key* scratch = out + count; Combine(*s.b, scratch, count, childDepth, mesh, bones); Blend(out, scratch, count, s.progress); }
        }
    }
};
int main() {
    Anim anims[2] = { {0, 0, 0}, {4, 0, 0} };
    State l0 = {0, 0, 1.0f, 0, 0, 0}, l1 = {0, 1, 3.0f, 0, 0, 0};
    State t = {2, 0, 0, &l0, &l1, 0.5f};
    Manager m = { anims, 0 }; Mesh mesh = { 10 }; unsigned long bones[2] = { 1, 2 }; Key keys[4];
    m.Combine(t, keys, 2, 0, mesh, bones);
    // l0 abs: 2, 3 ; l1 rel: 14, 15 ; blend 0.5: 8, 9
    if (keys[0].v != 8.0f || keys[1].v != 9.0f || anims[0].absCalls != 2 || anims[1].relCalls != 2 || m.blends != 1) { std::printf("00aa0c50_TEST FAIL\n"); return 1; }
    Key deep[4]; m.blends = 0; m.Combine(t, deep, 2, 4, mesh, bones);   // depth limit: start state only
    if (m.blends != 0 || deep[0].v != 2.0f) { std::printf("00aa0c50_TEST FAIL\n"); return 1; }
    std::printf("00aa0c50_TEST PASS\n"); return 0;
}