// CTCDParticleEmitter::GetEmitterPosition() const -- the live emitter's current position (the translation of its transform), or the parent thing's position when no emitter exists yet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0620, length 100 bytes (+ int3 padding); returns C3DVector through the hidden pointer.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), CThing::GetPosition @ 0x004C73D0.
// Layout: the retail NParticleEngine::CParticleEmitter keeps a 3x4 transform at +0xB0 whose translation row is at +0xD4 (+0x24 in
// the matrix); CTCDParticleEmitter as in the landed SetAlwaysUpdate 0x006E0590 (PEmitter +0x14).
// Shape: the emitter's inline by-value getter returns `C3DVector(t.X, t.Y, t.Z)` (arguments evaluated right to left: Z and Y on
// the x87 stack, X through ecx; the dead `add eax,0B0h` is the matrix address); the absent arm copies the parent position through
// C3DVector's user copy ctor (the implicit copy routes the result pointer through edx).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CMatrix34 {                              // 48 bytes
    float Right[3];                             // +0x00
    float Up[3];                                // +0x0c
    float Forward[3];                           // +0x18
    float Translation[3];                       // +0x24
    C3DVector GetTranslation() const { return C3DVector(Translation[0], Translation[1], Translation[2]); }
};
namespace NParticleEngine {
class CParticleEmitter { public:
    unsigned char _pad00[0xb0];
    CMatrix34 Transform;                        // +0xb0
    C3DVector GetPosition() const { return Transform.GetTranslation(); }
};
}
class CThing { public:
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {                // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                // +0x04
    NParticleEngine::CParticleEmitter* GetPItem() const;   // 0x00A01B50
};
class CTCDParticleEmitter { public:
    virtual void v00();
    CThing*                 ParentThing;        // +0x04 (CTCBase)
    unsigned char           _base08[0x4];
    void*                   EmitterName;        // +0x0c CCharString
    long                    ParticleTypeID;     // +0x10
    CBaseIntelligentPointer PEmitter;           // +0x14
    C3DVector GetEmitterPosition() const;
};

C3DVector CTCDParticleEmitter::GetEmitterPosition() const
{
    if (PEmitter.GetPItem())
        return PEmitter.GetPItem()->GetPosition();
    return ParentThing->GetPosition();
}