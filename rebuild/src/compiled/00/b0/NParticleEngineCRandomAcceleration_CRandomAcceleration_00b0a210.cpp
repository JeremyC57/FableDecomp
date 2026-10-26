// NParticleEngine::CRandomAcceleration::CRandomAcceleration(unsigned long&) -- draw the three noise-table indices and the three time offsets from the caller's random seed, and zero the previous offset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A210, length 230 bytes, `ret 4` (returns `this`)
// (PDB ??0CRandomAcceleration@NParticleEngine@@QAE@AAK@Z).
// Leaf: ??0CRandomAcceleration
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, GFROR13 0x00497890 (`__fastcall` rotate of the
// seed). Vtable 0x0129FC08 (RTTI NParticleEngine::CRandomAcceleration: slot 0 the destructor, slot 1 Delete).
// Layout (PDB, 24 bytes): TimeOffsetX/Y/Z +4..+6, IndexX/Y/Z +7..+9, C3DVector OldOffset +0x0a -- the vector really is
// at an even, non-4-aligned offset in the PDB, so the class is packed to 2.
// Shape: each draw is the engine's linear-congruential step written back to the seed before and after the rotate
// (seed = seed * 0x24A1 + 0x24DF; seed = GFROR13(seed)), the low byte stored; the indices are drawn before the time
// offsets; OldOffset is assigned a zero C3DVector temporary (three frame words copied through registers).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
long __fastcall GFROR13(unsigned long);  // 0x00497890

struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

static inline unsigned long NextRandom(unsigned long& seed)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    return seed;
}

namespace NParticleEngine {

#pragma pack(push, 2)
class CRandomAcceleration : public CBaseClassNonCopyable {
public:
    unsigned char TimeOffsetX;                           // +0x04
    unsigned char TimeOffsetY;                           // +0x05
    unsigned char TimeOffsetZ;                           // +0x06
    unsigned char IndexX;                                // +0x07
    unsigned char IndexY;                                // +0x08
    unsigned char IndexZ;                                // +0x09
    C3DVector     OldOffset;                             // +0x0a
    CRandomAcceleration(unsigned long& seed);
    virtual ~CRandomAcceleration();
    virtual void Delete();
};
#pragma pack(pop)

CRandomAcceleration::CRandomAcceleration(unsigned long& seed)
{
    IndexX = (unsigned char)NextRandom(seed);
    IndexY = (unsigned char)NextRandom(seed);
    IndexZ = (unsigned char)NextRandom(seed);
    TimeOffsetX = (unsigned char)NextRandom(seed);
    TimeOffsetY = (unsigned char)NextRandom(seed);
    TimeOffsetZ = (unsigned char)NextRandom(seed);
    OldOffset = C3DVector(0.0f, 0.0f, 0.0f);
}

}