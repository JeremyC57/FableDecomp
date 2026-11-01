// Behaviour test for 0x00767DA0 CTCObjectAugmentations::UpdateParticleAttachment: helper point first, dummy object as the
// fallback (seeded with the owner's frame), START / END vector parameters keyed by CRC, sorted, handed to the emitter only
// when the effect has a particle-emitter component.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };
struct Param { unsigned long key; C3DVector value; };
struct Params { Param v[4]; int count; bool dirty; Params() : count(0), dirty(false) { }
    void Insert(const Param& p) { v[count++] = p; dirty = true; }
    void Update() { if (dirty) { for (int i = 1; i < count; i++) for (int j = i; j > 0 && v[j].key < v[j - 1].key; j--) { Param t = v[j]; v[j] = v[j - 1]; v[j - 1] = t; } dirty = false; } } };
static unsigned long Calc(const char* s, unsigned long n) { unsigned long h = 0; for (unsigned long i = 0; i < n; i++) h = h * 131 + (unsigned char)s[i]; return h; }
struct Appearance {
    bool hasHelper[2]; C3DVector helper[2], dummy[2]; int dummyCalls;
    bool GetHelperPoint(int name, C3DVector& out) const { if (!hasHelper[name]) return false; out = helper[name]; return true; }
    bool GetDummyObject(int name, C3DVector& out, CRightHandedSet& frame) { dummyCalls++; out = dummy[name]; frame.Up.X += 1.0f; return true; }
};
struct Emitter { Params got; int calls; };
static void UpdateParticleAttachment(Appearance& app, const CRightHandedSet& last, Emitter* emitter, bool hasEmitterTC, int startPoint, int endPoint)
{
    CRightHandedSet frame = last;
    C3DVector start;
    if (!app.GetHelperPoint(startPoint, start)) app.GetDummyObject(startPoint, start, frame);
    C3DVector end;
    if (!app.GetHelperPoint(endPoint, end)) app.GetDummyObject(endPoint, end, frame);
    Param s = { Calc("START", 5), start }, e = { Calc("END", 3), end };
    Params parameters;
    parameters.Insert(s);
    parameters.Insert(e);
    parameters.Update();
    if (hasEmitterTC && emitter) { emitter->got = parameters; emitter->calls++; }
}
int main()
{
    Appearance app = { { true, false }, { { 1, 2, 3 }, { 0, 0, 0 } }, { { 9, 9, 9 }, { 4, 5, 6 } }, 0 };
    CRightHandedSet last = { { 0, 0, 1 }, { 0, 1, 0 } };
    Emitter em; em.calls = 0;
    UpdateParticleAttachment(app, last, &em, true, 0, 1);
    if (em.calls != 1 || app.dummyCalls != 1 || em.got.count != 2 || em.got.dirty) { std::printf("00767da0_TEST FAIL\n"); return 1; }
    if (!(em.got.v[0].key < em.got.v[1].key)) { std::printf("00767da0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 2; i++) {
        const Param& p = em.got.v[i];
        const C3DVector& want = p.key == Calc("START", 5) ? app.helper[0] : app.dummy[1];
        if (p.value.X != want.X || p.value.Y != want.Y || p.value.Z != want.Z) { std::printf("00767da0_TEST FAIL\n"); return 1; }
    }
    UpdateParticleAttachment(app, last, &em, false, 0, 1);
    UpdateParticleAttachment(app, last, 0, true, 0, 1);
    if (em.calls != 1) { std::printf("00767da0_TEST FAIL\n"); return 1; }
    std::printf("00767da0_TEST PASS\n");
    return 0;
}