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