// Grade: MATCH (Fable landing 2026-10-19 after the Codex review hold: test uses an EmitterModel without the real bitfields)
// CParticleEmitter::Set2DPosition(const C3DVector&, long layer) -- for a 2D emitter, set both interpolator positions (z = 0),
//   optionally the 7-bit 2D layer, and copy the positions into the start/end transforms' translations
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AED090, 167 bytes.
// Layout: PDB (Ego_r) CParticleEmitter names and bitfields, retail offset = PDB offset - 0x0c: the ushort flag word +0x40
//   (Priority:3 .. ContinuousEmitter bits 0..7, Initialised/WaitingToSleep/Sleeping/Emitter2D bits 8..11), the ushort
//   Emitter2DLayer:7 at +0x42, PositionInterpolator +0x94, Transform +0xb0, EndTransform +0x110 (CMatrix3x4 position
//   at +0x24). engine/CParticleEmitter.h carries the Ego_r offsets (all 0x0c late here), so this is a local model.
class C3DVector { public: float X, Y, Z; };
class C3DPositionInterpolator { public: virtual ~C3DPositionInterpolator(); C3DVector StartPosition; C3DVector EndPosition; };
class CMatrix3x4 { public:
    C3DVector Right, Up, Forward;                       // +0x00..+0x23
    C3DVector Position;                                 // +0x24
    void SetPosition(const C3DVector& v) { Position.X = v.X; Position.Y = v.Y; Position.Z = v.Z; }
};
struct CParticleEmitter {
    virtual ~CParticleEmitter();
    unsigned char _base04[0x3c];                        // +0x04..+0x3f
    unsigned short Priority : 3;                        // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;                       // +0x40 bit 11
    unsigned short Emitter2DLayer : 7;                  // +0x42
    unsigned char _body44[0x94 - 0x44];                 // flag bytes, OldPos, Velocity, OrientationInterpolator
    C3DPositionInterpolator PositionInterpolator;       // +0x94
    CMatrix3x4 Transform;                               // +0xb0
    CMatrix3x4 InverseTransform;                        // +0xe0
    CMatrix3x4 EndTransform;                            // +0x110
    bool IsEmitter2D() const { return Emitter2D; }
    void Set2DPosition(const C3DVector& position, long layer);
};
void CParticleEmitter::Set2DPosition(const C3DVector& position, long layer)
{
    if (IsEmitter2D()) {
        PositionInterpolator.StartPosition.X = position.X; PositionInterpolator.StartPosition.Y = position.Y; PositionInterpolator.StartPosition.Z = 0;
        PositionInterpolator.EndPosition.X = position.X; PositionInterpolator.EndPosition.Y = position.Y; PositionInterpolator.EndPosition.Z = 0;
        if (layer != -1) Emitter2DLayer = layer;
        Transform.SetPosition(PositionInterpolator.StartPosition);
        EndTransform.SetPosition(PositionInterpolator.EndPosition);
    }
}