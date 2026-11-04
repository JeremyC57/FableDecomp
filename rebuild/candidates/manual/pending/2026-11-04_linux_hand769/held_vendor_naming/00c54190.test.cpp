// Behaviour test for CDecodedStream::SetProperties 0x00C54190: the audible flag follows room > -10000; position and
// velocity always go to their setters; each property that changed is stored and pushed to its pending setter (millibel
// levels converted, unsigned references as floats), unchanged ones are skipped; the HF reference is always pushed; the
// effect is configured from the pending block with mode 0.
#include <cstdio>
#include <cstring>
#include <cmath>

struct Vec { float X, Y, Z; };
struct Props {
    Vec Position, Velocity; long Room, RoomLF, RoomHF; float DecayTime, DecayLFRatio, DecayHFRatio; long Reflections;
    float ReflectionsDelay; long Reverb; float ReverbDelay, EnvironmentSize, m44, m48, EchoTime, EchoDepth, ModulationTime,
    ModulationDepth; unsigned int HFReference, LFReference; float m64, m68;
};
struct Update { unsigned char pad[0xc]; Props Properties; };
static int g_ids[32]; static float g_vals[32]; static int g_n;
static void Hit(int id, float v) { if (g_n < 32) { g_ids[g_n] = id; g_vals[g_n] = v; } ++g_n; }
static float MillibelsToAmplitude(long mb) { return mb <= -10000 ? 0.0f : (float)std::pow(10.0, mb / 2000.0); }
struct Pending {
    unsigned int DirtyFlags; long room, roomHF, roomLF;
    void SetRoom(long v) { room = v; DirtyFlags |= 0x20; }
    void SetRoomHF(long v) { roomHF = v; DirtyFlags |= 0x40; }
    void SetRoomLF(long v) { roomLF = v; DirtyFlags |= 0x80; }
};
struct Stream {
    bool audible; Props cur; Pending pending; const Pending* configured; long mode; int posCalls, velCalls; Vec pos, vel;
#define SETF(T, name, id, conv) void Set##name(T v) { if (cur.name != v) { cur.name = v; Hit(id, conv); } }
    SETF(float, DecayTime, 0x10, v) SETF(float, DecayLFRatio, 0x18, v) SETF(float, DecayHFRatio, 0x14, v)
    SETF(long, Reflections, 0x24, MillibelsToAmplitude(v)) SETF(float, ReflectionsDelay, 0x28, v) SETF(long, Reverb, 0x2c, MillibelsToAmplitude(v))
    SETF(float, ReverbDelay, 0x30, v) SETF(float, EnvironmentSize, 0x64, v) SETF(float, m44, 0x34, v) SETF(float, m48, 0x38, v) SETF(float, EchoTime, 0x3c, v)
    SETF(float, EchoDepth, 0x40, v) SETF(float, ModulationTime, 0x44, v) SETF(float, ModulationDepth, 0x48, v) SETF(unsigned int, LFReference, 0x1c, (float)v)
    void SetProperties(const Update* u)
    {
        const Props& p = u->Properties;
        audible = p.Room > -10000;
        pos = p.Position; ++posCalls; vel = p.Velocity; ++velCalls;
        if (cur.Room != p.Room) { cur.Room = p.Room; pending.SetRoom(p.Room); }
        if (cur.RoomLF != p.RoomLF) { cur.RoomLF = p.RoomLF; pending.SetRoomLF(p.RoomLF); }
        if (cur.RoomHF != p.RoomHF) { cur.RoomHF = p.RoomHF; pending.SetRoomHF(p.RoomHF); }
        SetDecayTime(p.DecayTime); SetDecayLFRatio(p.DecayLFRatio); SetDecayHFRatio(p.DecayHFRatio);
        SetReflections(p.Reflections); SetReflectionsDelay(p.ReflectionsDelay); SetReverb(p.Reverb); SetReverbDelay(p.ReverbDelay);
        SetEnvironmentSize(p.EnvironmentSize); Setm44(p.m44); Setm48(p.m48); SetEchoTime(p.EchoTime); SetEchoDepth(p.EchoDepth);
        SetModulationTime(p.ModulationTime); SetModulationDepth(p.ModulationDepth); SetLFReference(p.LFReference);
        Hit(0x20, (float)p.HFReference);
        configured = &pending; mode = 0;
    }
};
int main()
{
    bool ok = true;
    Stream s; std::memset(&s, 0, sizeof s); s.mode = -1;
    Update u; std::memset(&u, 0, sizeof u);
    u.Properties.Room = -10000; u.Properties.Position.X = 1.0f;
    g_n = 0; s.SetProperties(&u);
    ok = ok && !s.audible && s.posCalls == 1 && s.velCalls == 1 && s.pos.X == 1.0f && s.cur.Room == -10000 && s.pending.room == -10000
            && s.pending.DirtyFlags == 0x20 && g_n == 1 && g_ids[0] == 0x20 && g_vals[0] == 0.0f && s.configured == &s.pending && s.mode == 0;
    u.Properties.Room = -500; u.Properties.RoomHF = -100; u.Properties.Reverb = 2000; u.Properties.EchoDepth = 0.5f;
    u.Properties.LFReference = 0x80000000u; u.Properties.HFReference = 5000;
    g_n = 0; s.pending.DirtyFlags = 0; s.SetProperties(&u);
    ok = ok && s.audible && s.pending.room == -500 && s.pending.roomHF == -100 && s.pending.DirtyFlags == (0x20 | 0x40)
            && g_n == 4 && g_ids[0] == 0x2c && std::fabs(g_vals[0] - 10.0f) < 1e-4f && g_ids[1] == 0x40 && g_vals[1] == 0.5f
            && g_ids[2] == 0x1c && g_vals[2] == 2147483648.0f && g_ids[3] == 0x20 && g_vals[3] == 5000.0f && s.cur.Reverb == 2000;
    g_n = 0; s.SetProperties(&u);
    ok = ok && g_n == 1 && g_ids[0] == 0x20;
    if (ok) std::printf("00c54190_TEST PASS\n"); else std::printf("00c54190_TEST FAIL\n");
    return ok ? 0 : 1;
}
