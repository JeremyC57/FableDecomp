// Behaviour test for 0x00C34440 CVoiceParams::GetGain: the channel's parameter block maps gain * gainScale (with the
// gain mask); a zero result or an unpositioned voice returns it unchanged, otherwise it is scaled by the distance
// falloff and by (1 - second falloff * amount).
#include <cstdio>
#include <cmath>

static float g_lastBlockValue = 0.0f;
static unsigned short g_lastMask = 0;
static float g_blockFactor = 1.0f;

struct ParamBlock {
    float GetGain(float value, unsigned short mask) const { g_lastBlockValue = value; g_lastMask = mask; return value * g_blockFactor; }
};
struct Channel { ParamBlock* Params; };
struct Vec { float x, y, z; };
struct Context { Channel* Chan; Vec Listener; };
struct Position {
    Vec P;
    float GetDistance(const Vec* l) const
    {
        float dx = P.x - l->x, dy = P.y - l->y, dz = P.z - l->z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};
struct Falloff {
    float Near, Far;
    float GetAttenuation(float d) const
    {
        if (d <= Near) return 1.0f;
        if (d >= Far) return 0.0f;
        return (Far - d) / (Far - Near);
    }
};
struct Voice {
    Position Pos;
    unsigned long Positioned;
    Falloff First, Second;
    float SecondAmount, Gain, GainScale;
    unsigned short GainMask;
    float GetGain(const Context* context) const
    {
        float gain = GainScale * Gain;
        float value = context->Chan->Params->GetGain(gain, GainMask);
        if (value != 0.0f && Positioned) {
            float distance = Pos.GetDistance(&context->Listener);
            float attenuation = First.GetAttenuation(distance);
            float falloff = 1.0f - Second.GetAttenuation(distance) * SecondAmount;
            float scale = attenuation * value;
            value = scale * falloff;
        }
        return value;
    }
};

static int near_(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    ParamBlock block; Channel chan = { &block };
    Context ctx; ctx.Chan = &chan; ctx.Listener.x = 0.0f; ctx.Listener.y = 0.0f; ctx.Listener.z = 0.0f;
    Voice v;
    v.Pos.P.x = 6.0f; v.Pos.P.y = 0.0f; v.Pos.P.z = 8.0f;          // distance 10
    v.Positioned = 0; v.First.Near = 5.0f; v.First.Far = 15.0f;     // attenuation 0.5 at 10
    v.Second.Near = 0.0f; v.Second.Far = 20.0f;                      // second attenuation 0.5 at 10
    v.SecondAmount = 0.5f; v.Gain = 0.8f; v.GainScale = 0.5f; v.GainMask = 0x21;
    int ok = 1;
    if (!near_(v.GetGain(&ctx), 0.4f) || !near_(g_lastBlockValue, 0.4f) || g_lastMask != 0x21) ok = 0;   // unpositioned
    v.Positioned = 1;
    if (!near_(v.GetGain(&ctx), 0.4f * 0.5f * (1.0f - 0.5f * 0.5f))) ok = 0;                          // 0.15
    g_blockFactor = 0.0f;
    if (v.GetGain(&ctx) != 0.0f) ok = 0;                                                               // zero short-cut
    g_blockFactor = 2.0f; ctx.Listener.x = 6.0f; ctx.Listener.z = 8.0f;                              // distance 0
    if (!near_(v.GetGain(&ctx), 0.8f * 1.0f * (1.0f - 1.0f * 0.5f))) ok = 0;                          // 0.4
    std::printf(ok ? "00c34440_TEST PASS\n" : "00c34440_TEST FAIL\n");
    return ok ? 0 : 1;
}