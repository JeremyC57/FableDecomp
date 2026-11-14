// Behaviour test for 0x0077B350 CTCGraphicAppearanceAnimatedMesh::OnAppearanceDraw: the first draw registers a persistent animated-mesh
// primitive (type 2, the main graphic's bank index) and then draws; later draws whose reason is 1, 2 or 3 only refresh the primitive's
// animation / position / orientation interpolators. A full draw builds a plain animating mesh when the draw info has no texture morphs,
// no decals and no hidden materials, otherwise a morphed animating mesh (an empty morph list standing in for a missing one; decals copied
// when present). Size is scale * render size; alpha, tint, fade distance and lighting channel come from the appearance; only the plain
// mesh carries the bullet-time flag. The mesh effect is attached and the primitive graphics are set with flags 0. Always returns true.
#include <cstdio>
#include <vector>
struct Info { std::vector<int>* morphs; std::vector<int>* decals; const char* hidden; unsigned long reasons; bool hiddenUsed; };
struct Appearance {
    float scale, renderSize, fade; long bank; unsigned char alpha; int tint, channel, effect;
    int added, addedType; long addedBank; int refreshed, set;
    bool morphed; float size; unsigned char mAlpha; int mTint, mChannel, mEffect; float mFade; bool mBullet; long mMesh;
    size_t morphCount, decalCount; const char* mHidden; int setHandle;
};
struct Component {
    int handle; bool bulletTime;
    bool Draw(Appearance& a, const Info& d) {
        float size = a.scale * a.renderSize;
        float fade = a.fade;
        if (handle == 0) { ++a.added; a.addedType = 2; a.addedBank = a.bank; handle = 77; }
        else if (d.reasons == 2 || d.reasons == 1 || d.reasons == 3) { ++a.refreshed; return true; }
        if (d.morphs == 0 && d.decals == 0 && !d.hiddenUsed) {
            a.morphed = false; a.mBullet = bulletTime; a.morphCount = 0; a.decalCount = 0; a.mHidden = 0;
        } else {
            const std::vector<int>* morphs = d.morphs; std::vector<int> none; if (morphs == 0) morphs = &none;
            a.morphed = true; a.mBullet = false; a.morphCount = morphs->size(); a.mHidden = d.hidden;
            a.decalCount = d.decals ? d.decals->size() : 0;
        }
        a.size = size; a.mAlpha = a.alpha; a.mTint = a.tint; a.mFade = fade; a.mChannel = a.channel; a.mEffect = a.effect; a.mMesh = a.bank;
        ++a.set; a.setHandle = handle;
        return true;
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static Appearance make() { Appearance a = Appearance(); a.scale = 2.0f; a.renderSize = 1.5f; a.fade = 40.0f; a.bank = 9; a.alpha = 200; a.tint = 0x112233; a.channel = 5; a.effect = 3; return a; }
int main() {
    Info d = { 0, 0, "hidden", 0, false };
    Component c = { 0, true }; Appearance a = make();
    check(c.Draw(a, d) && a.added == 1 && a.addedType == 2 && a.addedBank == 9 && c.handle == 77, "first draw registers the primitive");
    check(a.set == 1 && !a.morphed && a.size == 3.0f && a.mAlpha == 200 && a.mTint == 0x112233 && a.mFade == 40.0f && a.mChannel == 5 &&
          a.mEffect == 3 && a.mMesh == 9 && a.mBullet && a.setHandle == 77, "first draw also draws a plain mesh");
    d.reasons = 1; check(c.Draw(a, d) && a.refreshed == 1 && a.set == 1 && a.added == 1, "reason 1 refreshes only");
    d.reasons = 2; c.Draw(a, d); d.reasons = 3; c.Draw(a, d); check(a.refreshed == 3 && a.set == 1, "reasons 2 and 3 refresh only");
    d.reasons = 4; c.Draw(a, d); check(a.refreshed == 3 && a.set == 2 && a.added == 1, "other reasons redraw without re-registering");
    Component c2 = { 0, false }; Appearance a2 = make(); d.reasons = 1;
    c2.Draw(a2, d); check(a2.added == 1 && a2.set == 1 && a2.refreshed == 0, "a first draw ignores the refresh reason");
    std::vector<int> morphs(2), decals(3);
    d.reasons = 0; d.morphs = &morphs; c.Draw(a, d);
    check(a.morphed && a.morphCount == 2 && a.decalCount == 0 && !a.mBullet && a.mHidden == d.hidden, "morphs -> morphed mesh, no decals");
    d.morphs = 0; d.decals = &decals; c.Draw(a, d);
    check(a.morphed && a.morphCount == 0 && a.decalCount == 3, "decals only -> morphed mesh with an empty morph list");
    d.decals = 0; d.hiddenUsed = true; c.Draw(a, d);
    check(a.morphed && a.morphCount == 0 && a.decalCount == 0, "hidden materials only -> morphed mesh");
    d.hiddenUsed = false; c.Draw(a, d); check(!a.morphed && a.mBullet, "nothing special -> plain mesh again");
    if (!fails) std::printf("0077b350_TEST PASS\n");
    return fails;
}
