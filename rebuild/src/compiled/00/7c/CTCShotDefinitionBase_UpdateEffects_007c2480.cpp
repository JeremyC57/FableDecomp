// CTCShotDefinitionBase::UpdateEffects() -- while the shot's effect thing is alive and the shot's frame movement is not longer than 1e-4 (the retail test is `!(|d|^2 > eps^2)`), hand the effect's particle-emitter driver that movement as the MOVE_DIRECTION vector parameter
// Leaf: UpdateEffects
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C2480, 284 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CRC Calc @ 0x004014A0 (`__fastcall(seed, data,
//   length)` over "MOVE_DIRECTION" @ 0x0126E160, 14), CBaseClass ctor @ 0x0099A2D0, vector<pair<unsigned long, C3DVector> >
//   ::push_back @ 0x0076B120, CTCDParticleEmitter::SetEmitterUpdateParameters(const CParticleUpdateParameters&) @ 0x006E11A0,
//   ~CParticleUpdateParameters @ 0x006E2500. CParticleUpdateParameters<unsigned long, unsigned long> vtable 0x0125FFDC;
//   g_epsilon_0129ba3c (1e-4f) squared at run time.
// Layout: Ego_r.pdb CTCShotDefinitionBase PEffect +0x0C; retail CThing interface bitset +0x20 (0x4B = word +0x28 bit 11),
//   PTCDriver +0x5C (PDB 0x6C), PhysicsTC +0x60 (PDB 0x70); CTCPhysicsBase Position +0x0C, OldPosition +0x18;
//   CParticleUpdateParameters (the landed 0x006E2570 model): vptr, VectorParams map +0x04 (vector, comparator, Dirty +0x11),
//   UpdateParams map +0x14 (Dirty +0x21).
// Shape: the effect's liveness is the inline two-GetPItem test; the driver is a zero-preloaded pointer filled under HasTC
//   (raw masked word: `test ch,8`); the movement is a named C3DVector whose squared length is summed Z, Y, X (statement
//   order) and tested as `!(lengthSquared > eps * eps)` (`fcompp ; test ah,5 ; jnp skip`: NaN also takes the body); the
//   {crc, movement} pair is a named local built before the parameter block (a push_back temporary is built after it); the parameter block is a local with an inline constructor (CBaseClass base, two empty maps), filled by
//   push_back of the pair and released by its out-of-line destructor.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)
class CBaseClass { public:                                     // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                       // 0x0099A2D0
};
namespace NParticleEngine {
struct SVectorParam {                                   // pair<unsigned long, C3DVector>
    unsigned long m_key;
    C3DVector     m_value;
    SVectorParam(const unsigned long& key, const C3DVector& value) : m_key(key), m_value(value) {}
};
struct SUpdateParam;
struct CVectorParamVector {                             // retail STLport vector (12 bytes)
    SVectorParam* _M_start;
    SVectorParam* _M_finish;
    SVectorParam* _M_end_of_storage;
    CVectorParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void push_back(const SVectorParam& value);          // 0x0076B120
};
struct CUpdateParamVector {
    SUpdateParam* _M_start;
    SUpdateParam* _M_finish;
    SUpdateParam* _M_end_of_storage;
    CUpdateParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CCompare {};
struct CVectorParamMap {
    CVectorParamVector Vector;                          // +0x00
    CCompare           Compare;                         // +0x0c
    bool               Dirty;                           // +0x0d
    CVectorParamMap() : Dirty(false) {}
};
struct CUpdateParamMap {
    CUpdateParamVector Vector;                          // +0x00
    CCompare           Compare;                         // +0x0c
    bool               Dirty;                           // +0x0d
    CUpdateParamMap() : Dirty(false) {}
};
struct CParticleUpdateParameters : CBaseClass {         // <unsigned long, unsigned long>, vtable 0x0125FFDC
    CVectorParamMap VectorParams;                       // +0x04
    CUpdateParamMap UpdateParams;                       // +0x14
    CParticleUpdateParameters() {}
    virtual ~CParticleUpdateParameters();               // 0x006E2500
};
}
class CTCDParticleEmitter { public:
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters);   // 0x006E11A0
};
class CTCPhysicsBase { public:
    unsigned char _pad00[0x0c];
    C3DVector     Position;                             // +0x0c
    C3DVector     OldPosition;                          // +0x18
};
class CThing {
public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    unsigned char   _pad44[0x5c - 0x44];
    void*           PTCDriver;                          // +0x5c
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
};
struct CBaseIntelligentPointer {
    void*   __vftable;                                  // +0x00
    CThing* m_pItem;                                    // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCShotDefinitionBase {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    CBaseIntelligentPointer PEffect;                    // +0x0c
    void UpdateEffects();
};

void CTCShotDefinitionBase::UpdateEffects()
{
    if (!IS_THING_ALIVE(PEffect))
        return;
    CTCDParticleEmitter* emitter = 0;
    CThing* effect = PEffect.GetPItem();
    if (effect->HasTC(0x4b))
        emitter = (CTCDParticleEmitter*)effect->PTCDriver;
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    C3DVector movement(physics->Position.X - physics->OldPosition.X, physics->Position.Y - physics->OldPosition.Y,
                       physics->Position.Z - physics->OldPosition.Z);
    float lengthSquared = movement.Z * movement.Z;
    lengthSquared += movement.Y * movement.Y;
    lengthSquared += movement.X * movement.X;
    if (!(lengthSquared > g_epsilon_0129ba3c * g_epsilon_0129ba3c)) {
        unsigned long key = Calc_004014a0(0, "MOVE_DIRECTION", 14);
        NParticleEngine::SVectorParam moveDirection(key, movement);
        NParticleEngine::CParticleUpdateParameters parameters;
        parameters.VectorParams.Vector.push_back(moveDirection);
        emitter->SetEmitterUpdateParameters(parameters);
    }
}