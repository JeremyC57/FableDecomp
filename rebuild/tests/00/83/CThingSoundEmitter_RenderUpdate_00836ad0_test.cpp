// Behaviour test for CThingSoundEmitter::RenderUpdate (0x00836AD0): the dead / base / attached-thing-died / finished /
// follow-position paths, with the interpolated position OldPosition + (Position - OldPosition) * t.
#include <cstdio>

struct Vec { float X, Y, Z; };

struct Physics {
    Vec Position, OldPosition;
    Vec GetPosition(float t) const
    {
        Vec r = { (Position.X - OldPosition.X) * t + OldPosition.X, (Position.Y - OldPosition.Y) * t + OldPosition.Y,
                  (Position.Z - OldPosition.Z) * t + OldPosition.Z };
        return r;
    }
};

struct Thing {
    bool Dead;
    Physics* PhysicsTC;
    int killed;
};

struct Sound {
    bool playing;
    int stops, updates, sets;
    Vec pos;
};

struct Emitter {
    Thing base;
    Sound* SoundObject;
    bool AttachedToThing;
    Thing* PAttachedThing;
    bool ContinuePlayingAfterAttachedThingDeath;
};

static bool IsThingAlive(const Thing* t) { return t != 0 && !t->Dead; }
static bool BaseRenderUpdate() { return false; }   // CThing::RenderUpdate 0x004C7460

static bool RenderUpdate(Emitter& e, float wfInterpolate)
{
    if (!e.base.Dead) {
        if (BaseRenderUpdate() == true)
            return true;
        if (e.AttachedToThing && !IsThingAlive(e.PAttachedThing)) {
            if (!e.ContinuePlayingAfterAttachedThingDeath) { ++e.base.killed; return true; }
            ++e.SoundObject->stops;
        }
        if (!e.SoundObject->playing) { ++e.base.killed; return true; }
        if (e.AttachedToThing && IsThingAlive(e.PAttachedThing)) {
            Vec pos;
            pos = e.PAttachedThing->PhysicsTC->GetPosition(wfInterpolate);
            e.SoundObject->pos = pos;
            ++e.SoundObject->sets;
        }
        ++e.SoundObject->updates;
    }
    return false;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    int bad = 0;
    Physics ph = { { 10.0f, 20.0f, 30.0f }, { 0.0f, 0.0f, 0.0f } };
    Thing target = { false, &ph, 0 };
    Sound snd = { true, 0, 0, 0, { 0.0f, 0.0f, 0.0f } };
    Emitter e = { { false, 0, 0 }, &snd, true, &target, false };

    if (RenderUpdate(e, 0.5f) != false) bad |= 1;                 // follows the target
    if (snd.sets != 1 || snd.updates != 1 || !Near(snd.pos.X, 5.0f) || !Near(snd.pos.Z, 15.0f)) bad |= 2;

    target.Dead = true;                                            // target died, emitter not kept
    if (RenderUpdate(e, 0.5f) != true || e.base.killed != 1 || snd.stops != 0) bad |= 4;

    e.ContinuePlayingAfterAttachedThingDeath = true;               // kept: loops stopped, keeps updating
    if (RenderUpdate(e, 0.5f) != false || snd.stops != 1 || snd.sets != 1 || snd.updates != 2) bad |= 8;

    snd.playing = false;                                           // finished -> killed
    if (RenderUpdate(e, 0.5f) != true || e.base.killed != 2 || snd.updates != 2) bad |= 16;

    e.base.Dead = true;                                            // dead emitter: nothing
    if (RenderUpdate(e, 0.5f) != false || snd.stops != 2 || e.base.killed != 2) bad |= 32;
    // (the dead check comes first, so the previous call's stop is the last one)
    if (snd.stops != 2) bad |= 64;

    if (bad) { std::printf("00836ad0_TEST FAIL\n"); return 1; }
    std::printf("00836ad0_TEST PASS\n");
    return 0;
}