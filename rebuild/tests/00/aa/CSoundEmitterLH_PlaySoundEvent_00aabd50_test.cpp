#include <cstdio>
// Behaviour model of CSoundEmitterLH::PlaySoundEvent 0x00AABD50: option building and the play / track decision.
// The send-fader setters are modelled through SetMaskBit 0x00C0AE70 as retail runs it: `cmp ecx,count ; jae` ignores an
// index >= the bit count (16 for SetFlag16 0x00C0A8A0, 8 for SetFlag 0x00C0A8C0), else ORs the bit into the touched word and
// sets it in the state word. Shl1 is x86 `shl r32,cl` (count taken mod 32), which retail uses for every `1 << bit`.
struct Vec { float X, Y, Z; };
struct Options { unsigned int EmitterId; float Pitch; bool Is3D; Vec Pos; bool HeadRelative;
                 unsigned short WhichGain, WhichPitch, Gain, PitchF; };
struct Init { float Pitch; unsigned long GainFaderBits, PitchFaderBits; long OverrideID; };
static unsigned long Shl1(unsigned long count) { return 1ul << (count % 32); }
static int FindFirstSetBit(unsigned long mask, unsigned long numBytes)
{
    for (unsigned long i = 0; i < numBytes * 8; ++i) if (mask & Shl1(i)) return (int)i;
    return -1;
}
static void SetMaskBit(unsigned long index, bool set, unsigned short* pMask, unsigned short* pTouched, unsigned long count)
{
    if (index >= count) return;
    unsigned short b = (unsigned short)Shl1(index);
    *pTouched |= b;
    if (set) *pMask |= b; else *pMask &= (unsigned short)~b;
}
struct Emitter { bool hasSystem, enabled; Vec head, Position; bool Positional, Playing; long EmitterID; int tracked; long nextHandle; Options last; };
static unsigned long Play(Emitter& e, bool loop, const Init& init, bool attached)
{
    if (!e.hasSystem || !e.enabled) return (unsigned long)-1;
    Options o = { 0, 1.0f, false, {0,0,0}, false, 0, 0, 0, 0 };
    o.Pitch = init.Pitch;
    o.Is3D = loop ? false : e.Positional;
    o.HeadRelative = attached;
    unsigned long bits = init.GainFaderBits;
    while (bits) {
        int bit = FindFirstSetBit(bits, 16);
        SetMaskBit((unsigned long)bit, true, &o.Gain, &o.WhichGain, 16);
        bits ^= Shl1((unsigned long)bit);
    }
    unsigned long pitchBits = init.PitchFaderBits;
    while (pitchBits) {
        int bit = FindFirstSetBit(bits, 16);          // as retail: the exhausted gain mask, so always -1
        SetMaskBit((unsigned long)bit, true, &o.PitchF, &o.WhichPitch, 8);
        pitchBits ^= Shl1((unsigned long)bit);
    }
    if (o.HeadRelative) o.Pos = e.head; else if (o.Is3D) o.Pos = e.Position;
    o.EmitterId = (init.OverrideID == -1) ? e.EmitterID : init.OverrideID;
    long handle = e.nextHandle;
    if (handle >= 0) e.tracked++;
    e.Playing = true;
    e.last = o;
    return (unsigned long)handle;
}
int main()
{
    Emitter e = { true, true, {1,2,3}, {4,5,6}, true, false, 7, 0, 5 };
    Init init = { 0.5f, 0x5, 0, -1 };
    bool ok = true;
    if (Play(e, false, init, false) != 5 || e.tracked != 1 || !e.Playing) ok = false;
    if (e.last.Gain != 5 || e.last.WhichGain != 5 || e.last.Pitch != 0.5f || !e.last.Is3D || e.last.Pos.X != 4 || e.last.EmitterId != 7) ok = false;
    // Pitch-fader branch: the hoisted index is -1, SetFlag ignores it, and `1 << -1` is bit 31 -- so the loop ends only for
    // bit 31 (any other mask spins forever in retail); no pitch fader bit is ever set.
    init.PitchFaderBits = 0x80000000ul; init.GainFaderBits = 0x8002;
    if (Play(e, false, init, false) != 5 || e.last.PitchF != 0 || e.last.WhichPitch != 0 || e.last.Gain != 0x8002 || e.last.WhichGain != 0x8002) ok = false;
    if (FindFirstSetBit(0, 16) != -1 || Shl1((unsigned long)-1) != 0x80000000ul) ok = false;
    init.PitchFaderBits = 0; init.GainFaderBits = 0;
    init.OverrideID = 9;
    if (Play(e, true, init, true) != 5 || e.last.Is3D || e.last.Pos.Z != 3 || e.last.EmitterId != 9 || e.last.Gain != 0) ok = false;
    e.nextHandle = -2; e.Playing = false;
    if ((long)Play(e, false, init, false) != -2 || e.tracked != 3 || !e.Playing) ok = false;
    e.enabled = false;
    if (Play(e, false, init, false) != (unsigned long)-1) ok = false;
    if (!ok) { std::printf("00aabd50_TEST FAIL\n"); return 1; }
    std::printf("00aabd50_TEST PASS\n");
    return 0;
}