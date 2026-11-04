// Behaviour test for 0x00753380 CTCLook::FrameUpdateHeadInterested(): a model. With a targeting component that looks at the camera
// the head looks at the camera position (target or not); otherwise with no nearest interesting thing the head is centred and
// nothing else happens; with one, the head looks at that thing's head (look component), else its loaded focal point, else its
// physics position. Before looking, the medium speed bit 0x10 is set when none of the speed bits 0x38 is.
#include <cstdio>

struct Vec { float X, Y, Z; };
struct Thing { bool hasLook; Vec head; bool hasAppearance; bool focalLoaded; Vec focal; Vec position; };
struct Targeting { Thing* nearest; bool lookAtCamera; };
struct Look { long flags; int centred; int looks; Vec lookAt; };

static void FrameUpdateHeadInterested(Look& me, const Targeting* targeting, const Vec& camera)
{
    const Thing* target = 0;
    if (targeting)
        target = targeting->nearest;
    if (target == 0 && !(targeting && targeting->lookAtCamera)) {   // retail reads an unset pointer when there is no targeting
        ++me.centred;
        return;
    }
    Vec pos;
    if (targeting->lookAtCamera)
        pos = camera;
    else if (target->hasLook)
        pos = target->head;
    else if (target->hasAppearance && target->focalLoaded)
        pos = target->focal;
    else
        pos = target->position;
    if (!(me.flags & 0x38))
        me.flags |= 0x10;
    ++me.looks;
    me.lookAt = pos;
}

int main()
{
    int ok = 1;
    Vec camera = { 9, 9, 9 };
    Thing a = { true, { 1, 1, 1 }, true, true, { 2, 2, 2 }, { 3, 3, 3 } };
    Thing b = { false, { 0, 0, 0 }, true, true, { 2, 2, 2 }, { 3, 3, 3 } };
    Thing c = { false, { 0, 0, 0 }, true, false, { 2, 2, 2 }, { 3, 3, 3 } };
    Thing d = { false, { 0, 0, 0 }, false, true, { 2, 2, 2 }, { 4, 4, 4 } };
    Look me = { 0, 0, 0, { 0, 0, 0 } };
    Targeting none = { 0, false };
    FrameUpdateHeadInterested(me, &none, camera);
    ok &= me.centred == 1 && me.looks == 0 && me.flags == 0;
    Targeting cam = { 0, true };
    FrameUpdateHeadInterested(me, &cam, camera);
    ok &= me.looks == 1 && me.lookAt.X == 9 && me.flags == 0x10;
    Targeting camT = { &a, true };
    FrameUpdateHeadInterested(me, &camT, camera);
    ok &= me.lookAt.X == 9;
    Targeting ta = { &a, false }, tb = { &b, false }, tc = { &c, false }, td = { &d, false };
    me.flags = 0x08;
    FrameUpdateHeadInterested(me, &ta, camera); ok &= me.lookAt.X == 1 && me.flags == 0x08;
    FrameUpdateHeadInterested(me, &tb, camera); ok &= me.lookAt.X == 2;
    FrameUpdateHeadInterested(me, &tc, camera); ok &= me.lookAt.X == 3;
    FrameUpdateHeadInterested(me, &td, camera); ok &= me.lookAt.X == 4 && me.centred == 1 && me.looks == 6;
    std::printf(ok ? "00753380_TEST PASS\n" : "00753380_TEST FAIL\n");
    return ok ? 0 : 1;
}