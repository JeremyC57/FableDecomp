#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#define free test_free
#define malloc test_malloc
// Behaviour test for 00ae2050: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// CParticleSimulation::WriteToFile(CDataOutputStream&) -- save the timestep settings, the particle count and positions, the per-particle alphas (raw), then the gravity/wind strengths and the dragging, acceleration and damping settings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE2050, length 171 bytes (`ret 4`; manifest label correct).
// Callees: CDataOutputStream::WriteFloat @ 0x00993EE0 (x7), WriteULong @ 0x00993EA0 (x2), WriteBool @ 0x00993E30 (x3),
//          the stream's virtual Write(const void*, long) (slot 4), the C4DVector array writer @ 0x00AE18C0 (__fastcall
//          positions/count + stream; its manifest label "WriteAnimationData" is propagated).
// Layout: Ego_r.pdb CParticleSimulation (0x80 bytes, retail identical): Timestep +0x0c, TimestepChanged +0x10,
// TimestepMultiplier +0x14, Size +0x18, Positions +0x1c, SimulationAlphas +0x2c, GravityStrength +0x34,
// WindStrength +0x38, DraggingEnable/Rotational +0x3c/+0x3d, DraggingStrength +0x40, AccelerationEnable +0x44,
// GlobalDamping +0x48. MaximumAdjustment (+0x30) is not saved.
// Shape: the TimestepChanged flag goes out as a ULONG (`movzx`), the other flags through WriteBool.
struct C4DVector { float X, Y, Z, W; };

class CDataOutputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);   // slot 4 (+0x10)
    void WriteBool(bool value);                            // 0x00993E30
    void WriteULong(unsigned long value);                  // 0x00993EA0
    void WriteFloat(float value);                          // 0x00993EE0
};

void __fastcall WriteVectors_00ae18c0(const C4DVector* vectors, unsigned long count, CDataOutputStream& stream);   // 0x00AE18C0

struct CParticleSimulation {
    void*         __vftable;                       // +0x00
    unsigned char RenderData[8];                   // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float         Timestep;                        // +0x0c
    bool          TimestepChanged;                 // +0x10
    float         TimestepMultiplier;              // +0x14
    unsigned long Size;                            // +0x18
    C4DVector*    Positions;                       // +0x1c
    C4DVector*    LastPositions;                   // +0x20
    C4DVector*    SavedPositions;                  // +0x24
    C4DVector*    Accelerations;                   // +0x28
    float*        SimulationAlphas;                // +0x2c
    float         MaximumAdjustment;               // +0x30
    float         GravityStrength;                 // +0x34
    float         WindStrength;                    // +0x38
    bool          DraggingEnable;                  // +0x3c
    bool          DraggingRotational;              // +0x3d
    float         DraggingStrength;                // +0x40
    bool          AccelerationEnable;              // +0x44
    float         GlobalDamping;                   // +0x48
    void WriteToFile(CDataOutputStream& stream);
};

void CParticleSimulation::WriteToFile(CDataOutputStream& stream)
{
    stream.WriteFloat(Timestep);
    stream.WriteULong(TimestepChanged);
    stream.WriteFloat(TimestepMultiplier);
    stream.WriteULong(Size);
    WriteVectors_00ae18c0(Positions, Size, stream);
    stream.Write(SimulationAlphas, Size * sizeof(float));
    stream.WriteFloat(GravityStrength);
    stream.WriteFloat(WindStrength);
    stream.WriteBool(DraggingEnable);
    stream.WriteBool(DraggingRotational);
    stream.WriteFloat(DraggingStrength);
    stream.WriteBool(AccelerationEnable);
    stream.WriteFloat(GlobalDamping);
}

// ---- stubs and driver ----
static char g_log[512];
static char g_num[64];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
void CDataOutputStream::v00() {} void CDataOutputStream::v01() {} void CDataOutputStream::v02() {} void CDataOutputStream::v03() {}
void CDataOutputStream::Write(const void* data, long length) { std::sprintf(g_num, "W%ld:%g", length, ((const float*)data)[0]); Log(g_num); }
void CDataOutputStream::WriteBool(bool v) { Log(v ? "T" : "F"); }
void CDataOutputStream::WriteULong(unsigned long v) { std::sprintf(g_num, "U%lu", v); Log(g_num); }
void CDataOutputStream::WriteFloat(float v) { std::sprintf(g_num, "F%g", v); Log(g_num); }
void __fastcall WriteVectors_00ae18c0(const C4DVector* v, unsigned long n, CDataOutputStream&) { std::sprintf(g_num, "V%lu:%g", n, v[0].X); Log(g_num); }
int main()
{
    C4DVector pos[2] = { { 3, 0, 0, 1 }, { 4, 0, 0, 1 } };
    float alphas[2] = { 0.75f, 1 };
    CParticleSimulation s;
    std::memset(&s, 0, sizeof(s));
    s.Timestep = 0.5f; s.TimestepChanged = true; s.TimestepMultiplier = 2; s.Size = 2; s.Positions = pos; s.SimulationAlphas = alphas;
    s.MaximumAdjustment = 99; s.GravityStrength = 9.5f; s.WindStrength = 1.25f; s.DraggingEnable = true; s.DraggingRotational = false;
    s.DraggingStrength = 0.125f; s.AccelerationEnable = true; s.GlobalDamping = 0.25f;
    CDataOutputStream* stream = (CDataOutputStream*)0;
    struct CStreamImpl : CDataOutputStream {} impl;
    s.WriteToFile(impl);
    bool ok = std::strcmp(g_log, "F0.5,U1,F2,U2,V2:3,W8:0.75,F9.5,F1.25,T,F,F0.125,T,F0.25,") == 0;
    (void)stream;
    if (!ok) { std::printf("00ae2050_TEST FAIL\n"); return 1; }
    std::printf("00ae2050_TEST PASS\n");
    return 0;
}